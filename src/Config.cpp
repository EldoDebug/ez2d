#include "Config.hpp"
#include <filesystem>
#include <numeric>
#include <sstream>
#include <fstream>

Config::Config(const std::filesystem::path& filePath, const std::string& magic, const std::string& version)
    : filePath(filePath), magicString(magic), versionString(version) {
    magicValue = stringToMagic(magic);
    versionValue = stringToVersion(version);
}

uint32_t Config::stringToMagic(const std::string& str) const {
    uint32_t hash = 0x811c9dc5;
    for (char c : str) {
        hash ^= static_cast<uint32_t>(static_cast<unsigned char>(c));
        hash *= 0x01000193;
    }
    return hash;
}

uint16_t Config::stringToVersion(const std::string& str) const {

    std::istringstream iss(str);
    std::string token;
    uint16_t result = 0;
    int multiplier = 100;
    
    while (std::getline(iss, token, '.') && multiplier >= 1) {
        try {
            int num = std::stoi(token);
            result += static_cast<uint16_t>(num * multiplier);
            multiplier /= 10;
        } catch (...) {
            break;
        }
    }
    
    return result > 0 ? result : 1;
}

uint32_t Config::calculateChecksum(const std::vector<uint8_t>& data) const {
    return std::accumulate(data.begin(), data.end(), 0u,
        [](uint32_t sum, uint8_t byte) { return sum + byte; });
}

uint64_t Config::getCurrentTimestamp() const {
    return static_cast<uint64_t>(std::time(nullptr));
}

std::shared_ptr<ConfigSection> Config::getSection(const std::string& sectionName) {
    auto it = sections.find(sectionName);
    if (it == sections.end()) {
        auto section = std::make_shared<ConfigSection>(sectionName);
        sections[sectionName] = section;
        return section;
    }
    return it->second;
}

bool Config::hasSection(const std::string& sectionName) const {
    return sections.find(sectionName) != sections.end();
}

void Config::removeSection(const std::string& sectionName) {
    sections.erase(sectionName);
}

std::vector<std::string> Config::getSectionNames() const {
    std::vector<std::string> names;
    for (const auto& [name, _] : sections) {
        names.push_back(name);
    }
    return names;
}

bool Config::save() {

    if (filePath.has_parent_path()) {
        std::filesystem::create_directories(filePath.parent_path());
    }
    
    std::ofstream file(filePath, std::ios::binary);

    if (!file) {
        return false;
    }

    std::ostringstream dataStream;
    
    for (const auto& [sectionName, section] : sections) {

        uint32_t nameLength = static_cast<uint32_t>(sectionName.length());
        dataStream.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        dataStream.write(sectionName.c_str(), nameLength);
        
        const auto& values = section->getValues();
        uint32_t keyCount = static_cast<uint32_t>(values.size());
        dataStream.write(reinterpret_cast<const char*>(&keyCount), sizeof(keyCount));
        
        for (const auto& [key, value] : values) {
            uint32_t keyLength = static_cast<uint32_t>(key.length());
            dataStream.write(reinterpret_cast<const char*>(&keyLength), sizeof(keyLength));
            dataStream.write(key.c_str(), keyLength);
            
            uint8_t type = static_cast<uint8_t>(value.index());
            dataStream.write(reinterpret_cast<const char*>(&type), sizeof(type));
            
            std::visit([&dataStream](const auto& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, std::string>) {
                    uint32_t strLength = static_cast<uint32_t>(v.length());
                    dataStream.write(reinterpret_cast<const char*>(&strLength), sizeof(strLength));
                    dataStream.write(v.c_str(), strLength);
                } else {
                    dataStream.write(reinterpret_cast<const char*>(&v), sizeof(v));
                }
            }, value);
        }
    }

    Config::Header header = {};
    header.magic = magicValue;
    header.version = versionValue;
    header.sectionCount = static_cast<uint16_t>(sections.size());
    header.timestamp = getCurrentTimestamp();
    
    std::string dataStr = dataStream.str();
    std::vector<uint8_t> dataVec(dataStr.begin(), dataStr.end());
    header.checksum = calculateChecksum(dataVec);

    file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    file.write(dataStr.c_str(), dataStr.length());

    return file.good();
}

bool Config::load() {    
    
    std::ifstream file(filePath, std::ios::binary);
    
    if (!file) {
        return false;
    }

    Config::Header header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));    
    
    if (header.magic != magicValue) {
        return false;
    }

    sections.clear();

    for (uint16_t i = 0; i < header.sectionCount; ++i) {

        uint32_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        
        std::string sectionName(nameLength, '\0');
        file.read(&sectionName[0], nameLength);
        
        auto section = std::make_shared<ConfigSection>(sectionName);
        
        uint32_t keyCount;
        file.read(reinterpret_cast<char*>(&keyCount), sizeof(keyCount));
        
        for (uint32_t j = 0; j < keyCount; ++j) {

            uint32_t keyLength;
            file.read(reinterpret_cast<char*>(&keyLength), sizeof(keyLength));
            
            std::string key(keyLength, '\0');
            file.read(&key[0], keyLength);
            
            uint8_t type;
            file.read(reinterpret_cast<char*>(&type), sizeof(type));
            
            switch (type) {
                case 0: {
                    bool value;
                    file.read(reinterpret_cast<char*>(&value), sizeof(value));
                    section->set(key, value);
                    break;
                }
                case 1: {
                    int32_t value;
                    file.read(reinterpret_cast<char*>(&value), sizeof(value));
                    section->set(key, static_cast<int>(value));
                    break;
                }
                case 2: {
                    int64_t value;
                    file.read(reinterpret_cast<char*>(&value), sizeof(value));
                    section->set(key, value);
                    break;
                }
                case 3: {
                    float value;
                    file.read(reinterpret_cast<char*>(&value), sizeof(value));
                    section->set(key, value);
                    break;
                }
                case 4: {
                    double value;
                    file.read(reinterpret_cast<char*>(&value), sizeof(value));
                    section->set(key, value);
                    break;
                }
                case 5: {
                    uint32_t strLength;
                    file.read(reinterpret_cast<char*>(&strLength), sizeof(strLength));
                    
                    std::string value(strLength, '\0');
                    file.read(&value[0], strLength);
                    section->set(key, value);
                    break;
                }
            }
        }
        
        sections[sectionName] = section;
    }

    return true;
}
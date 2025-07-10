#include "Config.hpp"

ConfigSection::ConfigSection(const std::string& name) : sectionName(name) {}

void ConfigSection::set(const std::string& key, bool value) {
    values[key] = value;
}

void ConfigSection::set(const std::string& key, int value) {
    values[key] = static_cast<int32_t>(value);
}

void ConfigSection::set(const std::string& key, int64_t value) {
    values[key] = value;
}

void ConfigSection::set(const std::string& key, float value) {
    values[key] = value;
}

void ConfigSection::set(const std::string& key, double value) {
    values[key] = value;
}

void ConfigSection::set(const std::string& key, const std::string& value) {
    values[key] = value;
}

void ConfigSection::set(const std::string& key, const char* value) {
    values[key] = std::string(value);
}

template<typename T>
T ConfigSection::get(const std::string& key, const T& defaultValue) const {
    auto it = values.find(key);
    if (it != values.end()) {
        try {
            return std::get<T>(it->second);
        } catch (const std::bad_variant_access&) {
        }
    }
    return defaultValue;
}

bool ConfigSection::getBool(const std::string& key, bool defaultValue) const {
    return get<bool>(key, defaultValue);
}

int ConfigSection::getInt(const std::string& key, int defaultValue) const {
    auto it = values.find(key);
    if (it != values.end()) {
        try {
            return static_cast<int>(std::get<int32_t>(it->second));
        } catch (const std::bad_variant_access&) {
        }
    }
    return defaultValue;
}

int64_t ConfigSection::getInt64(const std::string& key, int64_t defaultValue) const {
    return get<int64_t>(key, defaultValue);
}

float ConfigSection::getFloat(const std::string& key, float defaultValue) const {
    return get<float>(key, defaultValue);
}

double ConfigSection::getDouble(const std::string& key, double defaultValue) const {
    return get<double>(key, defaultValue);
}

std::string ConfigSection::getString(const std::string& key, const std::string& defaultValue) const {
    return get<std::string>(key, defaultValue);
}

bool ConfigSection::hasKey(const std::string& key) const {
    return values.find(key) != values.end();
}

void ConfigSection::removeKey(const std::string& key) {
    values.erase(key);
}

std::vector<std::string> ConfigSection::getKeys() const {
    std::vector<std::string> keys;
    for (const auto& [key, _] : values) {
        keys.push_back(key);
    }
    return keys;
}

void ConfigSection::clear() {
    values.clear();
}

template bool ConfigSection::get<bool>(const std::string&, const bool&) const;
template int32_t ConfigSection::get<int32_t>(const std::string&, const int32_t&) const;
template int64_t ConfigSection::get<int64_t>(const std::string&, const int64_t&) const;
template float ConfigSection::get<float>(const std::string&, const float&) const;
template double ConfigSection::get<double>(const std::string&, const double&) const;
template std::string ConfigSection::get<std::string>(const std::string&, const std::string&) const;
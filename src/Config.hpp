#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

using ConfigValue = std::variant<bool, int32_t, int64_t, float, double, std::string>;

class ConfigSection;

class Config {

    public:
        struct Header {
            uint32_t magic;
            uint16_t version;
            uint16_t sectionCount;
            uint64_t timestamp;
            uint32_t checksum;
        };    
        
    protected:
        std::filesystem::path filePath;
        uint32_t magicValue;
        uint16_t versionValue;
        std::string magicString;
        std::string versionString;
        std::unordered_map<std::string, std::shared_ptr<ConfigSection>> sections;

        uint32_t stringToMagic(const std::string& str) const;
        uint16_t stringToVersion(const std::string& str) const;
        uint32_t calculateChecksum(const std::vector<uint8_t>& data) const;
        uint64_t getCurrentTimestamp() const;

    public:
        Config(const std::filesystem::path& filePath, const std::string& magic, const std::string& version);
        virtual ~Config() = default;

        std::shared_ptr<ConfigSection> getSection(const std::string& name);
        bool hasSection(const std::string& name) const;
        void removeSection(const std::string& name);
        std::vector<std::string> getSectionNames() const;

        virtual bool save();
        virtual bool load();

        const std::filesystem::path& getFilePath() const { return filePath; }
        uint32_t getMagicValue() const { return magicValue; }
        uint16_t getVersionValue() const { return versionValue; }
        const std::string& getMagicString() const { return magicString; }
        const std::string& getVersionString() const { return versionString; }
};

class ConfigSection {

    private:
        std::string sectionName;
        std::unordered_map<std::string, ConfigValue> values;
        std::weak_ptr<Config> parentConfig;

    public:
        ConfigSection(const std::string& name);

        void set(const std::string& key, bool value);
        void set(const std::string& key, int value);
        void set(const std::string& key, int64_t value);
        void set(const std::string& key, float value);
        void set(const std::string& key, double value);
        void set(const std::string& key, const std::string& value);
        void set(const std::string& key, const char* value);

        template<typename T>
        T get(const std::string& key, const T& defaultValue = T{}) const;

        bool getBool(const std::string& key, bool defaultValue = false) const;
        int getInt(const std::string& key, int defaultValue = 0) const;
        int64_t getInt64(const std::string& key, int64_t defaultValue = 0) const;
        float getFloat(const std::string& key, float defaultValue = 0.0f) const;
        double getDouble(const std::string& key, double defaultValue = 0.0) const;
        std::string getString(const std::string& key, const std::string& defaultValue = "") const;

        bool hasKey(const std::string& key) const;
        void removeKey(const std::string& key);
        std::vector<std::string> getKeys() const;
        void clear();

        const std::unordered_map<std::string, ConfigValue>& getValues() const { return values; }
        const std::string& getName() const { return sectionName; }
};
#pragma once
#include <string>
#include <random>
#include <sstream>
#include <iomanip>
#include <array>
#include <cstdint>
#include <algorithm>
#include <iostream>

class UUID {

    private:
        std::array<uint8_t, 16> data;

        static std::string toHexString(uint8_t byte) {
            std::stringstream ss;
            ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);
            return ss.str();
        }

        static uint8_t fromHexString(const std::string& hex, size_t offset) {
            std::string byteString = hex.substr(offset, 2);
            return static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
        }

    public:
        UUID() {
            data.fill(0);
        }

        explicit UUID(const std::array<uint8_t, 16>& bytes) : data(bytes) {}
        explicit UUID(const std::string& uuidString) {
            parseFromString(uuidString);
        }

        UUID(const UUID& other) = default;

        UUID& operator=(const UUID& other) = default; 
        static UUID randomUUID() {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<int> dis(0, 255);

            std::array<uint8_t, 16> bytes;
            for (auto& byte : bytes) {
                byte = static_cast<uint8_t>(dis(gen));
            }

            bytes[6] = (bytes[6] & 0x0F) | 0x40;
            bytes[8] = (bytes[8] & 0x3F) | 0x80;

            return UUID(bytes);
        }

        static UUID nilUUID() {
            return UUID();
        }

        static UUID fromString(const std::string& uuidString) {
            UUID uuid;
            uuid.parseFromString(uuidString);
            return uuid;
        }

        std::string toString() const {
            std::stringstream ss;
            
            for (size_t i = 0; i < 4; ++i) {
                ss << toHexString(data[i]);
            }
            ss << "-";
            
            for (size_t i = 4; i < 6; ++i) {
                ss << toHexString(data[i]);
            }
            ss << "-";
            
            for (size_t i = 6; i < 8; ++i) {
                ss << toHexString(data[i]);
            }
            ss << "-";
            
            for (size_t i = 8; i < 10; ++i) {
                ss << toHexString(data[i]);
            }
            ss << "-";
            
            for (size_t i = 10; i < 16; ++i) {
                ss << toHexString(data[i]);
            }
            
            return ss.str();
        }

        void parseFromString(const std::string& uuidString) {
            std::string cleanString = uuidString;
            
            cleanString.erase(std::remove(cleanString.begin(), cleanString.end(), '-'), cleanString.end());
            
            if (cleanString.length() != 32) {
                data.fill(0);
                return;
            }
            
            for (size_t i = 0; i < 16; ++i) {
                data[i] = fromHexString(cleanString, i * 2);
            }
        }

        const std::array<uint8_t, 16>& getBytes() const {
            return data;
        }

        bool isNil() const {
            for (const auto& byte : data) {
                if (byte != 0) return false;
            }
            return true;
        }

        int getVersion() const {
            return (data[6] >> 4) & 0x0F;
        }

        int getVariant() const {
            return (data[8] >> 6) & 0x03;
        }

        bool operator==(const UUID& other) const {
            return data == other.data;
        }

        bool operator!=(const UUID& other) const {
            return !(*this == other);
        }

        bool operator<(const UUID& other) const {
            return data < other.data;
        }

        bool operator<=(const UUID& other) const {
            return data <= other.data;
        }

        bool operator>(const UUID& other) const {
            return data > other.data;
        }

        bool operator>=(const UUID& other) const {
            return data >= other.data;
        }

        size_t hash() const {
            size_t result = 0;
            for (const auto& byte : data) {
                result = result * 31 + byte;
            }
            return result;
        }

        friend std::ostream& operator<<(std::ostream& os, const UUID& uuid) {
            os << uuid.toString();
            return os;
        }
};

namespace std {
    template<>
    struct hash<UUID> {
        size_t operator()(const UUID& uuid) const {
            return uuid.hash();
        }
    };
}

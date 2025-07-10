#pragma once

#include "../src/api/CacheManager.hpp"
#include "Example.hpp"
#include <string>
#include <vector>
#include <ctime>

struct StringCacheKey : public CacheKeyBase<StringCacheKey> {
    std::string key;
    
    StringCacheKey() = default;
    StringCacheKey(const std::string& k) : key(k) {}
    
    bool operator==(const StringCacheKey& other) const {
        return key == other.key;
    }
    
    bool operator!=(const StringCacheKey& other) const {
        return !(*this == other);
    }
    
    bool isEqual(const StringCacheKey& other) const {
        return *this == other;
    }
    
    std::size_t getHash() const {
        return calculateHash(key);
    }
};

struct StringCacheKeyHash : public CacheKeyHashBase<StringCacheKey> {};

struct UserProfileKey : public CacheKeyBase<UserProfileKey> {
    int userId;
    std::string profileType;
    
    UserProfileKey() = default;
    UserProfileKey(int id, const std::string& type) : userId(id), profileType(type) {}
    
    bool operator==(const UserProfileKey& other) const {
        return userId == other.userId && profileType == other.profileType;
    }
    
    bool operator!=(const UserProfileKey& other) const {
        return !(*this == other);
    }
    
    bool isEqual(const UserProfileKey& other) const {
        return *this == other;
    }
    
    std::size_t getHash() const {
        return calculateHash(userId, profileType);
    }
};

struct UserProfileKeyHash : public CacheKeyHashBase<UserProfileKey> {};

struct FileCacheKey : public CacheKeyBase<FileCacheKey> {
    std::string filePath;
    std::size_t fileSize;
    std::time_t lastModified;
    
    FileCacheKey() = default;
    FileCacheKey(const std::string& path, std::size_t size, std::time_t modified)
        : filePath(path), fileSize(size), lastModified(modified) {}
    
    bool operator==(const FileCacheKey& other) const {
        return filePath == other.filePath && 
               fileSize == other.fileSize && 
               lastModified == other.lastModified;
    }
    
    bool operator!=(const FileCacheKey& other) const {
        return !(*this == other);
    }
    
    bool isEqual(const FileCacheKey& other) const {
        return *this == other;
    }
    
    std::size_t getHash() const {
        return calculateHash(filePath, fileSize, lastModified);
    }
};

struct FileCacheKeyHash : public CacheKeyHashBase<FileCacheKey> {};

struct GeometryCacheKey : public CacheKeyBase<GeometryCacheKey> {
    float x, y, z;
    int precision;
    
    GeometryCacheKey() = default;
    GeometryCacheKey(float x, float y, float z, int prec = 2) 
        : x(x), y(y), z(z), precision(prec) {}
    
    bool operator==(const GeometryCacheKey& other) const {
        return x == other.x && y == other.y && z == other.z && precision == other.precision;
    }
    
    bool operator!=(const GeometryCacheKey& other) const {
        return !(*this == other);
    }
    
    bool isEqual(const GeometryCacheKey& other) const {
        return *this == other;
    }
    
    std::size_t getHash() const {
        return calculateHash(x, y, z, precision);
    }
};

struct GeometryCacheKeyHash : public CacheKeyHashBase<GeometryCacheKey> {};

struct UserProfile {
    int id;
    std::string name;
    std::string email;
    int age;
    
    UserProfile() = default;
    UserProfile(int id, const std::string& name, const std::string& email, int age)
        : id(id), name(name), email(email), age(age) {}
};

struct FileData {
    std::string content;
    std::vector<uint8_t> binaryData;
    std::string mimeType;
    
    FileData() = default;
    FileData(const std::string& content, const std::string& mime)
        : content(content), mimeType(mime) {}
};

struct GeometryData {
    std::vector<float> vertices;
    std::vector<int> indices;
    std::string description;
    
    GeometryData() = default;
    GeometryData(const std::vector<float>& verts, const std::vector<int>& idx, const std::string& desc)
        : vertices(verts), indices(idx), description(desc) {}
};

class CacheManagerExample : public Example {
public:
    void run() override;
    
    static void runAllExamples();
    static void basicStringCacheExample();
    static void userProfileCacheExample();
    static void fileCacheExample();
    static void geometryCacheExample();
    static void polygonCacheExample();
    static void getOrCreateExample();
    static void conditionalRemovalExample();
    static void performanceComparisonExample();
};

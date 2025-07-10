#include "CacheManagerExample.hpp"
#include "../src/api/PolygonCacheKey.hpp"
#include <iostream>
#include <chrono>
#include <random>
#include <sstream>
#include <ctime>

void CacheManagerExample::run() {
    runAllExamples();
}

void CacheManagerExample::runAllExamples() {
    std::cout << "===== CacheManager Examples =====" << std::endl << std::endl;
    
    basicStringCacheExample();
    userProfileCacheExample();
    fileCacheExample();
    geometryCacheExample();
    polygonCacheExample();
    getOrCreateExample();
    conditionalRemovalExample();
    performanceComparisonExample();
    
    std::cout << "===== All Examples Completed =====" << std::endl;
}

void CacheManagerExample::basicStringCacheExample() {
    std::cout << "=== Basic String Cache Example ===" << std::endl;
    
    CacheManager<StringCacheKey, std::string, StringCacheKeyHash> cache;
    
    cache.put(StringCacheKey("hello"), "world");
    cache.put(StringCacheKey("foo"), "bar");
    cache.put(StringCacheKey("cache"), "manager");
    
    std::string value;
    if (cache.get(StringCacheKey("hello"), value)) {
        std::cout << "Found: hello -> " << value << std::endl;
    }
    
    if (cache.get(StringCacheKey("foo"), value)) {
        std::cout << "Found: foo -> " << value << std::endl;
    }
    
    if (cache.contains(StringCacheKey("cache"))) {
        std::cout << "Key 'cache' exists in cache" << std::endl;
    }
    
    if (!cache.contains(StringCacheKey("missing"))) {
        std::cout << "Key 'missing' does not exist in cache" << std::endl;
    }
    
    std::cout << "Cache size: " << cache.size() << std::endl;
    std::cout << std::endl;
}

void CacheManagerExample::userProfileCacheExample() {
    std::cout << "=== User Profile Cache Example ===" << std::endl;
    
    CacheManager<UserProfileKey, UserProfile, UserProfileKeyHash> cache;
    
    UserProfile user1(1, "Alice", "alice@example.com", 25);
    UserProfile user2(2, "Bob", "bob@example.com", 30);
    UserProfile user3(1, "Alice Admin", "alice.admin@example.com", 25);
    
    cache.put(UserProfileKey(1, "basic"), user1);
    cache.put(UserProfileKey(2, "basic"), user2);
    cache.put(UserProfileKey(1, "admin"), user3);
    
    UserProfile profile;
    if (cache.get(UserProfileKey(1, "basic"), profile)) {
        std::cout << "User 1 (basic): " << profile.name << " (" << profile.email << ")" << std::endl;
    }
    
    if (cache.get(UserProfileKey(1, "admin"), profile)) {
        std::cout << "User 1 (admin): " << profile.name << " (" << profile.email << ")" << std::endl;
    }
    
    if (cache.get(UserProfileKey(2, "basic"), profile)) {
        std::cout << "User 2 (basic): " << profile.name << " (" << profile.email << ")" << std::endl;
    }
    
    std::cout << "Profile cache size: " << cache.size() << std::endl;
    std::cout << std::endl;
}

void CacheManagerExample::fileCacheExample() {
    std::cout << "=== File Cache Example ===" << std::endl;
    
    CacheManager<FileCacheKey, FileData, FileCacheKeyHash> cache;
    
    std::time_t now = std::time(nullptr);
    FileCacheKey key1("config.txt", 1024, now);
    FileCacheKey key2("data.json", 2048, now - 3600);
    FileCacheKey key3("image.png", 51200, now - 7200);
    
    FileData file1("key=value\nsetting=true", "text/plain");
    FileData file2("{\"name\": \"test\", \"value\": 42}", "application/json");
    FileData file3("", "image/png");
    file3.binaryData = {0x89, 0x50, 0x4E, 0x47};
    
    cache.put(key1, file1);
    cache.put(key2, file2);
    cache.put(key3, file3);
    
    FileData data;
    if (cache.get(key1, data)) {
        std::cout << "Config file: " << data.content << " (type: " << data.mimeType << ")" << std::endl;
    }
    
    if (cache.get(key2, data)) {
        std::cout << "JSON file: " << data.content << " (type: " << data.mimeType << ")" << std::endl;
    }
    
    if (cache.get(key3, data)) {
        std::cout << "PNG file: " << data.binaryData.size() << " bytes (type: " << data.mimeType << ")" << std::endl;
    }
    
    std::cout << "File cache size: " << cache.size() << std::endl;
    std::cout << std::endl;
}

void CacheManagerExample::geometryCacheExample() {
    std::cout << "=== Geometry Cache Example ===" << std::endl;
    
    CacheManager<GeometryCacheKey, GeometryData, GeometryCacheKeyHash> cache;
    
    std::vector<float> triangleVerts = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f, 0.0f};
    std::vector<int> triangleIndices = {0, 1, 2};
    GeometryData triangle(triangleVerts, triangleIndices, "Simple triangle");
    
    std::vector<float> quadVerts = {-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f};
    std::vector<int> quadIndices = {0, 1, 2, 2, 3, 0};
    GeometryData quad(quadVerts, quadIndices, "Simple quad");
    
    cache.put(GeometryCacheKey(0.0f, 0.0f, 0.0f, 2), triangle);
    cache.put(GeometryCacheKey(10.0f, 5.0f, 0.0f, 2), quad);
    cache.put(GeometryCacheKey(0.0f, 0.0f, 0.0f, 4), triangle);
    
    GeometryData geom;
    if (cache.get(GeometryCacheKey(0.0f, 0.0f, 0.0f, 2), geom)) {
        std::cout << "Triangle at origin: " << geom.description 
                  << " (" << geom.vertices.size() / 3 << " vertices)" << std::endl;
    }
    
    if (cache.get(GeometryCacheKey(10.0f, 5.0f, 0.0f, 2), geom)) {
        std::cout << "Quad at (10,5): " << geom.description 
                  << " (" << geom.vertices.size() / 3 << " vertices)" << std::endl;
    }
    
    if (cache.get(GeometryCacheKey(0.0f, 0.0f, 0.0f, 4), geom)) {
        std::cout << "High precision triangle: " << geom.description << std::endl;
    }
    
    std::cout << "Geometry cache size: " << cache.size() << std::endl;
    std::cout << std::endl;
}

void CacheManagerExample::polygonCacheExample() {
    std::cout << "=== Polygon Cache Example ===" << std::endl;
    
    CacheManager<PolygonCacheKey, std::vector<std::vector<int>>, PolygonCacheKeyHash> cache;
    
    PolygonCacheKey key1("texture1.png", 100.0f, 100.0f);
    PolygonCacheKey key2("texture2.png", 200.0f, 150.0f, 0.7f, 2.0f);
    
    std::vector<std::vector<int>> polygons1 = {{0, 1, 2}, {3, 4, 5, 6}};
    std::vector<std::vector<int>> polygons2 = {{0, 1, 2, 3, 4}};
    
    cache.put(key1, polygons1);
    cache.put(key2, polygons2);
    
    std::vector<std::vector<int>> result;
    if (cache.get(key1, result)) {
        std::cout << "Texture1 polygons: " << result.size() << " polygons found" << std::endl;
        for (size_t i = 0; i < result.size(); ++i) {
            std::cout << "  Polygon " << i << ": " << result[i].size() << " vertices" << std::endl;
        }
    }
    
    if (cache.get(key2, result)) {
        std::cout << "Texture2 polygons: " << result.size() << " polygons found" << std::endl;
        for (size_t i = 0; i < result.size(); ++i) {
            std::cout << "  Polygon " << i << ": " << result[i].size() << " vertices" << std::endl;
        }
    }
    
    std::cout << "Polygon cache size: " << cache.size() << std::endl;
    std::cout << std::endl;
}

void CacheManagerExample::getOrCreateExample() {
    std::cout << "=== GetOrCreate Example ===" << std::endl;
    
    CacheManager<StringCacheKey, std::vector<int>, StringCacheKeyHash> cache;
    
    auto numbers1 = cache.getOrCreate(StringCacheKey("fibonacci"), []() {
        std::cout << "  Computing Fibonacci sequence..." << std::endl;
        std::vector<int> fib = {0, 1};
        for (int i = 2; i < 10; ++i) {
            fib.push_back(fib[i-1] + fib[i-2]);
        }
        return fib;
    });
    
    std::cout << "First call result: ";
    for (int num : numbers1) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    auto numbers2 = cache.getOrCreate(StringCacheKey("fibonacci"), []() {
        std::cout << "  This should not be printed!" << std::endl;
        return std::vector<int>();
    });
    
    std::cout << "Second call result (from cache): ";
    for (int num : numbers2) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Cache size: " << cache.size() << std::endl;
    std::cout << std::endl;
}

void CacheManagerExample::conditionalRemovalExample() {
    std::cout << "=== Conditional Removal Example ===" << std::endl;
    
    CacheManager<StringCacheKey, int, StringCacheKeyHash> cache;
    
    cache.put(StringCacheKey("small1"), 5);
    cache.put(StringCacheKey("small2"), 8);
    cache.put(StringCacheKey("medium1"), 50);
    cache.put(StringCacheKey("medium2"), 75);
    cache.put(StringCacheKey("large1"), 150);
    cache.put(StringCacheKey("large2"), 200);
    
    std::cout << "Initial cache size: " << cache.size() << std::endl;
    
    size_t removed = cache.removeIf([](const StringCacheKey& key, int value) {
        return value > 100;
    });
    
    std::cout << "Removed " << removed << " items with value > 100" << std::endl;
    std::cout << "Cache size after removal: " << cache.size() << std::endl;
    
    auto keys = cache.getKeys();
    std::cout << "Remaining items:" << std::endl;
    for (const auto& key : keys) {
        int value;
        if (cache.get(key, value)) {
            std::cout << "  " << key.key << " -> " << value << std::endl;
        }
    }
    
    std::cout << std::endl;
}

void CacheManagerExample::performanceComparisonExample() {
    std::cout << "=== Performance Comparison Example ===" << std::endl;
    
    const int numOperations = 100000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000);
    
    CacheManager<StringCacheKey, int, StringCacheKeyHash> cache;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numOperations; ++i) {
        std::stringstream ss;
        ss << "key_" << i;
        cache.put(StringCacheKey(ss.str()), dis(gen));
    }
    
    int found = 0;
    for (int i = 0; i < numOperations; ++i) {
        std::stringstream ss;
        ss << "key_" << i;
        int value;
        if (cache.get(StringCacheKey(ss.str()), value)) {
            found++;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "CacheManager performance:" << std::endl;
    std::cout << "  Operations: " << numOperations * 2 << " (insert + lookup)" << std::endl;
    std::cout << "  Time: " << duration.count() << " ms" << std::endl;
    std::cout << "  Items found: " << found << "/" << numOperations << std::endl;
    std::cout << "  Final cache size: " << cache.size() << std::endl;
    
    std::cout << std::endl;
}

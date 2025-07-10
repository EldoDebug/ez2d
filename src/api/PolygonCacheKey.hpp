#pragma once

#include "CacheManager.hpp"
#include <string>

struct PolygonCacheKey : public CacheKeyBase<PolygonCacheKey> {

    std::string texturePath;
    float targetWidth;
    float targetHeight;
    float alphaThreshold;
    float simplificationTolerance;
    
    PolygonCacheKey() = default;
    
    PolygonCacheKey(const std::string& path, float width, float height, 
                   float alpha = 0.5f, float tolerance = 1.0f)
        : texturePath(path), targetWidth(width), targetHeight(height),
          alphaThreshold(alpha), simplificationTolerance(tolerance) {}
    
    bool operator==(const PolygonCacheKey& other) const {
        return texturePath == other.texturePath &&
               targetWidth == other.targetWidth &&
               targetHeight == other.targetHeight &&
               alphaThreshold == other.alphaThreshold &&
               simplificationTolerance == other.simplificationTolerance;
    }
    
    bool operator!=(const PolygonCacheKey& other) const {
        return !(*this == other);
    }
    
    bool isEqual(const PolygonCacheKey& other) const {
        return *this == other;
    }
    
    std::size_t getHash() const {
        return calculateHash(texturePath, targetWidth, targetHeight,
                           alphaThreshold, simplificationTolerance);
    }
};

struct PolygonCacheKeyHash : public CacheKeyHashBase<PolygonCacheKey> {
};

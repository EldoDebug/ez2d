#pragma once

#include "Point.hpp"
#include "Size.hpp"
#include "CacheManager.hpp"
#include "PolygonCacheKey.hpp"
#include <vector>
#include <memory>

class Texture;

class PixelPerfectPolygon {
    public:
        using PolygonCache = CacheManager<PolygonCacheKey, std::vector<std::vector<Point>>, PolygonCacheKeyHash>;
        
        static std::vector<std::vector<Point>> extractPolygons(
            std::shared_ptr<Texture> texture, 
            const Size& targetSize,
            float alphaThreshold = 0.5f,
            float simplificationTolerance = 1.0f
        );
        
        static PolygonCacheKey createCacheKey(
            std::shared_ptr<Texture> texture,
            const Size& targetSize,
            float alphaThreshold,
            float simplificationTolerance
        );

    private:
        static std::vector<std::vector<float>> createAlphaMap(
            const unsigned char* pixels,
            int width,
            int height,
            int channels
        );

        static std::vector<std::vector<float>> createSignedDistanceMap(
            const std::vector<std::vector<float>>& alphaMap,
            float threshold
        );

        static std::vector<Point> marchingSquares(
            const std::vector<std::vector<float>>& alphaMap,
            int width,
            int height,
            float threshold
        );

        static std::vector<Point> simplifyPolygon(
            const std::vector<Point>& polygon,
            float tolerance
        );

        static std::vector<Point> scalePolygon(
            const std::vector<Point>& polygon,
            const Size& originalSize,
            const Size& targetSize
        );

        static std::vector<Point> validateForBox2D(const std::vector<Point>& polygon);
        static float cross(const Point& O, const Point& A, const Point& B);
        static std::vector<Point> convexHull(const std::vector<Point>& points);
        static float pointLineDistance(const Point& point, const Point& lineStart, const Point& lineEnd);
        static std::vector<std::vector<Point>> splitPolygonIntoMultiple(
            const std::vector<Point>& polygon,
            int maxVertices
        );

        static std::vector<Point> offsetPolygon(const std::vector<Point>& polygon, float offsetDistance);
        static std::vector<Point> approximateSpline(
            const std::vector<Point>& polygon,
            int subdivisions = 5
        );

        static std::vector<Point> smoothPolygon(const std::vector<Point>& polygon, int iterations = 1);
        
        static const int dx[8];
        static const int dy[8];
};

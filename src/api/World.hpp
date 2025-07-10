#pragma once
#include "Point.hpp"
#include "Rect.hpp"
#include "Size.hpp"
#include "PixelPerfectPolygon.hpp"
#include <box2d/box2d.h>
#include <vector>
#include <memory>
#include "Window.hpp"
#include <type_traits>

class Object;
class Camera;
class Size;
class Texture;

class World {
    public:
        World(Point gravity = Point(0.0f, -98.0f));
        ~World();

        void step(int subStepCount = 4);
        void setGravity(Point gravity);
        Point getGravity() const;

        std::shared_ptr<Object> createRectObject(Rect rect, bool isDynamic = true);

        template<typename T, typename... Args>
        std::shared_ptr<T> createRectObject(Rect rect, bool isDynamic = true, Args&&... args) {
            static_assert(std::is_base_of<Object, T>::value, "T must derive from Object");
            auto obj = std::make_shared<T>(this, rect, isDynamic, std::forward<Args>(args)...);
            objects.push_back(obj);
            return obj;
        }

        std::shared_ptr<Object> createCircleObject(Point position, float radius, bool isDynamic = true);

        template<typename T, typename... Args>
        std::shared_ptr<T> createCircleObject(Point position, float radius, bool isDynamic = true, Args&&... args) {
            static_assert(std::is_base_of<Object, T>::value, "T must derive from Object");
            auto obj = std::make_shared<T>(this, position, radius, isDynamic, std::forward<Args>(args)...);
            objects.push_back(obj);
            return obj;
        }

        std::shared_ptr<Object> createRoundedRectObject(Rect rect, float cornerRadius, bool isDynamic = true);

        template<typename T, typename... Args>
        std::shared_ptr<T> createRoundedRectObject(Rect rect, float cornerRadius, bool isDynamic = true, Args&&... args) {
            static_assert(std::is_base_of<Object, T>::value, "T must derive from Object");
            auto obj = std::make_shared<T>(this, rect, cornerRadius, isDynamic, std::forward<Args>(args)...);
            objects.push_back(obj);
            return obj;
        }

        std::shared_ptr<Object> createTriangleObject(Point point1, Point point2, Point point3, bool isDynamic = true);

        template<typename T, typename... Args>
        std::shared_ptr<T> createTriangleObject(Point point1, Point point2, Point point3, bool isDynamic = true, Args&&... args) {
            static_assert(std::is_base_of<Object, T>::value, "T must derive from Object");
            auto obj = std::make_shared<T>(this, point1, point2, point3, isDynamic, std::forward<Args>(args)...);
            objects.push_back(obj);
            return obj;
        }

        template<typename T, typename... Args>
        std::shared_ptr<T> createObject(Args&&... args) {
            static_assert(std::is_base_of<Object, T>::value, "T must derive from Object");
            auto obj = std::make_shared<T>(this, std::forward<Args>(args)...);
            objects.push_back(obj);
            return obj;
        }

        std::vector<std::shared_ptr<Object>> createPixelPerfectObjects(std::shared_ptr<Texture> texture, Rect rect, bool isDynamic = true);
        
        void preloadPixelPerfectPolygons(
            std::shared_ptr<Texture> texture, 
            const Size& targetSize,
            float alphaThreshold = 0.5f,
            float simplificationTolerance = 1.0f
        );
        
        void preloadPixelPerfectPolygons(
            std::shared_ptr<Texture> texture,
            const std::vector<Size>& targetSizes,
            float alphaThreshold = 0.5f,
            float simplificationTolerance = 1.0f
        );
        
        bool hasPolygonInCache(
            std::shared_ptr<Texture> texture,
            const Size& targetSize,
            float alphaThreshold = 0.5f,
            float simplificationTolerance = 1.0f
        ) const;
        
        std::vector<std::vector<Point>> extractPolygonsWithCache(
            std::shared_ptr<Texture> texture,
            const Size& targetSize,
            float alphaThreshold = 0.5f,
            float simplificationTolerance = 1.0f
        );
        
        void destroyObject(std::shared_ptr<Object> object);
        void drawAll();
        
        void clearPolygonCache();
        size_t getPolygonCacheSize() const;

        b2WorldId getWorldId() const { return worldId; }

    private:
        b2WorldId worldId;
        std::vector<std::shared_ptr<Object>> objects;
        mutable PixelPerfectPolygon::PolygonCache polygonCache;
        
        friend class Object;
};
#include "World.hpp"
#include "Object.hpp"
#include "PixelPerfectPolygon.hpp"
#include "../Window.hpp"
#include "../Camera.hpp"
#include "../Texture.hpp"
#include "Rect.hpp"
#include "Size.hpp"
#include <algorithm>

World::World(Point gravity) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {gravity.x, gravity.y};
    worldId = b2CreateWorld(&worldDef);
}

World::~World() {
    objects.clear();
    polygonCache.clear();
    if (B2_IS_NON_NULL(worldId)) {
        b2DestroyWorld(worldId);
    }
}

void World::step(int subStepCount) {
    float deltaTime = Window::getDeltaTime() / 1000.0f;
    b2World_Step(worldId, deltaTime, subStepCount);
}

void World::setGravity(Point gravity) {
    b2World_SetGravity(worldId, {gravity.x, gravity.y});
}

Point World::getGravity() const {
    b2Vec2 gravity = b2World_GetGravity(worldId);
    return Point(gravity.x, gravity.y);
}

std::shared_ptr<Object> World::createRectObject(Rect rect, bool isDynamic) {
    auto object = std::make_shared<Object>(this, Object::Type::Rect, rect, isDynamic);
    objects.push_back(object);
    return object;
}

std::shared_ptr<Object> World::createCircleObject(Point position, float radius, bool isDynamic) {
    auto object = std::make_shared<Object>(this, Object::Type::Circle, position, radius, isDynamic);
    objects.push_back(object);
    return object;
}

std::shared_ptr<Object> World::createRoundedRectObject(Rect rect, float cornerRadius, bool isDynamic) {
    auto object = std::make_shared<Object>(this, Object::Type::RoundedRect, rect, isDynamic);
    object->setCornerRadius(cornerRadius);
    objects.push_back(object);
    return object;
}

std::shared_ptr<Object> World::createTriangleObject(Point point1, Point point2, Point point3, bool isDynamic) {
    auto object = std::make_shared<Object>(this, Object::Type::Triangle, point1, point2, point3, isDynamic);
    objects.push_back(object);
    return object;
}

std::vector<std::shared_ptr<Object>> World::createPixelPerfectObjects(std::shared_ptr<Texture> texture, Rect rect, bool isDynamic) {
    
    std::vector<std::shared_ptr<Object>> result;
    
    Size size(rect.width, rect.height);
    Point position = rect.getCenter();
    
    auto polygons = extractPolygonsWithCache(texture, size);
    
    for (const auto& polygon : polygons) {
        if (polygon.size() >= 3) {

            auto object = std::make_shared<Object>(this, Object::Type::PixelPerfect, rect, isDynamic);
            
            object->pixelPerfectVertices = polygon;
            object->setTexture(texture);
            
            if (B2_IS_NON_NULL(object->bodyId)) {
                b2DestroyBody(object->bodyId);
            }

            object->createBody(position, isDynamic);
            object->createFixture();
            
            objects.push_back(object);
            result.push_back(object);
        }
    }
    
    return result;
}

void World::destroyObject(std::shared_ptr<Object> object) {
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [object](const std::shared_ptr<Object>& obj) {
                return obj == object;
            }),
        objects.end()
    );
}

void World::drawAll() {
    for (const auto& object : objects) {
        object->draw();
    }
}

void World::clearPolygonCache() {
    polygonCache.clear();
}

size_t World::getPolygonCacheSize() const {
    return polygonCache.size();
}

void World::preloadPixelPerfectPolygons(
    std::shared_ptr<Texture> texture, 
    const Size& targetSize,
    float alphaThreshold,
    float simplificationTolerance) {

    if (!texture) return;

    if (!hasPolygonInCache(texture, targetSize, alphaThreshold, simplificationTolerance)) {
        extractPolygonsWithCache(texture, targetSize, alphaThreshold, simplificationTolerance);
    }
}

void World::preloadPixelPerfectPolygons(
    std::shared_ptr<Texture> texture,
    const std::vector<Size>& targetSizes,
    float alphaThreshold,
    float simplificationTolerance) {
    
    if (!texture) return;
    
    for (const auto& size : targetSizes) {
        preloadPixelPerfectPolygons(texture, size, alphaThreshold, simplificationTolerance);
    }
}

bool World::hasPolygonInCache(
    std::shared_ptr<Texture> texture,
    const Size& targetSize,
    float alphaThreshold,
    float simplificationTolerance) const {
    
    if (!texture) return false;
    
    auto key = PixelPerfectPolygon::createCacheKey(
        texture, targetSize, alphaThreshold, simplificationTolerance
    );
    
    return polygonCache.contains(key);
}

std::vector<std::vector<Point>> World::extractPolygonsWithCache(
    std::shared_ptr<Texture> texture,
    const Size& targetSize,
    float alphaThreshold,
    float simplificationTolerance) {
    
    if (!texture || !texture->getPixelData()) {
        return {};
    }

    auto key = PixelPerfectPolygon::createCacheKey(
        texture, targetSize, alphaThreshold, simplificationTolerance
    );
    
    std::vector<std::vector<Point>> result;
    if (polygonCache.get(key, result)) {
        return result;
    }

    auto polygons = PixelPerfectPolygon::extractPolygons(
        texture, targetSize, alphaThreshold, simplificationTolerance
    );
    polygonCache.put(key, polygons);
    
    return polygons;
}

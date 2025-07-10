#include "Object.hpp"
#include "Rect.hpp"
#include "World.hpp"
#include "PixelPerfectPolygon.hpp"
#include "../Renderer.hpp"
#include "../Texture.hpp"
#include "../Sprite.hpp"
#include "../SpriteAnimation.hpp"
#include "../Camera.hpp"
#include "Size.hpp"
#include "box2d/box2d.h"
#include <numbers>
#include <cmath>
#include <algorithm>

Object::Object(World* world, Object::Type type, Rect rect, bool isDynamic, bool rotatable)
    : world(world), type(type), color(Color(255, 255, 255, 255)), cornerRadius(0.0f), 
      width(rect.width), height(rect.height), radius(0.0f), texture(nullptr), sprite(nullptr), spriteAnimation(nullptr), rotatable(rotatable) {
    id = UUID::randomUUID();
    createBody(rect.toPoint(), isDynamic);
    createFixture();
}

Object::Object(World* world, Object::Type type, Point position, float radius, bool isDynamic, bool rotatable)
    : world(world), type(type), color(Color(255, 255, 255, 255)), cornerRadius(0.0f),
      width(radius * 2), height(radius * 2), radius(radius), texture(nullptr), sprite(nullptr), spriteAnimation(nullptr), rotatable(rotatable) {
    id = UUID::randomUUID();
    createBody(position, isDynamic);
    createFixture();
}

Object::Object(World* world, Object::Type type, Point point1, Point point2, Point point3, bool isDynamic, bool rotatable)
    : world(world), type(type), color(Color(255, 255, 255, 255)), cornerRadius(0.0f),
      trianglePoint1(point1), trianglePoint2(point2), trianglePoint3(point3),
      texture(nullptr), sprite(nullptr), spriteAnimation(nullptr), rotatable(rotatable) {
    id = UUID::randomUUID();
    
    Point center((point1.x + point2.x + point3.x) / 3.0f, (point1.y + point2.y + point3.y) / 3.0f);
    float minX = std::min({point1.x, point2.x, point3.x});
    float maxX = std::max({point1.x, point2.x, point3.x});
    float minY = std::min({point1.y, point2.y, point3.y});
    float maxY = std::max({point1.y, point2.y, point3.y});
    width = maxX - minX;
    height = maxY - minY;
    radius = 0.0f;
    
    createBody(center, isDynamic);
    createFixture();
}

Object::Object(World* world, std::shared_ptr<Texture> texture, const Size& size, Point position, bool isDynamic, bool rotatable)
    : world(world), type(Object::Type::PixelPerfect), color(Color(255, 255, 255, 255)), cornerRadius(0.0f),
      width(size.width), height(size.height), radius(0.0f), texture(texture), sprite(nullptr), spriteAnimation(nullptr), rotatable(rotatable) {
    id = UUID::randomUUID();
    
    auto polygons = PixelPerfectPolygon::extractPolygons(texture, size);
    if (!polygons.empty()) {
        pixelPerfectVertices = polygons[0];
    }
    
    createBody(position, isDynamic);
    createFixture();
}

Object::~Object() {
    if (B2_IS_NON_NULL(bodyId)) {
        b2DestroyBody(bodyId);
    }
}

void Object::createBody(Point position, bool isDynamic) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
    bodyDef.position = {position.x, position.y};
    bodyDef.fixedRotation = !rotatable;
    bodyId = b2CreateBody(world->getWorldId(), &bodyDef);
}

void Object::createFixture() {
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;

    switch (type) {
        case Object::Type::Rect:
        case Object::Type::RoundedRect: {
            b2Polygon box = b2MakeBox(width / 2.0f, height / 2.0f);
            shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);
            break;
        }
        case Object::Type::Circle: {
            b2Circle circle = {{0.0f, 0.0f}, radius};
            shapeId = b2CreateCircleShape(bodyId, &shapeDef, &circle);
            break;
        }
        case Object::Type::Triangle: {

            Point center((trianglePoint1.x + trianglePoint2.x + trianglePoint3.x) / 3.0f,
                        (trianglePoint1.y + trianglePoint2.y + trianglePoint3.y) / 3.0f);
            
            b2Vec2 vertices[3] = {
                {trianglePoint1.x - center.x, trianglePoint1.y - center.y},
                {trianglePoint2.x - center.x, trianglePoint2.y - center.y},
                {trianglePoint3.x - center.x, trianglePoint3.y - center.y}
            };
            b2Hull hull = b2ComputeHull(vertices, 3);
            b2Polygon triangle = b2MakePolygon(&hull, 0.0f);
            shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &triangle);
            break;
        }
        case Object::Type::PixelPerfect: {
            if (!pixelPerfectVertices.empty() && pixelPerfectVertices.size() <= 8) {
                std::vector<b2Vec2> vertices;
                vertices.reserve(pixelPerfectVertices.size());
                
                for (const auto& vertex : pixelPerfectVertices) {
                    vertices.push_back({vertex.x, vertex.y});
                }
                
                b2Hull hull = b2ComputeHull(vertices.data(), static_cast<int>(vertices.size()));
                b2Polygon polygon = b2MakePolygon(&hull, 0.0f);
                shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &polygon);
            } else {
                b2Polygon box = b2MakeBox(width / 2.0f, height / 2.0f);
                shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);
            }
            break;
        }
    }
}

void Object::setPosition(Point position) {
    b2Body_SetTransform(bodyId, {position.x, position.y}, b2Body_GetRotation(bodyId));
}

Point Object::getPosition() const {
    b2Vec2 pos = b2Body_GetPosition(bodyId);
    return Point(pos.x, pos.y);
}

void Object::setSize(float width, float height) {
    this->width = width;
    this->height = height;
    if (type != Object::Type::Circle) {
        createFixture();
    }
}

void Object::setRadius(float radius) {
    this->radius = radius;
    this->width = radius * 2;
    this->height = radius * 2;
    if (type == Object::Type::Circle) {
        createFixture();
    }
}

float Object::getWidth() const {
    return width;
}

float Object::getHeight() const {
    return height;
}

float Object::getRadius() const {
    return radius;
}

void Object::setTrianglePoints(Point point1, Point point2, Point point3) {
    trianglePoint1 = point1;
    trianglePoint2 = point2;
    trianglePoint3 = point3;
    
    float minX = std::min({point1.x, point2.x, point3.x});
    float maxX = std::max({point1.x, point2.x, point3.x});
    float minY = std::min({point1.y, point2.y, point3.y});
    float maxY = std::max({point1.y, point2.y, point3.y});
    width = maxX - minX;
    height = maxY - minY;
    
    if (type == Object::Type::Triangle) {
        createFixture();
    }
}

Point Object::getTrianglePoint1() const {
    return trianglePoint1;
}

Point Object::getTrianglePoint2() const {
    return trianglePoint2;
}

Point Object::getTrianglePoint3() const {
    return trianglePoint3;
}

void Object::setDensity(float density) {
    b2Shape_SetDensity(shapeId, density, true);
}

void Object::setFriction(float friction) {
    b2Shape_SetFriction(shapeId, friction);
}

void Object::setRestitution(float restitution) {
    b2Shape_SetRestitution(shapeId, restitution);
}

void Object::setLinearVelocity(Point velocity) {
    b2Body_SetLinearVelocity(bodyId, {velocity.x, velocity.y});
}

Point Object::getLinearVelocity() const {
    b2Vec2 vel = b2Body_GetLinearVelocity(bodyId);
    return Point(vel.x, vel.y);
}

void Object::setAngularVelocity(float velocity) {
    b2Body_SetAngularVelocity(bodyId, velocity);
}

float Object::getAngularVelocity() const {
    return b2Body_GetAngularVelocity(bodyId);
}

void Object::applyForce(Point force) {
    b2Vec2 center = b2Body_GetWorldCenterOfMass(bodyId);
    b2Body_ApplyForce(bodyId, {force.x, force.y}, center, true);
}

void Object::applyImpulse(Point impulse) {
    b2Vec2 center = b2Body_GetWorldCenterOfMass(bodyId);
    b2Body_ApplyLinearImpulse(bodyId, {impulse.x, impulse.y}, center, true);
}

void Object::setAngle(float angle) {
    b2Vec2 pos = b2Body_GetPosition(bodyId);
    b2Rot rot = b2MakeRot(angle * (std::numbers::pi_v<float> / 180.0f));
    b2Transform transform = {pos, rot};
    b2Body_SetTransform(bodyId, transform.p, transform.q);
}

float Object::getAngle() const {
    b2Rot rot = b2Body_GetRotation(bodyId);
    return b2Rot_GetAngle(rot) * (180.0f / std::numbers::pi_v<float>);
}

void Object::setColor(Color color) {
    this->color = color;
}

Color Object::getColor() const {
    return color;
}

void Object::setCornerRadius(float radius) {
    this->cornerRadius = radius;
}

float Object::getCornerRadius() const {
    return cornerRadius;
}

bool Object::isDynamic() const {
    return b2Body_GetType(bodyId) == b2_dynamicBody;
}

void Object::setDynamic(bool dynamic) {
    b2Body_SetType(bodyId, dynamic ? b2_dynamicBody : b2_staticBody);
}

void Object::setTexture(std::shared_ptr<Texture> texture) {
    this->texture = texture;
    this->sprite = nullptr;
    this->spriteAnimation = nullptr;
}

void Object::setSprite(std::shared_ptr<Sprite> sprite, int index) {
    this->sprite = sprite;
    this->texture = nullptr;
    this->spriteAnimation = nullptr;
    this->spriteIndex = index;
}

void Object::setSpriteAnimation(std::shared_ptr<SpriteAnimation> spriteAnimation) {
    this->spriteAnimation = spriteAnimation;
    this->texture = nullptr;
    this->sprite = nullptr;
}

void Object::clearTexture() {
    texture = nullptr;
}

void Object::clearSprite() {
    sprite = nullptr;
}

void Object::clearSpriteAnimation() {
    spriteAnimation = nullptr;
}

bool Object::hasTexture() const {
    return texture != nullptr;
}

bool Object::hasSprite() const {
    return sprite != nullptr;
}

bool Object::hasSpriteAnimation() const {
    return spriteAnimation != nullptr;
}

void Object::updateAnimation() {
    if (spriteAnimation) {
        spriteAnimation->update();
    }
}

void Object::draw() {
    Point position = getPosition();
    float angle = getAngle();
    
    Renderer::save();
    
    switch (type) {
        case Object::Type::Rect: {
            
            Rect rect(position.x - width / 2, position.y - height / 2, width, height);
            
            if (angle != 0.0f) {
                Renderer::rotate(Point(position.x, position.y), angle);
            }
            
            if (spriteAnimation) {
                spriteAnimation->draw(rect);
            } else if (sprite) {
                Renderer::drawSprite(sprite, rect, spriteIndex);
            } else if (texture) {
                Renderer::drawTexture(texture, rect);
            } else {
                Renderer::drawRect(rect, color);
            }
            break;
        }
        case Object::Type::Circle: {
            if (texture) {
                Renderer::drawCircleTexture(texture, position, radius);
            } else {
                Renderer::drawCircle(position, radius, color);
            }
            break;
        }
        case Object::Type::RoundedRect: {

            Rect rect(position.x - width/2, position.y - height / 2, width, height);

            if (angle != 0.0f) {
                Renderer::rotate(Point(position.x, position.y), angle);
            }
            
            if (texture) {
                Renderer::drawRoundedTexture(texture, rect, cornerRadius);
            } else {
                Renderer::drawRoundedRect(rect, cornerRadius, color);
            }
            break;
        }
        case Object::Type::Triangle: {
            
            if (angle != 0.0f) {
                Renderer::rotate(Point(position.x, position.y), angle);
            }
            
            Point originalCenter((trianglePoint1.x + trianglePoint2.x + trianglePoint3.x) / 3.0f,
                                (trianglePoint1.y + trianglePoint2.y + trianglePoint3.y) / 3.0f);
            
            Point offset = Point(position.x - originalCenter.x, position.y - originalCenter.y);
            Point worldPoint1(trianglePoint1.x + offset.x, trianglePoint1.y + offset.y);
            Point worldPoint2(trianglePoint2.x + offset.x, trianglePoint2.y + offset.y);
            Point worldPoint3(trianglePoint3.x + offset.x, trianglePoint3.y + offset.y);
            
            if (texture) {
                Renderer::drawTexture(texture, Rect(worldPoint1.x, worldPoint1.y, width, height), 1.0f);
            } else if (spriteAnimation) {
                spriteAnimation->draw(Rect(worldPoint1.x, worldPoint1.y, width, height));
            } else if (sprite) {
                Renderer::drawSprite(sprite, Rect(worldPoint1.x, worldPoint1.y, width, height), spriteIndex);
            } else{
                Renderer::drawTriangle(worldPoint1, worldPoint2, worldPoint3, color);
            }
            break;
        }
        case Object::Type::PixelPerfect: {
            Rect rect(position.x - width / 2, position.y - height / 2, width, height);
            
            if (angle != 0.0f) {
                Renderer::rotate(Point(position.x, position.y), angle);
            }
            
            if (texture) {
                Renderer::drawTexture(texture, rect);
            } else {
                Renderer::drawRect(rect, color);
            }
            break;
        }
    }
    
    Renderer::restore();
}

void Object::setRotatable(bool rot)
{
    rotatable = rot;
    b2Body_SetFixedRotation(bodyId, !rot);
}

bool Object::isRotatable() const
{
    return rotatable;
}
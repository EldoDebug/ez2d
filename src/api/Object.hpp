#pragma once
#include "Point.hpp"
#include "Rect.hpp"
#include "Color.hpp"
#include "UUID.hpp"
#include "Size.hpp"
#include <box2d/box2d.h>
#include <memory>
#include <vector>

class World;
class Texture;
class Sprite;
class SpriteAnimation;
class Camera;

class Object {
    public:
        enum class Type {
            Rect,
            Circle,
            RoundedRect,
            Triangle,
            PixelPerfect
        };

        UUID id;
        
        Object(World* world, Object::Type type, Rect rect, bool isDynamic = true);
        Object(World* world, Object::Type type, Point position, float radius, bool isDynamic = true);
        Object(World* world, Object::Type type, Point point1, Point point2, Point point3, bool isDynamic = true);
        Object(World* world, std::shared_ptr<Texture> texture, const Size& size, Point position = Point(0, 0), bool isDynamic = true);
        ~Object();

        void setPosition(Point position);
        Point getPosition() const;
        void setSize(float width, float height);
        void setRadius(float radius);
        void setTrianglePoints(Point point1, Point point2, Point point3);
        Point getTrianglePoint1() const;
        Point getTrianglePoint2() const;
        Point getTrianglePoint3() const;
        float getWidth() const;
        float getHeight() const;
        float getRadius() const;

        void setDensity(float density);
        void setFriction(float friction);
        void setRestitution(float restitution);
        void setLinearVelocity(Point velocity);
        Point getLinearVelocity() const;
        void setAngularVelocity(float velocity);
        float getAngularVelocity() const;
        void applyForce(Point force);
        void applyImpulse(Point impulse);

        void setAngle(float angle);
        float getAngle() const;

        void setColor(Color color);
        Color getColor() const;
        void setCornerRadius(float radius);
        float getCornerRadius() const;

        void setTexture(std::shared_ptr<Texture> texture);
        void setSprite(std::shared_ptr<Sprite> sprite, int index = 0);
        void setSpriteAnimation(std::shared_ptr<SpriteAnimation> spriteAnimation);
        void clearTexture();
        void clearSprite();
        void clearSpriteAnimation();
        bool hasTexture() const;
        bool hasSprite() const;
        bool hasSpriteAnimation() const;
        
        void updateAnimation();

        void draw();
        bool isDynamic() const;
        void setDynamic(bool dynamic);
        
    private:
        World* world;
        b2BodyId bodyId;
        b2ShapeId shapeId;
        Object::Type type;
        
        Color color;
        float cornerRadius;
        float width, height, radius;
        Point trianglePoint1, trianglePoint2, trianglePoint3;
        
        std::vector<Point> pixelPerfectVertices;
        
        std::shared_ptr<Texture> texture;
        std::shared_ptr<Sprite> sprite;
        int spriteIndex;
        std::shared_ptr<SpriteAnimation> spriteAnimation;
        
        void createBody(Point position, bool isDynamic);
        void createFixture();
        
        friend class World;
};
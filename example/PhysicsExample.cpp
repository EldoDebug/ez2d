#include "PhysicsExample.hpp"
#include "Mouse.hpp"
#include "Window.hpp"
#include "TextureManager.hpp"
#include "api/Color.hpp"
#include "api/Point.hpp"
#include "api/Rect.hpp"
#include "api/Size.hpp"
#include "api/World.hpp"
#include "api/Object.hpp"
#include <memory>

void PhysicsExample::run() {

    Window::Config config;

    config.title = "Physics Example";
    config.size = Size(1280, 720);
    config.resizable = true;

    Window::create(config);
    Window::init();
    Window::setScene<PhysicsExample::PhysicsScene>();
    Window::show();
    Window::shutdown();
}

void PhysicsExample::PhysicsScene::onInit() {

    world = std::make_unique<World>(Point(0.0F, 198.0F));

    TextureManager::load("egg", "assets/egg.png");
    
    auto eggTexture = TextureManager::get("egg");
    if (eggTexture) {
        std::vector<Size> sizes = {
            Size(40.0f, 40.0f),
            Size(60.0f, 60.0f),
            Size(80.0f, 80.0f),
            Size(100.0f, 100.0f)
        };
        
        world->preloadPixelPerfectPolygons(eggTexture, sizes);
        
        printf("Preloaded %zu polygon sets in cache\n", world->getPolygonCacheSize());
    }

    ground = world->createRectObject(Rect(Window::getWidth() / 2.0F, Window::getHeight() - 15.0F, (float) Window::getWidth(), 30.0F), false);
    ground->setColor(Color(255, 0, 0));
}

void PhysicsExample::PhysicsScene::onRender() {
    if(world) {
        world->drawAll();
    }
}

void PhysicsExample::PhysicsScene::onUpdate() {
    if(world) {
        world->step();
    }
}

void PhysicsExample::PhysicsScene::onMousePressed(Point point, int button) {
    if(world && button == Mouse::Left) {
        auto circle = world->createCircleObject(point, 20.0F, true);
        circle->setColor(Color(0, 255, 0));
        circle->setRestitution(0.5F);
        circle->setDensity(1.0F);
    }
    else if(world && button == Mouse::Right) {
        Point point1(point.x - 35.0F, point.y + 35.0F);
        Point point2(point.x + 35.0F, point.y + 35.0F);
        Point point3(point.x, point.y - 15.0F);
        
        auto triangle = world->createTriangleObject(point1, point2, point3, true);
        triangle->setColor(Color(0, 0, 255));
        triangle->setRestitution(0.3F);
        triangle->setDensity(0.8F);
    }
    else if(world && button == Mouse::Middle) {
        auto texture = TextureManager::get("egg");
        if (texture) {
            float windowWidth = static_cast<float>(Window::getWidth());
            float relativeX = point.x / windowWidth;
            
            Size objectSize;
            if (relativeX < 0.25f) {
                objectSize = Size(40.0f, 40.0f);
            } else if (relativeX < 0.5f) {
                objectSize = Size(60.0f, 60.0f);
            } else if (relativeX < 0.75f) {
                objectSize = Size(80.0f, 80.0f);
            } else {
                objectSize = Size(100.0f, 100.0f);
            }
            
            Rect rect(point.x - objectSize.width/2, point.y - objectSize.height/2, objectSize.width, objectSize.height);
            
            if (world->hasPolygonInCache(texture, objectSize)) {
                printf("Using cached polygon for size %.0fx%.0f\n", objectSize.width, objectSize.height);
            }
            
            auto pixelPerfectObjects = world->createPixelPerfectObjects(texture, rect, true);
            
            for (auto& obj : pixelPerfectObjects) {
                obj->setColor(Color(255, 255, 0));
                obj->setRestitution(0.3F);
                obj->setDensity(0.8F);
                obj->setFriction(0.5F);
            }
        }
    }
}
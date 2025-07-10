#pragma once
#include "Texture.hpp"
#include "nanovg.h"
#include <memory>
#include "api/Point.hpp"
#include "api/Rect.hpp"
#include "api/Size.hpp"

class Color;
class Texture;
class Sprite;
class Font;
class Point;
class Rect;
class Size;

class Renderer {
    public:
        static inline NVGcontext* context = nullptr;
        
        static void init();

        static void drawLine(Point point1, Point point2, float strokeWidth, Color color);
        static void drawRect(Rect rect, Color color);
        static void drawRoundedRect(Rect rect, float radius, Color color);
        static void drawOutlineRect(Rect rect, float outlineWidth, Color color);
        static void drawRoundedOutlineRect(Rect rect, float radius, float outlineWidth, Color color);
        static void drawCircle(Point point, float radius, Color color);
        static void drawArc(Point point, float radius, float startAngle, float endAngle, float strokeWidth, Color color);
        static void drawTriangle(Point point1, Point point2, Point point3, Color color);
        static void drawPolygon(const std::vector<Point>& points, Color color);

        static void drawTexture(std::shared_ptr<Texture> texture, Rect rect, float alpha = 1.0F);
        static void drawTexture(int texture, Rect rect, float alpha = 1.0F);
        static void drawRoundedTexture(std::shared_ptr<Texture> texture, Rect rect, float radius, float alpha = 1.0F);
        static void drawCircleTexture(std::shared_ptr<Texture> texture, Point point, float radius, float alpha = 1.0F);
        static void drawSprite(std::shared_ptr<class Sprite> sprite, Rect rect, int index);

        static void drawText(std::string text, Point point, std::shared_ptr<Font> font, Color color, float size);
        static void drawCenteredText(std::string text, Point point, std::shared_ptr<Font> font, Color color, float size);

        static Size getTextSize(std::string text, std::shared_ptr<Font> font, float size);
        static float getTextWidth(std::string text, std::shared_ptr<Font> font, float size);
        static float getTextHeight(std::string text, std::shared_ptr<Font> font, float size);

        static void clear(Size size, Color color);
        
        static void translate(Point point);
        static void scale(Point point, float scale);
        static void scale(Rect rect, float scale);
        static void rotate(Point point, float angle);
        static void rotate(Rect rect, float angle);
        static void scaleAndRotate(Point point, float scale, float angle);
        static void scaleAndRotate(Rect rect, float scale, float angle);
        static void save();
        static void restore();

        static void scisor(Rect rect);
        static void resetScisor();

        static void shutdown();
};
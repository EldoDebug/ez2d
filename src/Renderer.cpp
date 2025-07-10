#include "Renderer.hpp"
#include "Texture.hpp"
#include "api/Point.hpp"
#include "api/Rect.hpp"
#include "nanovg.h"
#include "nanovg_bgfx.h"
#include "api/Color.hpp"
#include <memory>
#include <numbers>
#include <cmath>
#include "Sprite.hpp"
#include "Font.hpp"

void Renderer::init() {
    context = nvgCreate(0, 0);
}

void Renderer::drawLine(Point point1, Point point2, float strokeWidth, Color color) {
    nvgBeginPath(context);
    nvgMoveTo(context, point1.x, point1.y);
    nvgLineTo(context, point2.x, point2.y);
    nvgStrokeColor(context, color.toNVGColor());
    nvgStrokeWidth(context, strokeWidth);
    nvgStroke(context);
}

void Renderer::drawRect(Rect rect, Color color) {
    nvgBeginPath(context);
    nvgRect(context, rect.x, rect.y, rect.width, rect.height);
    nvgClosePath(context);
    nvgFillColor(context, color.toNVGColor());
    nvgFill(context);
}

void Renderer::drawRoundedRect(Rect rect, float radius, Color color) {
    nvgBeginPath(context);
    nvgRoundedRect(context, rect.x, rect.y, rect.width, rect.height, radius);
    nvgFillColor(context, color.toNVGColor());
    nvgFill(context);
}

void Renderer::drawOutlineRect(Rect rect, float outlineWidth, Color color) {
    nvgBeginPath(context);
    nvgRect(context, rect.x, rect.y, rect.width, rect.height);
    nvgStrokeColor(context, color.toNVGColor());
    nvgStrokeWidth(context, outlineWidth);
    nvgStroke(context);
}

void Renderer::drawRoundedOutlineRect(Rect rect, float radius, float outlineWidth, Color color) {
    nvgBeginPath(context);
    nvgRoundedRect(context, rect.x, rect.y, rect.width, rect.height, radius);
    nvgStrokeColor(context, color.toNVGColor());
    nvgStrokeWidth(context, outlineWidth);
    nvgStroke(context);
}

void Renderer::drawCircle(Point point, float radius, Color color) {
    nvgBeginPath(context);
    nvgCircle(context, point.x, point.y, radius);
    nvgFillColor(context, color.toNVGColor());
    nvgFill(context);
}

void Renderer::drawArc(Point point, float radius, float startAngle, float endAngle, float strokeWidth, Color color) {
    float degreesToRadians = (std::numbers::pi_v<float> / 180.0F);
    nvgBeginPath(context);
    nvgArc(context, point.x, point.y, radius, ((startAngle - 90) * degreesToRadians), ((endAngle - 90) * degreesToRadians), NVG_CW);
	nvgStrokeWidth(context, strokeWidth);
	nvgStrokeColor(context, color.toNVGColor());
	nvgStroke(context);
}

void Renderer::drawTriangle(Point point1, Point point2, Point point3, Color color) {
    nvgBeginPath(context);
    nvgMoveTo(context, point1.x, point1.y);
    nvgLineTo(context, point2.x, point2.y);
    nvgLineTo(context, point3.x, point3.y);
    nvgClosePath(context);
    nvgFillColor(context, color.toNVGColor());
    nvgFill(context);
}

void Renderer::drawPolygon(const std::vector<Point>& points, Color color) {
    
    if (points.size() < 3) {
        return;
    }

    nvgBeginPath(context);
    nvgMoveTo(context, points[0].x, points[0].y);

    for (size_t i = 1; i < points.size(); ++i) {
        nvgLineTo(context, points[i].x, points[i].y);
    }

    nvgClosePath(context);
    nvgFillColor(context, color.toNVGColor());
    nvgFill(context);
}

void Renderer::drawTexture(std::shared_ptr<Texture> texture, Rect rect, float alpha) {

    if(!texture || texture->handle == 0) {
        return;
    }

    nvgBeginPath(context);
    nvgRect(context, rect.x, rect.y, rect.width, rect.height);
    nvgFillPaint(context, nvgImagePattern(context, rect.x, rect.y, rect.width, rect.height, 0.0f, texture->handle, alpha));
    nvgFill(context);
}

void Renderer::drawTexture(int textureHandle, Rect rect, float alpha) {

    if(textureHandle == 0) {
        return;
    }

    nvgBeginPath(context);
    nvgRect(context, rect.x, rect.y, rect.width, rect.height);
    nvgFillPaint(context, nvgImagePattern(context, rect.x, rect.y, rect.width, rect.height, 0.0f, textureHandle, alpha));
    nvgFill(context);
}

void Renderer::drawRoundedTexture(std::shared_ptr<Texture> texture, Rect rect, float radius, float alpha) {

    if(!texture || texture->handle == 0) {
        return;
    }

    nvgBeginPath(context);
    nvgRoundedRect(context, rect.x, rect.y, rect.width, rect.height, radius);
    nvgFillPaint(context, nvgImagePattern(context, rect.x, rect.y, rect.width, rect.height, 0.0f, texture->handle, alpha));
    nvgFill(context);
}

void Renderer::drawCircleTexture(std::shared_ptr<Texture> texture, Point point, float radius, float alpha) {

    if(!texture || texture->handle == 0) {
        return;
    }

    nvgBeginPath(context);
    nvgCircle(context, point.x, point.y, radius);
    nvgFillPaint(context, nvgImagePattern(context, point.x - radius, point.y - radius, radius * 2.0f, radius * 2.0f, 0.0f, texture->handle, alpha));
    nvgFill(context);
}

void Renderer::drawSprite(std::shared_ptr<Sprite> sprite, Rect rect, int index) {

    if(!sprite || sprite->texture->handle == 0) {
        return;
    }

    Size textureSize = sprite->texture->size;
    Size spriteSize = sprite->size;
    
    float columnsPerRow = textureSize.width / textureSize.width;

    float hx = (index % (int) columnsPerRow) * spriteSize.width;
    float hy = static_cast<float>(std::floor(index / columnsPerRow)) * spriteSize.height;

    float scaleRatio = rect.height / spriteSize.height;

    nvgBeginPath(context);
    nvgRect(context, rect.x, rect.y, rect.width, rect.height);
    nvgFillPaint(context, nvgImagePattern(context, rect.x + -hx * scaleRatio, rect.y + -hy * scaleRatio, 
        textureSize.width * scaleRatio, textureSize.height * scaleRatio, 0.0f, sprite->texture->handle, 1.0f));
    nvgFill(context);
}

void Renderer::drawText(std::string text, Point point, std::shared_ptr<Font> font, Color color, float size) {

    nvgFontSize(context, size);
    nvgFontFaceId(context, font->handle);
    nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
    nvgFillColor(context, color.toNVGColor());

    float ascender, descender, lineh;

    nvgTextMetrics(context, &ascender, &descender, &lineh);
    nvgText(context, point.x, point.y + descender, text.c_str(), nullptr);
}

void Renderer::drawCenteredText(std::string text, Point point, std::shared_ptr<Font> font, Color color, float size) {

    nvgFontSize(context, size);
    nvgFontFaceId(context, font->handle);
    nvgTextAlign(context, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
    nvgFillColor(context, color.toNVGColor());

    float ascender, descender, lineh;

    nvgTextMetrics(context, &ascender, &descender, &lineh);
    nvgText(context, point.x, point.y + (lineh / 2.0f), text.c_str(), nullptr);
}

Size Renderer::getTextSize(std::string text, std::shared_ptr<Font> font, float size) {

    float bounds[4];

    nvgFontSize(context, size);
    nvgFontFaceId(context, font->handle);
    nvgTextBounds(context, 0.0f, 0.0f, text.c_str(), nullptr, bounds);
    nvgTextAlign(context, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

    return Size(bounds[2] - bounds[0], bounds[3] - bounds[1]);
}

float Renderer::getTextWidth(std::string text, std::shared_ptr<Font> font, float size) {
    return getTextSize(text, font, size).width;
}

float Renderer::getTextHeight(std::string text, std::shared_ptr<Font> font, float size) {
    return getTextSize(text, font, size).height;
}

void Renderer::translate(Point point) {
    if(point.x != 0.0f || point.y != 0.0f) {
        nvgTranslate(context, point.x, point.y);
    }
}

void Renderer::scale(Point point, float scale) {
    if(scale != 0.0F && scale != 1.0f) {
        nvgTranslate(context, point.x, point.y);
		nvgScale(context, scale, scale);
		nvgTranslate(context, -point.x, -point.y);
    }
}

void Renderer::scale(Rect rect, float scale) {
    if(scale != 0.0F && scale != 1.0f) {
        nvgTranslate(context, rect.x + rect.width / 2.0f, rect.y + rect.height / 2.0f);
        nvgScale(context, scale, scale);
        nvgTranslate(context, -(rect.x + rect.width / 2.0f), -(rect.y + rect.height / 2.0f));
    }
}

void Renderer::rotate(Point point, float angle) {
    if(angle != 0.0F) {
        float radians = (std::numbers::pi_v<float> / 180.0F) * angle;
        nvgTranslate(context, point.x, point.y);
        nvgRotate(context, radians);
        nvgTranslate(context, -point.x, -point.y);
    }
}

void Renderer::rotate(Rect rect, float angle) {
    if(angle != 0.0F) {
        float radians = (std::numbers::pi_v<float> / 180.0F) * angle;
        nvgTranslate(context, rect.x + rect.width / 2.0f, rect.y + rect.height / 2.0f);
        nvgRotate(context, radians);
        nvgTranslate(context, -(rect.x + rect.width / 2.0f), -(rect.y + rect.height / 2.0f));
    }
}

void Renderer::scaleAndRotate(Point point, float scale, float angle) {
    
    if((scale != 0.0F && scale != 1.0f) || angle != 0.0F) {
        nvgTranslate(context, point.x, point.y);
        
        if(scale != 0.0F && scale != 1.0f) {
            nvgScale(context, scale, scale);
        }
        
        if(angle != 0.0F) {
            float radians = (std::numbers::pi_v<float> / 180.0F) * angle;
            nvgRotate(context, radians);
        }
        
        nvgTranslate(context, -point.x, -point.y);
    }
}

void Renderer::scaleAndRotate(Rect rect, float scale, float angle) {

    if((scale != 0.0F && scale != 1.0f) || angle != 0.0F) {
        float centerX = rect.x + rect.width / 2.0f;
        float centerY = rect.y + rect.height / 2.0f;
        
        nvgTranslate(context, centerX, centerY);
        
        if(scale != 0.0F && scale != 1.0f) {
            nvgScale(context, scale, scale);
        }
        
        if(angle != 0.0F) {
            float radians = (std::numbers::pi_v<float> / 180.0F) * angle;
            nvgRotate(context, radians);
        }
        
        nvgTranslate(context, -centerX, -centerY);
    }
}

void Renderer::save() {
    nvgSave(context);
}

void Renderer::restore() {
    nvgRestore(context);
}

void Renderer::scisor(Rect rect) {
    nvgScissor(context, rect.x, rect.y, rect.width, rect.height);
}

void Renderer::resetScisor() {
    nvgResetScissor(context);
}

void Renderer::shutdown() {
    if (context) {
        nvgDelete(context);
        context = nullptr;
    }
}

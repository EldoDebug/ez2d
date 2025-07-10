#pragma once

#include <string>
#include <yoga/Yoga.h>
#include <memory>
#include <vector>
#include "../Point.hpp"
#include "../Size.hpp"
#include "../Color.hpp"
#include "../Rect.hpp"

class Texture;
class Sprite;
class SpriteAnimation;

class Element {
    public:
        enum class LayoutMode {
            Manual,
            Column,
            Row,
            Center,
            Grid
        };

        enum class FlexDirection {
            Column,
            ColumnReverse,
            Row,
            RowReverse
        };

        enum class JustifyContent {
            FlexStart,
            Center,
            FlexEnd,
            SpaceBetween,
            SpaceAround,
            SpaceEvenly
        };

        enum class AlignItems {
            Auto,
            FlexStart,
            Center,
            FlexEnd,
            Stretch,
            Baseline,
            SpaceBetween,
            SpaceAround
        };

        enum class AlignContent {
            Auto,
            FlexStart,
            Center,
            FlexEnd,
            Stretch,
            SpaceBetween,
            SpaceAround
        };

        enum class FlexWrap {
            NoWrap,
            Wrap,
            WrapReverse
        };

        enum class PositionType {
            Static,
            Relative,
            Absolute
        };

        Element();
        virtual ~Element();

        void setSize(float width, float height);
        void setWidth(float width);
        void setHeight(float height);
        void setMinSize(float minWidth, float minHeight);
        void setMaxSize(float maxWidth, float maxHeight);
        void setPadding(float top, float right, float bottom, float left);
        void setPadding(float all);
        void setMargin(float top, float right, float bottom, float left);
        void setMargin(float all);
        
        void addTopElement(std::shared_ptr<Element> element);
        void addBottomElement(std::shared_ptr<Element> element);
        void addLeftElement(std::shared_ptr<Element> element);
        void addRightElement(std::shared_ptr<Element> element);
        void addCenterElement(std::shared_ptr<Element> element);
        
        void addChild(std::shared_ptr<Element> element);
        void removeChild(std::shared_ptr<Element> element);
        void clearChildren();
        
        std::shared_ptr<Element> getChild(size_t index) const;
        size_t getChildCount() const;
        const std::vector<std::shared_ptr<Element>>& getChildren() const;
        
        void setLayoutMode(LayoutMode mode);
        void setFlexDirection(FlexDirection direction);
        void setJustifyContent(JustifyContent justify);
        void setAlignItems(AlignItems align);
        void setAlignContent(AlignContent align);
        void setFlexWrap(FlexWrap wrap);
        void setPositionType(PositionType position);
        
        void setFlexGrow(float grow);
        void setFlexShrink(float shrink);
        
        void setAutoSize();
        void setFillParent();
        void setFillWidth();
        void setFillHeight();
        
        void calculateLayout(float parentWidth = YGUndefined, float parentHeight = YGUndefined);
        
        virtual void draw();
        virtual void update();
        
        void setColor(Color color);
        void setTexture(std::shared_ptr<Texture> texture);
        void setSprite(std::shared_ptr<Sprite> sprite, int index = 0);
        void setSpriteAnimation(std::shared_ptr<SpriteAnimation> spriteAnimation);
        void setCornerRadius(float radius);
        
        Point getPosition() const;
        Size getSize() const;
        Rect getBounds() const;
        Rect getAbsoluteBounds() const;
        
        void setVisible(bool visible);
        bool isVisible() const;
        
        YGNodeRef getYogaNode() const { return yogaNode; }

        virtual bool handleMousePressed(Point point, int button);
        virtual bool handleMouseReleased(Point point, int button);
        virtual bool handleMouseMoved(Point point);
        virtual bool handleKeyPressed(int keyCode, int scanCode, int modifiers, bool repeat);
        virtual bool handleKeyReleased(int keyCode, int scanCode, int modifiers);
        virtual bool handleTextInput(const std::string& text);

        virtual void onMousePressed(Point point, int button) {}
        virtual void onMouseReleased(Point point, int button) {}
        virtual void onMouseMoved(Point point) {}
        virtual void onKeyPressed(int keyCode, int scanCode, int modifiers, bool repeat) {}
        virtual void onKeyReleased(int keyCode, int scanCode, int modifiers) {}
        virtual void onTextInput(const std::string& text) {}

        static YGFlexDirection toYGFlexDirection(FlexDirection direction);
        static YGJustify toYGJustify(JustifyContent justify);
        static YGAlign toYGAlign(AlignItems align);
        static YGAlign toYGAlignContent(AlignContent align);
        static YGWrap toYGWrap(FlexWrap wrap);
        static YGPositionType toYGPositionType(PositionType position);

    protected:
        YGNodeRef yogaNode;
        std::vector<std::shared_ptr<Element>> children;
        
        LayoutMode currentLayoutMode;
        bool layoutLocked;
        
        Color color;
        std::shared_ptr<Texture> texture;
        std::shared_ptr<Sprite> sprite;
        int spriteIndex;
        std::shared_ptr<SpriteAnimation> spriteAnimation;
        float cornerRadius;
        bool visible;
        
        void ensureLayoutMode(LayoutMode requiredMode);
        void applyLayoutMode(LayoutMode mode);
        virtual void drawBackground();
        virtual void drawContent();
        virtual void drawChildren();
};

#include "Element.hpp"
#include "../../Renderer.hpp"
#include "../../Texture.hpp"
#include "../../Sprite.hpp"
#include "../../SpriteAnimation.hpp"

Element::Element() 
    : currentLayoutMode(LayoutMode::Manual), layoutLocked(false),
      color(Color(255, 255, 255, 0)), cornerRadius(0.0f), visible(true) {
    yogaNode = YGNodeNew();
    YGNodeSetContext(yogaNode, this);
}

Element::~Element() {
    YGNodeFree(yogaNode);
}

void Element::setSize(float width, float height) {
    YGNodeStyleSetWidth(yogaNode, width);
    YGNodeStyleSetHeight(yogaNode, height);
}

void Element::setWidth(float width) {
    YGNodeStyleSetWidth(yogaNode, width);
}

void Element::setHeight(float height) {
    YGNodeStyleSetHeight(yogaNode, height);
}

void Element::setMinSize(float minWidth, float minHeight) {
    YGNodeStyleSetMinWidth(yogaNode, minWidth);
    YGNodeStyleSetMinHeight(yogaNode, minHeight);
}

void Element::setMaxSize(float maxWidth, float maxHeight) {
    YGNodeStyleSetMaxWidth(yogaNode, maxWidth);
    YGNodeStyleSetMaxHeight(yogaNode, maxHeight);
}

void Element::setPadding(float top, float right, float bottom, float left) {
    YGNodeStyleSetPadding(yogaNode, YGEdgeTop, top);
    YGNodeStyleSetPadding(yogaNode, YGEdgeRight, right);
    YGNodeStyleSetPadding(yogaNode, YGEdgeBottom, bottom);
    YGNodeStyleSetPadding(yogaNode, YGEdgeLeft, left);
}

void Element::setPadding(float all) {
    setPadding(all, all, all, all);
}

void Element::setMargin(float top, float right, float bottom, float left) {
    YGNodeStyleSetMargin(yogaNode, YGEdgeTop, top);
    YGNodeStyleSetMargin(yogaNode, YGEdgeRight, right);
    YGNodeStyleSetMargin(yogaNode, YGEdgeBottom, bottom);
    YGNodeStyleSetMargin(yogaNode, YGEdgeLeft, left);
}

void Element::setMargin(float all) {
    setMargin(all, all, all, all);
}

void Element::ensureLayoutMode(LayoutMode requiredMode) {

    if (layoutLocked && currentLayoutMode != requiredMode) {
    }
    
    if (currentLayoutMode != requiredMode) {
        currentLayoutMode = requiredMode;
        applyLayoutMode(requiredMode);
        layoutLocked = true;
    }
}

void Element::applyLayoutMode(LayoutMode mode) {
    switch (mode) {
        case LayoutMode::Column:
            setFlexDirection(FlexDirection::Column);
            break;
        case LayoutMode::Row:
            setFlexDirection(FlexDirection::Row);
            break;
        case LayoutMode::Center:
            setJustifyContent(JustifyContent::Center);
            setAlignItems(AlignItems::Center);
            break;
        case LayoutMode::Manual:
        case LayoutMode::Grid:
            break;
    }
}

void Element::addTopElement(std::shared_ptr<Element> element) {
    ensureLayoutMode(LayoutMode::Column);
    YGNodeInsertChild(yogaNode, element->yogaNode, 0);
    children.insert(children.begin(), element);
}

void Element::addBottomElement(std::shared_ptr<Element> element) {
    ensureLayoutMode(LayoutMode::Column);
    YGNodeInsertChild(yogaNode, element->yogaNode, YGNodeGetChildCount(yogaNode));
    children.push_back(element);
}

void Element::addLeftElement(std::shared_ptr<Element> element) {
    ensureLayoutMode(LayoutMode::Row);
    YGNodeInsertChild(yogaNode, element->yogaNode, 0);
    children.insert(children.begin(), element);
}

void Element::addRightElement(std::shared_ptr<Element> element) {
    ensureLayoutMode(LayoutMode::Row);
    YGNodeInsertChild(yogaNode, element->yogaNode, YGNodeGetChildCount(yogaNode));
    children.push_back(element);
}

void Element::addCenterElement(std::shared_ptr<Element> element) {
    ensureLayoutMode(LayoutMode::Center);
    addChild(element);
}

void Element::addChild(std::shared_ptr<Element> element) {
    YGNodeInsertChild(yogaNode, element->yogaNode, YGNodeGetChildCount(yogaNode));
    children.push_back(element);
}

void Element::removeChild(std::shared_ptr<Element> element) {
    auto it = std::find(children.begin(), children.end(), element);
    if (it != children.end()) {
        YGNodeRemoveChild(yogaNode, element->yogaNode);
        children.erase(it);
    }
}

void Element::clearChildren() {
    for (auto& child : children) {
        YGNodeRemoveChild(yogaNode, child->yogaNode);
    }
    children.clear();
}

void Element::setLayoutMode(LayoutMode mode) {
    currentLayoutMode = mode;
    applyLayoutMode(mode);
    layoutLocked = true;
}

void Element::setFlexDirection(FlexDirection direction) {
    YGNodeStyleSetFlexDirection(yogaNode, toYGFlexDirection(direction));
}

void Element::setJustifyContent(JustifyContent justify) {
    YGNodeStyleSetJustifyContent(yogaNode, toYGJustify(justify));
}

void Element::setAlignItems(AlignItems align) {
    YGNodeStyleSetAlignItems(yogaNode, toYGAlign(align));
}

void Element::setAlignContent(AlignContent align) {
    YGNodeStyleSetAlignContent(yogaNode, toYGAlignContent(align));
}

void Element::setFlexWrap(FlexWrap wrap) {
    YGNodeStyleSetFlexWrap(yogaNode, toYGWrap(wrap));
}

void Element::setPositionType(PositionType position) {
    YGNodeStyleSetPositionType(yogaNode, toYGPositionType(position));
}

void Element::setFlexGrow(float grow) {
    YGNodeStyleSetFlexGrow(yogaNode, grow);
}

void Element::setFlexShrink(float shrink) {
    YGNodeStyleSetFlexShrink(yogaNode, shrink);
}

void Element::setAutoSize() {
    YGNodeStyleSetWidth(yogaNode, YGUndefined);
    YGNodeStyleSetHeight(yogaNode, YGUndefined);
    YGNodeStyleSetFlexGrow(yogaNode, 0);
    YGNodeStyleSetFlexShrink(yogaNode, 1);
}

void Element::setFillParent() {
    YGNodeStyleSetFlexGrow(yogaNode, 1.0f);
    YGNodeStyleSetWidth(yogaNode, YGUndefined);
    YGNodeStyleSetHeight(yogaNode, YGUndefined);
}

void Element::setFillWidth() {
    YGNodeStyleSetFlexGrow(yogaNode, 1.0f);
    YGNodeStyleSetWidth(yogaNode, YGUndefined);
}

void Element::setFillHeight() {
    YGNodeStyleSetFlexGrow(yogaNode, 1.0f);
    YGNodeStyleSetHeight(yogaNode, YGUndefined);
}

void Element::calculateLayout(float parentWidth, float parentHeight) {
    YGNodeCalculateLayout(yogaNode, parentWidth, parentHeight, YGDirectionLTR);
}

void Element::draw() {
    if (!visible) return;
    
    drawBackground();
    drawContent();
    drawChildren();
}

void Element::update() {
    updateMoveAnimation();
    if (!visible) return;
    
    for (auto& child : children) {
        child->update();
    }
}

void Element::drawBackground() {
    if (color.alpha > 0 || texture || sprite || spriteAnimation) {
        Rect bounds = getAbsoluteBounds();
        
        if (spriteAnimation) {
            spriteAnimation->draw(bounds);
        } else if (sprite) {
            Renderer::drawSprite(sprite, bounds, spriteIndex);
        } else if (texture) {
            if (cornerRadius > 0) {
                Renderer::drawRoundedTexture(texture, bounds, cornerRadius);
            } else {
                Renderer::drawTexture(texture, bounds);
            }
        } else if (color.alpha > 0) {
            if (cornerRadius > 0) {
                Renderer::drawRoundedRect(bounds, cornerRadius, color);
            } else {
                Renderer::drawRect(bounds, color);
            }
        }
    }
}

void Element::drawContent() {}

void Element::drawChildren() {
    for (auto& child : children) {
        child->draw();
    }
}

void Element::setColor(Color color) {
    this->color = color;
}

void Element::setTexture(std::shared_ptr<Texture> texture) {
    this->texture = texture;
}

void Element::setSprite(std::shared_ptr<Sprite> sprite, int index) {
    this->sprite = sprite;
    this->spriteIndex = index;
}

void Element::setSpriteAnimation(std::shared_ptr<SpriteAnimation> spriteAnimation) {
    this->spriteAnimation = spriteAnimation;
}

void Element::setCornerRadius(float radius) {
    this->cornerRadius = radius;
}

void Element::setMoveAnimation(float moveX, float moveY, Easing easing) {
    moveAnimX = Animation(500.0f, moveX, 0.0f, easing);
    moveAnimY = Animation(500.0f, moveY, 0.0f, easing);
    moving = true;
}

void Element::updateMoveAnimation() {
    if (moving) {
        moveAnimX.update();
        moveAnimY.update();
        if (moveAnimX.isFinished() && moveAnimY.isFinished()) {
            moving = false;
        }
    }
}

Point Element::getPosition() const {
    float x = YGNodeLayoutGetLeft(yogaNode) + moveAnimX.getValue();
    float y = YGNodeLayoutGetTop(yogaNode) + moveAnimY.getValue();
    return Point(x, y);
}

Size Element::getSize() const {
    float width = YGNodeLayoutGetWidth(yogaNode);
    float height = YGNodeLayoutGetHeight(yogaNode);
    return Size(width, height);
}

Rect Element::getBounds() const {
    float x = YGNodeLayoutGetLeft(yogaNode);
    float y = YGNodeLayoutGetTop(yogaNode);
    float width = YGNodeLayoutGetWidth(yogaNode);
    float height = YGNodeLayoutGetHeight(yogaNode);
    return Rect(x, y, width, height);
}

Rect Element::getAbsoluteBounds() const {
    float x = 0;
    float y = 0;
    float width = YGNodeLayoutGetWidth(yogaNode);
    float height = YGNodeLayoutGetHeight(yogaNode);
    
    YGNodeRef currentNode = yogaNode;
    while (currentNode != nullptr) {
        x += YGNodeLayoutGetLeft(currentNode);
        y += YGNodeLayoutGetTop(currentNode);
        
        YGNodeRef parent = YGNodeGetParent(currentNode);
        if (parent != nullptr) {
            x += YGNodeLayoutGetPadding(parent, YGEdgeLeft);
            y += YGNodeLayoutGetPadding(parent, YGEdgeTop);
        }
        
        currentNode = parent;
    }
    
    return Rect(x, y, width, height);
}

std::shared_ptr<Element> Element::getChild(size_t index) const {
    if (index < children.size()) {
        return children[index];
    }
    return nullptr;
}

size_t Element::getChildCount() const {
    return children.size();
}

const std::vector<std::shared_ptr<Element>>& Element::getChildren() const {
    return children;
}

void Element::setVisible(bool visible) {
    this->visible = visible;
}

bool Element::isVisible() const {
    return visible;
}

bool Element::handleMousePressed(Point point, int button) {
    if (!visible) return false;

    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        if ((*it)->handleMousePressed(point, button)) {
            return true;
        }
    }

    if (point.isInside(getAbsoluteBounds())) {
        onMousePressed(point, button);
        return true;
    }

    return false;
}

bool Element::handleMouseReleased(Point point, int button) {
    if (!visible) return false;

    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        if ((*it)->handleMouseReleased(point, button)) {
            return true;
        }
    }

    if (point.isInside(getAbsoluteBounds())) {
        onMouseReleased(point, button);
        return true;
    }

    return false;
}

bool Element::handleMouseMoved(Point point) {
    if (!visible) return false;

    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        if ((*it)->handleMouseMoved(point)) {
            return true;
        }
    }

    if (point.isInside(getAbsoluteBounds())) {
        onMouseMoved(point);
        return true;
    }

    return false;
}

bool Element::handleKeyPressed(int keyCode, int scanCode, int modifiers, bool repeat) {
    if (!visible) return false;

    for (auto& child : children) {
        if (child->handleKeyPressed(keyCode, scanCode, modifiers, repeat)) {
            return true;
        }
    }

    onKeyPressed(keyCode, scanCode, modifiers, repeat);
    return false;
}

bool Element::handleKeyReleased(int keyCode, int scanCode, int modifiers) {
    if (!visible) return false;

    for (auto& child : children) {
        if (child->handleKeyReleased(keyCode, scanCode, modifiers)) {
            return true;
        }
    }

    onKeyReleased(keyCode, scanCode, modifiers);
    return false;
}

bool Element::handleTextInput(const std::string& text) {
    if (!visible) return false;

    for (auto& child : children) {
        if (child->handleTextInput(text)) {
            return true;
        }
    }

    onTextInput(text);
    return false;
}

YGFlexDirection Element::toYGFlexDirection(FlexDirection direction) {
    switch (direction) {
        case FlexDirection::Column: return YGFlexDirectionColumn;
        case FlexDirection::ColumnReverse: return YGFlexDirectionColumnReverse;
        case FlexDirection::Row: return YGFlexDirectionRow;
        case FlexDirection::RowReverse: return YGFlexDirectionRowReverse;
        default: return YGFlexDirectionColumn;
    }
}

YGJustify Element::toYGJustify(JustifyContent justify) {
    switch (justify) {
        case JustifyContent::FlexStart: return YGJustifyFlexStart;
        case JustifyContent::Center: return YGJustifyCenter;
        case JustifyContent::FlexEnd: return YGJustifyFlexEnd;
        case JustifyContent::SpaceBetween: return YGJustifySpaceBetween;
        case JustifyContent::SpaceAround: return YGJustifySpaceAround;
        case JustifyContent::SpaceEvenly: return YGJustifySpaceEvenly;
        default: return YGJustifyFlexStart;
    }
}

YGAlign Element::toYGAlign(AlignItems align) {
    switch (align) {
        case AlignItems::Auto: return YGAlignAuto;
        case AlignItems::FlexStart: return YGAlignFlexStart;
        case AlignItems::Center: return YGAlignCenter;
        case AlignItems::FlexEnd: return YGAlignFlexEnd;
        case AlignItems::Stretch: return YGAlignStretch;
        case AlignItems::Baseline: return YGAlignBaseline;
        case AlignItems::SpaceBetween: return YGAlignSpaceBetween;
        case AlignItems::SpaceAround: return YGAlignSpaceAround;
        default: return YGAlignStretch;
    }
}

YGAlign Element::toYGAlignContent(AlignContent align) {
    switch (align) {
        case AlignContent::Auto: return YGAlignAuto;
        case AlignContent::FlexStart: return YGAlignFlexStart;
        case AlignContent::Center: return YGAlignCenter;
        case AlignContent::FlexEnd: return YGAlignFlexEnd;
        case AlignContent::Stretch: return YGAlignStretch;
        case AlignContent::SpaceBetween: return YGAlignSpaceBetween;
        case AlignContent::SpaceAround: return YGAlignSpaceAround;
        default: return YGAlignStretch;
    }
}

YGWrap Element::toYGWrap(FlexWrap wrap) {
    switch (wrap) {
        case FlexWrap::NoWrap: return YGWrapNoWrap;
        case FlexWrap::Wrap: return YGWrapWrap;
        case FlexWrap::WrapReverse: return YGWrapWrapReverse;
        default: return YGWrapNoWrap;
    }
}

YGPositionType Element::toYGPositionType(PositionType position) {
    switch (position) {
        case PositionType::Static: return YGPositionTypeStatic;
        case PositionType::Relative: return YGPositionTypeRelative;
        case PositionType::Absolute: return YGPositionTypeAbsolute;
        default: return YGPositionTypeRelative;
    }
}

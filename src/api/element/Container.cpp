#include "Container.hpp"

VBoxContainer::VBoxContainer() {
    setLayoutMode(LayoutMode::Column);
    YGNodeStyleSetFlexDirection(yogaNode, YGFlexDirectionColumn);
}

void VBoxContainer::setSpacing(float spacing) {
    for (size_t i = 1; i < children.size(); ++i) {
        YGNodeStyleSetMargin(children[i]->getYogaNode(), YGEdgeTop, spacing);
    }
}

void VBoxContainer::setAlignment(AlignItems align) {
    setAlignItems(align);
}

HBoxContainer::HBoxContainer() {
    setLayoutMode(LayoutMode::Row);
    YGNodeStyleSetFlexDirection(yogaNode, YGFlexDirectionRow);
}

void HBoxContainer::setSpacing(float spacing) {
    for (size_t i = 1; i < children.size(); ++i) {
        YGNodeStyleSetMargin(children[i]->getYogaNode(), YGEdgeLeft, spacing);
    }
}

void HBoxContainer::setAlignment(AlignItems align) {
    setAlignItems(align);
}

CenterContainer::CenterContainer() {
    setLayoutMode(LayoutMode::Center);
    YGNodeStyleSetJustifyContent(yogaNode, YGJustifyCenter);
    YGNodeStyleSetAlignItems(yogaNode, YGAlignCenter);
}

FlexContainer::FlexContainer() {
    setLayoutMode(LayoutMode::Manual);
}

void FlexContainer::setDirection(FlexDirection direction) {
    setFlexDirection(direction);
}

void FlexContainer::setWrap(FlexWrap wrap) {
    setFlexWrap(wrap);
}

void FlexContainer::setJustify(JustifyContent justify) {
    setJustifyContent(justify);
}

void FlexContainer::setAlign(AlignItems align) {
    setAlignItems(align);
}

void FlexContainer::setAlignContent(AlignContent align) {
    Element::setAlignContent(align);
}

void FlexContainer::setGap(float gap) {
    YGNodeStyleSetGap(yogaNode, YGGutterAll, gap);
}

GridContainer::GridContainer(int columns, int rows) 
    : columns(columns), rows(rows), columnGap(0.0f), rowGap(0.0f), autoFlow(true) {
    setLayoutMode(LayoutMode::Grid);
    createGridStructure();
}

void GridContainer::setElement(int column, int row, std::shared_ptr<Element> element) {
    if (column >= columns || column < 0) return;
    
    auto it = std::find_if(gridItems.begin(), gridItems.end(), 
        [column, row](const GridItem& item) {
            return item.column == column && item.row == row;
        });
    
    if (it != gridItems.end()) {
        it->element = element;
    } else {
        gridItems.push_back({element, column, row});
    }
    
    rebuildLayout();
}

void GridContainer::setElement(int index, std::shared_ptr<Element> element) {
    int column, row;
    if (autoFlow) {
        column = index % columns;
        row = index / columns;
    } else {
        row = index % calculateRows();
        column = index / calculateRows();
    }
    
    setElement(column, row, element);
}

void GridContainer::setColumnWidth(int column, float width) {
    if (column < static_cast<int>(columnContainers.size())) {
        columnContainers[column]->setWidth(width);
    }
}

void GridContainer::setRowHeight(int row, float height) {
    if (row < static_cast<int>(rowContainers.size())) {
        rowContainers[row]->setHeight(height);
    }
}

void GridContainer::setColumnFlex(int column, float flex) {
    if (column < static_cast<int>(columnContainers.size())) {
        columnContainers[column]->setFlexGrow(flex);
    }
}

void GridContainer::setRowFlex(int row, float flex) {
    if (row < static_cast<int>(rowContainers.size())) {
        rowContainers[row]->setFlexGrow(flex);
    }
}

void GridContainer::setGap(float gap) {
    setColumnGap(gap);
    setRowGap(gap);
}

void GridContainer::setColumnGap(float gap) {
    columnGap = gap;
    rebuildLayout();
}

void GridContainer::setRowGap(float gap) {
    rowGap = gap;
    rebuildLayout();
}

void GridContainer::setAutoFlow(bool columnFirst) {
    autoFlow = columnFirst;
}

int GridContainer::calculateRows() const {
    if (rows > 0) return rows;
    
    int maxRow = 0;
    for (const auto& item : gridItems) {
        maxRow = std::max(maxRow, item.row);
    }
    return maxRow + 1;
}

void GridContainer::createGridStructure() {

    clearChildren();
    columnContainers.clear();
    rowContainers.clear();
    
    int actualRows = calculateRows();
    
    YGNodeStyleSetFlexDirection(yogaNode, YGFlexDirectionColumn);
    
    for (int r = 0; r < actualRows; ++r) {

        auto rowContainer = std::make_shared<HBoxContainer>();
        rowContainer->setFlexGrow(1.0f);

        if (r > 0 && rowGap > 0) {
            rowContainer->setMargin(rowGap, 0, 0, 0);
        }
        
        for (int c = 0; c < columns; ++c) {
            auto cellContainer = std::make_shared<Element>();
            cellContainer->setFlexGrow(1.0f);
            if (c > 0 && columnGap > 0) {
                cellContainer->setMargin(0, 0, 0, columnGap);
            }
            rowContainer->addChild(cellContainer);
        }
        
        addChild(rowContainer);
        rowContainers.push_back(rowContainer);
    }
}

void GridContainer::rebuildLayout() {
    createGridStructure();
    
    for (const auto& item : gridItems) {
        if (item.row < static_cast<int>(rowContainers.size())) {
            auto rowContainer = std::static_pointer_cast<HBoxContainer>(rowContainers[item.row]);
            if (item.column < static_cast<int>(rowContainer->getChildCount())) {
                auto cellContainer = rowContainer->getChild(item.column);
                cellContainer->clearChildren();
                cellContainer->addChild(item.element);
            }
        }
    }
}

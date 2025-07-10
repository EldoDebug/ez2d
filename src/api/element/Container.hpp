#pragma once

#include "Element.hpp"

class Container : public Element {
    public:
        Container() = default;
        virtual ~Container() = default;
        
        virtual void setSpacing(float spacing) = 0;
        virtual void setAlignment(AlignItems align) = 0;
        
    protected:
        float spacing = 0.0f;
        AlignItems alignment = AlignItems::Stretch;
};

class AdvancedContainer : public Container {
    public:
        AdvancedContainer() = default;
        virtual ~AdvancedContainer() = default;
        
        virtual void setGap(float gap) = 0;
        virtual void setColumnGap(float gap) {}
        virtual void setRowGap(float gap) {}
        
        virtual void setDirection(FlexDirection direction) {}
        virtual void setWrap(FlexWrap wrap) {}
        virtual void setJustify(JustifyContent justify) {}
        virtual void setAlign(AlignItems align) {}
        virtual void setAlignContent(AlignContent align) {}
    
    protected:
        float gap = 0.0f;
        float columnGap = 0.0f;
        float rowGap = 0.0f;
};

class VBoxContainer : public Container {
    public:
        VBoxContainer();
        
        void setSpacing(float spacing) override;
        void setAlignment(AlignItems align) override;
};

class HBoxContainer : public Container {
    public:
        HBoxContainer();
        
        void setSpacing(float spacing) override;
        void setAlignment(AlignItems align) override;
};

class CenterContainer : public Element {
    public:
        CenterContainer();
};

class FlexContainer : public AdvancedContainer {
    public:
        FlexContainer();
        
        void setSpacing(float spacing) override { setGap(spacing); }
        void setAlignment(AlignItems align) override { setAlign(align); }
        
        void setDirection(FlexDirection direction) override;
        void setWrap(FlexWrap wrap) override;
        void setJustify(JustifyContent justify) override;
        void setAlign(AlignItems align) override;
        void setAlignContent(AlignContent align) override;
        void setGap(float gap) override;
};

class GridContainer : public AdvancedContainer {
    public:
        GridContainer(int columns, int rows = -1);
        
        void setSpacing(float spacing) override { setGap(spacing); }
        void setAlignment(AlignItems align) override {}
        
        void setElement(int column, int row, std::shared_ptr<Element> element);
        void setElement(int index, std::shared_ptr<Element> element);
        
        void setColumnWidth(int column, float width);
        void setRowHeight(int row, float height);
        void setColumnFlex(int column, float flex);
        void setRowFlex(int row, float flex);
        
        void setGap(float gap) override;
        void setColumnGap(float gap) override;
        void setRowGap(float gap) override;
        
        void setAutoFlow(bool columnFirst = true);
        
        void rebuildLayout();
        
    private:
        int columns;
        int rows;
        float columnGap;
        float rowGap;
        bool autoFlow;
        
        struct GridItem {
            std::shared_ptr<Element> element;
            int column;
            int row;
            int columnSpan = 1;
            int rowSpan = 1;
        };
        
        std::vector<GridItem> gridItems;
        std::vector<std::shared_ptr<Element>> columnContainers;
        std::vector<std::shared_ptr<Element>> rowContainers;
        
        int calculateRows() const;
        void createGridStructure();
};

#pragma once

#include <cstddef>
#include "CoordinatePoint.hpp"
#include "GraphPrimitive.hpp"


class Document {
public:
    struct ExportableState {
        ViewOfPrimitives primitives;
    };

    Document() {}
    Document([[maybe_unused]] const ExportableState& state) {}
    ~Document() {}

    void AddElement([[maybe_unused]] const PrimitivePtr& element) {}
    
    void MoveElement([[maybe_unused]] std::size_t elementIdx, [[maybe_unused]] const CoordinatePoint& coord) {}
    
    void RemoveElement([[maybe_unused]] std::size_t elementIdx) {}
    
    ViewOfPrimitives GetDocumentPrimitives() const {
        return ViewOfPrimitives{};
    }

    ExportableState GetExportableState() const {
        return ExportableState{};
    }

private:
    CollectionOfPrimitives m_elements;
};
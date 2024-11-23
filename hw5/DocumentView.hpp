#pragma once

#include "Circle.hpp"
#include "Document.hpp"
#include "GraphPrimitive.hpp"
#include "Line.hpp"
#include "string"


class DocumentView {
public:
    DocumentView() {}

    void Update([[maybe_unused]] const ViewOfPrimitives& primitives) const {}
    void Update([[maybe_unused]] const std::string& message) const {}
private:
    void displayLine([[maybe_unused]] const Line& line) const {}
    void displayCircle([[maybe_unused]] const Circle& Circle) const {}
};

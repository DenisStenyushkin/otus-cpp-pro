#pragma once

#include <memory>
#include <vector>
#include "CoordinatePoint.hpp"


class GraphPrimitive {
public:
    virtual ~GraphPrimitive() {};
    virtual void MoveTo(const CoordinatePoint&) = 0;
};

using PrimitivePtr = std::shared_ptr<GraphPrimitive>;
using CollectionOfPrimitives = std::vector<PrimitivePtr>;
using ViewOfPrimitives = std::vector<const GraphPrimitive*>;
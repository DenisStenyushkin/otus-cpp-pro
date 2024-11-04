#pragma once

#include "CoordinatePoint.hpp"
#include "GraphPrimitive.hpp"


class Circle : public GraphPrimitive {
public:
    Circle([[maybe_unused]] const CoordinatePoint& centerPoint, [[maybe_unused]] float radius) {};
    
    Circle([[maybe_unused]] const Circle& other) {}
    
    Circle([[maybe_unused]] Circle&& other) {}
    
    ~Circle() override {};

    virtual void MoveTo([[maybe_unused]] const CoordinatePoint& newCenterPoint) override {};
    
    CoordinatePoint getCenter() const {
        return CoordinatePoint{0, 0};
    }

    float getRadius() const {
        return 0;
    }
};
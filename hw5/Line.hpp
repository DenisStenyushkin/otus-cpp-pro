#pragma once

#include "CoordinatePoint.hpp"
#include "GraphPrimitive.hpp"


class Line : public GraphPrimitive {
public:
    Line([[maybe_unused]] const CoordinatePoint& point1, [[maybe_unused]] const CoordinatePoint& point2) {};
    
    Line([[maybe_unused]] const Line& other) {}
    
    Line([[maybe_unused]] Line && other) {}
    
    ~Line() override {};

    
    virtual void MoveTo([[maybe_unused]] const CoordinatePoint& newCenterPoint) override {};
    
    CoordinatePoint getPoint1() const {
        return CoordinatePoint{0, 0};
    }
    
    CoordinatePoint getPoint2() const {
        return CoordinatePoint{0, 0};
    }
};

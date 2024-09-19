#ifndef AXIFY_POINT_H
#define AXIFY_POINT_H

#include "Double.h"

class Point{
public:
    Double x, y;
    Point(Double a, Double b){
        x = a;
        y = b;
    }
    Point(){
        x = 0;
        y = 0;
    }
    Point& operator = (const Point& p){
        if (this == &p){
            return *this;
        }
        x = p.x;
        y = p.y;
        return *this;
    }
    bool operator == (Point const &p) const{
        return (x == p.x and y == p.y);
    }
    bool operator != (Point const &p) const{
        return not (*this == p);
    }
    bool operator < (Point const &p) const{
        if (x < p.x){
            return true;
        }
        else if (x > p.x){
            return false;
        }
        else {
            return y < p.y;
        }
    }
    bool operator > (Point const &p) const{
        if (x > p.x){
            return true;
        }
        else if (x < p.x){
            return false;
        }
        else {
            return y > p.y;
        }
    }
    Point operator + (Point const &p) const{
        return Point(p.x + x, p.y + y);
    }
    Point operator - (Point const &p) const{
        return Point(x - p.x, y - p.y);
    }
    Point operator * (Double const &d) const{
        return Point(x * d, y * d);
    }
    Point operator / (Double const &d) const{
        return Point(x / d, y / d);
    }
    Point rotate(Double angle){
        return Point(x * cos(angle) - y * sin(angle),
                     x * sin(angle) + y * cos(angle));
    }
};

Double distance(Point p, Point q){
    return Double(sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y)));
}

bool isnan(Point p){
    return std::isnan(p.x) or std::isnan(p.y);
}

#endif //AXIFY_POINT_H

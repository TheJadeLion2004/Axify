#ifndef AXIFY_MEDIALEDGE_H
#define AXIFY_MEDIALEDGE_H

#include "Parabola.h"
#include <variant>

class MedialEdge{
public:
    std::variant<Segment, Ray, Parabola, int> shape;
    Point start_point, end_point;
    MedialEdge(){
        shape = 0;
        start_point = Point(Nan, Nan);
        end_point = Point(Nan, Nan);
    };
    MedialEdge(Segment seg, Point start, Point end){
        shape = seg;
        start_point = start;
        end_point = end;
    }
    MedialEdge(Ray ray, Point start, Point end = nanPoint){
        shape = ray;
        start_point = start;
        end_point = end;
    }
    MedialEdge(Parabola para, Point start, Point end = nanPoint){
        shape = para;
        start_point = start;
        end_point = end;
    }
    bool operator < (MedialEdge const &me) const{
        return start_point < me.start_point;
    }
};


#endif //AXIFY_MEDIALEDGE_H

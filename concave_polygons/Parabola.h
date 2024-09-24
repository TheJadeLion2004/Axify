#ifndef AXIFY_PARABOLA_H
#define AXIFY_PARABOLA_H

#include "Line.h"

class Parabola {
public:
    Point focus, start, end;
    Line directrix;
    Parabola(Point f, Line d){
        focus.x = f.x;
        focus.y = f.y;
        directrix.a = d.a;
        directrix.b = d.b;
        directrix.c = d.c;
        start = nanPoint;
        end = nanPoint;
    }
    Parabola(Point f, Line d, Point s, Point e){
        focus.x = f.x;
        focus.y = f.y;
        directrix.a = d.a;
        directrix.b = d.b;
        directrix.c = d.c;
        start = s;
        end = e;
    }
    Parabola rotate(Double angle){
        return Parabola(focus.rotate(angle), directrix.rotate(angle), start.rotate(angle), end.rotate(angle));
    }
    Double angle_to_rotate(){
        Parabola pa = rotate(-directrix.angle());
        if (isnan(pa.start) or isnan(pa.end) or pa.start.x < pa.end.x){
            return - directrix.angle();
        }
        else {
            return M_PI - directrix.angle();
        }
    }
    // y = a(x - b)^2 + c
    Double a(){
        if (directrix.a == 0){
            return 0.5 / (focus.y + directrix.c / directrix.b);
        }
        else {
            Parabola pa = rotate(angle_to_rotate());
            return 0.5 / (pa.focus.y + pa.directrix.c / pa.directrix.b);
        }
    }
    Double b(){
        if (directrix.a == 0){
            return focus.x;
        }
        else {
            Parabola pa = rotate(angle_to_rotate());
            return pa.focus.x;
        }
    }
    Double c(){
        if (directrix.a == 0){
            return (focus.y - directrix.c / directrix.b) / 2;
        }
        else {
            Parabola pa = rotate(angle_to_rotate());
            return (pa.focus.y - pa.directrix.c / pa.directrix.b) / 2;
        }
    }
};

bool isOn(Point p, Parabola pa, bool force = false){
    if (not force) {
        if (p.y != pa.a() * (p.x - pa.b()) * (p.x - pa.b()) + pa.c()) {
            return false;
        }
    }
    p = p.rotate(pa.angle_to_rotate());
    pa = pa.rotate(pa.angle_to_rotate());
    if (not (std::isnan(pa.start.x) or p.x >= pa.start.x)
        or not (std::isnan(pa.end.x) or p.x <= pa.end.x)){
        return false;
    }
    return true;
}

Parabola bisector(Point p, Segment s, Point start){
    Parabola pa(p, (Line)s);
    pa.start = start;
    return pa;
}

std::pair<Point,Point> intersection(Parabola pa, Line l){
    l = l.rotate(pa.angle_to_rotate());
    Double A = l.a, B = l.b, C = l.c, a = pa.a(), b = pa.b(), c = pa.c();
    std::pair<Double, Double> x = quadratic(a * B, A - 2 * a * b * B, a * b * b * B + c * B + C);
    std::pair<Point, Point> pts;
    if (std::isnan(x.first)){
        pts.first = nanPoint;
    }
    else {
        pts.first.x = x.first;
        pts.first.y = a * (x.first - b) * (x.first - b) + c;
        pts.first = pts.first.rotate(- pa.angle_to_rotate());
        if (not isOn(pts.first, pa, true)){
            pts.first = nanPoint;
        }
    }
    if (std::isnan(x.second)){
        pts.second = nanPoint;
    }
    else {
        pts.second.x = x.second;
        pts.second.y = a * (x.second - b) * (x.second - b) + c;
        pts.second = pts.second.rotate(- pa.angle_to_rotate());
        if (not isOn(pts.second, pa, true)){
            pts.second = nanPoint;
        }
    }
    return pts;
}

std::pair<Point,Point> intersection(Parabola pa, Segment s){
    std::pair<Point,Point> pts = intersection(pa, (Line)s);
    if (not isOn(pts.first, s, true)){
        pts.first = nanPoint;
    }
    if (not isOn(pts.second, s, true)){
        pts.second = nanPoint;
    }
    return pts;
}

std::pair<Point,Point> intersection(Parabola pa, Ray r){
    std::pair<Point,Point> pts = intersection(pa, (Line)r);
    if (not isOn(pts.first, r, true)){
        pts.first = nanPoint;
    }
    if (not isOn(pts.second, r, true)){
        pts.second = nanPoint;
    }
    return pts;
}


#endif //AXIFY_PARABOLA_H

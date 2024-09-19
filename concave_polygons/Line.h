#ifndef AXIFY_LINE_H
#define AXIFY_LINE_H

#include "Point.h"
#include <cassert>

class Line {
public:
    // ax + by + c = 0
    Double a, b, c;
    Line(Double A, Double B, Double C){
        assert(A != 0 or B != 0);
        a = A;
        b = B;
        c = C;
    }
    Line(Point p, Point q){
        assert(p != q);
        a = q.y - p.y;
        b = p.x - q.x;
        c = q.x * p.y - p.x * q.y;
    }
    Line(){
        a = 0;
        b = 0;
        c = 0;
    }
    Line& operator = (const Line& l){
        if (this == &l){
            return *this;
        }
        a = l.a;
        b = l.b;
        c = l.c;
        return *this;
    }
    bool operator == (Line const &l) const{
        return (a == l.a and b == l.b and c == l.c);
    }
    Double angle(){
        if (b == 0){
            return Double(M_PI_2);
        }
        else {
            return Double(atan(- a / b));
        }
    }
    Line rotate(Double angle){
        Point p, q;
        if (a == 0){
            p = Point(0, - c / b);
            q = Point(1, - c / b);
        }
        else if (b == 0){
            p = Point(- c / a, 0);
            q = Point(- c / a, 1);
        }
        else {
            p = Point(0, - c / b);
            q = Point(- c / a, 0);
        }
        return Line(p.rotate(angle), q.rotate(angle));
    }
};

bool isOn(Point p, Line l){
    return (l.a * p.x + l.b * p.y + l.c == 0);
}

bool isParallel(Line l1, Line l2){
    return (l1.a * l2.b == l1.b * l2.a);
}

Point intersection(Line l1, Line l2){
    if (isParallel(l1, l2)){
        return Point(Nan, Nan);
    }
    return Point((l1.b * l2.c - l2.b * l1.c) / (l1.a * l2.b - l2.a * l1.b),
                 (l1.c * l2.a - l2.c * l1.a) / (l1.a * l2.b - l2.a * l1.b));
}

class Segment : public Line{
public:
    Point start, end;
    Segment() : Line() {
        start = end = Point();
    }
    Segment(Point p, Point q) : Line(p, q) {
        start = p;
        end = q;
    }
    Segment& operator = (const Segment& s){
        if (this == &s){
            return *this;
        }
        a = s.a;
        b = s.b;
        c = s.c;
        start = s.start;
        end = s.end;
        return *this;
    }
    bool operator == (Segment const &s) const{
        return (a == s.a and b == s.b and c == s.c and start == s.start and end == s.end);
    }
    Segment rotate(Double angle){
        return Segment(start.rotate(angle), end.rotate(angle));
    }
    Segment operator * (const Double& d) const{
        return Segment(start, start + (end - start) * d);
    }
    Segment operator / (const Double& d) const{
        return Segment(start, start + (end - start) / d);
    }
    Double length(){
        return distance(start, end);
    }
};

bool isOn(Point p, Segment s, bool force = false){
    Point mid = (s.start + s.end) / 2;
    return (force or s.a * p.x + s.b * p.y + s.c == 0)
            and sign(mid.x - s.start.x) == sign(p.x - s.start.x)
            and sign(mid.x - s.end.x) == sign(p.x - s.end.x)
            and sign(mid.y - s.start.y) == sign(p.y - s.start.y)
            and sign(mid.y - s.end.y) == sign(p.y - s.end.y);
}

class Ray : public Line{
public:
    Point start, end;
    Ray() : Line() {
        start = end = Point();
    }
    Ray(Point p, Point q) : Line(p, q) {
        start = p;
        end = q;
    }
    Ray& operator = (const Ray& r){
        if (this == &r){
            return *this;
        }
        a = r.a;
        b = r.b;
        c = r.c;
        start = r.start;
        end = r.end;
        return *this;
    }
    Ray rotate(Double angle){
        return Ray(start.rotate(angle), end.rotate(angle));
    }
};

bool isOn(Point p, Ray r, bool force = false){
    return (force or r.a * p.x + r.b * p.y + r.c == 0)
           and sign(r.end.x - r.start.x) == sign(p.x - r.start.x)
           and sign(r.end.y - r.start.y) == sign(p.y - r.start.y);
}

Point intersection(Ray r1, Ray r2){
    Point p = intersection((Line)r1, (Line)r2);
    if (isOn(p, r1) and isOn(p, r2)){
        return p;
    }
    else {
        return Point(Nan, Nan);
    }
}

Point intersection(Ray r, Segment s){
    Point p = intersection((Line)r, (Line)s);
    if (isOn(p, r) and isOn(p, s)){
        return p;
    }
    else {
        return Point(Nan, Nan);
    }
}

Ray bisector(Segment s1, Segment s2, Point start){
    if (s1.end != s1.start){
        Point p = intersection((Line)s1, (Line)s2);
        s1.end = p;
        s2.start = p;
    }
    s2 = s2 * (s1.length() / s2.length());
    Ray r(s2.start, (s2.end + s1.start) / 2);
    r.start = start;
    return r;
}

Ray bisector(Point p1, Point p2, Point start){
    return Ray(start, start + (start - (p1 + p2) / 2));
}

Ray bisectorLocal(Segment s, Point p){
    if (s.start == p){
        return Ray(s.start, s.start + Point(s.start.y - s.end.y, s.end.x - s.start.x));
    }
    else if (s.end == p){
        return Ray(s.end, s.end + Point(s.start.y - s.end.y, s.end.x - s.start.x));
    }
}

bool isValid(Point p, Segment s){
    Ray r1 = bisectorLocal(s, s.start), r2 = bisectorLocal(s, s.end);
    return (sign(r1.a * p.x + r1.b * p.y + r1.c) * sign(r2.a * p.x + r2.b * p.y + r2.c) == -1);
}

bool isValid(Point p, Segment s1, Segment s2){
    assert(s1.end == s2.start);
    Ray r1 = bisectorLocal(s1, s1.end), r2 = bisectorLocal(s2, s2.start);
    Double a = r1.end.x - r1.start.x, b = r1.end.y - r1.start.y,
           c = r2.end.x - r2.start.x, d = r2.end.y - r2.start.y,
           x = p.x - s2.start.x, y = p.y - s2.start.y;
    return (sign(b * x - a * y) * sign(d * x - c * y) == -1);
}


#endif //AXIFY_LINE_H

#ifndef AXIFY_POLYGON_H
#define AXIFY_POLYGON_H

#include "line.h"
#include <vector>
#include <algorithm>

class Polygon{
public:
    std::vector<Segment> edges;
    std::vector<Point> vertices;
    std::vector<std::pair<int,int>> chains;
    int n;
    Polygon() {
        n = 0;
    }
    Polygon(std::vector<Segment> &sides){
        edges = sides;
        n = edges.size();
        for (Segment s : edges){
            vertices.push_back(s.end);
        }
        int i;
        for (i = 0; i < n; i++){
            Double a = edges[i].end.x - edges[i].start.x,
                   b = edges[i].end.y - edges[i].start.y,
                   c = edges[(i + 1) % n].end.x - edges[(i + 1) % n].start.x,
                   d = edges[(i + 1) % n].end.y - edges[(i + 1) % n].start.y;
            if (a * d - b * c >= 0){
                break;
            }
        }
        i++;
        i %= n;
        std::rotate(edges.begin(), edges.begin() + i, edges.end());
        std::rotate(vertices.begin(), vertices.begin() + i, vertices.end());
        std::pair<int,int> curr = {-1, -1};
        for (int i = 0; i < n; i++){
            if (curr.first == -1) {
                curr.first = i;
            }
            long double a = edges[i % n].end.x - edges[i % n].start.x,
                        b = edges[i % n].end.y - edges[i % n].start.y,
                        c = edges[(i + 1) % n].end.x - edges[(i + 1) % n].start.x,
                        d = edges[(i + 1) % n].end.y - edges[(i + 1) % n].start.y;
            if (a * d - b * c >= 0){
                curr.second = i;
                chains.push_back(curr);
                curr = {-1, -1};
            }
        }
    }
};

bool isOn(Point p, Polygon P){
    for (Segment s : P.edges){
        if (isOn(p, s)){
            return true;
        }
    }
    return false;
}

// Function to check if point P lies inside (or on) polygon P
bool isInside(Point p, Polygon P){
    // Special case: On the polygon
    if (isOn(p, P)){
        return true;
    }
    // Ray casting method : Check "flux" of the point along a ray emanating from it
    // If it intersects a boundary odd number of times, it's inside
    // Choose the ray as x >= p.x, y = p.y
    bool inside = false;
    for (Segment e : P.edges){
        Point inter = intersection(Line(p, Point(p.x + 1, p.y)), e);
        if (not isnan(inter)){
            if (inter.x >= p.x){
                inside = not inside;
            }
        }
    }
    // If the ray cuts a vertex, it is overcounted (both edges taken), so compensate for that
    for (Segment e : P.edges){
        if (abs(p.y - e.start.y) <= 1e-10 and e.start.x > p.x){
            inside = not inside;
        }
    }
    // Return final boolean answer
    return inside;
}


#endif //AXIFY_POLYGON_H

#include <bits/stdc++.h>

#include "Double.h"
#include "Point.h"
#include "Line.h"
#include "Polygon.h"
#include "Parabola.h"
#include "MedialEdge.h"

using namespace std;

bool isValid(Point p, Polygon P, int u, int i, int delta, bool strict = false){
    bool cond1, cond2;
    if (u % 2 == 1){
        cond1 = isValid(p, P.edges[u / 2], P.edges[(u / 2 + 1) % P.n]);
    }
    else {
        cond1 = isValid(p, P.edges[u / 2]);
    }
    if (i % 2 == 1){
        cond2 = isValid(p, P.edges[(i + P.n * 2) % (P.n * 2) / 2], P.edges[((i + P.n * 2) % (P.n * 2) / 2 + 1) % P.n]);
    }
    else {
        cond2 = isValid(p, P.edges[(i + P.n * 2) % (P.n * 2) / 2]);
    }
    if (strict) {
        return (cond1 and cond2 and (isInside(p, P) or isOn(p, P)));
    }
    else {
        return cond1 and cond2;
    }
}

Point closer(Point p, Point q, Point start){
    if (not isnan(p)){
        if (not isnan(q)){
            if (distance(p, start) < distance(q, start)){
                return p;
            }
            else {
                return q;
            }
        }
        else {
            return p;
        }
    }
    else {
        return q;
    }
}

Point validOverall(Polygon P, MedialEdge edge, variant<Ray, Parabola> curr, int u, int i, int delta){
    if (holds_alternative<Ray>(edge.shape)){
        if (holds_alternative<Ray>(curr)){
            Point p = intersection(get<Ray>(edge.shape), get<Ray>(curr));
            if (not isnan(p)){
                if (isValid(p, P, u, i, delta)){
                    return p;
                }
                else {
                    return nanPoint;
                }
            }
        }
        else if (holds_alternative<Parabola>(curr)){
            pair<Point, Point> pts = intersection(get<Parabola>(curr), get<Ray>(edge.shape));
            if (not isnan(pts.first)){
                if (not isValid(pts.first, P, u, i, delta)){
                    pts.first = nanPoint;
                }
            }
            if (not isnan(pts.second)){
                if (not isValid(pts.second, P, u, i, delta)){
                    pts.second = nanPoint;
                }
            }
            return closer(pts.first, pts.second, get<Parabola>(curr).start);
        }
    }
    
    else if (holds_alternative<Segment>(edge.shape)){
        if (holds_alternative<Ray>(curr)){
            Point p = intersection(get<Ray>(curr), get<Segment>(edge.shape));
            if (not isnan(p)){
                if (isValid(p, P, u, i, delta)){
                    return p;
                }
                else {
                    return nanPoint;
                }
            }
        }
        else if (holds_alternative<Parabola>(curr)){
            pair<Point, Point> pts = intersection(get<Parabola>(curr), get<Segment>(edge.shape));
            if (not isnan(pts.first)){
                if (not isValid(pts.first, P, u, i, delta)){
                    pts.first = nanPoint;
                }
            }
            if (not isnan(pts.second)){
                if (not isValid(pts.second, P, u, i, delta)){
                    pts.second = nanPoint;
                }
            }
            return closer(pts.first, pts.second, get<Parabola>(curr).start);
        }
    }
    return nanPoint;
}

void medialAxisMerge(const Polygon &P, int s1, int e1, int s2, int e2,
                     map<pair<int,int>, MedialEdge> &medial_edges){
    int u = P.chains[e1].second * 2, v = P.chains[s2].first * 2;
    variant<Ray, Parabola> curr;
    Point start = P.edges[u / 2].end;
    pair<int,int> prev_left = {u, u + 1}, prev_right = {v, v - 1};
    map<pair<int,int>, MedialEdge> merge_line;
    while (u >= P.chains[s1].first * 2 - 1 and v <= P.chains[e2].second * 2 + 1){
        if (u == 0 and v == 2 * (P.n - 1)){
            Segment s(start, P.vertices[P.n - 1]);
            MedialEdge me(s, s.start, s.end);
            merge_line[{u, v}] = me;
            break;
        }
        Point p, q, p_old, q_old;
        if (u % 2 == 1){
            if (v % 2 == 1){
                curr = bisector(P.vertices[u / 2], P.vertices[v / 2], start);
            }
            else {
                curr = bisector(P.vertices[u / 2], P.edges[v / 2], start);
            }
        }
        else {
            if (v % 2 == 1){
                curr = bisector(P.vertices[v / 2], P.edges[u / 2], start);
            }
            else {
                curr = bisector(P.edges[u / 2], P.edges[v / 2], start);
            }
        }
        if (holds_alternative<Ray>(curr)){
            Ray r = get<Ray>(curr);
        }
        
        int x = -1;
        MedialEdge edge_left;
        for (int i = prev_left.second - 1; i >= P.chains[s1].first * 2 - 1; i--){
            if (i == u){
                continue;
            }
            prev_left = {u, (i + P.n * 2) % (P.n * 2)};
            pair<int,int> key = {u, (i + P.n * 2) % (P.n * 2)};
            if (key.first > key.second){
                swap(key.first, key.second);
            }
            if (medial_edges.find(key) != medial_edges.end()){
                MedialEdge edge = medial_edges[key];
                p = validOverall(P, edge, curr, u, i, 1);
                if (not isnan(p)){
                    if (x == -1 or distance(p, start) < distance(p_old, start)){
                        x = (i + P.n * 2) % (P.n * 2);
                        edge_left = edge;
                        p_old = p;
                    }
                }
            }
        }
        p = p_old;
        int y = -1;
        MedialEdge edge_right;
        for (int i = prev_right.second + 1; i <= P.chains[e2].second * 2 + 1; i++){
            if (i == v){
                continue;
            }
            prev_right = {v, i % (P.n * 2)};
            pair<int,int> key = {v, i % (P.n * 2)};
            if (key.first > key.second){
                swap(key.first, key.second);
            }
            if (medial_edges.find(key) != medial_edges.end()){
                MedialEdge edge = medial_edges[key];
                q = validOverall(P, edge, curr, v, i % (P.n * 2), -1);
                if (not isnan(q)){
                    if (y == -1 or distance(q, start) < distance(q_old, start)){
                        y = i % (P.n * 2);
                        edge_right = edge;
                        q_old = q;
                    }
                }
            }
        }
        q = q_old;
        MedialEdge merge_line_curr;
        if (x == -1 or y == -1){
            if (holds_alternative<Ray>(curr)) {
                merge_line_curr = MedialEdge(get<Ray>(curr), start);
            }
            else if (holds_alternative<Parabola>(curr)){
                merge_line_curr = MedialEdge(get<Parabola>(curr), start);
            }
            merge_line[{u, v}] = merge_line_curr;
            break;
        }
        Point final = closer(p, q, start);
        if (holds_alternative<Ray>(curr)) {
            Segment s(start, final);
            merge_line_curr = MedialEdge(s, start, final);
        }
        else if (holds_alternative<Parabola>(curr)){
            get<Parabola>(curr).end = final;
            merge_line_curr = MedialEdge(get<Parabola>(curr), start, final);
        }
        merge_line[{u, v}] = merge_line_curr;
        start = final;
        if (final == p){
            if (holds_alternative<Ray>(edge_left.shape)){
                Segment s(get<Ray>(edge_left.shape).start, final);
                edge_left = MedialEdge(s, s.start, s.end);
            }
            else if (holds_alternative<Segment>(edge_left.shape)){
                get<Segment>(edge_left.shape).end = final;
            }
            else if (holds_alternative<Parabola>(edge_left.shape)){
                get<Parabola>(edge_left.shape).end = final;
            }
            pair<int,int> key = {u, x};
            if (key.first > key.second){
                swap(key.first, key.second);
            }
            merge_line[key] = edge_left;
            u = x;
        }
        else {
            if (holds_alternative<Ray>(edge_right.shape)){
                Segment s(get<Ray>(edge_right.shape).start, final);
                edge_right = MedialEdge(s, s.start, s.end);
            }
            else if (holds_alternative<Segment>(edge_right.shape)){
                get<Segment>(edge_right.shape).end = final;
            }
            else if (holds_alternative<Parabola>(edge_right.shape)){
                get<Parabola>(edge_right.shape).end = final;
            }
            pair<int,int> key = {v, y};
            if (key.first > key.second){
                swap(key.first, key.second);
            }
            merge_line[key] = edge_right;
            v = y;
        }
    }
    
    for (auto &pr : merge_line){
        medial_edges[pr.first] = pr.second;
    }
}

void medialAxisRecursive(const Polygon &P, int s, int e,
                         map<pair<int,int>, MedialEdge> &medial_edges){
    if (s == e){
        for (int i = 2 * P.chains[s].first; i <= 2 * P.chains[s].second; i += 2){
            Ray r;
            MedialEdge me;
            Double a, b, c, d;
            a = P.edges[i / 2].end.x - P.edges[i / 2].start.x;
            b = P.edges[i / 2].end.y - P.edges[i / 2].start.y;
            c = P.edges[(i / 2 + 1) % P.n].end.x - P.edges[(i / 2 + 1) % P.n].start.x;
            d = P.edges[(i / 2 + 1) % P.n].end.y - P.edges[(i / 2 + 1) % P.n].start.y;
            if (a * c + b * d > 0) {
                r = bisectorLocal(P.edges[i / 2], P.edges[i / 2].end);
                me = MedialEdge(r, P.edges[i / 2].end);
                medial_edges[{i, (i + 1) % (P.n * 2)}] = me;
            }
            c = P.edges[(i / 2 - 1 + P.n) % P.n].end.x - P.edges[(i / 2 - 1 + P.n) % P.n].start.x;
            d = P.edges[(i / 2 - 1 + P.n) % P.n].end.y - P.edges[(i / 2 - 1 + P.n) % P.n].start.y;
            if (a * c + b * d > 0) {
                r = bisectorLocal(P.edges[i / 2], P.edges[i / 2].start);
                me = MedialEdge(r, P.edges[i / 2].start);
                medial_edges[{(i - 1 + P.n * 2) % (P.n * 2) , i}] = me;
            }
        }
        return;
    }
    medialAxisRecursive(P, s, (s + e) / 2, medial_edges);
    medialAxisRecursive(P, (s + e) / 2 + 1, e, medial_edges);
    medialAxisMerge(P, s, (s + e) / 2, (s + e) / 2 + 1, e, medial_edges);
}

//todo: bug fixes

int main(){
    Point p6(3, 3), p5(6, 8), p4(11, 9), p3(22, 6), p2(18, 2), p1(4, 2);
    Segment s1(p1, p2), s2(p2, p3), s3(p3, p4), s4(p4, p5), s5(p5, p6), s6(p6, p1);
    vector<Segment> s = {s1, s2, s3, s4, s5, s6};
    Polygon P(s);
    for (auto element : P.vertices){
        cout << element.x << ' ' << element.y << endl;
    }
    cout << endl;
    map<pair<int,int>, MedialEdge> medial_edges;
    
    medialAxisRecursive(P, 0, P.chains.size() - 1, medial_edges);
    for (auto pr : medial_edges){
        auto me = pr.second.shape;
        if (holds_alternative<Segment>(me)){
            cout << "SEGMENT" << endl;
            cout << get<Segment>(me).start.x << ' ' << get<Segment>(me).start.y << endl;
            cout << get<Segment>(me).end.x << ' ' << get<Segment>(me).end.y << endl;
        }
    }
}
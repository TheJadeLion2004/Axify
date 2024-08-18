#include <bits/stdc++.h>

using namespace std;

class Point{
public:
    long double x, y;
    template<typename T>
    Point(T a, T b){
        x = a;
        y = b;
    }
    Point(){
        x = 0;
        y = 0;
    }
    
    // Overloaded boolean operators
    bool operator==(Point const &p) const{
        // Accounts for floating point errors
        return ((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) <= 1e-10);
    }
    bool operator!=(Point const &p) const{
        return not (*this == p);
    }
    bool operator<(Point const &p) const{
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
    bool operator>(Point const &p) const{
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
};

bool isEqual(long double a, long double b, int precision = 10){
    return abs(a - b) <= pow(10, -precision);
}

class Line{
public:
    // ax + by + c = 0
    long double a, b, c;
    Line operator=(Line l){
        Line l1(l.a, l.b, l.c);
        return l1;
    }
    Line(long double A, long double B, long double C){
        a = A;
        b = B;
        c = C;
    }
    Line(Point p, Point q){
        a = q.y - p.y;
        b = p.x - q.x;
        c = q.x * p.y - p.x * q.y;
    }
    Line(){
        a = 0;
        b = 0;
        c = 0;
    }
};

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
};

class Polygon{
public:
    vector<Segment> edges;
    Polygon() {}
    Polygon(vector<Segment> &sides){
        edges = sides;
    }
};

bool isParallel(Line l1, Line l2){
    return (l1.a * l2.b == l1.b * l2.a);
}

template<typename T1 = Line, typename T2 = Line>
pair<Line,Line> angleBisectors(T1 l1, T2 l2){
    pair<Line,Line> pr;
    if (isParallel(l1, l2)){
        if (l2.a != 0) {
            pr.first.a = pr.second.a = l1.a;
            pr.first.b = pr.second.b = l1.b;
            pr.first.c = pr.second.c = (l1.c * l2.a + l2.c * l1.a) / (2 * l2.a);
        }
        else {
            pr.first.a = pr.second.a = l1.a;
            pr.first.b = pr.second.b = l1.b;
            pr.first.c = pr.second.c = (l1.c * l2.b + l2.c * l1.b) / (2 * l2.b);
        }
        return pr;
    }
    else {
        long double d1 = sqrt(l1.a * l1.a + l1.b * l1.b), d2 = sqrt(l2.a * l2.a + l2.b * l2.b);
        pr.first.a = l1.a * d2 - l2.a * d1;
        pr.second.a = l1.a * d2 + l2.a * d1;
        pr.first.b = l1.b * d2 - l2.b * d1;
        pr.second.b = l1.b * d2 + l2.b * d1;
        pr.first.c = l1.c * d2 - l2.c * d1;
        pr.second.c = l1.c * d2 + l2.c * d1;
        return pr;
    }
}

template<typename T1 = Line, typename T2 = Line>
Point intersection(T1 l1, T2 l2){
    Point p;
    if (isEqual(l1.a * l2.b, l2.a * l1.b)){
        p.x = p.y = numeric_limits<long double>::quiet_NaN();
    }
    else {
        p.x = (l1.b * l2.c - l2.b * l1.c) / (l1.a * l2.b - l2.a * l1.b);
        p.y = (l1.c * l2.a - l2.c * l1.a) / (l1.a * l2.b - l2.a * l1.b);
    }
    return p;
}

bool intersects(Line l, Segment e){
    if (isParallel(l, e)){
        return false;
    }
    Point p = intersection(l, e);
    return (((p.x - e.start.x) * (p.x - e.end.x) <= 0)
            and ((p.y - e.start.y) * (p.y - e.end.y) <= 0));
}

bool intersects(Segment l, Segment e){
    if (isParallel(l, e)){
        return false;
    }
    Point p = intersection(l, e);
    return (((p.x - e.start.x) * (p.x - e.end.x) <= 0)
        and ((p.y - e.start.y) * (p.y - e.end.y) <= 0)
        and ((p.x - l.start.x) * (p.x - l.end.x) <= 0)
        and ((p.y - l.start.y) * (p.y - l.end.y) <= 0));
}

bool intersects(Line l, Line e){
    return not isParallel(l, e);
}

// Function to check if point p lies on the boundary of polygon P
bool isOn(Point p, Polygon P){
    for (Segment e : P.edges){
        if ((e.end.y - e.start.y) * (p.x - e.start.x) == (p.y - e.start.y) * (e.end.x - e.start.x)){
            if ((e.start.x <= p.x and p.x <= e.end.x and e.start.y <= p.y and p.y <= e.end.y)
                or (e.start.x >= p.x and p.x >= e.end.x and e.start.y >= p.y and p.y >= e.end.y)){
                return true;
            }
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
    pair<bool, Point> pr;
    bool inside = false;
    for (Segment e : P.edges){
        if (intersects(Line(p, Point(p.x + 1, p.y)), e)){
            if (intersection(Line(p, Point(p.x + 1, p.y)), e).x >= p.x){
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

template <typename T>
long double distance(Point p, T l){
    return abs(l.a * p.x + l.b * p.y + l.c) / sqrt(l.a * l.a + l.b * l.b);
}


int main(){
    int n;
    cin >> n;
    vector<Point> vertices(n);
    for (int i = 0; i < n; i++){
        cin >> vertices[i].x >> vertices[i].y;
    }
    vector<Segment> sides;
    for (int i = 0; i < n; i++){
        auto s = Segment(vertices[i], vertices[(i + 1) % n]);
        sides.push_back(s);
    }
    Polygon poly(sides);
    
    vector<pair<Line,pair<int,int>>> all_bisectors;
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            auto bisectors = angleBisectors(poly.edges[i], poly.edges[j]);
            
            int c1 = 0, c2 = 0;
            for (int k = 0; k < n; k++){
                if (k == i or k == j){
                    continue;
                }
                if (intersects(bisectors.first, poly.edges[k])){
                    c1++;
                }
                if (intersects(bisectors.second, poly.edges[k])){
                    c2++;
                }
            }
            if (c1 > c2){
                all_bisectors.push_back({bisectors.first, {i, j}});
            }
            else {
                all_bisectors.push_back({bisectors.second, {i, j}});
            }
        }
    }
    int m = all_bisectors.size();
    vector<Segment> all_segments;
    for (int i = 0; i < m; i++){
        vector<Point> endpoints;
        if (all_bisectors[i].second.first == (all_bisectors[i].second.second + 1) % n) {
            endpoints.push_back(poly.edges[all_bisectors[i].second.second].end);
        }
        else if (all_bisectors[i].second.second == (all_bisectors[i].second.first + 1) % n) {
            endpoints.push_back(poly.edges[all_bisectors[i].second.first].end);
        }
        for (int J = i + 1; J % m != i; J++){
            int j = J % m;
            if (intersects(all_bisectors[i].first, all_bisectors[j].first)){
                Point p = intersection(all_bisectors[i].first, all_bisectors[j].first);
                if (isInside(p, poly)){
                    int c = -2;
                    long double closest = distance(p, poly.edges[all_bisectors[i].second.first]);
                    for (int k = 0; k < n; k++){
                        long double d = distance(p, poly.edges[k]);
                        if (isEqual(d, closest, 5)){
                            c++;
                        }
                        else if (d < closest){
                            c = -3;
                            break;
                        }
                    }
                    if (c > 0){
                        endpoints.push_back(p);
                    }
                }
            }
        }
        sort(endpoints.begin(), endpoints.end());
        if (endpoints.size() > 1){
            all_segments.emplace_back(*endpoints.begin(), *endpoints.rbegin());
        }
    }
    
    vector<Segment> final_segments;
    for (auto s : all_segments){
        if (s.start != s.end){
            final_segments.push_back(s);
        }
    }
    
    ofstream output("line_segments.csv");
    for (auto s : final_segments){
        output << s.start.x << ',' << s.start.y << ',' << s.end.x << ',' << s.end.y << '\n';
    }
    output.close();
}
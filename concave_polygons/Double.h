#ifndef AXIFY_DOUBLE_H
#define AXIFY_DOUBLE_H

#include <cstdlib>
#include <limits>
#include <utility>
#include <cmath>

class Double {
private:
    long double value;
public:
    Double(){
        value = 0;
    }
    Double(const Double& v){
        value = v.value;
    }
    Double(long double v){
        value = v;
    }
    operator long double(){
        return value;
    }
    Double& operator = (const Double &d){
        if (this == &d){
            return *this;
        }
        value = d.value;
        return *this;
    }
    template<typename T>
    bool operator == (T const &d) const{
        return abs(value - Double(d).value) <= 1e-10;
    }
    template<typename T>
    bool operator != (T const &d) const{
        return not (*this == Double(d));
    }
    template<typename T>
    bool operator > (T const &d) const{
        return (*this != Double(d)) and value > Double(d).value;
    }
    template<typename T>
    bool operator < (T const &d) const{
        return (*this != Double(d)) and value < Double(d).value;
    }
    template<typename T>
    bool operator >= (T const &d) const{
        return not (*this < Double(d));
    }
    template<typename T>
    bool operator <= (T const &d) const{
        return not (*this > Double(d));
    }
    template<typename T>
    Double operator + (T const &d) const{
        return Double(value + Double(d).value);
    }
    template<typename T>
    Double& operator += (T const &d){
        value += Double(d).value;
        return *this;
    }
    template<typename T>
    Double operator - (T const &d) const{
        return Double(value - Double(d).value);
    }
    template<typename T>
    Double& operator -= (T const &d){
        value -= Double(d).value;
        return *this;
    }
    template<typename T>
    Double operator * (T const &d) const{
        return Double(value * Double(d).value);
    }
    template<typename T>
    Double& operator *= (T const &d){
        value *= Double(d).value;
        return *this;
    }
    template<typename T>
    Double operator / (T const &d) const{
        return Double(value / Double(d).value);
    }
    template<typename T>
    Double& operator /= (T const &d){
        value /= Double(d).value;
        return *this;
    }
};

Double Nan(std::numeric_limits<long double>::quiet_NaN());

int sign(Double x){
    if (x == Double(0)){
        return 0;
    }
    else if (x > Double(0)){
        return 1;
    }
    else {
        return -1;
    }
}

std::pair<Double, Double> quadratic(Double a, Double b, Double c){
    std::pair<Double, Double> ans;
    if (b * b < 4 * a * c or a == 0){
        ans.first = Nan;
        ans.second = Nan;
    }
    else {
        ans.first = (-b + sqrt(b * b - 4 * a * c)) / (2 * a);
        ans.second = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);
    }
    return ans;
}


#endif //AXIFY_DOUBLE_H

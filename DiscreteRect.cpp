//
//  DiscreteRect.cpp
//  Masonry
//

#include "DiscreteRect.h"

bool operator== (const DiscreteRect& r1, const DiscreteRect& r2) { return r1.x == r2.x && r1.y == r2.y && r1.width == r2.width && r1.height == r2.height; }

std::ostream & operator<< (std::ostream & str, DiscreteRect const & rect) {
    str << "{" << rect.x << " " << rect.y << "} {" << rect.width << " " << rect.height << "}";
    return str;
}
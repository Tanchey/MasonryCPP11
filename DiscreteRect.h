//
//  DiscreteRect.h
//  Masonry
//

#ifndef __Masonry__DiscreteRect__
#define __Masonry__DiscreteRect__

#include <iostream>

class DiscreteRect {
public:
    unsigned x, y, width, height;

    DiscreteRect (unsigned ax
                  , unsigned ay
                  , unsigned awidth
                  , unsigned aheight)
    : x(ax), y(ay), width(awidth), height(aheight) {}
    
    DiscreteRect (unsigned awidth
                  , unsigned aheight)
    :DiscreteRect(0, 0, awidth, aheight) {}

    DiscreteRect (DiscreteRect rect, unsigned ax, unsigned ay)
    :DiscreteRect(ax, ay, rect.width, rect.height) {}

    void incWidth () { width++; }
    void incHeight (unsigned const delta) { height += delta; }
    unsigned bottom () const { return y + height; }
    unsigned right () const { return x + width; }

    bool containsColumn (unsigned col) const { return x <= col && col < x + width; }

    bool isTopNeighborOf (DiscreteRect const &rect) const {return (x == rect.x) && (y + height == rect.y); }
    bool isLeftNeighborOf (DiscreteRect const &rect) const {return (x + width == rect.x) && (y == rect.y); }
};

std::ostream & operator<< (std::ostream & str, DiscreteRect const & rect);

bool operator== (const DiscreteRect& r1, const DiscreteRect& r2);

#endif /* defined(__Masonry__DiscreteRect__) */

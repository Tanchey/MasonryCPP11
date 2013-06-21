//
//  MasonryCore.h
//  Masonry
//

#ifndef __Masonry__MasonryCore__
#define __Masonry__MasonryCore__

#include <iostream>
#include <stdlib.h>
#include <vector>

#include "DiscreteRect.h"

std::vector<DiscreteRect> masonry(unsigned const gridWidth, std::vector<DiscreteRect> const &);

void strangeAndUglyTests();

#endif /* defined(__Masonry__MasonryCore__) */


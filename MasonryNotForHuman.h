//
//  MasonryNotForHuman.h
//  Masonry
//

#ifndef __Masonry__MasonryNotForHuman__
#define __Masonry__MasonryNotForHuman__

#include <iostream>
#include <stdlib.h>
#include <vector>

#include "DiscreteRect.h"

std::vector<DiscreteRect> masonry(unsigned const gridWidth, std::vector<DiscreteRect> const &);

std::vector<DiscreteRect> masonrySmoothEdges(unsigned const gridWidth, std::vector<DiscreteRect> const &photos);

void strangeAndUglyTests();
unsigned bottomInColumn (unsigned const col
                         , std::vector<DiscreteRect> const &rects);
std::vector<unsigned> lowerEdgeShape (unsigned const gridWidth
                                      , std::vector<DiscreteRect> const &rects);
    
unsigned freeYAtColumnForWidth (unsigned const col
                                , unsigned const width
                                , std::vector<unsigned> const &shape);
std::vector<unsigned> freePoints (unsigned const width
                                  , std::vector<unsigned> const &shape);
DiscreteRect findFirstGap (DiscreteRect const &rect
                           , std::vector<unsigned> const &shape);
std::vector<DiscreteRect>::iterator findTopNeighbor (DiscreteRect const &gap
                                                     , std::vector<DiscreteRect> &rects);
std::vector<DiscreteRect>::iterator findLeftNeighbor (DiscreteRect const &gap
                                                      , std::vector<DiscreteRect> &rects);
void layoutIteration (unsigned const gridWidth, std::vector<DiscreteRect> &rects, DiscreteRect const &photo);

void eliminateGap (unsigned const gridWidth
                   , DiscreteRect const &gap
                   , std::vector<DiscreteRect> &rects
                   , DiscreteRect const &newRect);
void refine (unsigned const gridWidth
             , std::vector<DiscreteRect> &rects
             , DiscreteRect const &newRect
             , std::vector<unsigned> const &shape);
std::vector<DiscreteRect> masonry(unsigned const gridWidth, std::vector<DiscreteRect> const &photos);

#endif /* defined(__Masonry__MasonryNotForHuman__) */

//
//  MasonryCore.cpp
//  Masonry
//

#include "MasonryCore.h"
#include <iterator>
#include <algorithm>

unsigned bottomInColumn (unsigned const col
                         , std::vector<DiscreteRect> const &rects)
{
    auto bottomRect = std::max_element(rects.begin()
                            , rects.end()
                            , [col](DiscreteRect r1, DiscreteRect r2){return r2.containsColumn(col) && ((r1.bottom() < r2.bottom()) || !r1.containsColumn(col)); });
    if (bottomRect != rects.end() && bottomRect->containsColumn(col)) return bottomRect->bottom();
    return 0;
}

std::vector<unsigned> lowerEdgeShape (unsigned const gridWidth
                                      , std::vector<DiscreteRect> const &rects)
{
    std::vector<unsigned> shape = std::vector<unsigned> (gridWidth);

    int i = 0;
    std::generate(shape.begin(), shape.end(), [&i, rects](){ return bottomInColumn(i++, rects); });

    return shape;
}

unsigned freeYAtColumnForWidth (unsigned const col
                                , unsigned const width
                                , std::vector<unsigned> const &shape)
{
    if (shape.size() < col + width) {
        throw "col + w > shape";
    }

    return *std::max_element(shape.begin() + col, shape.begin() + col + width);
}

std::vector<unsigned> freePoints (unsigned const width
                                  , std::vector<unsigned> const &shape)
{
    std::vector<unsigned> points = std::vector<unsigned> (shape.size() - width + 1);
    int i = 0;
    std::generate(points.begin(), points.end(), [&i, width, shape](){ return freeYAtColumnForWidth(i++, width, shape); });

    return points;
}

DiscreteRect findFirstGap (DiscreteRect const &rect
                           , std::vector<unsigned> const &shape)
{
    auto firstGap = std::min_element(shape.begin() + rect.x, shape.begin() + rect.x + rect.width);
    return DiscreteRect(static_cast<unsigned>(std::distance(shape.begin(), firstGap))
                        , *firstGap
                        , 1
                        , rect.y - *firstGap);
}

std::vector<DiscreteRect>::iterator findTopNeighbor (DiscreteRect const &gap
                                                     , std::vector<DiscreteRect> &rects)
{
    return std::find_if(rects.begin()
                         , rects.end()
                         , [gap](DiscreteRect const &rect){ return rect.isTopNeighborOf(gap); });
}

std::vector<DiscreteRect>::iterator findLeftNeighbor (DiscreteRect const &gap
                                                      , std::vector<DiscreteRect> &rects)
{
    return std::find_if(rects.begin()
                         , rects.end()
                         , [gap](DiscreteRect const &rect){ return rect.isLeftNeighborOf(gap); });
}

void layoutIteration (unsigned const gridWidth, std::vector<DiscreteRect> &rects, DiscreteRect const &photo);

void eliminateGap (unsigned const gridWidth
                   , DiscreteRect const &gap
                   , std::vector<DiscreteRect> &rects
                   , DiscreteRect const &newRect)
{
    std::vector<DiscreteRect>::iterator topNeighbor = findTopNeighbor(gap, rects);
    if (topNeighbor != rects.end()) {
        topNeighbor->incHeight(gap.height);
        rects.push_back(newRect);
    } else {
        std::vector<DiscreteRect>::iterator leftNeighbor = findLeftNeighbor(gap, rects);
        if (leftNeighbor == rects.end()) {
            throw "No neighbours for gap";
        }
        leftNeighbor->incWidth();
        layoutIteration(gridWidth, rects, newRect);
    }
}

void refine (unsigned const gridWidth
             , std::vector<DiscreteRect> &rects
             , DiscreteRect const &newRect
             , std::vector<unsigned> const &shape)
{
    DiscreteRect gap = findFirstGap(newRect, shape);
    if (gap.height) {
        eliminateGap(gridWidth, gap, rects, newRect);
    } else {
        rects.push_back(newRect);
    }
}

void layoutIteration (unsigned const gridWidth
                      , std::vector<DiscreteRect> &rects
                      , DiscreteRect const &photo)
{
    std::vector<unsigned> shape = lowerEdgeShape(gridWidth, rects);
    std::vector<unsigned> freePlaces = freePoints(photo.width, shape);
    auto place = std::min_element(freePlaces.begin(), freePlaces.end());
    DiscreteRect newRect = DiscreteRect(photo, static_cast<unsigned>(std::distance(freePlaces.begin(), place)), *place);
    refine(gridWidth, rects, newRect, shape);
}

std::vector<DiscreteRect> masonry(unsigned const gridWidth, std::vector<DiscreteRect> const &photos)
{
    std::vector<DiscreteRect> rects = std::vector<DiscreteRect> ();
    std::for_each(photos.begin(), photos.end()
                  , [gridWidth, &rects](DiscreteRect const &photo){ layoutIteration(gridWidth, rects, photo); });
    return rects;
}

std::vector<DiscreteRect> masonrySmoothEdges(unsigned const gridWidth, std::vector<DiscreteRect> const &photos)
{
    std::vector<DiscreteRect> rects = masonry(gridWidth, photos);
    layoutIteration(gridWidth, rects, DiscreteRect(gridWidth, 1));
    rects.pop_back();
    return rects;
}


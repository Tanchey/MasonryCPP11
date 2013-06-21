//
//  MasonryCore.cpp
//  Masonry
//

#include "MasonryCore.h"
#include <iterator>
#include <algorithm>
#include <assert.h>

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

void strangeAndUglyTests()
{
    
    std::vector<DiscreteRect> someRects = std::vector<DiscreteRect> ();
    
    someRects.push_back(DiscreteRect(1, 3, 2, 4));
    someRects.push_back(DiscreteRect(2, 5, 1, 1));
    someRects.push_back(DiscreteRect(3, 7, 1, 5));
    
    std::vector<DiscreteRect> someLayoutedRects = std::vector<DiscreteRect> ();
    
    someLayoutedRects.push_back(DiscreteRect(0, 3, 2, 4));
    someLayoutedRects.push_back(DiscreteRect(2, 5, 1, 1));
    someLayoutedRects.push_back(DiscreteRect(3, 7, 1, 5));
    
    std::vector<DiscreteRect> somePhotos = std::vector<DiscreteRect> ();
    
    somePhotos.push_back(DiscreteRect(1, 1));
    somePhotos.push_back(DiscreteRect(2, 1));
    somePhotos.push_back(DiscreteRect(1, 5));
    
    std::vector<DiscreteRect> rectsForSomePhotos = std::vector<DiscreteRect> ();
    
    rectsForSomePhotos.push_back(DiscreteRect(0, 0, 2, 1));
    rectsForSomePhotos.push_back(DiscreteRect(0, 1, 2, 1));
    rectsForSomePhotos.push_back(DiscreteRect(0, 2, 1, 5));
    
    assert(!DiscreteRect(3, 0, 2, 0).containsColumn(1));
    assert(!DiscreteRect(3, 0, 2, 0).containsColumn(2));
    assert(DiscreteRect(3, 0, 2, 0).containsColumn(3));
    assert(DiscreteRect(3, 0, 2, 0).containsColumn(4));
    assert(!DiscreteRect(3, 0, 2, 0).containsColumn(5));
    assert(!DiscreteRect(3, 0, 2, 0).containsColumn(6));
    assert(!DiscreteRect(3, 0, 2, 0).containsColumn(7));

    assert(8 == DiscreteRect(0, 3, 0, 5).bottom());

    assert(0 == bottomInColumn(0, someRects));
    assert(7 == bottomInColumn(1, someRects));
    assert(7 == bottomInColumn(2, someRects));
    assert(12 == bottomInColumn(3, someRects));
    assert(0 == bottomInColumn(4, someRects));
    assert(0 == bottomInColumn(5, someRects));
    assert(0 == bottomInColumn(0, {}));

    {
        std::vector<unsigned> shape = lowerEdgeShape(4, someRects);
        unsigned templateShape[] = {0, 7, 7, 12};
        
        assert(std::equal(shape.begin()
                          , shape.end()
                          , templateShape
                          , [](unsigned const x, unsigned const y){ return x == y; }));
    }

    {
        std::vector<unsigned> shape = lowerEdgeShape(4, someLayoutedRects);
        unsigned templateShape[] = {7, 7, 6, 12};

        assert(std::equal(shape.begin()
                          , shape.end()
                          , templateShape
                          , [](unsigned const x, unsigned const y){ return x == y; }));
    }

    assert(7 == freeYAtColumnForWidth(0, 2, {0, 7, 7}));
    assert(0 == freeYAtColumnForWidth(0, 1, {0, 7, 7}));

    {
        std::vector<DiscreteRect> rects = {
            DiscreteRect(0, 0, 1, 2)
            , DiscreteRect(1, 0, 1, 1)};
        
        std::vector<DiscreteRect> rectsEliminatedGaps = {
            DiscreteRect(0, 0, 1, 2)
            , DiscreteRect(1, 0, 1, 2)
            , DiscreteRect(0, 2, 2, 1)};
        
        DiscreteRect gap = DiscreteRect(1, 1, 1, 1);
        DiscreteRect rect = DiscreteRect(0, 2, 2, 1);
        
        eliminateGap(2, gap, rects, rect);
        
        assert(std::equal(rects.begin(), rects.end(), rectsEliminatedGaps.begin(), [](DiscreteRect const r1, DiscreteRect const r2){ return r1 == r2; }));
    }
    
    {
        std::vector<DiscreteRect> rects = {
            DiscreteRect(0, 0, 1, 1)};
        
        std::vector<DiscreteRect> rectsEliminatedGaps = {
            DiscreteRect(0, 0, 2, 1)
            , DiscreteRect(0, 1, 2, 1)};
        
        DiscreteRect gap = DiscreteRect(1, 0, 1, 1);
        DiscreteRect rect = DiscreteRect(0, 1, 2, 1);
        
        eliminateGap(2, gap, rects, rect);
        
        assert(std::equal(rects.begin(), rects.end(), rectsEliminatedGaps.begin(), [](DiscreteRect const r1, DiscreteRect const r2){ return r1 == r2; }));
    }

    assert(DiscreteRect(2, 0, 1, 1) == findFirstGap(DiscreteRect(1, 1, 2, 1), {1, 1, 0, 1}));
    assert(DiscreteRect(0, 0, 1, 1) == findFirstGap(DiscreteRect(0, 1, 3, 1), {0, 1, 0, 1}));
    assert(DiscreteRect(0, 0, 1, 1) == findFirstGap(DiscreteRect(0, 1, 2, 1), {0, 1, 1, 1}));
    assert(DiscreteRect(1, 1, 1, 0) == findFirstGap(DiscreteRect(1, 1, 2, 1), {1, 1, 1, 1}));

    {
        std::vector<unsigned> freePlacesTest = {7, 7, 12};
        std::vector<unsigned> freePlaces = freePoints(2, {0, 7, 7, 12});
        assert(std::equal(freePlacesTest.begin(), freePlacesTest.end(), freePlaces.begin(), [](unsigned const x, unsigned const y){ return x == y; }));
    }
    
    {
        std::vector<unsigned> freePlacesTest = {1, 1, 1};
        std::vector<unsigned> freePlaces = freePoints(2, {0, 1, 0, 1, 0});
        assert(std::equal(freePlacesTest.begin(), freePlacesTest.end(), freePlaces.begin(), [](unsigned const x, unsigned const y){ return x == y; }));
    }
    
    {
        std::vector<unsigned> freePlacesTest = {0, 7, 7, 12};
        std::vector<unsigned> freePlaces = freePoints(1, {0, 7, 7, 12});
        assert(std::equal(freePlacesTest.begin(), freePlacesTest.end(), freePlaces.begin(), [](unsigned const x, unsigned const y){ return x == y; }));
    }

/*-------- Layout iteration ---------*/
    {
        std::vector<DiscreteRect> rects = {DiscreteRect(0, 0, 2, 42)};
        std::vector<DiscreteRect> outRects = std::vector<DiscreteRect>();
        
        layoutIteration(2, outRects, DiscreteRect(2, 42));
        assert(std::equal(rects.begin(), rects.end(), outRects.begin(), [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
    }
    {
        std::vector<DiscreteRect> rects = {DiscreteRect(0, 0, 2, 42), DiscreteRect(0, 42, 2, 42)};
        std::vector<DiscreteRect> outRects = {DiscreteRect(0, 0, 2, 42)};
        
        layoutIteration(2, outRects, DiscreteRect(2, 42));
        assert(std::equal(rects.begin(), rects.end(), outRects.begin(), [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
    }
    {
        std::vector<DiscreteRect> rects = {DiscreteRect(0, 0, 2, 42), DiscreteRect(2, 0, 2, 42)};
        std::vector<DiscreteRect> outRects = {DiscreteRect(0, 0, 2, 42)};
        
        layoutIteration(4, outRects, DiscreteRect(2, 42));
        assert(std::equal(rects.begin(), rects.end(), outRects.begin(), [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
    }
    {
        std::vector<DiscreteRect> rects = std::vector<DiscreteRect> (someLayoutedRects);
        rects.push_back(DiscreteRect(0, 7, 2, 42));
        std::vector<DiscreteRect> outRects = std::vector<DiscreteRect> (someLayoutedRects);
        
        layoutIteration(4, outRects, DiscreteRect(2, 42));
        assert(std::equal(rects.begin(), rects.end(), outRects.begin(), [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
    }
    {
        std::vector<DiscreteRect> rects = std::vector<DiscreteRect> (someLayoutedRects);
        rects.push_back(DiscreteRect(2, 6, 1, 42));
        std::vector<DiscreteRect> outRects = std::vector<DiscreteRect> (someLayoutedRects);
        
        layoutIteration(4, outRects, DiscreteRect(1, 42));
        assert(std::equal(rects.begin(), rects.end(), outRects.begin(), [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
    }


/*------------ Layout ----------------*/
    
    {
        std::vector<DiscreteRect> photos = {};
        std::vector<DiscreteRect> rects = {};
        assert(std::equal(rects.begin(), rects.end(), masonry(2, rects).begin(),  [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
    }
    {
        std::vector<DiscreteRect> photos = {DiscreteRect(1, 1)};
        std::vector<DiscreteRect> rects = {DiscreteRect(0, 0, 1, 1)};
        assert(std::equal(rects.begin(), rects.end(), masonry(2, rects).begin(),  [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
    }
    assert(std::equal(rectsForSomePhotos.begin(), rectsForSomePhotos.end(), masonry(2, somePhotos).begin(), [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
}

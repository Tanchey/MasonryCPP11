
#include "MasonryNotForHuman.h"
#include <assert.h>

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
    
    std::vector<DiscreteRect> smoothRectsForSomePhotos = std::vector<DiscreteRect> ();
    
    smoothRectsForSomePhotos.push_back(DiscreteRect(0, 0, 2, 1));
    smoothRectsForSomePhotos.push_back(DiscreteRect(0, 1, 2, 1));
    smoothRectsForSomePhotos.push_back(DiscreteRect(0, 2, 2, 5));
    
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
    {
        std::vector<DiscreteRect> photos = {DiscreteRect(1, 3)
        , DiscreteRect(1, 1)
        , DiscreteRect(1, 1)
        , DiscreteRect(3, 1)};
        std::vector<DiscreteRect> rects = {DiscreteRect(0, 0, 1, 3)
            , DiscreteRect(1, 0, 1, 3)
            , DiscreteRect(2, 0, 1, 3)
            , DiscreteRect(0, 3, 3, 1)};
        assert(std::equal(rects.begin(), rects.end(), masonry(3, rects).begin(), [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
    }
    assert(std::equal(rectsForSomePhotos.begin(), rectsForSomePhotos.end(), masonry(2, somePhotos).begin(), [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
    
    assert(std::equal(smoothRectsForSomePhotos.begin()
                      , smoothRectsForSomePhotos.end()
                      , masonrySmoothEdges(2, somePhotos).begin(), [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
    {
        std::vector<DiscreteRect> photos = {DiscreteRect(1, 3)
            , DiscreteRect(1, 1)
            , DiscreteRect(1, 1)};
        std::vector<DiscreteRect> rects = {DiscreteRect(0, 0, 1, 3)
            , DiscreteRect(1, 0, 1, 3)
            , DiscreteRect(2, 0, 1, 3)};
        assert(std::equal(rects.begin(), rects.end(), masonrySmoothEdges(3, rects).begin(), [](DiscreteRect const &r1, DiscreteRect const &r2){ return r1 == r2; }));
    }
    
}

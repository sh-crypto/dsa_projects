/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    MosaicCanvas* canvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    std::vector<Point<3>> avg;
    for(unsigned int i = 0; i < theTiles.size(); ++i) {
        avg.push_back(convertToXYZ(theTiles[i].getAverageColor()));
    }
    KDTree<3> tree(avg);
    for(auto row = 0; row < theSource.getRows(); row++) {
        for(auto col = 0; col < theSource.getColumns(); col++) {
            Point <3> r = convertToXYZ(theSource.getRegionColor(row, col));
            Point <3> color = tree.findNearestNeighbor(r);
            auto iterate = std::find(avg.begin(), avg.end(), color);
            if(iterate != avg.end()) {
                TileImage* ct = &theTiles[(iterate - avg.begin())];
                canvas->setTile(row, col, ct);
            }
        }
    }
    return canvas;
}


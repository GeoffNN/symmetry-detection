//
// Created by antoinewdg on 19/10/15.
//

#ifndef SYMMETRY_DETECTION_GEDT_H
#define SYMMETRY_DETECTION_GEDT_H


#include <utility>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <bounding_box.h>
#include "common/common.h"
#include "octree.h"
#include "util/tensor.h"
#include "geometry/grid.h"

#define GEDT_SIGMA2 1.73
#define GEDT_MAXD 4


class GEDT {
    typedef std::pair<Vec3i, Vec3i> Pair;
public:
    GEDT(Mesh &mesh, int treeDepth);


    double operator()(const Vec &p) const {
        return values(grid.coordsFromPoint(p));
    }

private:
    Mesh &mesh;
    BoundingBox boundingBox;

    Tensord values;
    cv::Mat image;
    OCTree tree;
    const Grid &grid;
    list<Vec3i> seeds;


    static Vec3i immediateNeighbor(int i) {
        return Vec3i((i < 2) * (2 * i - 1), (i >= 2) * (i < 4) * (2 * i - 5), (i >= 4) * (2 * i - 9));
    }

    void regionGrowth(const Vec3i &c, const Vec3i &seed, double maxD2, double sigma2);

    void initializeSeeds();

    void normalize();

    bool areCoordsInGrid(Vec3i c) {
        return c[0] >= 0 && c[0] < grid.size &&
               c[1] >= 0 && c[1] < grid.size &&
               c[2] >= 0 && c[2] < grid.size;
    }

public:

    const Tensord &getValues() const {
        return values;
    }


    const OCTree &getTree() const {
        return tree;
    }

    const Grid &getGrid() const {
        return grid;
    }
};

#endif //SYMMETRY_DETECTION_GEDT_H

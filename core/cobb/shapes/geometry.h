//
// Created by cobble on 11/15/2024.
//

#ifndef EWRENDER_GEOMETRY_HPP
#define EWRENDER_GEOMETRY_HPP

#include "mesh.h"

namespace cobb {
    void createSphere(float radius, int subDivisions, MeshData* mesh);
    void createAsteroid(float radius, float variability, int subDivisions, MeshData* mesh);
    void createBalancedSphere(float radius, int pointCount, MeshData* mesh);
}



#endif //EWRENDER_GEOMETRY_HPP

#include <VVRScene/mesh.h>
#include <MathGeoLib.h>
#include "meshCalculations.h"


void trivialTriangulation(std::vector<vvr::Triangle>& filled_tris_A, std::vector<int> boundary_vert_indices, std::vector<vec> vertices);

/*
* Distance-aware triangulation
* Create triangle with nearest vertices and if triangle already exists, create triangle with next 2 nearest in order to avoid gaps
*/
void simpleTriangulation(std::vector<vvr::Triangle>& filled_tris_A, std::vector<int> boundary_vert_indices, std::vector<vec> vertices);

void delauny3DTriangulation(std::vector<vvr::Triangle>& triangles);

bool checkDelauncyViolation(std::vector<vvr::Triangle> triangles, std::vector<Sphere>& spheres);
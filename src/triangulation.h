#include <VVRScene/mesh.h>
#include <MathGeoLib.h>


void trivialTriangulation(std::vector<vvr::Triangle>& filled_tris_A, std::vector<int> boundary_vert_indices, std::vector<vec> vertices);

/*
* Distance-aware triangulation
* Create triangle with nearest vertices and if triangle already exists, create triangle with next 2 nearest in order to avoid gaps
*/
void simpleTriangulation(std::vector<vvr::Triangle>& filled_tris_A, std::vector<int> boundary_vert_indices, std::vector<vec> vertices);
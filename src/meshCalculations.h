#include <VVRScene/canvas.h>
#include <VVRScene/mesh.h>
#include <VVRScene/settings.h>
#include <VVRScene/utils.h>
#include <MathGeoLib.h>


void getMeshAABB(std::vector<vec>& vertices, vvr::Box3D& aabb);
bool checkBoxCollision(vvr::Box3D box1, vvr::Box3D box2);
bool checkTriangleCollision3D(vvr::Triangle tri1, vvr::Triangle tri2);
bool checkTriangleCollision2D(C2DTriangle tri1, C2DTriangle tri2);
void getModelWithHoles(std::vector<int>& intersections, vvr::Mesh& mesh, std::vector<vvr::Triangle>& removed_triangles);
void getModelBoundaryVertices(std::vector<vvr::Triangle> removedTriangles, std::vector<int>& boundary_vertex_indices);
bool checkCommonEdge(vec v1, vec v2, vvr::Triangle other_tri);
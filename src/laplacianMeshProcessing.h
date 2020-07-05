#include <Eigen/Sparse>
#include <VVRScene/mesh.h>
#include <MathGeoLib.h>

using namespace Eigen;

typedef SparseMatrix<double, RowMajor> SpMat;

void getAdjacencyMatrix(std::vector<vvr::Triangle> triangles, SpMat A);
#include <Eigen/Sparse>
#include <VVRScene/mesh.h>
#include <MathGeoLib.h>
#include "meshCalculations.h"

using namespace Eigen;

typedef SparseMatrix<double, RowMajor> SpMat;
typedef DiagonalMatrix<double, RowMajor> DiagMat;
typedef Eigen::Triplet<double> T;

void getAdjacencyMatrix(std::vector<vvr::Triangle> triangles, std::vector<vec> vertices, SpMat& A);

void getVertexDegreeMatrix(SpMat A, SpMat& D);
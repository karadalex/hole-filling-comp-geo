#include <Eigen/Sparse>
#include<Eigen/SparseQR>
#include <VVRScene/mesh.h>
#include <MathGeoLib.h>
#include "meshCalculations.h"

using namespace Eigen;

typedef SparseMatrix<double, RowMajor> SpMat;
typedef SparseMatrix<double, ColMajor> SpMatC;
typedef DiagonalMatrix<double, RowMajor> DiagMat;
typedef Eigen::Triplet<double> T;

/**
 * Calculate inverse of a square Sparse Matrix
*/
void getSpMatInverse(SpMat A, SpMatC& A_inv);

/**
 * Calculate pseudo-inverse of a square Sparse Matrix
*/
void getSpMatPseudoInverse(SpMat A, SpMatC& A_pinv);

void getAdjacencyMatrix(std::vector<vvr::Triangle> triangles, std::vector<vec> vertices, SpMat& A);

void getVertexDegreeMatrix(SpMat A, SpMat& D);

void getLaplacianMatrix(SpMat A, SpMat D, SpMat& L);

/**
 * Mesh Reconstruction calculation:
 * Inputs:
 *  - SpMat anchoredL
 *  - std::vector<VectorXd> anchoredDCoords
 *  - std::vector<VectorXd>& xyzCoords std::vector containing the 3 calculated VectorXd vectors (x,y,z)
*/
void getSurfaceReconstructionFromDCoords(SpMat anchoredL, std::vector<VectorXd> anchoredDCoords, std::vector<VectorXd>& xyzCoords);
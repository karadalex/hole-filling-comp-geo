#include <Eigen/Sparse>
#include <Eigen/SparseQR>
#include <string>
#include <vector>
#include <iostream>

using namespace Eigen;


typedef SparseMatrix<double, RowMajor> SpMat;
typedef SparseMatrix<double, ColMajor> SpMatC;
typedef DiagonalMatrix<double, RowMajor> DiagMat;
typedef Eigen::Triplet<double> T;


void printVector(std::vector<int> vec, std::string vec_name);

void printVector(VectorXd vec);

void printNonZeroIndices(SpMat A);

/**
 * Calculate inverse of a square Sparse Matrix
*/
void getSpMatInverse(SpMat A, SpMatC& A_inv);

/**
 * Calculate pseudo-inverse of a square Sparse Matrix
*/
void getSpMatPseudoInverse(SpMat A, SpMatC& A_pinv);
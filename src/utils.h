#include <Eigen/Sparse>
#include <Eigen/SparseQR>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>

using namespace Eigen;


typedef SparseMatrix<double, RowMajor> SpMat;
typedef SparseMatrix<double, ColMajor> SpMatC;
typedef DiagonalMatrix<double, RowMajor> DiagMat;
typedef Eigen::Triplet<double> T;

typedef std::chrono::steady_clock::time_point time_point;
typedef std::chrono::microseconds nanos;
typedef std::chrono::microseconds micros;
typedef std::chrono::milliseconds millis;
typedef std::chrono::seconds secs;
typedef std::chrono::steady_clock steady_clock;


void printVector(std::vector<int> vec, std::string vec_name);

void printVector(VectorXd vec);

void printNonZeroIndices(SpMat A);

void printTime(time_point start, time_point stop);

/**
 * Calculate inverse of a square Sparse Matrix
*/
void getSpMatInverse(SpMat A, SpMatC& A_inv);

/**
 * Calculate pseudo-inverse of a square Sparse Matrix
*/
void getSpMatPseudoInverse(SpMat A, SpMatC& A_pinv);
#include "utils.h"

using namespace std;


void printVector(vector<int> vec, string vec_name = "vector") {
	cout << vec_name << " = ";
	for (int i = 0; i < vec.size(); i++) {
		cout << vec.at(i) << " ";
	}
	cout << endl;
}


void printVector(VectorXd vec) {
	int N = vec.size();
	for (int i = 0; i < N; i++) {
		std::cout << vec(i) << std::endl;
	}
}


void printNonZeroIndices(SpMat A) {
	int N = A.rows();
	for (int k = 0; k < A.outerSize(); ++k) {
		for (SpMat::InnerIterator it(A, k); it; ++it) {
			std::cout << "(" << it.row() << ",";
			std::cout << it.col() << ")\t";
		}
	}
}


void printMatrix(MatrixXd A) {
	int N = A.rows();
	int M = A.cols();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			std::cout << A(i, j) << "  ";
		}
		std::cout << std::endl;
	}
}


void printTime(time_point start, time_point stop) {
	long secs_duration = std::chrono::duration_cast<secs>(stop - start).count();
	long millis_duration = std::chrono::duration_cast<millis>(stop - start).count();
	long nanos_duration = std::chrono::duration_cast<nanos>(stop - start).count();

	std::cout << secs_duration << " seconds = " << millis_duration << " milliseconds = " << nanos_duration << " nanoseconds" << std::endl;
}


void getSpMatInverse(SpMat A, SpMatC& A_inv) {
	int N = A.cols();
	SpMatC I = *(new SpMatC(N, N));
	I.setIdentity();

	SpMatC Ac = A;
	Ac.makeCompressed();

	// Sparse Matrices in Eigen don't have inverse method, so we have to calculate it manually
	// We will use the SparseQR solver to solve for the inverse of D
	// According to Documentation: SparseQR: QR factorization for Square matrices 
	SparseQR<SpMatC, COLAMDOrdering<int>> solver;

	// Compute the ordering permutation vector from the structural pattern of D
	// An important parameter of this class is the ordering method. 
	// It is used to reorder the columns (and eventually the rows) of the matrix to reduce the number 
	// of new elements that are created during numerical factorization. The cheapest method available 
	// is COLAMD. See the OrderingMethods module for the list of built-in and external ordering methods.
	solver.analyzePattern(Ac);

	// Compute the numerical factorization 
	solver.factorize(Ac);

	A_inv = solver.solve(I);

	// Test inverse results
	//std::cout << D * D_inv << std::endl;
}


void getSpMatPseudoInverse(SpMat A, SpMatC& A_pinv) {
	SpMat A_T = A.transpose();

	SpMat squareA = A_T * A;
	SpMatC squareAInv;
	getSpMatInverse(squareA, squareAInv);

	A_pinv = squareAInv * (SpMatC)A_T;
}


void getEigenValuesAndVectors(SpMat M, VectorXd& eigenValues, MatrixXd& eigenVectors) {
	//find the eigen values and eiven vectors and store them
	SelfAdjointEigenSolver<SpMat> eigen(M);
	eigenValues = eigen.eigenvalues();
	eigenVectors = eigen.eigenvectors();
	printMatrix(eigenVectors);
}
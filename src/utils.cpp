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


void getSpMatInverse(SpMat A, SpMatC& A_inv) {
	int N = A.cols();
	SpMatC I = *(new SpMatC(N, N));
	I.setIdentity();

	A.makeCompressed();

	// Sparse Matrices in Eigen don't have inverse method, so we have to calculate it manually
	// We will use the SparseLU solver to solve for the inverse of D
	// According to Documentation: SparseQR: QR factorization for Square matrices 
	SparseQR<SpMatC, COLAMDOrdering<int>> solver;

	// Compute the ordering permutation vector from the structural pattern of D
	// An important parameter of this class is the ordering method. 
	// It is used to reorder the columns (and eventually the rows) of the matrix to reduce the number 
	// of new elements that are created during numerical factorization. The cheapest method available 
	// is COLAMD. See the OrderingMethods module for the list of built-in and external ordering methods.
	solver.analyzePattern((SpMatC)A);

	// Compute the numerical factorization 
	solver.factorize((SpMatC)A);

	A_inv = solver.solve(I);

	// Test inverse results
	//std::cout << D * D_inv << std::endl;
}


void getSpMatPseudoInverse(SpMat A, SpMatC& A_pinv) {
	SpMat A_T = A.transpose();

	SpMat squareA = A_T * A;
	SpMatC squareAInv;
	getSpMatInverse(squareA, squareAInv);

	A_pinv = squareAInv * A_T;
}
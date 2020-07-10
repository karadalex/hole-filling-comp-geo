#include "laplacianMeshProcessing.h"


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


void getAdjacencyMatrix(std::vector<vvr::Triangle> triangles, std::vector<vec> vertices, SpMat& A) {
	int N = vertices.size();
	A = *(new SpMat(N, N));

	//A.reserve(VectorXi::Constant(N, 6));
	for (int i = 0; i < triangles.size(); i++) {
		vvr::Triangle tri = triangles.at(i);
		int vi1 = tri.vi1;
		int vi2 = tri.vi2;
		int vi3 = tri.vi3;

		A.coeffRef(vi1, vi1) = 1;
		A.coeffRef(vi2, vi2) = 1;
		A.coeffRef(vi3, vi3) = 1;

		A.coeffRef(vi1, vi2) = 1;
		A.coeffRef(vi2, vi1) = 1;

		A.coeffRef(vi1, vi3) = 1;
		A.coeffRef(vi3, vi1) = 1;

		A.coeffRef(vi2, vi3) = 1;
		A.coeffRef(vi3, vi2) = 1;
	}
}


void getVertexDegreeMatrix(SpMat A, SpMat& D) {
	int N = A.cols();
	D = *(new SpMat(N, N));

	for (int i = 0; i < N; i++)
		D.coeffRef(i,i) = (A.col(i).sum()>0) ? A.col(i).sum() - 1 : 0;
}


void getLaplacianMatrix(SpMat A, SpMat D, SpMat& L) {
	int N = A.cols();
	SpMatC D_inv;
	getSpMatInverse(D, D_inv);

	L = *(new SpMat(N, N));
	L = D_inv * A;
}


void getSurfaceReconstructionFromDCoords(SpMat anchoredL, std::vector<VectorXd> anchoredDCoords, std::vector<VectorXd>& xyzCoords) {
	SpMatC L_pinv;
	getSpMatPseudoInverse(anchoredL, L_pinv);

	xyzCoords.clear();
	for (int i = 0; i < 3; i++) {
		VectorXd b = anchoredDCoords.at(i);
		VectorXd x;
		x = L_pinv * b;
		xyzCoords.push_back(x);
	}
}
#include "laplacianMeshProcessing.h"


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
	SpMatC I = *(new SpMatC(N, N));
	I.setIdentity();
	
	D.makeCompressed();

	// Sparse Matrices in Eigen don't have inverse method, so we have to calculate it manually
	// We will use the SparseLU solver to solve for the inverse of D
	// According to Documentation: SparseQR: QR factorization for Square matrices 
	SparseQR<SpMatC, COLAMDOrdering<int>> solver;

	// Compute the ordering permutation vector from the structural pattern of D
	// An important parameter of this class is the ordering method. 
	// It is used to reorder the columns (and eventually the rows) of the matrix to reduce the number 
	// of new elements that are created during numerical factorization. The cheapest method available 
	// is COLAMD. See the OrderingMethods module for the list of built-in and external ordering methods.
	solver.analyzePattern((SpMatC)D);

	// Compute the numerical factorization 
	solver.factorize((SpMatC)D);

	SpMatC D_inv = solver.solve(I);

	// Test inverse results
	//std::cout << D * D_inv << std::endl;

	L = *(new SpMat(N, N));
	L = D_inv * A;
}
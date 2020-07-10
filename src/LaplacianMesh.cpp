#include "LaplacianMesh.h"


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
	int N = A.rows();
	D = *(new SpMat(N, N));

	for (int i = 0; i < N; i++)
		D.coeffRef(i,i) = (A.row(i).nonZeros()>0) ? A.row(i).nonZeros() - 1 : 0;
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


LaplacianMesh::LaplacianMesh(std::vector<vvr::Triangle> triangles, std::vector<vec> vertices) 
	: triangles(triangles), vertices(vertices)
{
	getAdjacencyMatrix(triangles, vertices, A_adj);
	std::cout << A_adj << std::endl;
	getVertexDegreeMatrix(A_adj, Vertex_degrees);
	std::cout << Vertex_degrees << std::endl;
	getLaplacianMatrix(A_adj, Vertex_degrees, Laplacian);
	std::cout << Laplacian << std::endl;
}
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


void getDeltaCoordinates(SpMat A, SpMat D, std::vector<vec> vertices, std::vector<VectorXd>& deltaCoords) {
	int N = A.rows();

	VectorXd dCoordX(N), dCoordY(N), dCoordZ(N);
	dCoordX.setZero(); dCoordY.setZero(); dCoordZ.setZero();

	// Iterate over non-zero values of the Adjacency matrix and calculate delta Coordinates
	for (int i = 0; i < A.outerSize(); ++i) {
		vec vi = vertices.at(i);
		double di = D.coeffRef(i,i);
		vec dCoord_i = vec(0, 0, 0);

		if (di > 0) { // Continue if there are neighbors
			for (SpMat::InnerIterator it(A, i); it; ++it) {
				//it.row();   // row index (here it is equal to i, because SpMat is RowMajor)
				//it.col();   // col index
				//it.index(); // inner index, here it is equal to it.col()

				int j = it.col();
				if (i != j) {
					vec vj = vertices.at(j);
					dCoord_i += vi - vj;
				}
			}
			dCoord_i /= di;
		}

		dCoordX(i) = dCoord_i.x;
		dCoordY(i) = dCoord_i.y;
		dCoordZ(i) = dCoord_i.z;
	}

	deltaCoords.push_back(dCoordX);
	deltaCoords.push_back(dCoordY);
	deltaCoords.push_back(dCoordZ);

	//printVector(dCoordX); std::cout << std::endl;
	//printVector(dCoordY); std::cout << std::endl;
	//printVector(dCoordZ);
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

		//printVector(x); std::cout << std::endl;
	}
}


LaplacianMesh::LaplacianMesh(std::vector<vvr::Triangle> triangles, std::vector<vec> vertices) 
	: triangles(triangles), vertices(vertices)
{
	time_point start_time, stop_time;
	
	start_time = steady_clock::now();
	getAdjacencyMatrix(triangles, vertices, A_adj);
	stop_time = steady_clock::now();
	std::cout << "Adjacency matrix was calculated in "; printTime(start_time, stop_time);
	//std::cout << A_adj << std::endl;

	start_time = steady_clock::now();
	getVertexDegreeMatrix(A_adj, Vertex_degrees);
	stop_time = steady_clock::now();
	std::cout << "Vertex degree matrix was calculated "; printTime(start_time, stop_time);
	////std::cout << Vertex_degrees << std::endl;

	start_time = steady_clock::now();
	getLaplacianMatrix(A_adj, Vertex_degrees, Laplacian);
	stop_time = steady_clock::now();
	std::cout << "Laplacian matrix was calculated "; printTime(start_time, stop_time);
	////std::cout << Laplacian << std::endl;

	start_time = steady_clock::now();
	getDeltaCoordinates(A_adj, Vertex_degrees, vertices, deltaCoords);
	stop_time = steady_clock::now();
	std::cout << "Delta coordinates were calculated "; printTime(start_time, stop_time);

	start_time = steady_clock::now();
	getSurfaceReconstructionFromDCoords(Laplacian, deltaCoords, xyzCoords);
	stop_time = steady_clock::now();
	std::cout << "Surface reconstruction was calculated "; printTime(start_time, stop_time);
}
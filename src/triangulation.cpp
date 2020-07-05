#include "triangulation.h"


void simpleTriangulation(std::vector<vvr::Triangle>& filled_tris_A, std::vector<int> boundary_vert_indices, std::vector<vec> vertices) {
	for (int i = 0; i < boundary_vert_indices.size()-2; i+=2) {
		int vi1 = boundary_vert_indices.at(i);
		int vi2 = boundary_vert_indices.at(i+1);
		int vi3 = boundary_vert_indices.at(i+2);
		filled_tris_A.push_back(vvr::Triangle(&vertices, vi1, vi2, vi3));
	}
}
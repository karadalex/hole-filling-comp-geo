#include "triangulation.h"


void trivialTriangulation(std::vector<vvr::Triangle>& filled_tris_A, std::vector<int> boundary_vert_indices, std::vector<vec> vertices) {
	filled_tris_A.clear();
	for (int i = 0; i < boundary_vert_indices.size()-2; i+=2) {
		int vi1 = boundary_vert_indices.at(i);
		int vi2 = boundary_vert_indices.at(i+1);
		int vi3 = boundary_vert_indices.at(i+2);
		filled_tris_A.push_back(vvr::Triangle(&vertices, vi1, vi2, vi3));
	}
}


void simpleTriangulation(std::vector<vvr::Triangle>& filled_tris_A, std::vector<int> boundary_vert_indices, std::vector<vec> vertices) {
	filled_tris_A.clear();

	std::vector<int> original_vert_indices = boundary_vert_indices;

	for (int i = 0; i < original_vert_indices.size(); i++) {
		int vi1 = original_vert_indices.at(i);
		vec v1 = vertices.at(original_vert_indices.at(i));

		sort(boundary_vert_indices.begin(), boundary_vert_indices.end(), [vertices, v1](int vi2, int vi3) {
			vec v2 = vertices.at(vi2);
			vec v3 = vertices.at(vi3);
			return (v2.DistanceSq(v1) < v3.DistanceSq(v1));
		});

		for (int j = 1; j < 10; j++) {
			int vi2 = boundary_vert_indices.at(j);
			int vi3 = boundary_vert_indices.at(j+1);
			filled_tris_A.push_back(vvr::Triangle(&vertices, vi1, vi2, vi3));
		}
	}
}
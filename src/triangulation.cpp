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

	sort(boundary_vert_indices.begin(), boundary_vert_indices.end(), [vertices](int vi1, int vi2) {
		vec v1 = vertices.at(vi1);
		vec v2 = vertices.at(vi2);
		return (v1.Length() < v2.Length());
	});

	std::vector<int> vi1s, vi2s, vi3s;

	for (int i = 0; i < boundary_vert_indices.size()-3; i++) {
		int vi1 = boundary_vert_indices.at(i);
		vec v1 = vertices.at(boundary_vert_indices.at(i));
		
		int vi2 = boundary_vert_indices.at(i+1);
		int vi3 = boundary_vert_indices.at(i+2);
		vec v2_min = vertices.at(vi2);
		vec v3_min = vertices.at(vi3);
		double d2_min = v1.Distance(v2_min);
		double d3_min = v1.Distance(v3_min);
		for (int j = 0; j < boundary_vert_indices.size() && j!=i && j!=i+1 && j!=i+2; j++) {
			vec v = vertices.at(boundary_vert_indices.at(j));
			double d = v1.Distance(v);

			//bool triDoesNotAlreadyExist = (
			//	(
			//		find(vi1s.begin(), vi1s.end(), vi1) == vi1s.end() 
			//		&& find(vi2s.begin(), vi2s.end(), vi2) == vi2s.end() 
			//		&& find(vi3s.begin(), vi3s.end(), vi3) == vi3s.end()
			//	) || (
			//		find(vi1s.begin(), vi1s.end(), vi1) == vi1s.end()
			//		&& find(vi2s.begin(), vi2s.end(), vi3) == vi2s.end()
			//		&& find(vi3s.begin(), vi3s.end(), vi2) == vi3s.end()
			//		) || (
			//		find(vi1s.begin(), vi1s.end(), vi3) == vi1s.end()
			//		&& find(vi2s.begin(), vi2s.end(), vi2) == vi2s.end()
			//		&& find(vi3s.begin(), vi3s.end(), vi1) == vi3s.end()
			//	) || (
			//		find(vi1s.begin(), vi1s.end(), vi2) == vi1s.end()
			//		&& find(vi2s.begin(), vi2s.end(), vi1) == vi2s.end()
			//		&& find(vi3s.begin(), vi3s.end(), vi3) == vi3s.end()
			//	)
			//);
			bool triDoesNotAlreadyExist = true;
			if (d < d2_min && triDoesNotAlreadyExist) {
				d3_min = d2_min;
				d2_min = d;
				v3_min = v2_min;
				v2_min = v;
				vi3 = vi2;
				vi2 = boundary_vert_indices.at(j);
			}
		}

		if (d2_min < 3) {
			filled_tris_A.push_back(vvr::Triangle(&vertices, vi1, vi2, vi3));
			vi1s.push_back(vi1);
			vi2s.push_back(vi2);
			vi3s.push_back(vi3);
		}
	}
}
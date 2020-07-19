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


void delauny3DTriangulation(std::vector<vvr::Triangle>& triangles) {
	std::vector<vvr::Triangle> new_triangles;
	for (int i = 0; i < triangles.size(); i++) {
		vvr::Triangle tri1 = triangles.at(i);
		vec a = tri1.v1();
		vec b = tri1.v2();
		vec c = tri1.v3();

		Sphere sphere = getTriangleCircumsphere(tri1);

		for (int j = 0; j < triangles.size() && j!=i; j++) {
			vvr::Triangle tri2 = triangles.at(j);
			vec a2 = tri2.v1();
			vec b2 = tri2.v2();
			vec c2 = tri2.v3();


			if ((a.Equals(a2) && b.Equals(b2)) || (a.Equals(b2) && b.Equals(a2))) {
				// ab is common
				if (sphere.Contains(c2)) {
					// Flip common edge
					b = c2;
					tri1.vi2 = tri2.vi3;
					a2 = c;
					break;
				}
			} else if ((a.Equals(a2) && c.Equals(c2)) || (a.Equals(c2) && c.Equals(a2))) {
				// ac is common
				if (sphere.Contains(b2)) {
					// Flip common edge
					c = b2;
					tri1.vi3 = tri2.vi2;
					a2 = b;
					break;
				}
			} else if ((b.Equals(b2) && c.Equals(c2)) || (b.Equals(c2) && c.Equals(b2))) {
				// bc is common
				if (sphere.Contains(a2)) {
					// Flip common edge
					c = a2;
					tri1.vi3 = tri2.vi1;
					b2 = a;
					break;
				}
			}
		}

		new_triangles.push_back(tri1);
	}

	// Replace old triangles with new ones
	triangles.clear();
	for (int i = 0; i < new_triangles.size(); i++) {
		triangles.push_back(new_triangles.at(i));
	}
}


bool checkDelauncyViolation(std::vector<vvr::Triangle> triangles, std::vector<Sphere>& spheres) {
	bool thereAreViolations = false;

	for (int i = 0; i < triangles.size(); i++) {
		vvr::Triangle tri1 = triangles.at(i);
		vec a = tri1.v1();
		vec b = tri1.v2();
		vec c = tri1.v3();

		Sphere sphere = getTriangleCircumsphere(tri1);

		for (int j = 0; j < triangles.size() && j != i; j++) {
			vvr::Triangle tri2 = triangles.at(j);
			vec a2 = tri2.v1();
			vec b2 = tri2.v2();
			vec c2 = tri2.v3();


			if ((a.Equals(a2) && b.Equals(b2)) || (a.Equals(b2) && b.Equals(a2))) {
				// ab is common
				if (sphere.Contains(c2)) {
					thereAreViolations = true;
					spheres.push_back(sphere);
				}
			}
			else if ((a.Equals(a2) && c.Equals(c2)) || (a.Equals(c2) && c.Equals(a2))) {
				// ac is common
				if (sphere.Contains(b2)) {
					thereAreViolations = true;
					spheres.push_back(sphere);
				}
			}
			else if ((b.Equals(b2) && c.Equals(c2)) || (b.Equals(c2) && c.Equals(b2))) {
				// bc is common
				if (sphere.Contains(a2)) {
					thereAreViolations = true;
					spheres.push_back(sphere);
				}
			}
		}
	}

	return thereAreViolations;
}
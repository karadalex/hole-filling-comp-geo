#include "meshCalculations.h"

using namespace std;
using namespace vvr;


void getMeshAABB(std::vector<vec>& vertices, vvr::Box3D& aabb) {
	double min_x, min_y, min_z, max_x, max_y, max_z;
	min_x = max_x = vertices[0].x;
	min_y = max_y = vertices[0].y;
	min_z = max_z = vertices[0].z;

	for each (auto vertex in vertices) {
		if (vertex.x < min_x) min_x = vertex.x;
		if (vertex.y < min_y) min_y = vertex.y;
		if (vertex.z < min_z) min_z = vertex.z;

		if (vertex.x > max_x) max_x = vertex.x;
		if (vertex.y > max_y) max_y = vertex.y;
		if (vertex.z > max_z) max_z = vertex.z;
	}
	aabb = Box3D(min_x, min_y, min_z, max_x, max_y, max_z);
}


bool checkBoxCollision(vvr::Box3D box1, vvr::Box3D box2) {
	if (box1.x1 > box2.x2 || box1.x2 < box2.x1) return false;
	if (box1.y1 > box2.y2 || box1.y2 < box2.y1) return false;
	if (box1.z1 > box2.z2 || box1.z2 < box2.z1) return false;
	return true;
}


bool checkTriangleCollision3D(vvr::Triangle tri1, vvr::Triangle tri2) {
	math::Triangle _tri1, _tri2;
	_tri1 = math::Triangle(tri1.v1(), tri1.v2(), tri1.v3());
	_tri2 = math::Triangle(tri2.v1(), tri2.v2(), tri2.v3());
	return _tri1.Intersects(_tri2);

	// Custom implementation - TODO
	//C2DTriangle tri1_2D, tri2_2D;
	//int collisionInProj = 0;
	//// XY Projected triangles - collicion check
	//tri1_2D = C2DTriangle(C2DPoint(tri1.v1().x, tri1.v1().y), C2DPoint(tri1.v2().x, tri1.v2().y), C2DPoint(tri1.v3().x, tri1.v3().y));
	//tri2_2D = C2DTriangle(C2DPoint(tri2.v1().x, tri2.v1().y), C2DPoint(tri2.v2().x, tri2.v2().y), C2DPoint(tri2.v3().x, tri2.v3().y));
	//collisionInProj += checkTriangleCollision2D(tri1_2D, tri2_2D);

	//// XZ Projected triangles - collicion check
	//tri1_2D = C2DTriangle(C2DPoint(tri1.v1().x, tri1.v1().z), C2DPoint(tri1.v2().x, tri1.v2().z), C2DPoint(tri1.v3().x, tri1.v3().z));
	//tri2_2D = C2DTriangle(C2DPoint(tri2.v1().x, tri2.v1().z), C2DPoint(tri2.v2().x, tri2.v2().z), C2DPoint(tri2.v3().x, tri2.v3().z));
	//collisionInProj += checkTriangleCollision2D(tri1_2D, tri2_2D);

	//// YZ Projected triangles - collicion check
	//tri1_2D = C2DTriangle(C2DPoint(tri1.v1().y, tri1.v1().z), C2DPoint(tri1.v2().y, tri1.v2().z), C2DPoint(tri1.v3().y, tri1.v3().z));
	//tri2_2D = C2DTriangle(C2DPoint(tri2.v1().y, tri2.v1().z), C2DPoint(tri2.v2().y, tri2.v2().z), C2DPoint(tri2.v3().y, tri2.v3().z));
	//collisionInProj += checkTriangleCollision2D(tri1_2D, tri2_2D);

	//cout << collisionInProj << endl;

	//return true;
}

bool checkTriangleCollision2D(C2DTriangle tri1, C2DTriangle tri2) {
	return true;
}

void getModelWithHoles(std::vector<int>& intersections, vvr::Mesh& mesh, std::vector<vvr::Triangle>& removed_triangles) {
	removed_triangles.clear();

	vvr::Mesh original_mesh = mesh;
	mesh = *(new Mesh());
	
	std::vector<vvr::Triangle>& original_triangles = original_mesh.getTriangles();
	std::vector<vec>& original_vertices = original_mesh.getVertices();
	std::vector<vvr::Triangle>& new_triangles = mesh.getTriangles();
	std::vector<vec>& new_vertices = mesh.getVertices();

	for (int i = 0; i < original_vertices.size(); i++) {
		vec v = original_vertices.at(i);
		new_vertices.push_back(v);
	}

	int k = 0;  // Intersections index. Intersections array is assumed sorted
	int intersections_num = intersections.size();
	for (int i = 0; i < original_triangles.size(); i++) {
		bool can_add_new_triangle = true;
		if (k < intersections_num)
			if (i == intersections.at(k)) {
				can_add_new_triangle = false;
				k++;
			}

		vvr::Triangle orig_tri = original_triangles.at(i);
		if (can_add_new_triangle) {
			new_triangles.push_back(vvr::Triangle(&new_vertices, orig_tri.vi1, orig_tri.vi2, orig_tri.vi3));
		}
		else {
			removed_triangles.push_back(vvr::Triangle(&new_vertices, orig_tri.vi1, orig_tri.vi2, orig_tri.vi3));
		}
	}

	intersections.clear();
}


void getModelBoundaryVertices(vector<vvr::Triangle> removedTriangles, vector<int>& boundary_vertex_indices) {
	boundary_vertex_indices.clear();
	for (int i = 0; i < removedTriangles.size()-1; i++) {
		vvr::Triangle tri = removedTriangles.at(i);
		// Add boundary vertices to list only once
		if (find(boundary_vertex_indices.begin(), boundary_vertex_indices.end(), tri.vi1) == boundary_vertex_indices.end())
			boundary_vertex_indices.push_back(tri.vi1);
		if (find(boundary_vertex_indices.begin(), boundary_vertex_indices.end(), tri.vi2) == boundary_vertex_indices.end())
			boundary_vertex_indices.push_back(tri.vi2);
		if (find(boundary_vertex_indices.begin(), boundary_vertex_indices.end(), tri.vi3) == boundary_vertex_indices.end())
			boundary_vertex_indices.push_back(tri.vi3);
	}
}


bool checkCommonEdge(vec v1, vec v2, vvr::Triangle other_tri) {
	//cout << vi1 << " " << vi2 << " " << other_tri.vi1 << " " << other_tri.vi2 << " " << other_tri.vi3 << endl;
	return ((v1.Equals(other_tri.v1()) && v2.Equals(other_tri.v2())) || (v1.Equals(other_tri.v2()) && v2.Equals(other_tri.v1())) // other_tri: v1v2 or v2v1
		|| (v1.Equals(other_tri.v2()) && v2.Equals(other_tri.v3())) || (v1.Equals(other_tri.v3() )&& v2.Equals(other_tri.v2())) // other_tri: v2v3 or v3v2
		|| (v1.Equals(other_tri.v1()) && v2.Equals(other_tri.v3())) || (v1.Equals(other_tri.v3() )&& v2.Equals(other_tri.v1())) // other_tri: v1v3 or v3v1
		);
}
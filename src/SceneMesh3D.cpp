#include "SceneMesh3D.h"

#define SPLIT_INSTEAD_OF_INTERSECT 0

using namespace std;
using namespace vvr;

Mesh3DScene::Mesh3DScene()
{
	// Load Scene settings.
	vvr::Shape::DEF_LINE_WIDTH = 4;
	vvr::Shape::DEF_POINT_SIZE = 10;
	m_perspective_proj = true;
	m_bg_col = Colour("768E77");
	m_obj_col = Colour("454545");
	m_perspective_proj = true;
	m_hide_log = false;
	m_hide_sliders = true;
	m_fullscreen = false;

	
	for (int i = 0; i < 18; i++) {
		cout << i+1 << " " << obj_models[i] << endl;
	}
	string model_A_file, model_B_file;
	int indA, indB;
	cout << "Select file for model A (1-18): " << endl;
	cin >> indA;
	cout << "Select file for model B (1-18): " << endl;
	cin >> indB;
	model_A_file = obj_models[indA - 1];
	model_B_file = obj_models[indB - 1];

	// Load models
	const string objDir = getBasePath() + "resources/obj/";
	// Load model A
	const string objFile_A = objDir + model_A_file;
	m_model_original_A = vvr::Mesh(objFile_A);
	// Load model B
	const string objFile_B = objDir + model_B_file;
	m_model_original_B = vvr::Mesh(objFile_B);

	show_model_A_with_holes = false;

	reset();
}

void Mesh3DScene::reset()
{
	Scene::reset();

	//! Define what will be vissible by default
	m_style_flag = 0;
	m_style_flag |= FLAG_SHOW_SOLID;
	m_style_flag |= FLAG_SHOW_WIRE;
	m_style_flag |= FLAG_SHOW_AXES;
	m_style_flag |= FLAG_SHOW_AABB;
}

void Mesh3DScene::resize()
{
	//! By Making `first_pass` static and initializing it to true,
	//! we make sure that the if block will be executed only once.

	static bool first_pass = true;

	if (first_pass)
	{
		// Setup model A
		m_model_original_A.setBigSize(getSceneWidth() / 2);
		m_model_original_A.update();
		m_model_A = m_model_original_A;

		// Setup model B
		m_model_original_B.setBigSize(getSceneWidth() / 2);
		m_model_original_B.update();
		m_model_B = m_model_original_B;

		Tasks();
		first_pass = false;
	}
}

void Mesh3DScene::Tasks()
{
	m_center_mass = vec(-10, 0, 0);

	// Calculate AABB for the 2 loaded models
	getMeshAABB(m_model_A.getVertices(), m_aabb_A);
	getMeshAABB(m_model_B.getVertices(), m_aabb_B);

}

void Mesh3DScene::arrowEvent(ArrowDir dir, int modif)
{
	// Move model B up, down, left or right and recalculate the AABB
	math::float3* translation;
	if (dir == UP) translation = new math::float3(0, 0.5, 0);
	if (dir == DOWN) translation = new math::float3(0, -0.5, 0);
	else if (dir == LEFT) translation = new math::float3(-0.5, 0, 0);
	else if (dir == RIGHT) translation = new math::float3(0.5, 0, 0);
	m_model_B.move(*translation);

	vector<vvr::Triangle>& trianglesA = m_model_A.getTriangles();
	vector<vvr::Triangle>& trianglesB = m_model_B.getTriangles();
	m_intersections.clear();
	// Check for model collisions
	if (checkBoxCollision(m_aabb_A, m_aabb_B)) {
		cout << "AABBs are in collision" << endl;
		for (int i = 0; i < trianglesA.size(); i++) {
			vvr::Triangle triA = trianglesA.at(i);

			for (int j = 0; j < trianglesB.size(); j++) {
				vvr::Triangle triB = trianglesB.at(j);

				if (checkTriangleCollision3D(triA, triB)) {
					m_intersections.push_back(i);
					break;
				}
			}
		}
	}
	else cout << "AABBs are not in collision" << endl;

}

void Mesh3DScene::keyEvent(unsigned char key, bool up, int modif)
{
	Scene::keyEvent(key, up, modif);
	key = tolower(key);

	switch (key)
	{
		case 's': m_style_flag ^= FLAG_SHOW_SOLID; break;
		case 'w': m_style_flag ^= FLAG_SHOW_WIRE; break;
		case 'n': m_style_flag ^= FLAG_SHOW_NORMALS; break;
		case 'a': m_style_flag ^= FLAG_SHOW_AXES; break;
		case 'b': m_style_flag ^= FLAG_SHOW_AABB; break;
		case 'h': 
			if (show_model_A_with_holes) {
				show_model_A_with_holes = false;
				m_model_A = m_model_original_A;
			}
			else {
				show_model_A_with_holes = true;
				getModelWithHoles(m_intersections, m_model_A);
			}
			break;
	}
}

void Mesh3DScene::draw()
{
	// Draw models
	if (m_style_flag & FLAG_SHOW_SOLID) {
		m_model_A.draw(m_obj_col, SOLID);
	}
	if (m_style_flag & FLAG_SHOW_WIRE) {
		m_model_A.draw(Colour::black, WIRE);
	}
	if (m_style_flag & FLAG_SHOW_NORMALS) {
		m_model_A.draw(Colour::black, NORMALS);
	}
	if (m_style_flag & FLAG_SHOW_AXES) {
		m_model_A.draw(Colour::black, AXES);
	}

	if (!show_model_A_with_holes) {
		if (m_style_flag & FLAG_SHOW_SOLID) {
			m_model_B.draw(m_obj_col, SOLID);
		}
		if (m_style_flag & FLAG_SHOW_WIRE) {
			m_model_B.draw(Colour::black, WIRE);
		}
		if (m_style_flag & FLAG_SHOW_NORMALS) {
			m_model_B.draw(Colour::black, NORMALS);
		}
		if (m_style_flag & FLAG_SHOW_AXES) {
			m_model_B.draw(Colour::black, AXES);
		}
	}

	// Recalculate model B AABB because it is free to move and thus AABB is changed
	getMeshAABB(m_model_B.getVertices(), m_aabb_B);

	//! Draw AABB of the 2 loaded models
	if (m_style_flag & FLAG_SHOW_AABB) {
		m_aabb_A.setColour(Colour::black);
		m_aabb_A.setTransparency(1);
		m_aabb_A.draw();

		if (!show_model_A_with_holes) {
			m_aabb_B.setColour(Colour::red);
			m_aabb_B.setTransparency(1);
			m_aabb_B.draw();
		}
	}

	// Models' triangles
	vector<vvr::Triangle>& trianglesA = m_model_A.getTriangles();
	vector<vvr::Triangle>& trianglesB = m_model_B.getTriangles();

	//! Draw center mass
	Point3D(m_center_mass.x, m_center_mass.y, m_center_mass.z, Colour::red).draw();

	//! Draw intersecting triangles of model
	for (int i = 0; i < m_intersections.size(); i++) {
		vvr::Triangle &t = trianglesA.at(m_intersections[i]);
		Triangle3D t3d(
			t.v1().x, t.v1().y, t.v1().z,
			t.v2().x, t.v2().y, t.v2().z,
			t.v3().x, t.v3().y, t.v3().z,
			Colour::green);
		t3d.draw();
	}
}


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

void getModelWithHoles(std::vector<int>& intersections, vvr::Mesh& mesh) {
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

		if (can_add_new_triangle) {
			vvr::Triangle orig_tri = original_triangles.at(i);
			new_triangles.push_back(vvr::Triangle(&new_vertices, orig_tri.vi1, orig_tri.vi2, orig_tri.vi3));
		}
	}

	intersections.clear();
}


void printVector(std::vector<int> vec, string vec_name) {
	cout << vec_name << " = ";
	for (int i = 0; i < vec.size(); i++) {
		cout << vec.at(i) << " ";
	}
	cout << endl;
}
 

int main(int argc, char* argv[])
{
	try {
		return vvr::mainLoop(argc, argv, new Mesh3DScene);
	}
	catch (std::string exc) {
		cerr << exc << endl;
		return 1;
	}
	catch (...)
	{
		cerr << "Unknown exception" << endl;
		return 1;
	}
}




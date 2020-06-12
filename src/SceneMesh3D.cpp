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

	// Load models
	const string objDir = getBasePath() + "resources/obj/";
	// Load model A
	const string objFile_A = objDir + "suzanne.obj";
	m_model_original_A = vvr::Mesh(objFile_A);
	// Load model B
	const string objFile_B = objDir + "armadillo_low_low.obj";
	m_model_original_B = vvr::Mesh(objFile_B);

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
	if (dir == UP) {}
	if (dir == DOWN) {}
	// Move model B left or right and recalculate the AABB
	else if (dir == LEFT) {
		math::float3* translation = new math::float3(-0.5, 0, 0);
		m_model_B.move(*translation);
		getMeshAABB(m_model_B.getVertices(), m_aabb_B);
		cout << checkBoxCollision(m_aabb_A, m_aabb_B) << endl;
	}
	else if (dir == RIGHT) {
		math::float3* translation = new math::float3(0.5, 0, 0);
		m_model_B.move(*translation);
		getMeshAABB(m_model_B.getVertices(), m_aabb_B);
		cout << checkBoxCollision(m_aabb_A, m_aabb_B) << endl;
	}
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
	}
}

void Mesh3DScene::draw()
{
	// Draw models
	if (m_style_flag & FLAG_SHOW_SOLID) {
		m_model_A.draw(m_obj_col, SOLID);
		m_model_B.draw(m_obj_col, SOLID);
	}
	if (m_style_flag & FLAG_SHOW_WIRE) {
		m_model_A.draw(Colour::black, WIRE);
		m_model_B.draw(Colour::black, WIRE);
	}
	if (m_style_flag & FLAG_SHOW_NORMALS) {
		m_model_A.draw(Colour::black, NORMALS);
		m_model_B.draw(Colour::black, NORMALS);
	}
	if (m_style_flag & FLAG_SHOW_AXES) {
		m_model_A.draw(Colour::black, AXES);
		m_model_B.draw(Colour::black, AXES);
	}

	//! Draw AABB of the 2 loaded models
	if (m_style_flag & FLAG_SHOW_AABB) {
		m_aabb_A.setColour(Colour::black);
		m_aabb_A.setTransparency(1);
		m_aabb_A.draw();

		m_aabb_B.setColour(Colour::red);
		m_aabb_B.setTransparency(1);
		m_aabb_B.draw();
	}

	//! Draw center mass
	Point3D(m_center_mass.x, m_center_mass.y, m_center_mass.z, Colour::red).draw();

	//! Draw intersecting triangles of model
	vector<vvr::Triangle> &triangles = m_model_A.getTriangles();
	for (int i = 0; i < m_intersections.size(); i++) {
		vvr::Triangle &t = triangles[m_intersections[i]];
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




#include "SceneMesh3D.h"

#define SPLIT_INSTEAD_OF_INTERSECT 0

using namespace std;
using namespace vvr;

Mesh3DScene::Mesh3DScene()
{
	//! Load settings.
	vvr::Shape::DEF_LINE_WIDTH = 4;
	vvr::Shape::DEF_POINT_SIZE = 10;
	m_perspective_proj = true;
	m_bg_col = Colour("768E77");
	m_obj_col = Colour("454545");
	const string objDir = getBasePath() + "resources/obj/";
	const string objFile = objDir + "vvrlab.obj";
	m_model_original = vvr::Mesh(objFile);
	reset();
}

void Mesh3DScene::reset()
{
	Scene::reset();

	//! Define plane
	m_plane_d = 0;
	m_plane = Plane(vec(0, 1, 1).Normalized(), m_plane_d);

	//! Define what will be vissible by default
	m_style_flag = 0;
	m_style_flag |= FLAG_SHOW_SOLID;
	m_style_flag |= FLAG_SHOW_WIRE;
	m_style_flag |= FLAG_SHOW_AXES;
	m_style_flag |= FLAG_SHOW_AABB;
	//m_style_flag |= FLAG_SHOW_PLANE;
}

void Mesh3DScene::resize()
{
	//! By Making `first_pass` static and initializing it to true,
	//! we make sure that the if block will be executed only once.

	static bool first_pass = true;

	if (first_pass)
	{
		m_model_original.setBigSize(getSceneWidth() / 2);
		m_model_original.update();
		m_model = m_model_original;
		Tasks();
		first_pass = false;
	}
}

void Mesh3DScene::Tasks()
{
	m_center_mass = vec(-10, 0, 0);

	// Calculate AABB for the 2 loaded models
	getMeshAABB(m_model_original.getVertices(), m_aabb);

}

void Mesh3DScene::arrowEvent(ArrowDir dir, int modif)
{
	math::vec n = m_plane.normal;
	if (dir == UP) m_plane_d += 1;
	if (dir == DOWN) m_plane_d -= 1;
	else if (dir == LEFT) n = math::float3x3::RotateY(DegToRad(1)).Transform(n);
	else if (dir == RIGHT) n = math::float3x3::RotateY(DegToRad(-1)).Transform(n);
	m_plane = Plane(n.Normalized(), m_plane_d);
 
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
		case 'p': m_style_flag ^= FLAG_SHOW_PLANE; break;
		case 'b': m_style_flag ^= FLAG_SHOW_AABB; break;
	}
}

void Mesh3DScene::draw()
{
	//! Draw plane
	if (m_style_flag & FLAG_SHOW_PLANE) {
		vvr::Colour colPlane(0x41, 0x14, 0xB3);
		float u = 20, v = 20;
		math::vec p0(m_plane.Point(-u, -v, math::vec(0, 0, 0)));
		math::vec p1(m_plane.Point(-u, v, math::vec(0, 0, 0)));
		math::vec p2(m_plane.Point(u, -v, math::vec(0, 0, 0)));
		math::vec p3(m_plane.Point(u, v, math::vec(0, 0, 0)));
		math2vvr(math::Triangle(p0, p1, p2), colPlane).draw();
		math2vvr(math::Triangle(p2, p1, p3), colPlane).draw();
	}

	if (m_style_flag & FLAG_SHOW_SOLID) m_model.draw(m_obj_col, SOLID);
	if (m_style_flag & FLAG_SHOW_WIRE) m_model.draw(Colour::black, WIRE);
	if (m_style_flag & FLAG_SHOW_NORMALS) m_model.draw(Colour::black, NORMALS);
	if (m_style_flag & FLAG_SHOW_AXES) m_model.draw(Colour::black, AXES);

	//! Draw AABB
	if (m_style_flag & FLAG_SHOW_AABB) {
		m_aabb.setColour(Colour::black);
		m_aabb.setTransparency(1);
		m_aabb.draw();
	}

	//! Draw center mass
	Point3D(m_center_mass.x, m_center_mass.y, m_center_mass.z, Colour::red).draw();

	//! Draw intersecting triangles of model
	vector<vvr::Triangle> &triangles = m_model.getTriangles();
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




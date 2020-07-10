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
	show_model_A_boundaries = false;
	show_filled_triangles = false;

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
		getUniqueVertices(m_model_A.getVertices(), unique_verts_A, unique_verts_ind_A);
		getTrianglesWithUniqueVertices(m_model_A.getTriangles(), unique_tris_A, unique_verts_A, unique_verts_ind_A, m_model_A.getVertices());

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
				getModelWithHoles(m_intersections, m_model_A, m_removed_triangles);
			}
			break;
		case 'e':
			getModelBoundaryVertices(m_removed_triangles, boundaryA_vertices);
			//detectAndGetModelBoundaryVertices(unique_tris_A, boundaryA_vertices);
			show_model_A_boundaries = !show_model_A_boundaries;
			break;
		case 't':
			show_filled_triangles = !show_filled_triangles;
			if (show_filled_triangles) trivialTriangulation(filled_tris_A, boundaryA_vertices, m_model_A.getVertices());
			break;
		case 'f':
			show_filled_triangles = !show_filled_triangles;
			if (show_filled_triangles) simpleTriangulation(filled_tris_A, boundaryA_vertices, m_model_A.getVertices());
			cout << endl;
			break;
		case 'l':
			show_filled_triangles = !show_filled_triangles;
			getUniqueVertices(m_model_A.getVertices(), unique_verts_A, unique_verts_ind_A);
			getTrianglesWithUniqueVertices(m_model_A.getTriangles(), unique_tris_A, unique_verts_A, unique_verts_ind_A, m_model_A.getVertices());

			LaplacianA = LaplacianMesh(unique_tris_A, m_model_A.getVertices());
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

	if (show_filled_triangles) {
		for (int i = 0; i < filled_tris_A.size(); i++) {
			vvr::Triangle &t = filled_tris_A.at(i);
			vec v1 = m_model_A.getVertices().at(t.vi1);
			vec v2 = m_model_A.getVertices().at(t.vi2);
			vec v3 = m_model_A.getVertices().at(t.vi3);
			Triangle3D t3d(
				v1.x, v1.y, v1.z,
				v2.x, v2.y, v2.z,
				v3.x, v3.y, v3.z,
				Colour::orange);
			t3d.draw();
		}			
	}

	if (show_model_A_boundaries) drawBoundaries();

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


void Mesh3DScene::drawBoundaries() {
	std::vector<vec>& model_A_vertices = m_model_A.getVertices();
	//cout << "boundaryA_vertices = " << boundaryA_vertices.size() << endl;
	for each (int v_ind in boundaryA_vertices) {
		vec v = model_A_vertices.at(v_ind);
		Point3D(v.x, v.y, v.z, Colour::magenta).draw();
	}
}
 

int main(int argc, char* argv[])
{
	//Eigen::setNbThreads(1);  // Disable parallelization
	cout << "Using number of threads = " << Eigen::nbThreads() << endl;

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

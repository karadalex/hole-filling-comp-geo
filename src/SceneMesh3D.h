#include <VVRScene/canvas.h>
#include <VVRScene/mesh.h>
#include <VVRScene/settings.h>
#include <VVRScene/utils.h>
#include <MathGeoLib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <set>
#include "meshCalculations.h"
#include "utils.h"
#include "triangulation.h"
#include "LaplacianMesh.h"


#define FLAG_SHOW_AXES       1
#define FLAG_SHOW_WIRE       2
#define FLAG_SHOW_SOLID      4
#define FLAG_SHOW_NORMALS    8
#define FLAG_SHOW_AABB      16


class Mesh3DScene : public vvr::Scene 
{
public:
	Mesh3DScene();
	const char* getName() const { return "3D Scene"; }
	void keyEvent(unsigned char key, bool up, int modif) override;
	void arrowEvent(vvr::ArrowDir dir, int modif) override;
	void drawBoundaries();

private:
	void draw() override;
	void reset() override;
	void resize() override;
	void Tasks();

private:
	int m_style_flag;
	bool show_model_A_with_holes;
	bool show_model_A_boundaries;
	bool show_filled_triangles;
	vvr::Canvas2D m_canvas;
	vvr::Colour m_obj_col;
	math::vec m_center_mass;
	math::vec m_pca_cen;
	math::vec m_pca_dir;
	// List of triangle indices that are intersected with triangles of model B
	std::vector<int> m_intersections;

	// Model A
	vvr::Mesh m_model_original_A, m_model_A;
	vvr::Box3D m_aabb_A;
	std::vector<vec> unique_verts_A;
	std::vector<int> unique_verts_ind_A;
	std::vector<vvr::Triangle> unique_tris_A;
	// List of vertex indices that are on the boundary of the m_model_A
	std::vector<int> boundaryA_vertices;
	std::vector<vvr::Triangle> m_removed_triangles;
	// Triangulation that filled holes
	std::vector<vvr::Triangle> filled_tris_A;
	// Laplacian Mesh of model A:
	// used for laplacian mesh processing of the model A
	LaplacianMesh LaplacianA;

	// Model B
	vvr::Mesh m_model_original_B, m_model_B;
	vvr::Box3D m_aabb_B;

	std::string obj_models[18] = {
		"armadillo_low_low.obj",
		"b66_L2.obj",
		"bone.obj",
		"bunny_low.obj",
		"cube.obj",
		"dolphin.obj",
		"dragon_low_low.obj",
		"flashlight.obj",
		"flashlightNoCentered.obj",
		"hand2.obj",
		"icosahedron.obj",
		"Phone_v02.obj",
		"pins.obj",
		"polyhedron.obj",
		"suzanne.obj",
		"teapotMultiMesh.obj",
		"unicorn_low.obj",
		"unicorn_low_low.obj"
	};
};


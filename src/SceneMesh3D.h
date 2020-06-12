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
#include "symmetriceigensolver3x3.h"


#define FLAG_SHOW_AXES       1
#define FLAG_SHOW_WIRE       2
#define FLAG_SHOW_SOLID      4
#define FLAG_SHOW_NORMALS    8
#define FLAG_SHOW_AABB      16


void getMeshAABB(std::vector<vec>& vertices, vvr::Box3D& aabb);
bool checkBoxCollision(vvr::Box3D box1, vvr::Box3D box2);
bool checkTriangleCollision3D(vvr::Triangle tri1, vvr::Triangle tri2);
bool checkTriangleCollision2D(C2DTriangle tri1, C2DTriangle tri2);

struct Tri;

class Mesh3DScene : public vvr::Scene 
{
public:
	Mesh3DScene();
	const char* getName() const { return "3D Scene"; }
	void keyEvent(unsigned char key, bool up, int modif) override;
	void arrowEvent(vvr::ArrowDir dir, int modif) override;

private:
	void draw() override;
	void reset() override;
	void resize() override;
	void Tasks();

private:
	int m_style_flag;
	vvr::Canvas2D m_canvas;
	vvr::Colour m_obj_col;
	math::vec m_center_mass;
	math::vec m_pca_cen;
	math::vec m_pca_dir;
	std::vector<int> m_intersections;

	// Model A
	vvr::Mesh m_model_original_A, m_model_A;
	vvr::Box3D m_aabb_A;

	// Model B
	vvr::Mesh m_model_original_B, m_model_B;
	vvr::Box3D m_aabb_B;
};


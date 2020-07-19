#include <VVRScene/canvas.h>
#include <VVRScene/mesh.h>
#include <VVRScene/settings.h>
#include <VVRScene/utils.h>
#include <MathGeoLib.h>
#include "utils.h"


void getUniqueVertices(std::vector<vec>& vertices, std::vector<vec>& unique_vertices, std::vector<int>& unique_vertex_indices);

void getTrianglesWithUniqueVertices(
	std::vector<vvr::Triangle>& triangles, std::vector<vvr::Triangle>& unique_triangles, 
	std::vector<vec> unique_vertices, std::vector<int> unique_vertex_indices, std::vector<vec> orig_vertices);

void getMeshAABB(std::vector<vec>& vertices, vvr::Box3D& aabb);

bool checkBoxCollision(vvr::Box3D box1, vvr::Box3D box2);

bool checkTriangleCollision3D(vvr::Triangle tri1, vvr::Triangle tri2);

bool checkTriangleCollision2D(C2DTriangle tri1, C2DTriangle tri2);

void getModelWithHoles(std::vector<int>& intersections, vvr::Mesh& mesh, std::vector<vvr::Triangle>& removed_triangles);

void getModelBoundaryVertices(std::vector<vvr::Triangle> removedTriangles, std::vector<int>& boundary_vertex_indices);

void detectAndGetModelBoundaryVertices(std::vector<vvr::Triangle> triangles, std::vector<int>& boundary_vertex_indices);

bool checkCommonEdge(vec v1, vec v2, vvr::Triangle other_tri);

bool checkCommonEdge(int vi1, int vi2, vvr::Triangle other_tri);

Sphere getTriangleCircumsphere(vvr::Triangle tri);

/**
 * Return smoothed mesh using moving average filter
 * vvr::Mesh& mesh : model's mesh to smooth
 * SpMat Adj : sparse adjacency matrix of mesh
 * SpMat D : sparse diagonal matrix with degree of each vertex (number of neighboors)
*/
void meshSmothingAverage(vvr::Mesh& mesh, SpMat Adj, SpMat D);

/**
 * Return smoothed mesh around specified vertices using moving average filter
 * vvr::Mesh& mesh : model's mesh to smooth
 * SpMat Adj : sparse adjacency matrix of mesh
 * SpMat D : sparse diagonal matrix with degree of each vertex (number of neighboors)
 * std::vector<int> vertices_to_edit : indices of vertices to smooth
*/
void meshSmothingAverage(vvr::Mesh& mesh, std::vector<int> vertices_to_edit, SpMat A, SpMat D);

/**
 * Calculate density of triangles around given vertex, using sphere with given vertex as center and given radius
*/
int getTrianglesDensityAroundVertex(vvr::Mesh mesh, int vertex, float radius);
int getTrianglesDensityAroundVertex(vvr::Mesh mesh, vec vertex, float radius);

/**
 * Add vertices if local triangle density is lower than that of neighbourhoods
*/
void meshDensityImprovement(vvr::Mesh& mesh, std::vector<int> vertices_to_edit, SpMat A);
#include <VVRScene/mesh.h>
#include <MathGeoLib.h>
#include "meshCalculations.h"
#include "utils.h"

using namespace Eigen;


void getAdjacencyMatrix(std::vector<vvr::Triangle> triangles, std::vector<vec> vertices, SpMat& A);

void getVertexDegreeMatrix(SpMat A, SpMat& D);

void getLaplacianMatrix(SpMat A, SpMat D, SpMat& L);

void getDeltaCoordinates(SpMat A, SpMat D, std::vector<vec> vertices, std::vector<VectorXd>& deltaCoords);

/**
 * Mesh Reconstruction calculation:
 * Inputs:
 *  - SpMat anchoredL
 *  - std::vector<VectorXd> anchoredDCoords
 *  - std::vector<VectorXd>& xyzCoords std::vector containing the 3 calculated VectorXd vectors (x,y,z)
*/
void getSurfaceReconstructionFromDCoords(SpMat anchoredL, std::vector<VectorXd> anchoredDCoords, std::vector<VectorXd>& xyzCoords);


class LaplacianMesh {
public:
    LaplacianMesh(){}
    LaplacianMesh(std::vector<vvr::Triangle> triangles, std::vector<vec> vertices);

    std::vector<vvr::Triangle> triangles;
    std::vector<vec> vertices;

    SpMat A_adj;

    SpMat Vertex_degrees;

    SpMat Laplacian;

    std::vector<VectorXd> deltaCoords;
    std::vector<VectorXd> xyzCoords;
};
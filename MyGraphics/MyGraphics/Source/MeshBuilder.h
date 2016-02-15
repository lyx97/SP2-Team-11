#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "Mesh.h"
#include "Vertex.h"
#include "MyMath.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateOBJ(const std::string &meshName, const std::string &file_path);
	static Mesh* GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol);

	static Mesh* GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ);
	static Mesh* GenerateQuad(const std::string &meshName, Color color);
	static Mesh* GenerateCube(const std::string &meshName, Color color);
	static Mesh* GenerateCircle(const std::string &meshName, Color color, unsigned numSlices);
	static Mesh* GenerateRing(const std::string &meshName, Color color, float innerRadius, unsigned numSlices);
	static Mesh* GenerateSphere(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices);
	static Mesh* GenerateHemi(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices);
	static Mesh* GenerateCylin(const std::string &meshName, Color color, unsigned numSlices);
	static Mesh* GenerateCone(const std::string &meshName, Color color, unsigned numSlices);
	static Mesh* GenerateTorus(const std::string &meshName, Color color, unsigned numSlices, unsigned numStacks, float innerRadius, float outerRadius);
	static Mesh* GenerateHalfTorus(const std::string &meshName, Color color, unsigned numSlices, unsigned numStacks, float innerRadius, float outerRadius);
	static Mesh* GenerateHalfHemi(const std::string &meshName, Color color, unsigned numStacks, unsigned numSlices);
};

#endif // MESH_BUILDER_H
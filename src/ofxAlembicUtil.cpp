#include "ofxAlembicUtil.h"


static bool inited = false;

void ofxAlembic::init()
{
	if (inited) return;
	inited = true;
	
	ofLogVerbose("ofxAlembic") << "alembic version: " << Alembic::Abc::GetLibraryVersionShort();
}

void ofxAlembic::transform(ofMesh &mesh, const glm::mat4 &m)
{
	std::vector<glm::vec3>& vertices = mesh.getVertices();

	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = glm::vec4(vertices[i], 1.f) * m;
	}

	if (mesh.hasNormals())
	{
		std::vector<glm::vec3>& normals = mesh.getNormals();

		for (int i = 0; i < normals.size(); i++)
		{
			const glm::vec3& v = normals[i];
			normals[i] = glm::vec4(v.x, v.y, v.z, 0) * m;
		}
	}
}

std::vector<glm::vec3> toOf(const vector<ofxAlembic::Point>& v)
{
	std::vector<glm::vec3> arr(v.size());
	for (int i = 0; i < v.size(); i++)
		arr[i] = v[i].pos;

	return arr;
}

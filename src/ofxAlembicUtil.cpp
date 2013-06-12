#include "ofxAlembicUtil.h"

void ofxAlembic::transform(ofMesh &mesh, const ofMatrix4x4 &m)
{
	vector<ofVec3f>& vertices = mesh.getVertices();

	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = vertices[i] * m;
	}

	if (mesh.hasNormals())
	{
		vector<ofVec3f>& normals = mesh.getNormals();

		for (int i = 0; i < normals.size(); i++)
		{
			const ofVec3f& v = normals[i];
			normals[i] = ofVec4f(v.x, v.y, v.z, 0) * m;
		}
	}
}

vector<ofVec3f> toOf(const vector<ofxAlembic::Point>& v)
{
	vector<ofVec3f> arr(v.size());
	for (int i = 0; i < v.size(); i++)
		arr[i] = v[i].pos;

	return arr;
}
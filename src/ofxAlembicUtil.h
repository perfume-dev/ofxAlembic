#pragma once

#include "ofMain.h"

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/AbcCoreOgawa/All.h>

#include "ofxAlembicType.h"

namespace ofxAlembic
{
	struct Point;
	
	void init();
	void transform(ofMesh &mesh, const glm::mat4 &m);
}

inline ofVec3f toOf(const Alembic::AbcGeom::V3f& v)
{
	return ofVec3f(v.x, v.y, v.z);
}

inline Alembic::AbcGeom::V3f toAbc(const ofVec3f& v)
{
	return Alembic::AbcGeom::V3f(v.x, v.y, v.z);
}

inline Alembic::AbcGeom::V3f toAbc(const glm::vec3& v)
{
	return Alembic::AbcGeom::V3f(v.x, v.y, v.z);
}

inline ofVec2f toOf(const Alembic::AbcGeom::V2f& v)
{
	return ofVec2f(v.x, v.y);
}

inline Alembic::AbcGeom::V2f toAbc(const ofVec2f& v)
{
	return Alembic::AbcGeom::V2f(v.x, v.y);
}

inline Alembic::AbcGeom::V2f toAbc(const glm::vec2& v)
{
	return Alembic::AbcGeom::V2f(v.x, v.y);
}

inline ofMatrix4x4 toOf(const Imath::M44f& v)
{
	return ofMatrix4x4(v[0][0], v[0][1], v[0][2], v[0][3],
		v[1][0], v[1][1], v[1][2], v[1][3],
		v[2][0], v[2][1], v[2][2], v[2][3],
		v[3][0], v[3][1], v[3][2], v[3][3]);
}

inline Imath::M44f toAbc(const glm::mat4& v)
{
	const float *p = glm::value_ptr(v);
	return Imath::M44f(p[0], p[1], p[2], p[3],
		p[4], p[5], p[6], p[7],
		p[8], p[9], p[10], p[11],
		p[12], p[13], p[14], p[15]);
}

std::vector<glm::vec3> toOf(const vector<ofxAlembic::Point>& v);
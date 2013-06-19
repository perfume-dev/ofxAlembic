#pragma once

#include "ofMain.h"

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>

#include "ofxAlembicType.h"

namespace ofxAlembic
{
	void init();
	
	void transform(ofMesh &mesh, const ofMatrix4x4 &m);
}

inline ofVec3f toOf(const Alembic::AbcGeom::V3f& v)
{
	return ofVec3f(v.x, v.y, v.z);
}

inline Alembic::AbcGeom::V3f toAbc(const ofVec3f& v)
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

vector<ofVec3f> toOf(const vector<ofxAlembic::Point>& v);
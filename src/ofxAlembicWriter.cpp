#include "ofxAlembicWriter.h"

using namespace ofxAlembic;
using namespace Alembic::AbcGeom;

bool Writer::open(string path, float fps)
{
	path = ofToDataPath(path);

	archive = OArchive(Alembic::AbcCoreHDF5::WriteArchive(), path);
	if (!archive.valid()) return false;

	archive.setCompressionHint(1);

	inv_fps = 1. / fps;
	rewind();

	return true;
}

void Writer::close()
{
	map<string, Alembic::AbcGeom::OObject*>::iterator it = object_map.begin();
	while (it != object_map.end())
	{
		delete it->second;
		it++;
	}

	object_map.clear();

	if (archive.valid())
		archive.reset();
}

void Writer::addPoints(const string& path, const Points& points)
{
	typedef OPoints Type;
	typedef Type::schema_type Schema;
	typedef Schema::Sample Sample;

	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();

	points.get(schema);
}

void Writer::addPolyMesh(const string& path, const PolyMesh& polymesh)
{
	typedef OPolyMesh Type;
	typedef Type::schema_type Schema;
	typedef Schema::Sample Sample;

	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();

	polymesh.get(schema);
}

void Writer::addCurves(const string& path, const Curves& curves)
{
	typedef OCurves Type;
	typedef Type::schema_type Schema;
	typedef Schema::Sample Sample;

	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();

	curves.get(schema);
}

// time

void Writer::setTime(float time)
{
	current_time = time;
}

void Writer::flashFrame()
{
	setTime(getTime() + inv_fps);
}

void Writer::rewind()
{
	setTime(0);
}
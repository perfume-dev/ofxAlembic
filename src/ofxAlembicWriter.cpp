#include "ofxAlembicWriter.h"

using namespace ofxAlembic;
using namespace Alembic::AbcGeom;

bool Writer::open(const string& path, float fps)
{
	ofxAlembic::init();
	
	archive = OArchive(Alembic::AbcCoreHDF5::WriteArchive(), ofToDataPath(path));
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

	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();

	points.get(schema);
}

void Writer::addPolyMesh(const string& path, const PolyMesh& polymesh)
{
	typedef OPolyMesh Type;
	typedef Type::schema_type Schema;

	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();

	polymesh.get(schema);
}

void Writer::addCurves(const string& path, const Curves& curves)
{
	typedef OCurves Type;
	typedef Type::schema_type Schema;

	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();

	curves.get(schema);
}

void Writer::addXform(const string& path, const XForm& xform)
{
	typedef OXform Type;
	typedef Type::schema_type Schema;
	
	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();
	
	xform.get(schema);
}

void Writer::addCamera(const string& path, const Camera& camera)
{
	typedef OCamera Type;
	typedef Type::schema_type Schema;
	
	Type &object = getObject<Type>(path);
	Schema &schema = object.getSchema();
	
	camera.get(schema);
}

void Writer::addOfCamera(const string& path, const ofCamera& ofcamera)
{
	addXform(path, ofcamera.getGlobalTransformMatrix());
	
	Camera camera(ofcamera);
	camera.updateSample(ofcamera);
	addCamera(path + "/camera_self", camera);
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
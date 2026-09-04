#include "ofxAlembicWriter.h"

#include <cmath>

using namespace ofxAlembic;
using namespace Alembic::AbcGeom;

Writer::Writer()
	: inv_fps(1.f / 30.f)
	, current_time(0)
{
}

bool Writer::open(const string& path, float fps, Alembic::AbcCoreFactory::IFactory::CoreType type)
{
	ofxAlembic::init();
	close();

	if (path.empty())
	{
		ofLogError("ofxAlembic::Writer") << "output path must not be empty";
		return false;
	}

	if (!std::isfinite(fps) || fps <= 0)
	{
		ofLogError("ofxAlembic::Writer") << "fps must be a finite number greater than zero";
		return false;
	}

	if (type != Alembic::AbcCoreFactory::IFactory::kOgawa)
	{
		ofLogError("ofxAlembic::Writer") << "only the Ogawa backend is supported";
		return false;
	}

	try
	{
		archive = OArchive(Alembic::AbcCoreOgawa::WriteArchive(), ofToDataPath(path));
	}
	catch (const std::exception& error)
	{
		ofLogError("ofxAlembic::Writer") << "could not open '" << path << "': " << error.what();
		close();
		return false;
	}

	if (!archive.valid())
		return false;

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

	inv_fps = 1.f / 30.f;
	current_time = 0;
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

void Writer::addCamera(const string& path, const ofCamera& ofcamera)
{
	addXform(path, ofcamera.getGlobalTransformMatrix());
	
	Camera camera(ofcamera);
	camera.updateSample(ofcamera);
	addCamera(path + "/cameraShape", camera);
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

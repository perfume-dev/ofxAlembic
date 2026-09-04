#pragma once

#include "ofMain.h"

#include "ofxAlembicType.h"

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>
#include <Alembic/AbcCoreOgawa/All.h>

#include <stdexcept>

namespace ofxAlembic
{
class Writer;
}

class ofxAlembic::Writer
{
public:

		Writer();
		~Writer() { close(); }

	bool open(const string& path, float fps = 30, Alembic::AbcCoreFactory::IFactory::CoreType type = Alembic::AbcCoreFactory::IFactory::kOgawa);
	void close();

	void addPoints(const string& path, const Points& points);
	void addPolyMesh(const string& path, const PolyMesh& polymesh);
	void addCurves(const string& path, const Curves& curves);
	void addXform(const string& path, const XForm& xform);
	void addCamera(const string& path, const Camera& camera);
	void addCamera(const string& path, const ofCamera& camera);

	void setTime(float time);
	float getTime() const { return current_time; }

	void rewind();

		void flashFrame();
		inline bool isOpen() const { return archive.valid(); }

protected:

	map<string, Alembic::AbcGeom::OObject*> object_map;
	Alembic::AbcGeom::OArchive archive;

	float inv_fps;
	float current_time;

	template <typename T>
		T& getObject(const string& path)
		{
			using namespace Alembic::AbcGeom;

			if (!archive.valid())
			{
				throw std::logic_error("ofxAlembic::Writer is not open");
			}

		// validation
		if (path.empty()
			|| path[0] != '/'
			|| path[path.size() - 1] == '/')
			{
				ofLogError("ofxAlembic::Writer") << "invalid path: '" << path << "'";
				throw std::invalid_argument("Alembic object paths must start with '/' and must not end with '/'");
			}
		
		map<string, OObject*>::iterator it = object_map.find(path);
		
		// return if already created
			if (it != object_map.end())
			{
				T* typed_object = dynamic_cast<T*>(it->second);
				if (typed_object == NULL)
					throw std::logic_error("Alembic object path is already used by a different schema type: " + path);
				return *typed_object;
			}

		vector<string> e = ofSplitString(path, "/", true, true);
		string new_object_name = e.back();
		
		TimeSampling Ts(inv_fps, current_time);
		Alembic::Util::uint32_t tsidx = archive.addTimeSampling(Ts);
		
		if (e.size() > 1)
		{
			// root object
			string parent_path = "/" + ofJoinString(vector<string>(e.begin(), e.end() - 1), "/");
			
			map<string, OObject*>::iterator parent_it = object_map.find(parent_path);
			if (parent_it == object_map.end())
				{
					ofLogError("ofxAlembic::Writer") << "parent object not found: '" << path << "'";
					throw std::invalid_argument("Alembic parent object must be created before its children: " + parent_path);
				}
			
			OObject* parent_object = parent_it->second;
			
			T *t = new T(*parent_object, new_object_name);
			t->getSchema().setTimeSampling(tsidx);
			object_map[path] = t;
			
			return *t;
		}
		else
		{
			T *t = new T(archive.getTop(), new_object_name);
			t->getSchema().setTimeSampling(tsidx);
			object_map[path] = t;
			
			return *t;
		}
	}
};

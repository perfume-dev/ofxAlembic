#pragma once

#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreOgawa/All.h>

#include "ofMain.h"

#include "ofxAlembicUtil.h"
#include "ofxAlembicType.h"

namespace ofxAlembic
{
class Reader;
class IGeom;

class IXform;
class IPoints;
class ICurves;
class IPolyMesh;
class ICamera;

template <typename T>
inline ofxAlembic::Type type2enum() { return ofxAlembic::UNKHOWN; }

template <>
inline ofxAlembic::Type type2enum<ofxAlembic::XForm>() { return ofxAlembic::XFORM; }
	
template <>
inline ofxAlembic::Type type2enum<ofxAlembic::Points>() { return ofxAlembic::POINTS; }

template <>
inline ofxAlembic::Type type2enum<ofxAlembic::Curves>() { return ofxAlembic::CURVES; }

template <>
inline ofxAlembic::Type type2enum<ofxAlembic::PolyMesh>() { return ofxAlembic::POLYMESH; }
}

class ofxAlembic::Reader
{
public:

	~Reader() {}

	bool open(const string& path);
	void close();
	
	void dumpNames();
	void dumpFullnames();

	void setTime(double time);
	float getTime() const { return current_time; }

	inline float getMinTime() const { return m_minTime; }
	inline float getMaxTime() const { return m_maxTime; }

	void draw();
	void debugDraw();

	inline size_t size() const { return object_arr.size(); }
	inline const vector<string>& getNames() const { return object_name_arr; }
	inline const vector<string>& getFullnames() const { return object_fullname_arr; }

	bool get(const string& path, ofMatrix4x4& matrix);
	bool get(const string& path, ofMesh& mesh);
	bool get(const string& path, vector<ofPolyline>& curves);
	bool get(const string& path, vector<glm::vec3>& points);
	bool get(const string& path, ofCamera &camera);

	bool get(size_t idx, ofMatrix4x4& matrix);
	bool get(size_t idx, ofMesh& mesh);
	bool get(size_t idx, vector<ofPolyline>& curves);
	bool get(size_t idx, vector<ofVec3f>& points);
	bool get(size_t idx, ofCamera &camera);

	inline IGeom* get(size_t idx) { return object_arr[idx]; }
	
	IGeom* get(const string& path)
	{
		if (object_name_map.find(path) != object_name_map.end())
			return object_name_map[path];
		if (object_fullname_map.find(path) != object_fullname_map.end())
			return object_fullname_map[path];

		return NULL;
	}
	
protected:

	Alembic::AbcGeom::IArchive m_archive;

	ofPtr<IGeom> m_root;

	map<string, IGeom*> object_name_map;
	map<string, IGeom*> object_fullname_map;
	
	vector<IGeom*> object_arr;
	vector<string> object_name_arr;
	vector<string> object_fullname_arr;

	Alembic::AbcGeom::chrono_t m_minTime;
	Alembic::AbcGeom::chrono_t m_maxTime;

	float current_time;
};

// Geom

class ofxAlembic::IGeom
{
	friend class Reader;

public:

	IGeom();
	IGeom(Alembic::AbcGeom::IObject object);
	virtual ~IGeom();

	virtual bool valid() { return m_object; }

	void draw();
	void debugDraw();
	
	inline const ofMatrix4x4& getGlobalTransform() const { return transform; }

	size_t getIndex() const { return index; }
	
	string getName() const;
	string getFullName() const;
	
	virtual const char* getTypeName() const { return ""; }

	inline bool isTypeOf(Type t) const { return type == t; }

	template <typename T>
	inline bool isTypeOf() const { return type == type2enum<T>(); }

	template <typename T>
	inline bool get(T &out)
	{
		ofLogError("ofxAlembic::IGeom") << "invalid type";
		return false;
	}

protected:

	Type type;
	
	size_t index;
	ofMatrix4x4 transform;

	Alembic::AbcGeom::IObject m_object;
	vector<ofPtr<IGeom> > m_children;

	virtual void setupWithObject(Alembic::AbcGeom::IObject);
	void updateWithTime(double time, Imath::M44f& xform);

	virtual void updateWithTimeInternal(double time, Imath::M44f& xform) {}
	virtual void drawInternal() {}
	virtual void debugDrawInternal() {}

	Alembic::AbcGeom::chrono_t m_minTime;
	Alembic::AbcGeom::chrono_t m_maxTime;

	static void visit_geoms(ofPtr<IGeom> &obj, map<string, IGeom*> &object_name_map, map<string, IGeom*> &object_fullname_map);
	
	template <typename T>
	void update_timestamp(T& object);
};

class ofxAlembic::IXform : public ofxAlembic::IGeom
{
public:
	
	XForm xform;
	
	IXform(Alembic::AbcGeom::IXform object);
	~IXform();
	
	const char* getTypeName() const { return "Xform"; }
	
protected:
	
	Alembic::AbcGeom::IXform m_xform;
	
	void updateWithTimeInternal(double time, Imath::M44f& xform);
	void debugDrawInternal()
	{
		ofPushStyle();
		
		stringstream ss;
		ss << "[" << getName() << "]";
		
		ofSetColor(255);
		ofDrawBitmapString(ss.str(), 0, 0);
		
		ofPopStyle();
		
		xform.draw();
	}
};

class ofxAlembic::IPoints : public ofxAlembic::IGeom
{
public:

	Points points;

	IPoints(Alembic::AbcGeom::IPoints object);
	~IPoints()
	{
		if (m_points)
			m_points.reset();
	}

	const char* getTypeName() const { return "Points"; }

protected:

	Alembic::AbcGeom::IPoints m_points;

	void updateWithTimeInternal(double time, Imath::M44f& xform);
	void drawInternal() { points.draw(); }
};

class ofxAlembic::ICurves : public ofxAlembic::IGeom
{
public:

	Curves curves;

	ICurves(Alembic::AbcGeom::ICurves object);
	~ICurves()
	{
		if (m_curves)
			m_curves.reset();
	}

	const char* getTypeName() const { return "Curves"; }

protected:

	Alembic::AbcGeom::ICurves m_curves;

	void updateWithTimeInternal(double time, Imath::M44f& xform);
	void drawInternal() { curves.draw(); }
};

class ofxAlembic::IPolyMesh : public ofxAlembic::IGeom
{
public:

	PolyMesh polymesh;

	IPolyMesh(Alembic::AbcGeom::IPolyMesh object);
	~IPolyMesh()
	{
		if (m_polyMesh)
			m_polyMesh.reset();
	}

	const char* getTypeName() const { return "PolyMesh"; }

protected:

	Alembic::AbcGeom::IPolyMesh m_polyMesh;

	void updateWithTimeInternal(double time, Imath::M44f& xform);
	void drawInternal() { polymesh.draw(); }
};

class ofxAlembic::ICamera : public ofxAlembic::IGeom
{
public:
	
	Camera camera;
	
	ICamera(Alembic::AbcGeom::ICamera object);
	~ICamera()
	{
		if (m_camera)
			m_camera.reset();
	}
	
	const char* getTypeName() const { return "Camera"; }
	
protected:
	
	Alembic::AbcGeom::ICamera m_camera;
	
	void updateWithTimeInternal(double time, Imath::M44f& xform);
	void drawInternal() { camera.draw(); }

};

//

template <>
inline bool ofxAlembic::IGeom::get(ofMatrix4x4 &o)
{
	if (type != ofxAlembic::XFORM)
	{
		ofLogError("ofxAlembic::IXform") << "cast error";
		return false;
	}
	
	o = transform;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(ofxAlembic::Points &o)
{
	if (type != ofxAlembic::POINTS)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((IPoints*)this)->points;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(vector<ofxAlembic::Point> &o)
{
	if (type != ofxAlembic::POINTS)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((IPoints*)this)->points.points;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(std::vector<glm::vec3> &o)
{
	if (type != ofxAlembic::POINTS)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = toOf(((IPoints*)this)->points.points);
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(ofxAlembic::Curves &o)
{
	if (type != ofxAlembic::CURVES)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((ICurves*)this)->curves;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(vector<ofPolyline> &o)
{
	if (type != ofxAlembic::CURVES)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((ICurves*)this)->curves.curves;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(ofxAlembic::PolyMesh &o)
{
	if (type != ofxAlembic::POLYMESH)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((IPolyMesh*)this)->polymesh;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(ofMesh &o)
{
	if (type != ofxAlembic::POLYMESH)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}

	o = ((IPolyMesh*)this)->polymesh.mesh;
	return true;
}

template <>
inline bool ofxAlembic::IGeom::get(ofCamera &o)
{
	if (type != ofxAlembic::CAMERA)
	{
		ofLogError("ofxAlembic::IGeom") << "cast error";
		return false;
	}
	
	((ICamera*)this)->camera.updateParams(o, transform);
	return true;
}

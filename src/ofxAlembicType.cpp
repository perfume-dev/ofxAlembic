#include "ofxAlembicType.h"

using namespace ofxAlembic;
using namespace Alembic::AbcGeom;

#pragma mark - XForm

XForm::XForm(const ofMatrix4x4& matrix)
	: mat(toAbc(matrix))
{
	
}

void XForm::draw()
{
	ofDrawAxis(10);
}

// via. https://github.com/satoruhiga/ofxEulerAngles/

inline static ofVec3f toEulerXYZ(const ofMatrix4x4 &m)
{
	ofVec3f v;
	
	float &thetaX = v.x;
	float &thetaY = v.y;
	float &thetaZ = v.z;
	
	const float &r00 = m(0, 0);
	const float &r01 = m(1, 0);
	const float &r02 = m(2, 0);
	
	const float &r10 = m(0, 1);
	const float &r11 = m(1, 1);
	const float &r12 = m(2, 1);
	
	const float &r20 = m(0, 2);
	const float &r21 = m(1, 2);
	const float &r22 = m(2, 2);
	
	if (r02 < +1)
	{
		if (r02 > -1)
		{
			thetaY = asinf(r02);
			thetaX = atan2f(-r12, r22);
			thetaZ = atan2f(-r01, r00);
		}
		else     // r02 = -1
		{
			// Not a unique solution: thetaZ - thetaX = atan2f(r10,r11)
			thetaY = -PI / 2;
			thetaX = -atan2f(r10, r11);
			thetaZ = 0;
		}
	}
	else // r02 = +1
	{
		// Not a unique solution: thetaZ + thetaX = atan2f(r10,r11)
		thetaY = +PI / 2;
		thetaX = atan2f(r10, r11);
		thetaZ = 0;
	}
	
	thetaX = ofRadToDeg(thetaX);
	thetaY = ofRadToDeg(thetaY);
	thetaZ = ofRadToDeg(thetaZ);
	
	return v;
}

void XForm::get(Alembic::AbcGeom::OXformSchema &schema) const
{
	XformSample samp;
	
	XformOp transop(kTranslateOperation, kTranslateHint);
	XformOp rotatop(kRotateOperation, kRotateHint);
	XformOp scaleop(kScaleOperation, kScaleHint);

	ofMatrix4x4 m = toOf(mat);
	ofVec3f t, s;
	ofQuaternion R, so;
	m.decompose(t, R, s, so);
	
	ofVec3f xyz = toEulerXYZ(R);
	
	samp.addOp(transop, toAbc(t));
	samp.addOp(rotatop, V3d(1.0, 0.0, 0.0), xyz.x);
	samp.addOp(rotatop, V3d(0.0, 1.0, 0.0), xyz.y);
	samp.addOp(rotatop, V3d(0.0, 0.0, 1.0), xyz.z);
	samp.addOp(scaleop, toAbc(s));

	schema.set(samp);
}

void XForm::set(Alembic::AbcGeom::IXformSchema &schema, float time)
{
	ISampleSelector ss(time, ISampleSelector::kNearIndex);
	
	const M44d& m = schema.getValue(ss).getMatrix();
	const double *src = m.getValue();
	float *dst = mat.getValue();
	
	for (int i = 0; i < 16; i++)
		dst[i] = src[i];
}

#pragma mark - Points

Points::Points(const vector<ofVec3f>& ofpoints)
{
	for (int i = 0; i < ofpoints.size(); i++)
	{
		points.push_back(ofpoints[i]);
	}
}

void Points::get(OPointsSchema &schema) const
{
	int num = points.size();

	vector<V3f> positions(num);
	vector<uint64_t> ids(num);

	for (int i = 0; i < num; i++)
	{
		positions[i] = toAbc(points[i].pos);
		ids[i] = points[i].id;
	}

	OPointsSchema::Sample sample((P3fArraySample(positions)),
								 UInt64ArraySample(ids));
	schema.set(sample);
}

void Points::set(IPointsSchema &schema, float time)
{
	ISampleSelector ss(time, ISampleSelector::kNearIndex);
	IPointsSchema::Sample sample;
	schema.get(sample, ss);

	P3fArraySamplePtr m_positions = sample.getPositions();

	size_t num_points = m_positions->size();
	const V3f *src = m_positions->get();
	V3f dst;

	points.resize(num_points);

	for (int i = 0; i < num_points; i++)
	{
		const V3f& v = src[i];
		points[i].pos.set(v.x, v.y, v.z);
	}
}

void Points::draw()
{
	glBegin(GL_POINTS);
	for (int i = 0; i < points.size(); i++)
	{
		glVertex3fv(points[i].pos.getPtr());
	}
	glEnd();
}

#pragma mark - PolyMesh

void PolyMesh::get(OPolyMeshSchema &schema) const
{
	vector<V3f> positions;
	vector<int32_t> indexes;
	vector<int32_t> counts;
	vector<V2f> uvs;
	vector<N3f> norms;

	OV2fGeomParam::Sample uv_sample;
	ON3fGeomParam::Sample norm_sample;

	if (mesh.getNumIndices())
	{
		const int num_samples = mesh.getNumIndices();

		const vector<ofIndexType>& idx = mesh.getIndices();

		{
			indexes.resize(num_samples);
			for (int i = 0; i < num_samples; i++)
				indexes[i] = i;
		}

		{
			const vector<ofVec3f>& verts = mesh.getVertices();
			positions.resize(num_samples);

			for (int i = 0; i < num_samples; i++)
				positions[i] = toAbc(verts[idx[i]]);
		}

		if (mesh.getNumTexCoords() == mesh.getNumVertices())
		{
			const vector<ofVec2f> &v = mesh.getTexCoords();

			uvs.resize(num_samples);
			for (int i = 0; i < num_samples; i++)
				uvs[i] = toAbc(v[idx[i]]);
		}
		assert(uvs.size() == 0 || uvs.size() == num_samples);

		if (mesh.getNumNormals() == mesh.getNumVertices())
		{
			const vector<ofVec3f> &v = mesh.getNormals();

			norms.resize(num_samples);
			for (int i = 0; i < num_samples; i++)
				norms[i] = toAbc(v[idx[i]].getNormalized() * -1);
		}
		assert(norms.size() == 0 || norms.size() == num_samples);
	}
	else
	{
		const int num_samples = mesh.getNumVertices();

		{
			indexes.resize(num_samples);
			for (int i = 0; i < num_samples; i++)
				indexes[i] = i;
		}

		{
			const vector<ofVec3f>& verts = mesh.getVertices();
			positions.resize(num_samples);

			for (int i = 0; i < num_samples; i++)
				positions[i] = toAbc(verts[i]);
		}

		if (mesh.getNumTexCoords() == num_samples)
		{
			const vector<ofVec2f> &v = mesh.getTexCoords();

			uvs.resize(num_samples);
			for (int i = 0; i < num_samples; i++)
				uvs[i] = toAbc(v[i]);
		}
		assert(uvs.size() == 0 || uvs.size() == num_samples);

		if (mesh.getNumNormals() == num_samples)
		{
			const vector<ofVec3f> &v = mesh.getNormals();

			norms.resize(num_samples);
			for (int i = 0; i < num_samples; i++)
				norms[i] = toAbc(v[i].getNormalized() * -1);
		}
		assert(norms.size() == 0 || norms.size() == num_samples);
	}

	// supports only triangles
	{
		int num_tris = indexes.size() / 3;
		counts.resize(num_tris);
		for (int i = 0; i < num_tris; i++)
			counts[i] = 3;
	}

	if (!uvs.empty())
	{
		uv_sample.setScope(kVertexScope);
		uv_sample.setVals(V2fArraySample(uvs));
	}

	if (!norms.empty())
	{
		norm_sample.setScope(kVertexScope);
		norm_sample.setVals(N3fArraySample(norms));
	}

	OPolyMeshSchema::Sample sample((P3fArraySample(positions)),
								   Int32ArraySample(indexes),
								   Int32ArraySample(counts),
								   uv_sample,
								   norm_sample);
	schema.set(sample);
}

void PolyMesh::set(IPolyMeshSchema &schema, float time)
{
	ISampleSelector ss(time, ISampleSelector::kNearIndex);
	IPolyMeshSchema::Sample sample;
	schema.get(sample, ss);

	P3fArraySamplePtr m_meshP = sample.getPositions();
	Int32ArraySamplePtr m_meshIndices = sample.getFaceIndices();
	Int32ArraySamplePtr m_meshCounts = sample.getFaceCounts();

	mesh.clear();

	size_t numFaces = m_meshCounts->size();
	size_t numIndices = m_meshIndices->size();
	size_t numPoints = m_meshP->size();
	if (numFaces < 1 ||
		numIndices < 1 ||
		numPoints < 1)
	{
		return;
	}

	// TODO: organaize face index
	// make Triangle and Quad class

	typedef Imath::Vec3<unsigned int> Tri;
	typedef std::vector<Tri> TriArray;

	TriArray m_triangles;

	size_t faceIndexBegin = 0;
	size_t faceIndexEnd = 0;
	for (size_t face = 0; face < numFaces; ++face)
	{
		faceIndexBegin = faceIndexEnd;
		size_t count = (*m_meshCounts)[face];
		faceIndexEnd = faceIndexBegin + count;

		// Check this face is valid
		if (faceIndexEnd > numIndices ||
			faceIndexEnd < faceIndexBegin)
		{
			ofLogError("ofxAlembic") << "Mesh update quitting on face: "
			<< face
			<< " because of wonky numbers"
			<< ", faceIndexBegin = " << faceIndexBegin
			<< ", faceIndexEnd = " << faceIndexEnd
			<< ", numIndices = " << numIndices
			<< ", count = " << count;

			// Just get out, make no more triangles.
			break;
		}

		// Make triangles to fill this face.
		if (count >= 3)
		{
			m_triangles.push_back(Tri((unsigned int)faceIndexBegin + 0,
									  (unsigned int)faceIndexBegin + 1,
									  (unsigned int)faceIndexBegin + 2));
			for (size_t c = 3; c < count; ++c)
			{
				m_triangles.push_back(Tri((unsigned int)faceIndexBegin + 0,
										  (unsigned int)faceIndexBegin + c - 1,
										  (unsigned int)faceIndexBegin + c));
			}
		}
	}

	{
		const V3f *points = m_meshP->get();
		const int32_t *indices = m_meshIndices->get();

		V3f dst;
		vector<ofVec3f> verts;

		for (int i = 0; i < numPoints; i++)
		{
			const V3f& v = points[i];
			verts.push_back(ofVec3f(v.x, v.y, v.z));
		}

		for (int i = 0; i < m_triangles.size(); i++)
		{
			Tri &t = m_triangles[i];
			mesh.addVertex(verts[indices[t[0]]]);
			mesh.addVertex(verts[indices[t[1]]]);
			mesh.addVertex(verts[indices[t[2]]]);
		}
	}

	{
		IN3fGeomParam N = schema.getNormalsParam();
		if (N.valid())
		{
			if (N.isIndexed())
			{
				ofLogError("ofxAlembic::PolyMesh") << "indexed normal is not supported";
			}
			else
			{
				N3fArraySamplePtr norm_ptr = N.getExpandedValue(ss).getVals();
				N3f norm;
				vector<ofVec3f> norms;

				for (int i = 0; i < norm_ptr->size(); i++)
				{
					const N3f& v = (*norm_ptr)[i];
					norms.push_back(ofVec3f(v.x, v.y, v.z));
				}

				for (int i = 0; i < m_triangles.size(); i++)
				{
					Tri &t = m_triangles[i];
					mesh.addNormal(norms[t[0]]);
					mesh.addNormal(norms[t[1]]);
					mesh.addNormal(norms[t[2]]);
				}
			}
		}
	}

	{
		IV2fGeomParam UV = schema.getUVsParam();
		if (UV.valid())
		{
			if (UV.isIndexed())
			{
				ofLogError("ofxAlembic::PolyMesh") << "indexed uv is not supported";
			}
			else
			{
				V2fArraySamplePtr uv_ptr = UV.getExpandedValue(ss).getVals();

				for (int i = 0; i < m_triangles.size(); i++)
				{
					Tri &t = m_triangles[i];
					mesh.addTexCoord(toOf((*uv_ptr)[t[0]]));
					mesh.addTexCoord(toOf((*uv_ptr)[t[1]]));
					mesh.addTexCoord(toOf((*uv_ptr)[t[2]]));
				}
			}
		}
	}
}

void PolyMesh::draw()
{
	if (ofGetStyle().bFill)
	{
		mesh.draw();
	}
	else
	{
		mesh.drawWireframe();
	}
}

#pragma mark - Curves

void Curves::get(OCurvesSchema &schema) const
{
	vector<V3f> positions;
	vector<int32_t> num_vertices;

	for (int n = 0; n < curves.size(); n++)
	{
		const ofPolyline &polyline = curves[n];

		for (int i = 0; i < polyline.size(); i++)
		{
			positions.push_back(toAbc(polyline[i]));
		}

		num_vertices.push_back(polyline.size());
	}

	OCurvesSchema::Sample sample((P3fArraySample(positions)),
								 Int32ArraySample(num_vertices),
								 kLinear,
								 kNonPeriodic);
	schema.set(sample);
}

void Curves::set(ICurvesSchema &schema, float time)
{
	ISampleSelector ss(time, ISampleSelector::kNearIndex);
	ICurvesSchema::Sample sample;
	schema.get(sample, ss);

	P3fArraySamplePtr m_positions = sample.getPositions();
	std::size_t m_nCurves = sample.getNumCurves();

	const V3f *src = m_positions->get();
	const Alembic::Util::int32_t *nVertices = sample.getCurvesNumVertices()->get();
	V3f dst;

	curves.resize(m_nCurves);

	for (int i = 0; i < m_nCurves; i++)
	{
		ofPolyline &polyline = curves[i];
		const int num = nVertices[i];

		polyline.clear();

		for (int n = 0; n < num; n++)
		{
			const V3f& v = *src;
			polyline.addVertex(ofVec3f(v.x, v.y, v.z));
			src++;
		}
	}
}

void Curves::draw()
{
	for (int i = 0; i < curves.size(); i++)
	{
		curves[i].draw();
	}
}


#pragma mark - Camera

void Camera::get(OCameraSchema &schema) const
{
	ofLogError("ofxAlembic::Camera") << "not implemented";
}

void Camera::set(ICameraSchema &schema, float time)
{
	ISampleSelector ss(time, ISampleSelector::kNearIndex);
	schema.get(sample, ss);
}

void Camera::updateParams(ofCamera &camera, ofMatrix4x4 xform)
{
	float w, h;
	if (width == 0 || height == 0)
	{
		w = ofGetViewportWidth();
		h = ofGetViewportHeight();
	}
	else
	{
		w = width;
		h = height;
	}
	
	float fovH = sample.getFieldOfView();
	float fovV = ofRadToDeg(2 * atanf(tanf(ofDegToRad(fovH) / 2) * (h / w)));
	camera.setFov(fovV);
	camera.setTransformMatrix(xform);
	
	// TODO: lens offset
}

void Camera::draw()
{
}

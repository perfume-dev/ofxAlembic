#include "ofApp.h"

#include "ofxAlembic.h"

ofEasyCam cam;

ofxAlembic::Reader abc;

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0);
	
	string path = "sample.abc";
	
	{
		ofCamera outcam;
		ofxAlembic::Writer writer;
		
		if (writer.open(path, 30)) // export at 30fps
		{
			for (int f = 0; f < 60; f++)
			{
				// points
				{
                    vector<glm::vec3> points;
					
					for (int i = 0; i < 10; i++)
					{
                        glm::vec3 p;
						p.x = ofRandom(-300, 300);
						p.y = ofRandom(-300, 300);
						p.z = ofRandom(-300, 300);
						points.push_back(p);
					}
					
					writer.addPoints("/points", points);
				}
				
				// curves
				{
					vector<ofPolyline> curves;
					
					for (int i = 0; i < 10; i++)
					{
						ofPolyline poly;
						
						for (int n = 0; n < 100; n++)
						{
							ofVec3f v;
							v.x = ofSignedNoise(1, 0, 0, n * 0.01 + f * 10 + i) * 300;
							v.y = ofSignedNoise(0, 1, 0, n * 0.01 + f * 10 + i) * 300;
							v.z = ofSignedNoise(0, 0, 1, n * 0.01 + f * 10 + i) * 300;
							poly.addVertex(v);
						}
						
						curves.push_back(poly);
					}
					
					writer.addCurves("/curves", curves);
				}
				
				// mesh
				{
					ofMesh mesh;
					
					int num = ofRandom(1, 10) * 3;
					
					for (int i = 0; i < num; i++)
					{
						ofVec3f p;
						p.x = ofRandom(-300, 300);
						p.y = ofRandom(-300, 300);
						p.z = ofRandom(-300, 300);
						mesh.addVertex(p);
					}
					
					writer.addPolyMesh("/polymesh", mesh);
				}
				
				// mesh with xform
				{
					ofMatrix4x4 mat;
					mat.glRotate(f * 5, 0, 1, 0);
					writer.addXform("/box", toGlm(mat));
					
					if (f == 0)
					{
						// write only first frame
						
						ofBoxPrimitive box;
						box.set(100);
						writer.addPolyMesh("/box/boxShape", box.getMesh());
					}
				}
				
				// camera
				{
					outcam.setFov(20 + f * 0.5);
					outcam.orbitDeg(f, 50.0, 600.0 + 600.0 * ofNoise(f / 60.0 + 10.0));
					outcam.begin();
					outcam.end();
					
					// implicitly add xform node
					writer.addCamera("/of_camera", outcam);
				}
			}
		}
	}
	
	if (!abc.open(path))
	{
		ofLogError("example-write") << "Could not open generated file " << path;
		return;
	}
	
	abc.dumpNames();
}

void ofApp::exit()
{
	abc.close();
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (!abc.isOpen() || abc.getMaxTime() <= 0)
		return;

	float t = fmodf(ofGetElapsedTimef(), abc.getMaxTime());
	abc.setTime(t);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	abc.get("/of_camera/cameraShape", cam);
	
	cam.begin();

	glPointSize(4);
	
	{
		ofMesh mesh;
		abc.get("/polymesh", mesh);
		
		ofSetColor(255, 0, 0);
		mesh.draw();
	}

	{
        vector<glm::vec3> points;
		abc.get("/points", points);
		
		ofSetColor(0, 255, 0);
		glBegin(GL_POINTS);
		for (int i = 0; i < points.size(); i++)
			glVertex3fv(&points[i].x);
		glEnd();
	}

	{
		vector<ofPolyline> curves;
		abc.get("/curves", curves);
		
		ofSetColor(0, 0, 255);
		for (int i = 0; i < curves.size(); i++)
			curves[i].draw();
	}

	{
		ofSetColor(255, 255, 0);
		if (ofxAlembic::IGeom* box = abc.get("/box"))
			box->draw(); // draw box with xform
	}

	// or simply, abc.draw();
	
	cam.end();
	
	ofSetColor(255);
	
	ofDrawBitmapString(ofToString(abc.getTime()) + "/" + ofToString(abc.getMaxTime()), 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}

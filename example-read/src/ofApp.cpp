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
	
	string path = "sample_ogawa.abc";
	
	// load allembic file
	if (!abc.open(path))
	{
		ofLogError("example-read") << "Could not open " << path;
		return;
	}
	
	// show all drawable names
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
	
	// update alemblic reader with time in sec
	abc.setTime(t);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	cam.begin();

	glPointSize(4);
	
	// get meshes and draw
	
	{
		ofMesh mesh;
		abc.get("/Cloner/ClonerShape", mesh);
		
		ofSetColor(255, 0, 0);
		mesh.draw();
	}

	{
        std::vector<glm::vec3> points;
		abc.get("/Emitter/EmitterCloud", points);
		
		ofSetColor(0, 255, 0);
		glBegin(GL_POINTS);
		for (int i = 0; i < points.size(); i++)
			glVertex3fv(&points[i].x);
		glEnd();
	}

	{
		vector<ofPolyline> curves;
		abc.get("/Tracer/TracerSpline", curves);
		
		ofSetColor(0, 0, 255);
		for (int i = 0; i < curves.size(); i++)
			curves[i].draw();
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

#include "testApp.h"

#include "ofxObjLoader.h"
#include "ofxAlembic.h"

ofxAlembic::Reader abc;
ofxAlembic::Writer writer;

ofEasyCam cam;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0);
	
	string path = "write_test.abc";

	if (writer.open(path, 30)) // 30fps
	{
		// list objfile
		ofDirectory dir;
		dir.allowExt("obj");
		
		int n = dir.listDir( __PATH_TO_OBJ_SEQUENCE_DIR__ );
		
		dir.sort();

		for (int i = 0; i < n; i++)
		{
			cout << i << ": " << dir.getPath(i) << endl;
			
			ofMesh mesh;
			ofxObjLoader::load(dir.getPath(i), mesh);
			
			// custom mesh transform
			ofMatrix4x4 mm;
			mm.glScale(100, 100, 100);
			mm.glRotate(90, -1, 0, 0);
			
			// apply transform
			ofxAlembic::transform(mesh, mm);

			writer.addPolyMesh("test", mesh);
		}
		
		writer.close();
		
		cout << "done" << endl;
	}

	// open saved alembic file
	abc.open(path);
}

//--------------------------------------------------------------
void testApp::update()
{
	float t = fmodf(ofGetElapsedTimef(), abc.getMaxTime());
	abc.setTime(t);
}

//--------------------------------------------------------------
void testApp::draw()
{
	glEnable(GL_DEPTH_TEST);
	
	cam.begin();
	ofSetColor(255);
	abc.draw();
	cam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}
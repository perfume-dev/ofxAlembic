#include "ofApp.h"

#include "ofxAlembic.h"

ofEasyCam cam;
ofxAlembic::Reader abc;

ofShader shader;

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0);
	
	string path = "alembic_test_ogawa.abc";
	
	if (!abc.open(path))
	{
		ofLogError("example-shader") << "Could not open " << path;
		return;
	}
	
	shader.load("shader.vert", "shader.frag");
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
    shader.begin();
    shader.setUniform1f("elapsedTime", ofGetElapsedTimef());
    shader.setUniform2f("mouse", mouseX, mouseY);
    shader.setUniform2f("resolution", ofGetWindowWidth(), ofGetWindowHeight());
    shader.end();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	
	ofEnableAlphaBlending();
    
	cam.begin();

    shader.begin();

    for (int i = 0; i < abc.size(); i++)
    {
        ofMesh mesh;
        if (abc.get(i, mesh))
        {
            ofSetColor(255);
            mesh.draw();
            // mesh.drawWireframe();
        }
    }

    shader.end();

	cam.end();
    
    glDisable(GL_DEPTH_TEST);
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

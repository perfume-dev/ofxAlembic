#include "testApp.h"

#include "ofxAlembic.h"

ofEasyCam cam;
ofxAlembic::Reader abc;

class AutoLoadShader : public ofShader
{
public:
	
	Poco::Timestamp lastFragTimestamp, lastVertTimestamp;
	ofFile vertShader, fragShader;
	
	void watch(string vertName, string fragName)
	{
		vertShader.open(vertName);
		fragShader.open(fragName);
		
		ofAddListener(ofEvents().update, this, &AutoLoadShader::onUpdate);
	}
	
	void reload()
	{
		load(vertShader.path(), fragShader.path());
	}
	
	void onUpdate(ofEventArgs &e)
	{
		if (lastVertTimestamp != vertShader.getPocoFile().getLastModified()
			|| lastFragTimestamp != fragShader.getPocoFile().getLastModified())
		{
			reload();
		}
	}
};

AutoLoadShader shader;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(0);
	
	string path = "perfume_global_site_003_sequence.abc";
	
	abc.open(path);
	
	shader.watch("shader.vert", "shader.frag");
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
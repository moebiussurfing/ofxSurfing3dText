#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

#if 1
	// Customize ofxGui Theme
	ofxSurfing::setOfxGuiTheme();

	// App window
	ofxSurfing::setWindowTitleAsProjectName();
	ofxSurfing::setWindowAtMonitor(-1);
#endif

	t.setup();

	//--

	// Pass the local camera
	pbr.setup(camera);

	// Pass the scene renderer to be processed
	callback_t f = std::bind(&ofApp::renderScene, this);
	pbr.setFunctionRenderScene(f);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {

	drawScene();

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawScene() {

	ofEnableDepthTest();

	pbr.draw();
}

//--------------------------------------------------------------
void ofApp::drawGui() {

	ofDisableDepthTest();

	pbr.drawGui();

	t.drawGui();
}

//--------------------------------------------------------------
void ofApp::renderScene() {

	ofEnableDepthTest();

	pbr.drawFloor();

	pbr.beginMaterial();
	{
		drawMyScene();
	}
	pbr.endMaterial();
}

//--------------------------------------------------------------
void ofApp::drawMyScene() {

	//ofEnableLighting();

	bool b = t.bDrawBounds || t.bDrawMeshes;
	
	if (!b) {
		pbr.drawTestScene();
	}
	
	else {
		pbr.beginMaterial();
		{
			t.drawMeshes();
		}
		pbr.endMaterial();
	}

	//--

	//ofDisableLighting();
	t.drawBounds();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofLogNotice(__FUNCTION__) << " : " << key;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	ofLogNotice(__FUNCTION__) << w << "," << h;
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofLogNotice(__FUNCTION__);

	pbr.exit(); // Only required to save camera on exit
}

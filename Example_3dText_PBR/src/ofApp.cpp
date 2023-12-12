#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

#if 1
	ofxSurfing::setOfxGuiTheme();
	ofxSurfing::setWindowTitleAsProjectName();
	ofxSurfing::setWindowAtMonitor(-1);
#endif

	t.setup();

	//--

	pbr.setup(camera);

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

	pbr.draw();
}

//--------------------------------------------------------------
void ofApp::drawGui() {

	pbr.drawGui();

	t.drawGui();
}

//--------------------------------------------------------------
void ofApp::renderScene() {

	drawMyScene();
}

//--------------------------------------------------------------
void ofApp::drawMyScene() {

	pbr.drawFloor();

	pbr.drawTestScene();

	pbr.beginMaterial();
	{
		t.drawMeshes();
	}
	pbr.endMaterial();

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

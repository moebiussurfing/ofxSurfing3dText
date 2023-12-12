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
		//// CULL the back faces of the geometry for rendering
		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CW);
		//glCullFace(GL_BACK);

		t.drawMeshes();

		//glDisable(GL_CULL_FACE);
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

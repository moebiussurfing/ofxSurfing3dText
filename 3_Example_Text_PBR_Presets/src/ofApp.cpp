#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

#if 1
	ofxSurfing::setWindowTitleAsProjectName();
	ofxSurfing::setWindowAtMonitor(-1);
	ofxSurfing::setOfxGuiTheme(0);
#endif

	t.setup();

	// select only what we want to store,
	// to simplify or adapt to our workflow.
	paramsPreset.setName("TEXT_PARAMS");
	paramsPreset.add(t.paramsPreset);

	presets.setKitName("TEXT\\Presets");
	presets.setup(paramsPreset);

	pbr.setup(camera);
	callback_t f = std::bind(&ofApp::renderScene, this);
	pbr.setFunctionRenderScene(f);

	paramsApp.setName("ofApp");
	paramsApp.add(presets.bGui);
	paramsApp.add(t.bGui);
	paramsApp.add(pbr.bGui);
	guiApp.setup(paramsApp);

	//refresh layout
	ofxSurfing::setGuiPositionToLayout(guiApp, ofxSurfing::SURFING_LAYOUT_BOTTOM_LEFT);

	//TODO
	//t.refreshGui(presets.guiParams, "TEXT_NODE");
	//t.refreshGui(presets.guiParams, presets.parameters);
	//TODO: fix bc seems renamed on runtime..?
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
	presets.drawGui();

	guiApp.draw();
}

//--------------------------------------------------------------
void ofApp::renderScene() {
	drawMyScene();
}

//--------------------------------------------------------------
void ofApp::drawMyScene() {
	ofEnableDepthTest();

	pbr.drawFloor();

	pbr.beginMaterial();
	{
		pbr.drawTestScene();
		t.drawMeshes();
	}
	pbr.endMaterial();

	t.drawBounds();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofLogNotice(__FUNCTION__) << ": " << key;
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
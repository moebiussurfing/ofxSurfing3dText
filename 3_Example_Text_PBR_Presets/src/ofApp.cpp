#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

#if 1
	ofxSurfing::setWindowTitleAsProjectName();
	ofxSurfing::setWindowAtMonitor(1);
	ofxSurfing::setOfxGuiTheme(true);
#endif

	t.setup();

	// select only what we want to store, 
	// to simplify or adapt to our workflow.
	paramsPreset.setName("TEXT");

	//letters
	paramsPreset.add(t.extrusion);
	paramsPreset.add(t.letterSpacing);
	paramsPreset.add(t.heightLine);
	paramsPreset.add(t.pathFont);

	//transform
	paramsPreset.add(t.transformNode.paramsPreset);

	presets.setKitName("TEXT\\Presets");
	presets.setup(paramsPreset);

	pbr.setup(camera);
	callback_t f = std::bind(&ofApp::renderScene, this);
	pbr.setFunctionRenderScene(f);

	paramsApp.setName("ofApp");
	paramsApp.add(presets.bGui);
	paramsApp.add(t.bGui);
	paramsApp.add(pbr.bGui);
	gui.setup(paramsApp);

	//refresh
	ofxSurfing::setGuiPositionToLayout(gui, ofxSurfing::SURFING_LAYOUT_BOTTOM_LEFT);
	//TODO
	t.transformNode.refreshGui(presets.guiParams, "TEXT_PRESET");
	//t.transformNode.refreshGui(presets.guiParams, presets.parameters);
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

	gui.draw();
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

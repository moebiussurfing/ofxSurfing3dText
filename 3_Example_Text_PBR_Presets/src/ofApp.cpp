#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

#if 1
	ofxSurfing::setWindowTitleAsProjectName();
	ofxSurfing::setWindowAtMonitor(-1);
	ofxSurfing::setOfxGuiTheme(true);
#endif

	t.setup();

	gPreset.setName("TEXT");
	gPreset.add(t.extrusion);
	gPreset.add(t.heightLine);
	gPreset.add(t.letterSpacing);
	gPreset.add(t.transform.scale);
	gPreset.add(t.transform.position);
	gPreset.add(t.transform.vResetPosition);
	gPreset.add(t.transform.rotation);
	gPreset.add(t.transform.vResetRotation);
	gPreset.add(t.transform.vReset);

	p.setPathPresets("TEXT\\Presets");
	p.addGroup(gPreset);

	pbr.setup(camera);
	callback_t f = std::bind(&ofApp::renderScene, this);
	pbr.setFunctionRenderScene(f);

	gGui.setName("ofApp");
	gGui.add(p.bGui);
	gGui.add(t.bGui);
	gGui.add(pbr.bGui);
	gui.setup(gGui);
	ofxSurfing::setGuiPositionToLayout(gui, ofxSurfing::SURFING_LAYOUT_BOTTOM_LEFT);
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
	p.drawGui();

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

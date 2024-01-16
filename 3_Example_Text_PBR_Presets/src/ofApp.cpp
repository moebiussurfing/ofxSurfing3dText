#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

#if 1
	ofxSurfing::setWindowTitleAsProjectName();
	ofxSurfing::setWindowAtMonitor(-1);
	ofxSurfing::setOfxGuiTheme();
#endif

	//--

	// Text

	text3d.setup();

	//--

	// Pbr

	pbr.setup(camera);
	callback_t f = std::bind(&ofApp::renderScene, this);
	pbr.setFunctionRenderScene(f);
	pbr.bDrawTestScene = false;

	//--

	// Presets

	string name = "myTextScene";
	paramsPreset.setName(name);
	paramsPreset.add(text3d.paramsPreset);
	//paramsPreset.add(pbr.paramsPreset);//TODO

	//presets.setKitName("TEXT\\Presets"); // Optional: custom sub path for multiple preset kits!
	presets.setup(paramsPreset);
	
	//TODO
	//refresh gui
	//text3d.refreshGuiUserParams(presets.guiParams, presets.guiParams.getGroup(paramsPreset.getName()));
	//text3d.refreshGuiUserParams(presets.guiParams);

	//--

	// App UI
	paramsApp.setName("ofApp");
	paramsApp.add(presets.bGui);
	paramsApp.add(text3d.bGui);
	paramsApp.add(pbr.bGui);

	guiApp.setup(paramsApp);

	// Refresh layout
	ofxSurfing::setGuiPositionToLayout(guiApp, ofxSurfing::SURFING_LAYOUT_BOTTOM_LEFT);
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
	if (!bGui) return;

	ofDisableDepthTest();

	pbr.drawGui();
	text3d.drawGui();
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
		text3d.drawMeshes();
		pbr.drawTestScene();//must be enable on PBR_SCENE/TestScene/Draw TestScene
	}
	pbr.endMaterial();

	text3d.drawBounds();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofLogNotice(__FUNCTION__) << ": " << key;

	if (key == 'g') bGui = !bGui;
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
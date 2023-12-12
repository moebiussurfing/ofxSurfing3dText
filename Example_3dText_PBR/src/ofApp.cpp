#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

#if 1
	// Theme
	ofxSurfing::setOfxGuiTheme(); // Customize ofxGui theme.

	// App window
	{
		ofxSurfing::setWindowTitleAsProjectName(); // Name the window app.

		// Move and shape the window app.
		// Customize settings: 60fps and vSync off.
		// Move to left display and set landscape.
		ofxSurfing::setWindowAtMonitor(-1);
	}
#endif

	t.setup();

	//--

	//// Pass the local camera
	//pbr.setup(camera);

	//// Pass the scene renderer to be processed
	//callback_t f = std::bind(&ofApp::renderScene, this);
	//pbr.setFunctionRenderScene(f);

	// Pass render functions required to do the shadow passes!
	callback_t f = std::bind(&ofApp::renderScene, this);
	sceneManager.setFunctionRenderScene(f);

	string nm;
	nm = "MAT_" + ofToString(0);
	sceneManager.addMaterial(nm);

	sceneManager.setupBuild();
}

//--------------------------------------------------------------
void ofApp::exit() {
	ofLogNotice(__FUNCTION__);
	
	//pbr.exit(); // Only required to save camera on exit
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
	//pbr.draw();

	ofEnableDepthTest();

	sceneManager.updateLights();

	camera.begin();
	{
		sceneManager.beginLights();
		{
			renderScene();

			sceneManager.drawDebugLights();
		}
		sceneManager.endLights();
	}
	camera.end();
}

//--------------------------------------------------------------
void ofApp::drawGui() {

	ofDisableDepthTest();

	t.drawGui();

	sceneManager.drawGui();

	//pbr.drawGui();
}

//--------------------------------------------------------------
void ofApp::renderScene() {
	//// The camera is internal!

	//// Plane floor (Material A / simple)
	//// don't need to begin/end material manually.
	//pbr.drawPlane();

	//// For other objects (Material B / full parameters)
	//pbr.beginMaterial();
	//{
		drawYourScene();
	//}
	//pbr.endMaterial();

	//sceneManager.beginMaterial(0);

	//sceneManager.endMaterial(0);
}

//--------------------------------------------------------------
void ofApp::drawYourScene() {
	ofEnableLighting();

	// Draws an internal bundled testing scene with the three prims
	//pbr.drawTestScene();

	//pbr.beginMaterial();
	//{
	//t.drawMeshes();
	//}
	//pbr.endMaterial();

	sceneManager.beginMaterial(0);
	{
		t.drawMeshes();
	}
	sceneManager.endMaterial(0);

	//--

	ofDisableLighting();
	
	t.drawBounds();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofLogNotice(__FUNCTION__) << " : " << key;

	//pbr.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::exit() {
	//pbr.exit(); // Only required to save camera on exit
}
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
		ofxSurfing::setWindowAtMonitor(-1); // Move to left display and set landscape.
		//ofxSurfing::setWindowAtMonitor(1, true); // Move to right display and set portrait.
		//ofxSurfing::setWindowAtMonitor(); // Stay at main display (center in my setup) and landscape.
	}
#endif

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

	t.drawHelp();

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

	return;
	ofDisableLighting();
	t.drawBounds();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//pbr.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}

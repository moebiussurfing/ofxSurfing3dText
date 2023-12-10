#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	light.setup();
	light.enable();
	light.setPointLight();
	light.setAmbientColor(ofFloatColor(0.6));
	light.setDiffuseColor(ofFloatColor(1, 1, 1));
	light.setSpecularColor(ofFloatColor(1, 1, 1));

	light.setPosition(-50, -300, 600);

	material.setAmbientColor(ofColor(247, 59, 103));
	material.setDiffuseColor(ofColor(247, 59, 103));
	material.setSpecularColor(ofFloatColor(1.0, 1.0));
	material.setShininess(100);

	camera.setDistance(1100);
	camera.orbitDeg(0, -30, 1100);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor(10), ofColor(50));

	ofEnableDepthTest();
	camera.begin();
	{
		ofEnableLighting();
		material.begin();
		{
			t.drawMeshes();
		}
		material.end();
		ofDisableLighting();

		t.drawBounds();
	}
	camera.end();
	ofDisableDepthTest();

	t.drawHelp();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
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

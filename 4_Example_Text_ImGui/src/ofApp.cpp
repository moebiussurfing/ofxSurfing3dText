#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#if 1
	ofxSurfing::setOfxGuiTheme();
	ofxSurfing::setWindowAtMonitor(-1);
#endif

	setupLights();

	material.setSpecularColor(ofFloatColor(1.0, 1.0));
	material.setShininess(100);

	camera.setDistance(1100);
	camera.orbitDeg(0, -30, 1100);

	t.setup(camera);
	camera.disableMouseInput();
	bMouseCam = camera.getMouseInputEnabled();
}

//--------------------------------------------------------------
void ofApp::setupLights() {
	ofSetSmoothLighting(true);

	for (int i = 0; i < OF_APP__NUM_LIGHTS; i++) {
		auto pointLight = make_shared<ofLight>();
		pointLight->setup();
		pointLight->enable();
		pointLight->setPointLight();
		pointLight->setDiffuseColor(ofColor(0.f, 255.f, 0.f));
		pointLight->setAmbientColor(ofFloatColor(0.2f));
		pointLight->setSpecularColor(ofColor(255.f, 255.f, 255.f));
		pointLight->setAttenuation(1.0, 0.00001, 0.00001);
		pointLights.push_back(pointLight);
	}

	colorHue = ofRandom(0, 250);

	lightColor.setBrightness(180.f);
	lightColor.setSaturation(150.f);
}

//--------------------------------------------------------------
void ofApp::update() {
	updateLights();
}

//--------------------------------------------------------------
void ofApp::draw() {

	drawScene();

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawScene() {
	ofBackgroundGradient(ofColor(10), ofColor(50));

	ofEnableDepthTest();

	camera.begin();
	{
		if (bEnableLights) ofEnableLighting();

		material.begin();
		{
			t.drawMeshes();
		}
		material.end();
		t.drawBounds();

		if (bEnableLights) ofDisableLighting();

		// debug/draw point lights
		if (t.bDebug) {
			ofPushStyle();
			for (int i = 0; i < (int)pointLights.size(); i++) {
				ofSetColor(pointLights[i]->getDiffuseColor());
				if (!pointLights[i]->getIsEnabled()) {
					ofSetColor(40);
				}
				pointLights[i]->draw();
			}
			ofPopStyle();
		}
	}
	camera.end();
}

//--------------------------------------------------------------
void ofApp::drawGui() {
	if (!bGui) return;

	ofDisableDepthTest();

	t.drawGui(); //ofxGui

#ifdef SURFING__USE__IMGUI
	t.drawImGui();

	//bool b_ = camera.getMouseInputEnabled();
	//if (b_) {
	//	bool b = t.ui.isMouseOverGui();
	//	if (b) camera.disableMouseInput();
	//}
#endif

	if (bHelp) {
		buildHelp();
		ofxSurfing::ofDrawBitmapStringBox(sHelp, ofxSurfing::SURFING_LAYOUT_BOTTOM_LEFT);
	}
}

//--------------------------------------------------------------
void ofApp::updateLights() {
	if (!bEnableLights) return;

	float xdist = ofGetViewportWidth() * 0.9;
	float ydist = (ofGetViewportHeight() / 2.f) * 0.7;

	float deltaTime = ofClamp(ofGetLastFrameTime(), 1.f / 5000.f, 1.f / 5.f);

	if (bAnim) angle += deltaTime;

	colorHue += 1.f;
	if (colorHue >= 255) {
		colorHue = 0.f;
	}
	lightColor.setHue(colorHue);

	for (int i = 0; i < (int)pointLights.size(); i++) {
		auto & l = pointLights[i];
		lightColor.setHue(ofWrap(colorHue + (float)i * 12.5, 0, 255.f));
		l->setDiffuseColor(lightColor);

		float tx = ofMap(i, 0, OF_APP__NUM_LIGHTS - 1, -xdist * 0.5, xdist * 0.5f);
		float ty = sin(angle + (float)i * cos(angle * 0.05) * 1.9f) * ydist;
		float tz = 50 + t.extrusion;
		l->setPosition(tx, ty, tz);
	}

	material.setAmbientColor(t.color);
	material.setDiffuseColor(t.color);
}

//--------------------------------------------------------------
void ofApp::buildHelp() {

	bool b1 = pointLights[0]->getIsEnabled();
	bool b2 = pointLights[1]->getIsEnabled();
	bool b3 = pointLights[2]->getIsEnabled();
	bool b4 = pointLights[3]->getIsEnabled();
	bool b5 = pointLights[4]->getIsEnabled();

	string s = "";
	s += ofxSurfing::getProjectName() + "\n\n";
	s += "KEYS\n\n";
	s += "GUI\n";
	s += "G       " + string(bGui ? "ON" : "OFF") + "\n\n";
	s += "HELP\n";
	s += "H       " + string(bHelp ? "ON" : "OFF") + "\n\n";
	s += "MOUSE\n";
	s += "M       " + string(bMouseCam ? "ON" : "OFF") + "\n\n";
	s += "LIGHTS\n";
	s += "A Anim  " + string(bAnim ? "ON" : "OFF") + "\n";
	s += "L All   " + string(bEnableLights ? "ON" : "OFF") + "\n";
	s += "L1      " + string(b1 ? "ON" : "OFF") + "\n";
	s += "L2      " + string(b2 ? "ON" : "OFF") + "\n";
	s += "L3      " + string(b3 ? "ON" : "OFF") + "\n";
	s += "L4      " + string(b4 ? "ON" : "OFF") + "\n";
	s += "L5      " + string(b5 ? "ON" : "OFF");

	sHelp = s;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'G') {
		bGui = !bGui;
		return;
	}

	if (key == 'L') {
		bEnableLights = !bEnableLights;
		for (int i = 0; i < (int)pointLights.size(); i++) {
			auto & l = pointLights[i];
			if (bEnableLights)
				l->enable();
			else
				l->disable();
		}
		return;
	}

	if (key == 'H') {
		bHelp = !bHelp;
		return;
	}

	if (key == 'A') {
		bAnim = !bAnim;
		return;
	}

	if (key == 'M') {
		bool b = camera.getMouseInputEnabled();
		if (b)
			camera.disableMouseInput();
		else
			camera.enableMouseInput();
		bMouseCam = camera.getMouseInputEnabled();
		return;
	}

	int lindex = -1;
	if (key == '1') {
		lindex = 0;
	} else if (key == '2') {
		lindex = 1;
	} else if (key == '3') {
		lindex = 2;
	} else if (key == '4') {
		lindex = 3;
	} else if (key == '5') {
		lindex = 4;
	}
	if (lindex > -1 && lindex < (int)pointLights.size()) {
		if (pointLights[lindex]->getIsEnabled()) {
			pointLights[lindex]->disable();
		} else {
			pointLights[lindex]->enable();
		}
	}
}
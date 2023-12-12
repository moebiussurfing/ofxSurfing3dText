#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//ofxSurfing::setWindowAtMonitor(-1);

	setupLights();

	material.setAmbientColor(ofColor(247, 59, 103));
	material.setDiffuseColor(ofColor(247, 59, 103));
	material.setSpecularColor(ofFloatColor(1.0, 1.0));
	material.setShininess(100);

	camera.setDistance(1100);
	camera.orbitDeg(0, -30, 1100);

	t.setup();
}

//--------------------------------------------------------------
void ofApp::setupLights() {
	ofSetSmoothLighting(true);

	float xdist = 1200;
	float ty = 500;
	float tz = 500;

	for (int i = 0; i < OF_APP__NUM_LIGHTS; i++) {
		auto pointLight = make_shared<ofLight>();
		pointLight->setup();
		pointLight->enable();
		pointLight->setPointLight();
		pointLight->setDiffuseColor(ofColor(0.f, 255.f, 0.f));
		pointLight->setAmbientColor(ofFloatColor(0.2f));
		pointLight->setSpecularColor(ofColor(255.f, 255.f, 255.f));
		float tx = ofMap(i, 0, OF_APP__NUM_LIGHTS - 1, -xdist * 0.5, xdist * 0.5f);
		pointLight->setPosition(tx, ty, tz);
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

	//--

	drawGui();
}

//--------------------------------------------------------------
void ofApp::drawScene() {
	ofBackgroundGradient(ofColor(10), ofColor(50));

	ofEnableDepthTest();

	if (bEnableLights) ofEnableLighting();

	camera.begin();
	{
		ofEnableLighting();
		material.begin();
		{
			t.drawMeshes();
		}
		material.end();
		t.drawBounds();

		if (bEnableLights) ofDisableLighting();

		// debug
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
	camera.end();
}

//--------------------------------------------------------------
void ofApp::drawGui() {
	ofDisableDepthTest();

	t.drawGui();

	if (bHelp) {
		buildHelp();
		ofxSurfing::ofDrawBitmapStringBox(sHelp, ofxSurfing::SURFING_LAYOUT_BOTTOM_LEFT);
	}
}

//--------------------------------------------------------------
void ofApp::updateLights() {
	float h = 100;

	float deltaTime = ofClamp(ofGetLastFrameTime(), 1.f / 5000.f, 1.f / 5.f);
	angle += deltaTime;

	colorHue += 1.f;
	if (colorHue >= 255) {
		colorHue = 0.f;
	}
	lightColor.setHue(colorHue);

	for (int i = 0; i < (int)pointLights.size(); i++) {
		auto & l = pointLights[i];
		lightColor.setHue(ofWrap(colorHue + (float)i * 12.5, 0, 255.f));
		l->setDiffuseColor(lightColor);

		float positionY = sin(angle + (float)i * cos(angle * 0.05) * 1.9f) * h;
		l->setPosition(l->getX(), positionY, 200.0);
	}
}

//--------------------------------------------------------------
void ofApp::buildHelp() {

	bool b1 = pointLights[0]->getIsEnabled();
	bool b2 = pointLights[1]->getIsEnabled();
	bool b3 = pointLights[2]->getIsEnabled();
	bool b4 = pointLights[3]->getIsEnabled();
	bool b5 = pointLights[4]->getIsEnabled();

	string s = "";
	s += "KEYS\n\n";
	s += "HELP\n";
	s += "H " + string(bHelp ? "ON" : "OFF") + "\n\n";
	s += "DEBUG LIGHTS\n";
	s += "L " + string(bEnableLights ? "ON" : "OFF") + "\n\n";
	s += "ENABLE LIGHT\n";
	s += "1 " + string(b1 ? "ON" : "OFF") + "\n";
	s += "2 " + string(b2 ? "ON" : "OFF") + "\n";
	s += "3 " + string(b3 ? "ON" : "OFF") + "\n";
	s += "4 " + string(b4 ? "ON" : "OFF") + "\n";
	s += "5 " + string(b5 ? "ON" : "OFF");

	sHelp = s;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'l' || key == 'L') {
		bEnableLights = !bEnableLights;
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
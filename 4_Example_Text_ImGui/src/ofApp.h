#pragma once
#include "ofMain.h"

#include "ofxSurfing3dText.h"
/* 
Must enable
#define SURFING__USE__IMGUI
at the top of the ofxSurfing3dText.h file!
*/

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	
	void draw();
	void drawScene();
	void drawGui();
	bool bGui = true;

	void keyPressed(int key);

	ofEasyCam camera;
	bool bMouseCam = true;

	ofxSurfing3dText t;

	ofMaterial material;

	const int OF_APP__NUM_LIGHTS = 5;
	void setupLights();
	void updateLights();
	bool bEnableLights = true;
	bool bAnim = true;
	vector<shared_ptr<ofLight>> pointLights;
	float colorHue = 0.0f;
	float angle = 0.0f;
	ofColor lightColor;
	void buildHelp();
	string sHelp = "";
	bool bHelp = true;
};

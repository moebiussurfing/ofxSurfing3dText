#pragma once
#include "ofMain.h"

#include "ofxSurfing3dText.h"
#include "ofxSurfingPBR.h"
#include "ofxSurfingPresetsLiteOfxGui.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void exit();
	void update();
	void draw();
	void drawGui();
	void drawScene();
	void keyPressed(int key);
	void windowResized(int w, int h);

	ofEasyCam camera;
	ofxSurfingPBR pbr;
	void renderScene();
	void drawMyScene();
	
	ofxSurfing3dText t;

	ofParameterGroup gPreset;
	SurfingPresetsLiteOfxGui p;

	ofxPanel gui;
	ofParameterGroup gGui;
};

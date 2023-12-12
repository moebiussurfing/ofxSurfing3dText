#pragma once

#include "ofMain.h"

#include "ofxSurfing3dText.h"

class ofApp : public ofBaseApp {
	public:
		void setup();
		void update();
		void draw();
		void drawScene();
		void drawGui();

		void keyPressed(int key);

		ofxSurfing3dText t;

		ofMaterial material;
		ofEasyCam camera;

		const int OF_APP__NUM_LIGHTS = 5;
		bool bEnableLights = true;
		vector<shared_ptr<ofLight>> pointLights;
		float colorHue = 0.0f;
		float angle = 0.0f;
		ofColor lightColor;
		void setupLights();
		void updateLights();
		void buildHelp();
		string sHelp = "";
		bool bHelp = true;
};

#pragma once

#include "ofMain.h"

#include "ofxSurfing3dText.h"

#include "ofxSurfingPBR.h"

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

		ofxSurfing3dText t;

		ofxSurfingPBR pbr;

		void renderScene();
		void drawMyScene();

};

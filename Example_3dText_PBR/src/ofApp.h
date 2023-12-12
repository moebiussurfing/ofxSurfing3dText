#pragma once

#include "ofMain.h"

#include "ofxSurfing3dText.h"

//#include "ofxSurfingPBR.h"
#include "SurfingSceneManager.h"

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

		ofxSurfing3dText t;

		SurfingSceneManager sceneManager;

		//ofxSurfingPBR pbr;

		void renderScene();
		void drawYourScene();

		ofEasyCam camera;
};

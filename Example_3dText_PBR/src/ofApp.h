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
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofxSurfing3dText t;

		SurfingSceneManager sceneManager;

		//ofxSurfingPBR pbr;

		void renderScene();
		void drawYourScene();

		ofEasyCam camera;
};

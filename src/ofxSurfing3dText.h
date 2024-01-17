/*
	TODO
	fix link scale vs scale normalized
	fix link scale to power scale 
	fix node rotator to euler deg
*/

//--

// Optional: UI
// Uncomment to use ImGui!
// ofxGui panel will be there but hidden!
#define SURFING__USE__IMGUI

//--

// NOTE: Path to mesh original code copied from OF core
// openFrameworks\examples\3d\pathsToMeshesExample

// Optional:
//#define SURFING__USE__LINE_WIDTH_FOR_FONT_INTERLETTER

#pragma once

#include "ofMain.h"

#include "SurfingFilesBrowserFonts.h"
#include "SurfingTransformNode.h"
#include "ofxSurfingHelpersLite.h"

#ifdef SURFING__USE__IMGUI
	#include "ofxSurfingImGui.h"
	#include "SurfingImGuizmo.h"
#endif

class MeshNode {
public:
	void calculateVertexBounds() {
		min.x = std::numeric_limits<float>::max();
		max.x = -min.x;

		min.y = min.x;
		max.y = max.x;

		min.z = min.x;
		max.z = max.x;

		const auto & verts = mesh.getVertices();
		for (const auto & v : verts) {
			if (v.x > max.x) {
				max.x = v.x;
			}
			if (v.x < min.x) {
				min.x = v.x;
			}

			if (v.y > max.y) {
				max.y = v.y;
			}
			if (v.y < min.y) {
				min.y = v.y;
			}

			if (v.z > max.z) {
				max.z = v.z;
			}
			if (v.z < min.z) {
				min.z = v.z;
			}
		}
	}

	ofNode node;
	ofMesh mesh;

	glm::vec3 min;
	glm::vec3 max;

	vector<ofMesh> srcMeshes;
	vector<ofMesh> offsetMeshes;
	vector<ofMesh> sideMeshes;
	vector<ofPolyline> polylines;
};

class ofxSurfing3dText {
public:
	ofxSurfing3dText();
	~ofxSurfing3dText();

	void setup();
	void setupFontsBowser(string path);

	// Camera
		void setup(ofCamera & camera_); // use only one setup method! don't call two setup methods!
public:
	void setCameraPtr(ofCamera & camera_); //don't need use when camera is passed to setup function!
	void setCameraPtr(ofCamera * camera_); //don't need use when camera is passed to setup function!

	// For getting camera from the parent class/ofApp
	// (TODO: Currently is not required bc the cam is instantiated on there!)
	ofCamera * getOfCameraPtr();
	ofEasyCam * getOfEasyCamPtr();

private:
	ofCamera * camera;

private:
	bool bFlagSetupFont = false;
	void setupFont(string path = "");
	uint64_t timeFlagSetupFont = 0;
	const uint64_t timeFlagSetupFontGap = 50; // delay in ms to reduce re calls..

	void setupFonts(string path = "");

	bool bFlagSetupText = false;
	void setupText(string text = "");

	void setupParams();
	void setupParamsPreset();
	void setupGui();
	void startup();

public:
	void exit();

	void draw(); //draws meshes but also bounds but could be with the material applied.

	void drawMeshes(); //draws meshes without bound
	void drawBounds(); //draws bound to allow be drawn out of any material.

private:
	// different compositions with the letters!
	void drawMeshesMode0(); //raw
	void drawMeshesMode1(); //multilayer
	void drawMeshesMode2(); //deformed mesh

public:
	void drawGui();

	void refreshGui();
	//public methods to be used on parent gui panels!
	//for example user simplified UI or exposed params on a presets manager!
	void refreshGuiUserParams(ofxPanel & gui_, ofxGuiGroup & group_);
	void refreshGuiUserParams(ofxPanel & gui_);

private:
	void drawHelp();
	void buildHelp();
	bool bFlagBuildHelp = false;

	string sHelp = "";

	void update();
	void updateAnimMode0();
	void update(ofEventArgs & args);

	void keyPressed(int key);
	void keyPressed(ofKeyEventArgs & eventArgs);

	ofTrueTypeFont font;
	vector<MeshNode> meshNodes;

	glm::vec3 meshMin;
	glm::vec3 meshMax;
	glm::vec3 meshCentroid = { 0, 0, 0 }; // can be used in place of glm::vec3(0,0,0);

	void Changed(ofAbstractParameter & e);
	void ChangedFont(ofAbstractParameter & e);
	//void ChangedTransform(ofAbstractParameter & e);

public:
	glm::vec3 getBoundBoxShape() const;

public:
	ofParameterGroup paramsPreset;
	// some selected params to be used on presets manager or user/UI
	// to simplify or adapt to our workflow: select only what we want to store,
	ofParameterGroup paramsPresetFont;

	ofParameterGroup parameters; //exposed to UI and settings

	ofParameterGroup paramsFile;
	ofEventListener listenerIndex; //get class internal index changed

	ofParameterGroup paramsFont;
	ofParameter<string> textMessage;
	ofEventListener listenerTextMessage;

	ofParameter<string> pathFont;
	ofParameter<string> nameFont;
	ofParameter<int> extrusion;
	ofParameter<float> letterSpacing;
	ofParameter<float> heightLine;
	ofParameter<float> sizeFont;
	ofParameter<bool> bUppercase;

	ofParameterGroup paramsMeshDeform;
	ofParameter<int> indexModeDeform;
	ofParameter<float> powerDeform;
	ofParameter<bool> bAnim;
	ofParameter<float> rotateChars;
	ofParameter<float> control1;
	ofParameter<float> control2;

	ofParameter<void> vResetFont;
	ofParameter<ofFloatColor> color;

#ifdef SURFING__USE__LINE_WIDTH_FOR_FONT_INTERLETTER
	ofParameter<int> lineWidth;
#endif

private:
	ofEventListener listenerResetFont;

public:
	void doResetFont();
	//void doResetTransform();

public:
	ofParameterGroup paramsInternal;
	ofParameter<bool> bGui;
	ofParameter<bool> bKeys;
	ofParameter<bool> bDebug;
	ofParameter<bool> bHelp;

	ofParameterGroup paramsDraw;
	ofParameterGroup paramsDebug;
	ofParameter<bool> bDrawMeshes;
	ofParameter<bool> bDrawBBoxBounds;
	ofParameter<bool> bDrawBBox;

	ofxPanel gui;

private:
	ofParameter<bool> bGui_ofxGui { "ofxGui", true };

public:
	void setEnableOfxGui(bool b) { bGui_ofxGui = b; }

#ifdef SURFING__USE__IMGUI
	ofxSurfingGui ui;
	void setupImGui();
	void drawImGui();
	SurfingImGuizmo surfingImGuizmo;
#endif

private:
	SurfingOfxGuiPanelsManager guiManager;

	string path = "ofxSurfing3dText_Settings.json";

private:
	void drawBounds(glm::vec3 min, glm::vec3 max, float size);
	void stringToMeshNodes(string astring, float extrudeAmount);

	SurfingAutoSaver autoSaver;

	bool load();
	void save();

public:
	TransformNode transformNode;
	SurfingFilesBrowserFonts filesBrowserFonts;
	ofParameter<int> & getIndexParam() { return filesBrowserFonts.indexFile; }

	void setNamePresetParams(string name) { namePresetParams = name; };

private:
	string namePresetParams = "TEXT_PRESET";

public:
	string getName() const {
		return namePresetParams;
	}

};

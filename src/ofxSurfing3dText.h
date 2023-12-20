/*
	TODO

	fix/remove bg bright, just the color

	add presets manger. make a ofxSurfinPresetsLite with ofGui instead of ofxGui

	add sceneManager public methods to pbr
		add material
		begin/end material

	fonts browser using file browser helper
		try to speed up pre loading all fonts..
*/

//--

// NOTE: Path to mesh code copied from OF core
// openFrameworks\examples\3d\pathsToMeshesExample

// OPTIONAL:
//#define SURFING__USE_LINE_WIDTH_FOR_FONT_INTERLETTER

#pragma once

#include "ofMain.h"

#include "SurfingTransformNode.h"
#include "ofxSurfingHelpersLite.h"

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

private:
	bool bFlagSetupFont = false;
	void setupFont(string path = "");
	uint64_t timeFlagSetupFont = 0;
	const uint64_t timeFlagSetupFontGap = 50; // delay in ms to reduce re calls..

	bool bFlagSetupText = false;
	void setupText(string text = "");

	void setupParams();
	void setupGui();
	void startup();

public:
	void exit();
	
	void draw();//draws meshes but also bounds but could be with the material applied.

	void drawMeshes();//draws meshes without bound
	void drawBounds();//draws bound to allow be drawn out of any material.
	
private:
	void drawMeshesMode0();
	void drawMeshesMode1();

public:
	void drawGui();

private:
	void refreshGui();

	void drawHelp();
	void buildHelp();

	string sHelp = "";

	void update();
	void updateAnim();
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

public:
	glm::vec3 getBoundBoxShape() const;

public:
	ofParameterGroup parameters;
	
	ofParameterGroup fontParams;
	ofParameter<string> textMessage;
	ofParameter<string> pathFont;
	ofParameter<string> nameFont;
	ofParameter<int> extrusion;
	ofParameter<float> letterSpacing;
	ofParameter<float> heightLine;
	ofParameter<float> sizeFont;
	ofParameter<bool> bAnim;
	ofParameter<bool> bUppercase;
	ofParameter<int> indexMode;
	ofParameter<void> vResetFont;
	ofParameter<ofFloatColor> color;

#ifdef SURFING__USE_LINE_WIDTH_FOR_FONT_INTERLETTER
	ofParameter<int> lineWidth;
#endif

private:
	ofEventListener listenerResetFont;
public:
	void doResetFont();

public:
	ofParameterGroup internalParams;
	ofParameter<bool> bGui;
	ofParameter<bool> bKeys;
	ofParameter<bool> bDebug;
	ofParameter<bool> bHelp;

	ofParameterGroup drawParams;
	ofParameterGroup debugParams;
	ofParameter<bool> bDrawMeshes;
	ofParameter<bool> bDrawBounds;
	ofParameter<bool> bDrawBBox;

	ofxPanel gui;
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
	TransformNode transform;
};

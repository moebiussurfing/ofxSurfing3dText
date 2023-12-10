/*

	TODO

	add sceneManager to pbr

	use pbr manager
		camera engine
		floor plane / box modes

	add params
		2nd mode
		b anim
		b draw bounds
		gui

	chars spacing
	uppercase
	fonts browser

*/

#pragma once

#include "ofMain.h"

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

class ofxSurfing3dText
{
public:

	ofxSurfing3dText();
	~ofxSurfing3dText();

	void setup();
	void exit();

	void drawMeshes();
	void drawBounds();
	void drawHelp();

private:
	void update();
	void draw();
	void update(ofEventArgs & args);
	void draw(ofEventArgs & args);

	void keyPressed(int key);
	void keyPressed(ofKeyEventArgs &eventArgs);

	string path_Params_AppSettings = "ofxSurfing3dText_AppSettings.xml";
	ofParameterGroup params_AppSettings;
	ofParameter<bool> bKeys;

	ofTrueTypeFont font;
	vector<MeshNode> meshNodes;

	string message;

	glm::vec3 meshMin;
	glm::vec3 meshMax;
	glm::vec3 meshCentroid = { 0, 0, 0 }; // can be used in place of glm::vec3(0,0,0);

	float extrusion = 100;

	void drawBounds(glm::vec3 min, glm::vec3 max, float size);
	void stringToMeshNodes(string astring, float extrudeAmount);

private:

};


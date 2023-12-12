#include "ofxSurfing3dText.h"

//--------------------------------------------------------------
ofxSurfing3dText::ofxSurfing3dText() {
	ofLogNotice("ofxSurfing3dText") << "~ofxSurfing3dText() Constructor";

	ofAddListener(ofEvents().update, this, &ofxSurfing3dText::update);
	ofAddListener(ofEvents().keyPressed, this, &ofxSurfing3dText::keyPressed);
}

//--------------------------------------------------------------
ofxSurfing3dText::~ofxSurfing3dText() {
	ofLogNotice("ofxSurfing3dText") << "~ofxSurfing3dText() Destructor";

	ofRemoveListener(ofEvents().update, this, &ofxSurfing3dText::update);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfing3dText::keyPressed);

	ofRemoveListener(parameters.parameterChangedE(), this, &ofxSurfing3dText::Changed);
	ofRemoveListener(fontParams.parameterChangedE(), this, &ofxSurfing3dText::ChangedFont);
}

//--------------------------------------------------------------
void ofxSurfing3dText::Changed(ofAbstractParameter & e) {

	std::string name = e.getName();

	ofLogVerbose("ofxSurfing3dText") << "Changed " << name << ": " << e;

	if (e.isSerializable()) {
		autoSaver.saveSoon();
	}
}

//--------------------------------------------------------------
void ofxSurfing3dText::ChangedFont(ofAbstractParameter & e) {

	std::string name = e.getName();

	ofLogVerbose("ofxSurfing3dText") << "ChangedFont " << name << ": " << e;

	if (name == sizeFont.getName()) {
		bFlagSetupFont = true;
		timeFlagSetupFont = ofGetElapsedTimeMillis();
	}

	else if (name == pathFont.getName()) {
		bFlagSetupFont = true;
		timeFlagSetupFont = ofGetElapsedTimeMillis();
	}

	else if (name == letterSpacing.getName()) {
		bFlagSetupFont = true;
		timeFlagSetupFont = ofGetElapsedTimeMillis();
	}

	else if (name == heightLine.getName()) {
		bFlagSetupFont = true;
		timeFlagSetupFont = ofGetElapsedTimeMillis();
	}

	//-

	if (name == textMessage.getName()) {
		bFlagSetupText = true;
	}

	else if (name == bUppercase.getName()) {
		bFlagSetupText = true;
	}

	else if (name == extrusion.getName()) {
		bFlagSetupText = true;
	}
}

//--------------------------------------------------------------
void ofxSurfing3dText::save() {
	ofLogNotice("ofxSurfing3dText") << "save -> " << path;

	// save scene
	ofxSurfing::saveSettings(parameters, path);
}

//--------------------------------------------------------------
bool ofxSurfing3dText::load() {
	ofLogNotice("ofxSurfing3dText") << "load -> " << path;

	autoSaver.pause();

	bool b = ofxSurfing::loadSettings(parameters, path);

	autoSaver.start();

	return b;
}

//--------------------------------------------------------------
void ofxSurfing3dText::setupParams() {
	ofLogNotice("ofxSurfingPBR") << "setupParams()";

	pathFont.set("P", "assets/fonts/NotoSansMono-Regular.ttf");
	nameFont.set("N", "NotoSansMono-Regular.ttf");//hardcode

	textMessage.set("Text", "Eternteinment");
	extrusion.set("Extrusion", 100, 0, 1000);
	sizeFont.set("Size font", 150, 20, 1000);
	letterSpacing.set("Letter Spacing", 0, -1, 1);
	heightLine.set("Height Line", 0, -1, 1);
	indexMode.set("Mode", 0, 0, 1);
	bAnim.set("Anim", false);
	bUppercase.set("Uppercase", false);

	bGui.set("UI 3dText", true);
	bKeys.set("Keys", true);
	bDrawMeshes.set("Draw", true);
	bHelp.set("Help", true);
	bDebug.set("Debug", true);
	bDrawMeshes.set("Draw Meshes", true);
	bDrawBounds.set("Draw Bounds", false);
	vResetFont.set("Reset Font");

	//-

	parameters.setName("3dText");

	drawParams.setName("DRAW");
	drawParams.add(bDrawMeshes);
	drawParams.add(bDrawBounds);
	parameters.add(drawParams);

	fontParams.setName("Font");
	fontParams.add(textMessage);
	fontParams.add(sizeFont);
	fontParams.add(extrusion);
	fontParams.add(letterSpacing);
	fontParams.add(heightLine);
	fontParams.add(bUppercase);
	fontParams.add(indexMode);
	fontParams.add(bAnim);
	fontParams.add(nameFont);
	fontParams.add(pathFont);
	fontParams.add(vResetFont);
	parameters.add(fontParams);

	internalParams.setName("Internal");
	internalParams.add(bGui);
	internalParams.add(bHelp);
	internalParams.add(bKeys);
	internalParams.add(bDebug);
	parameters.add(internalParams);

	nameFont.setSerializable(false);

	//-

	ofAddListener(parameters.parameterChangedE(), this, &ofxSurfing3dText::Changed);
	ofAddListener(fontParams.parameterChangedE(), this, &ofxSurfing3dText::ChangedFont);

	listenerResetFont = vResetFont.newListener([this](void) {
		doResetFont();
	});

	callback_t f = std::bind(&ofxSurfing3dText::save, this);
	autoSaver.setFunctionSaver(f);

	//--

	setupGui();
	
	startup();
}

//--------------------------------------------------------------
void ofxSurfing3dText::setupGui() {
	ofLogNotice("ofxSurfingPBR") << "setupGui()";

	gui.setup(parameters);

	ofxSurfing::setGuiPositionToLayout(gui, ofxSurfing::SURFING_LAYOUT_TOP_RIGHT);

	gui.getGroup(internalParams.getName()).minimize();
	gui.getGroup(drawParams.getName()).minimize();
}

//--------------------------------------------------------------
void ofxSurfing3dText::startup() {
	ofLogNotice("ofxSurfingPBR") << "startup()";

	load();
}

//--------------------------------------------------------------
void ofxSurfing3dText::setupText(string text) {
	ofLogNotice("ofxSurfingPBR") << "setupText() " << text;

	if (text != "") {
		textMessage.set(text);
	}

	string s = "";
	if (bUppercase)
		s = ofToUpper(textMessage);
	else
		s = textMessage;

	stringToMeshNodes(s, extrusion);

	buildHelp();
}

//--------------------------------------------------------------
void ofxSurfing3dText::setupFont(string path) {
	ofLogNotice("ofxSurfingPBR") << "setupFont() " << path;

	if (path != "") {
		pathFont.set(path);
	}

	// load(const of::filesystem::path& filename, int fontSize,
	// bool antialiased, bool fullCharacterSet, bool makeContours,
	// float simplifyAmt, int dpi)
	bool b = font.load(pathFont.get(), sizeFont, true, true, true);

	if (b) {
		ofLogNotice("ofxSurfingPBR") << "Font successfully downloaded: " << pathFont;
		nameFont = ofFilePath::getBaseName(pathFont.get());
	} else {
		ofLogError("ofxSurfingPBR") << "Error loading font: " << pathFont;
		nameFont = "NONE";
	}

	bFlagSetupText = true;
}

//--------------------------------------------------------------
void ofxSurfing3dText::doResetFont() {
	ofLogNotice("ofxSurfingPBR") << "doResetFont()";

	extrusion = 100;
	sizeFont = 150;
	letterSpacing = 0.f;
	heightLine = 0.f;
	indexMode = 0;
	bAnim = false;
	bUppercase = false;
}

//--------------------------------------------------------------
void ofxSurfing3dText::setup() {
	ofLogNotice("ofxSurfingPBR") << "setup()";

	setupParams();

	setupFont();
}

//--------------------------------------------------------------
void ofxSurfing3dText::update(ofEventArgs & args) {
	update();
}

//--------------------------------------------------------------
void ofxSurfing3dText::update() {

	if (bAnim) updateAnim();

	uint64_t t = ofGetElapsedTimeMillis() - timeFlagSetupFont;
	if (bFlagSetupFont && t >= timeFlagSetupFontGap) {
		bFlagSetupFont = false;

		setupFont();
	}

	if (bFlagSetupText) {
		bFlagSetupText = false;

		setupText();
	}
}

//--------------------------------------------------------------
void ofxSurfing3dText::updateAnim() {

	float elapsedTime = ofGetElapsedTimef();
	glm::vec3 axis(0, -1, 0);
	if (ofGetFrameNum() % 200 < 100) {
		axis = glm::vec3(1, 0, 0);
	}
	for (int i = 0; i < meshNodes.size(); i++) {
		float sine = sinf((float)i + elapsedTime);
		float spin = ofMap(sine, 0.4, 0.6, 0, 360, true);
		// we moved the vertices to be relative to the center at 0,0,0 so now we can move each one individually
		meshNodes[i].node.setOrientation(glm::angleAxis(ofDegToRad(spin), axis));
	}
}

//--------------------------------------------------------------
//void ofxSurfing3dText::drawGui(ofxPanel * guiPtr) {
//if (guiPtr != nullptr) {
//	glm::vec3 p;
//	p = gui.getShape().getTopRight() + glm::vec2(SURFING__PAD_OFXGUI_BETWEEN_PANELS, 0);
//}
//}

void ofxSurfing3dText::drawGui() {
	if (!bGui) return;
	ofDisableDepthTest();

	gui.draw();

	drawHelp();
}

//--------------------------------------------------------------
void ofxSurfing3dText::refreshGui() {
	ofLogNotice("ofxSurfing3dText") << "refreshGui()";

	// top-left
	gui.setPosition(SURFING__PAD_TO_WINDOW_BORDERS, SURFING__PAD_TO_WINDOW_BORDERS);

	// minimize sub panels

	gui.getGroup(parameters.getName())
		.getGroup(internalParams.getName())
		.minimize();
}

//--------------------------------------------------------------
void ofxSurfing3dText::buildHelp() {

	ofLogNotice("ofxSurfingPBR") << "buildHelp()";

	sHelp = "HELP 3D_TEXT";
	sHelp += "\n\n";
	sHelp += "TEXT:\n";
	sHelp += textMessage.get();
	sHelp += "\n\n";
	sHelp += "UP/DOWN:\nExtrusion " + ofToString(extrusion.get(), 0);
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawHelp() {
	if (!bHelp) return;

	ofxSurfing::ofDrawBitmapStringBox(sHelp, ofxSurfing::SURFING_LAYOUT_BOTTOM_CENTER);
}

//--------------------------------------------------------------
void ofxSurfing3dText::draw() {

	if (bDrawMeshes) drawMeshes();

	if (bDrawBounds) drawBounds();
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawMeshes() {
	if (!bDrawMeshes) return;

	if (indexMode == 0) {
		for (auto & meshNode : meshNodes) {
			meshNode.node.transformGL();
			meshNode.mesh.draw();
			meshNode.node.restoreTransformGL();
		}
	}

	else if (indexMode == 1) {
		ofPushStyle();
		ofPushMatrix();
		float elapsedTime = ofGetElapsedTimef();

		ofColor c = (255, 255);
		float ow = 50;

		for (auto & meshNode : meshNodes) {
			ofPushMatrix();
			ofTranslate(meshNode.node.getPosition());

			//ofTranslate(0, 0, -1000 - extrusion * 2);
			ofTranslate(0, 0, -(extrusion * 3) / 2.f);

			//ofSetColor(184, 180, 176);
			ofSetColor(c.r, c.g, c.b, 176);
			for (auto & pline : meshNode.polylines) {
				pline.draw();
			}

			float offset;
			if (bAnim)
				offset = 1.f - ofClamp(sinf(elapsedTime), 0, 1);
			else
				offset = .25f;

			//ofSetColor(55, 206, 49);
			ofSetColor(c.r, c.g, c.b, 59);
			ofTranslate(0, 0, 2 * ow * offset);
			for (auto & mesh : meshNode.srcMeshes) {
				mesh.draw();
			}

			//ofSetColor(124, 120, 116);
			ofSetColor(c.r, c.g, c.b, 116);
			ofTranslate(0, 0, ow * offset + extrusion);
			for (auto & mesh : meshNode.sideMeshes) {
				mesh.draw();
			}

			//ofSetColor(55, 206, 49);
			ofSetColor(c.r, c.g, c.b, 49);
			ofTranslate(0, 0, ow * offset);
			for (auto & mesh : meshNode.srcMeshes) {
				mesh.draw();
			}

			ofPopMatrix();
		}

		ofPopMatrix();
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawBounds() {
	if (!bDrawBounds) return;

	ofPushStyle();
	{
		if (indexMode == 0) {

			ofPushMatrix();
			ofTranslate(-meshCentroid);

			ofSetColor(0, 255);
			//ofSetColor(184, 180, 176);

			drawBounds(meshMin, meshMax, 100);
			ofPopMatrix();

			for (auto & meshNode : meshNodes) {
				meshNode.node.transformGL();

				ofSetColor(0, 255);
				//ofSetColor(92, 237, 184);

				drawBounds(meshNode.min, meshNode.max, 30);
				meshNode.node.restoreTransformGL();
			}
		}
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofxSurfing3dText::keyPressed(ofKeyEventArgs & eventArgs) {
	const int key = eventArgs.key;

	// modifiers
	bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
	bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
	bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
	bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

	ofLogNotice(__FUNCTION__) << " : " << key;

	keyPressed(key);
}

//--------------------------------------------------------------
void ofxSurfing3dText::exit() {
}

////--------------------------------------------------------------
//void ofxSurfing3dText::setBool(bool b) {
//	ofLogNotice(__FUNCTION__) << ofToString(b ? "true" : "false");
//}
//
////--------------------------------------------------------------
//bool ofxSurfing3dText::getBool() {
//	return true;
//}

//--------------------------------------------------------------
void ofxSurfing3dText::drawBounds(glm::vec3 min, glm::vec3 max, float size) {
	glm::vec3 up(0, size, 0);
	glm::vec3 right(size, 0, 0);
	glm::vec3 forward(0, 0, size);

	ofMesh drawMesh;
	drawMesh.setMode(OF_PRIMITIVE_LINES);

	glm::vec3 cornerPt = min;
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + right);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + up);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + forward);

	cornerPt = glm::vec3(min.x, min.y, max.z);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + right);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + up);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - forward);

	cornerPt = glm::vec3(min.x, max.y, min.z);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + right);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - up);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + forward);

	cornerPt = glm::vec3(min.x, max.y, max.z);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + right);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - up);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - forward);

	cornerPt = glm::vec3(max.x, min.y, min.z);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - right);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + up);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + forward);

	cornerPt = glm::vec3(max.x, min.y, max.z);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - right);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + up);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - forward);

	cornerPt = glm::vec3(max.x, max.y, min.z);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - right);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - up);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt + forward);

	cornerPt = glm::vec3(max.x, max.y, max.z);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - right);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - up);
	drawMesh.addVertex(cornerPt);
	drawMesh.addVertex(cornerPt - forward);

	drawMesh.draw();
}

//--------------------------------------------------------------
void ofxSurfing3dText::stringToMeshNodes(string astring, float extrudeAmount) {
	ofLogNotice("ofxSurfing3dText") << "stringToMeshNodes()";

	meshNodes.clear();

	if (astring == "") {
		// the string does not have any characters so do not proceed
		ofLogError("ofxSurfing3dText") << "Skip bc text is empty!";
		return;
	}

	if (!font.isLoaded()) {
		ofLogError("ofxSurfing3dText") << "Skip bc error as ofTrueTypeFont font not allocated!";
		return;
	}

	// letterSpacing
	const float spMin = 0.5f;
	const float spMax = 4.f;
	float sp = 1.f;
	if (letterSpacing < 0)
		sp = ofMap(letterSpacing, 0, -1, 1, spMin);
	else if (letterSpacing > 0)
		sp = ofMap(letterSpacing, 0, 1, 1, spMax);
	font.setLetterSpacing(sp);

	// heightLine
	const float lhMax = 2.f;
	float lh = 1.f;
	if (heightLine < 0)
		lh = ofMap(heightLine, 0, -1, 1, 1 / lhMax);
	else if (heightLine > 0)
		lh = ofMap(heightLine, 0, 1, 1, lhMax);
	font.setLineHeight(lh);

	//--

	// get the font paths as filled
	// we pass true as the third argument to request filled characters, so they have a mesh, but no outlines
	vector<ofPath> fontPaths = font.getStringAsPoints(astring, true, true);
	// we pass false as the third argument to request non-filled characters, so they have outlines
	vector<ofPath> fontPathsOutlined = font.getStringAsPoints(astring, true, false);

	// loop through all of the character paths returned from font.
	for (int i = 0; i < fontPaths.size(); i++) {
		MeshNode meshNode;
		auto & path = fontPaths[i];
		// this mesh will be used to store the front, side and back meshes all in one
		ofMesh compedMesh;
		// grab the mesh from the fontPaths
		ofMesh pathMesh = path.getTessellation();
		// grag the verts from the mesh
		// auto& will compile to vector<glm::vec3>&
		auto & pathVerts = pathMesh.getVertices();
		// flip the y value so it aligns with the camera up
		for (auto & pv : pathVerts) {
			pv.y = -pv.y;
		}
		// the pathMesh does not have any normals
		// this will be the front face of the mesh that will will construct
		// so we know that the normals can all be facing forward in z space
		vector<glm::vec3> meshNormals;
		meshNormals.assign(pathMesh.getNumVertices(), glm::vec3(0, 0, 1));

		meshNode.srcMeshes.push_back(pathMesh);
		meshNode.srcMeshes.back().addNormals(meshNormals);
		compedMesh.append(meshNode.srcMeshes.back());

		// duplicate the front face, we will push this back in z space
		// reverse the normals since it will be the back face
		ofMesh offsetMesh = pathMesh;
		meshNormals.assign(offsetMesh.getNumVertices(), glm::vec3(0, 0, -1));
		offsetMesh.addNormals(meshNormals);
		// move all the vertices back by the extrudeAmount argument
		auto & verts = offsetMesh.getVertices();
		for (auto & v : verts) {
			v.z -= extrudeAmount;
		}

		meshNode.offsetMeshes.push_back(offsetMesh);
		compedMesh.append(offsetMesh);

		// check the paths stored in fontPathsOutlined
		if (i < fontPathsOutlined.size()) {
			auto & path = fontPathsOutlined[i];
			// ofPath.getOutline returns a vector of ofPolyline
			auto & outlines = path.getOutline();
			// loop through all of the polylines
			for (auto & outline : outlines) {
				ofMesh pmesh;
				pmesh.setMode(OF_PRIMITIVE_TRIANGLES);
				// loop through all of the verts in the polyline
				for (int k = 0; k < outline.size(); k++) {
					auto pvert = outline[k];
					// reverse the y so that it aligns with the camera up direction
					pvert.y = -pvert.y;
					pmesh.addVertex(pvert);
					// add a vertex that is pushed back in z space by the extrudeAmount argument
					pmesh.addVertex(pvert - glm::vec3(0, 0, extrudeAmount));

					auto normal = outline.getNormalAtIndex(k);
					// flip the normal's y since we flipped the vertex's y position
					normal.y *= -1.f;
					pmesh.addNormal(normal);
					pmesh.addNormal(normal);

					// build the triangle indices
					if (k < outline.size() - 1) {
						pmesh.addIndex(k * 2 + 1);
						pmesh.addIndex(k * 2 + 0);
						pmesh.addIndex(k * 2 + 2);

						pmesh.addIndex(k * 2 + 3);
						pmesh.addIndex(k * 2 + 1);
						pmesh.addIndex(k * 2 + 2);
					} else {
						// if we are the last point, then connect to the first and second point
						pmesh.addIndex(k * 2 + 1);
						pmesh.addIndex(k * 2 + 0);
						pmesh.addIndex(0);

						pmesh.addIndex(1);
						pmesh.addIndex(k * 2 + 1);
						pmesh.addIndex(0);
					}
				}
				if (pmesh.getNumVertices() > 0) {
					meshNode.sideMeshes.push_back(pmesh);
					compedMesh.append(pmesh);
				}
				meshNode.polylines.push_back(outline);
			}
		}

		if (compedMesh.getNumVertices() > 0) {
			meshNode.mesh = compedMesh;
			meshNode.node.setPosition(meshNode.mesh.getCentroid());
		}

		meshNodes.push_back(meshNode);
	}

	// set the minimum values very high
	// and the maximum values very low so that we can check against them
	meshMin.x = std::numeric_limits<float>::max();
	meshMax.x = -meshMin.x;

	meshMin.y = meshMin.x;
	meshMax.y = meshMax.x;

	meshMin.z = meshMin.x;
	meshMax.z = meshMax.x;

	meshCentroid = glm::vec3(0, 0, 0);

	// calculate the min and max vertex positions
	// and average out all of the centroids
	int totalNumberCentroids = 0;
	for (auto & meshNode : meshNodes) {
		if (meshNode.mesh.getNumVertices() > 1) {
			meshCentroid += meshNode.mesh.getCentroid();
			totalNumberCentroids++;
		}

		const auto & verts = meshNode.mesh.getVertices();
		for (const auto & v : verts) {
			if (v.x > meshMax.x) {
				meshMax.x = v.x;
			}
			if (v.x < meshMin.x) {
				meshMin.x = v.x;
			}

			if (v.y > meshMax.y) {
				meshMax.y = v.y;
			}
			if (v.y < meshMin.y) {
				meshMin.y = v.y;
			}

			if (v.z > meshMax.z) {
				meshMax.z = v.z;
			}
			if (v.z < meshMin.z) {
				meshMin.z = v.z;
			}
		}
	}

	if (totalNumberCentroids > 0) {
		meshCentroid /= (float)totalNumberCentroids;
	}

	for (auto & meshNode : meshNodes) {
		// now that we have the centroid for all of the meshes combined
		// offset the vertices from its own mesh centroid
		// this make the vertices relative to their mesh's centroid
		auto & meshVerts = meshNode.mesh.getVertices();
		for (auto & vert : meshVerts) {
			vert = vert - meshNode.node.getPosition();
		}

		for (auto & srcMesh : meshNode.srcMeshes) {
			auto & verts = srcMesh.getVertices();
			for (auto & vert : verts) {
				vert = vert - meshNode.node.getPosition();
			}
		}
		for (auto & offsetMesh : meshNode.offsetMeshes) {
			auto & verts = offsetMesh.getVertices();
			for (auto & vert : verts) {
				vert = vert - meshNode.node.getPosition();
			}
		}
		for (auto & sideMesh : meshNode.sideMeshes) {
			auto & verts = sideMesh.getVertices();
			for (auto & vert : verts) {
				vert = vert - meshNode.node.getPosition();
			}
		}

		for (auto & line : meshNode.polylines) {
			auto & verts = line.getVertices();
			for (auto & vert : verts) {
				vert.y = -vert.y;
				vert = vert - meshNode.node.getPosition();
			}
		}
		// make the node position relative to centroid
		meshNode.node.setPosition(meshNode.node.getPosition() - meshCentroid);
		// calculate the min and max values on the vertices per node
		meshNode.calculateVertexBounds();
	}
}

//--------------------------------------------------------------
void ofxSurfing3dText::keyPressed(int key) {
	if (!bKeys) return;

	if (key == 'g') {
		bGui = !bGui;
	}

	if (key == OF_KEY_UP) {
		extrusion += 10;
	} else if (key == OF_KEY_DOWN) {
		extrusion -= 10;
		if (extrusion < 0) {
			extrusion = 0;
		}
	}

	//bool b = 0;
	//if (!b) return;

	//if (key == OF_KEY_DEL || key == 8) {
	//	if (textMessage.get().length() > 0) {
	//		textMessage.pop_back();
	//		bRebuildMeshes = true;
	//	}
	//} else if (key == OF_KEY_RETURN || key == 13) {
	//	textMessage += "\n";
	//} else if (key < 300) {
	//	unsigned char letter = (unsigned char)key;
	//	textMessage += (letter);
	//	bRebuildMeshes = true;
	//}

	//if (bRebuildMeshes) {
	//	stringToMeshNodes(textMessage, extrusion);
	//}
}
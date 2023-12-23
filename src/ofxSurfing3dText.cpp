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

	buildHelp();
}

//--------------------------------------------------------------
void ofxSurfing3dText::ChangedFont(ofAbstractParameter & e) {

	std::string name = e.getName();

	ofLogVerbose("ofxSurfing3dText") << "ChangedFont " << name << ": " << e;

	if (name == sizeFont.getName()) {
		bFlagSetupFont = true;
		timeFlagSetupFont = ofGetElapsedTimeMillis();
	} else if (name == pathFont.getName()) {
		bFlagSetupFont = true;
		timeFlagSetupFont = ofGetElapsedTimeMillis();
	}

	else if (name == letterSpacing.getName()) {
		bFlagSetupText = true;
	} else if (name == heightLine.getName()) {
		bFlagSetupText = true;
	}

#ifdef SURFING__USE_LINE_WIDTH_FOR_FONT_INTERLETTER
	else if (name == lineWidth.getName()) {
		if (!font.isLoaded()) return;

		//https://forum.openframeworks.cc/t/calculate-letter-spacing-for-oftruetypefont-setletterspacing/42897/5?u=moebiussurfing
		string s = textMessage.get();
		auto desired_width = lineWidth;
		auto missing_px = desired_width - font.stringWidth(s);
		auto target_w = missing_px / float(s.size() - 1);
		auto ratio = 1.0 + (target_w / font.getCharAdvance(' '));

		letterSpacing = ratio;

		bFlagSetupText = true;
	}
#endif

	//-

	else if (name == textMessage.getName()) {
		bFlagSetupText = true;
	}

	else if (name == bUppercase.getName()) {
		bFlagSetupText = true;
	}

	else if (name == extrusion.getName()) {
		bFlagSetupText = true;
	}

	else if (name == bAnim.getName()) {
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
	nameFont.set("N", "NotoSansMono-Regular.ttf"); //hardcode

	textMessage.set("Text", "Eternteinment");
	extrusion.set("Extrusion", 100, 0, 1000);
	sizeFont.set("Size font", 150, 20, 1000);

#ifdef SURFING__USE_LINE_WIDTH_FOR_FONT_INTERLETTER
	lineWidth.set("Line Width", 1000, 0, 10000);
	letterSpacing.set("Letter Spacing", 0, 0, 20);
#else
	letterSpacing.set("Letter Spacing", 0, -1, 1);
#endif

	heightLine.set("Height Line", 0, -1, 1);
	indexMode.set("Mode", 0, 0, 1);
	bAnim.set("Animate", false);
	bUppercase.set("Uppercase", false);
	color.set("Color", ofColor(128, 255), ofColor(0, 0), ofColor(255, 255));

	bGui.set("UI 3dText", true);
	bKeys.set("Keys", true);
	bDrawMeshes.set("Draw", true);
	bHelp.set("Help", true);
	bDebug.set("Debug", true);
	bDrawMeshes.set("Draw Meshes", true);
	bDrawBounds.set("Draw Bounds", false);
	bDrawBBox.set("Draw BBox", false);
	vResetFont.set("Reset Font");

	//-

	parameters.setName("3dText");

	drawParams.setName("DRAW");
	drawParams.add(bDrawMeshes);
	debugParams.setName("DEBUG");
	debugParams.add(bDebug);
	debugParams.add(bDrawBBox);
	debugParams.add(bDrawBounds);
	drawParams.add(debugParams);
	parameters.add(drawParams);

	fontParams.setName("Font");
	fontParams.add(textMessage);
	fontParams.add(color);
	fontParams.add(sizeFont);
	fontParams.add(extrusion);

#ifdef SURFING__USE_LINE_WIDTH_FOR_FONT_INTERLETTER
	fontParams.add(lineWidth);
#endif

	fontParams.add(letterSpacing);
	fontParams.add(heightLine);
	fontParams.add(bUppercase);
	fontParams.add(indexMode);
	fontParams.add(bAnim);
	fontParams.add(nameFont);
	fontParams.add(pathFont);
	fontParams.add(vResetFont);
	parameters.add(fontParams);

	//--

	transform.setPowRatio(0.01);
	parameters.add(transform.parameters);

	//--

	internalParams.setName("Internal");
	internalParams.add(guiManager.bAutoLayout);
	internalParams.add(bHelp);
	internalParams.add(bKeys);
	//internalParams.add(bGui);
	parameters.add(internalParams);

	nameFont.setSerializable(false);

	//--

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
	guiManager.setup(&gui);

	ofxSurfing::setGuiPositionToLayout(gui, ofxSurfing::SURFING_LAYOUT_TOP_RIGHT);

	gui.getGroup(internalParams.getName()).minimize();
	
	gui.getGroup(drawParams.getName()).minimize();
	
	gui.getGroup(drawParams.getName()).getGroup(debugParams.getName())
		.minimize();

	gui.getGroup(fontParams.getName()).minimize();

	gui.getGroup(transform.parameters.getName()).minimize();
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

	sizeFont = 100;
	extrusion = sizeFont * 0.5;
	letterSpacing = 0.f;
	heightLine = 0.f;
	color = ofColor(128, 255);
	indexMode = 0;
	bAnim = false;
	bUppercase = false;
	bDrawBBox = true;
	bDrawBounds = false;

#ifdef SURFING__USE_LINE_WIDTH_FOR_FONT_INTERLETTER
	lineWidth = 1000;
#endif
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
		// we moved the vertices to be relative to the center
		// at 0,0,0 so now we can move each one individually
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

//--------------------------------------------------------------
void ofxSurfing3dText::drawGui() {
	if (!bGui) return;

	ofDisableDepthTest();

	gui.draw();

	drawHelp();
}

//--------------------------------------------------------------
void ofxSurfing3dText::refreshGui() {
	if (!guiManager.bAutoLayout) return;

	ofLogNotice("ofxSurfing3dText") << "refreshGui()";

	// top-left
	gui.setPosition(SURFING__PAD_TO_WINDOW_BORDERS, SURFING__PAD_TO_WINDOW_BORDERS);

	// minimize sub panels
	gui.getGroup(parameters.getName()).getGroup(internalParams.getName())
		.minimize();
}

//--------------------------------------------------------------
void ofxSurfing3dText::buildHelp() {

	ofLogNotice("ofxSurfingPBR") << "buildHelp()";

	sHelp = "HELP\n3D_TEXT";
	sHelp += "\n\n";
	sHelp += "TEXT\n\n";
	sHelp += textMessage.get();
	sHelp += "\n\n";
	if (!bKeys) {
		sHelp += "KEYS DISABLED\n";
		return;
	}
	sHelp += "g: Gui\n";
	sHelp += "h: Help\n";
	sHelp += "d: Debug     " + string(bDebug ? "ON " : "OFF") + "\n";
	sHelp += "u: Uppercase " + string(bUppercase ? "ON " : "OFF") + "\n";
	sHelp += "a: Animate   " + string(bAnim ? "ON " : "OFF") + "\n";
	sHelp += "\n";
	sHelp += "BACKSPACE:   Reset\n";
	sHelp += "E/e:         Extrusion     " + ofToString(extrusion.get(), 0) + "\n";
	sHelp += "LEFT/RIGHT:  LetterSpacing " + ofToString(letterSpacing.get(), 2) + "\n";
	sHelp += "UP/DOWN:     HeightLine    " + ofToString(heightLine.get(), 2) + "\n";
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawHelp() {
	if (!bHelp) return;

	ofxSurfing::ofDrawBitmapStringBox(sHelp, ofxSurfing::SURFING_LAYOUT_BOTTOM_CENTER);
}

//--------------------------------------------------------------
void ofxSurfing3dText::draw() {
	if (!transform.isEnabled()) return;

	drawMeshes();
	drawBounds();
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawMeshes() {
	if (!bDrawMeshes) return;

	transform.push();
	{
		if (indexMode == 0) {
			drawMeshesMode0();
		}

		else if (indexMode == 1) {
			drawMeshesMode1();
		}
	}
	transform.pop();
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawMeshesMode0() {
	ofPushMatrix();
	for (auto & meshNode : meshNodes) {
		meshNode.node.transformGL();
		meshNode.mesh.draw();
		meshNode.node.restoreTransformGL();
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawMeshesMode1() {
	ofPushMatrix();
	ofPushStyle();

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

	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawBounds() {
	if (!bDebug) return;
	if (!bDrawBBox && !bDrawBounds) return;

	transform.push();
	{
		ofColor c;

		int a = 150;
		//int a = ofMap(glm::sin(ofGetElapsedTimef()), -1, 1, 0, 200);//blink

		c = ofColor(255, a); //white
		//c = ofColor(0, a); //black

		ofPushStyle();
		{
			if (indexMode == 0) {

				if (bDrawBBox) {
					ofPushMatrix();
					{
						ofTranslate(-meshCentroid);

						ofSetColor(c);

						drawBounds(meshMin, meshMax, 100);
					}
					ofPopMatrix();
				}

				if (bDrawBounds) {
					for (auto & meshNode : meshNodes) {
						meshNode.node.transformGL();

						ofSetColor(c);

						drawBounds(meshNode.min, meshNode.max, 30);
						meshNode.node.restoreTransformGL();
					}
				}
			}
		}
		ofPopStyle();
	}
	transform.pop();
}

//--------------------------------------------------------------
void ofxSurfing3dText::exit() {
}

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
glm::vec3 ofxSurfing3dText::getBoundBoxShape() const {
	glm::vec3 p;
	p = (meshMax - meshMin);
	return p;
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

	//--

	//https://forum.openframeworks.cc/t/calculate-letter-spacing-for-oftruetypefont-setletterspacing/42897/5?u=moebiussurfing
	font.setLetterSpacing(letterSpacing);
	font.setSpaceSize(font.getSpaceSize() * letterSpacing);

	// letterSpacing
#ifndef SURFING__USE_LINE_WIDTH_FOR_FONT_INTERLETTER
	const float spMin = 0.2f;
	const float spMax = 4.f;
	float sp = 1.f;
	if (letterSpacing < 0)
		sp = ofMap(letterSpacing, 0, -1, 1, spMin);
	else if (letterSpacing > 0)
		sp = ofMap(letterSpacing, 0, 1, 1, spMax);
	font.setLetterSpacing(sp);
#endif

	// heightLine
	const float lZero = 1.4f * sizeFont;
	const float lhMin = 0.2f * sizeFont;
	const float lhMax = 3.f * sizeFont;
	float lh = lZero;
	if (heightLine < 0)
		lh = ofMap(heightLine, 0, -1, lZero, lhMin);
	else if (heightLine > 0)
		lh = ofMap(heightLine, 0, 1, lZero, lhMax);
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
void ofxSurfing3dText::keyPressed(int key) {
	if (!bKeys) return;

	if (key == 'h') {
		bHelp = !bHelp;
	}

	if (key == 'g') {
		bGui = !bGui;
	}
	if (key == 'd') {
		bDebug = !bDebug;
	}
	if (key == 'u') {
		bUppercase = !bUppercase;
	}
	if (key == 'a') {
		bAnim = !bAnim;
	}

	if (key == OF_KEY_BACKSPACE) {
		doResetFont();
	}

	if (key == OF_KEY_RIGHT) {
		letterSpacing += 0.01;
		if (letterSpacing > letterSpacing.getMax()) letterSpacing = letterSpacing.getMax();
	} else if (key == OF_KEY_LEFT) {
		letterSpacing -= 0.01;
		if (letterSpacing < letterSpacing.getMin()) letterSpacing = letterSpacing.getMin();
	}

	if (key == OF_KEY_UP) {
		heightLine += 0.01;
		if (heightLine > heightLine.getMax()) heightLine = heightLine.getMax();
	} else if (key == OF_KEY_DOWN) {
		heightLine -= 0.01;
		if (heightLine < heightLine.getMin()) heightLine = heightLine.getMin();
	}

	if (key == 'E') {
		extrusion += 10;
		if (extrusion > extrusion.getMax()) extrusion = extrusion.getMax();
	} else if (key == 'e') {
		extrusion -= 10;
		if (extrusion < extrusion.getMin()) extrusion = extrusion.getMin();
	}
}
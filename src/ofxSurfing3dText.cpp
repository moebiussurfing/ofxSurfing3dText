#include "ofxSurfing3dText.h"

//--------------------------------------------------------------
ofxSurfing3dText::ofxSurfing3dText() {
	ofLogNotice("ofxSurfing3dText") << "Constructor";

	ofAddListener(ofEvents().update, this, &ofxSurfing3dText::update);
	ofAddListener(ofEvents().keyPressed, this, &ofxSurfing3dText::keyPressed);
}

//--------------------------------------------------------------
ofxSurfing3dText::~ofxSurfing3dText() {
	ofLogNotice("ofxSurfing3dText") << "Destructor";

	ofRemoveListener(ofEvents().update, this, &ofxSurfing3dText::update);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfing3dText::keyPressed);

	ofRemoveListener(parameters.parameterChangedE(), this, &ofxSurfing3dText::Changed);
	ofRemoveListener(paramsFont.parameterChangedE(), this, &ofxSurfing3dText::ChangedFont);
}

// Camera
//--------------------------------------------------------------
void ofxSurfing3dText::setup(ofCamera & camera_) {
	setCameraPtr(dynamic_cast<ofCamera *>(&camera_));

	this->setup();
}
//--------------------------------------------------------------
void ofxSurfing3dText::setCameraPtr(ofCamera & camera_) {
	setCameraPtr(dynamic_cast<ofCamera *>(&camera_));
}
//--------------------------------------------------------------
void ofxSurfing3dText::setCameraPtr(ofCamera * camera_) {
	camera = camera_;

	////FORCED
	//camera->setFarClip(SURFING__PBR__SCENE_CAMERA_FAR_CLIP);
}
//--------------------------------------------------------------
ofCamera * ofxSurfing3dText::getOfCameraPtr() {
	if (camera != nullptr)
		return camera;
	else
		return nullptr;
}
//--------------------------------------------------------------
ofEasyCam * ofxSurfing3dText::getOfEasyCamPtr() {
	ofEasyCam * easyCam = dynamic_cast<ofEasyCam *>(camera);
	if (easyCam != nullptr)
		return easyCam;
	else
		return nullptr;
}

//--------------------------------------------------------------
void ofxSurfing3dText::Changed(ofAbstractParameter & e) {

	std::string name = e.getName();

	ofLogNotice("ofxSurfing3dText") << "Changed " << name << ": " << e;

	if (e.isSerializable()) {
		autoSaver.saveSoon();
	}

	if (name == bAnim.getName()) {
		bFlagSetupText = true;
	}

	bFlagBuildHelp = true;
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
		bFlagSetupText = true;
	}

	else if (name == heightLine.getName()) {
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

	else if (name == bUppercase.getName()) {
		bFlagSetupText = true;
	}

	else if (name == extrusion.getName()) {
		bFlagSetupText = true;
	}

	//else if (name == textMessage.getName()) {
	//	bFlagSetupText = true;
	//}

	//else if (name == bKeys.getName()) {
	//	buildHelp();
	//}
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
	ofLogNotice("ofxSurfing3dText") << "setupParams()";

	pathFont.set("P", "assets/fonts/NotoSansMono-Regular.ttf");
	nameFont.set("N", "NotoSansMono-Regular.ttf"); //hardcoded

	paramsFile.setName("File");
	paramsFile.add(nameFont);
	paramsFile.add(pathFont);

	textMessage.set("Text", "Eternteinment");

	int u = SURFING__PBR__SCENE_SIZE_UNIT / 5;
	extrusion.set("Extrusion", 100, 0, u);
	sizeFont.set("Size font", 100, 10, u);

#ifdef SURFING__USE_LINE_WIDTH_FOR_FONT_INTERLETTER
	lineWidth.set("Line Width", 1000, 0, 10000);
	letterSpacing.set("Letter Spacing", 0, 0, 20);
#else
	letterSpacing.set("Letter Spacing", 0, -1, 1);
#endif

	heightLine.set("Height Line", 0, -1, 1);
	rotateChars.set("RotateChars", 0, -180, 180);

	indexModeDeform.set("Mode", 0, 0, 2);
	powerDeform.set("PowerDeform", 0, 0, 1);
	control1.set("Control1", .5, 0, 1);
	control2.set("Control2", .5, 0, 1);

	bAnim.set("Animate", false);

	bUppercase.set("Uppercase", false);
	color.set("Color", ofColor(128, 255), ofColor(0, 0), ofColor(255, 255));
	bKeys.set("Keys", true);
	bDrawMeshes.set("Draw", true);
	bHelp.set("Help", true);
	bDebug.set("Debug", true);
	bDrawMeshes.set("Draw Meshes", true);
	bDrawBBoxBounds.set("Draw Bounds", false);
	bDrawBBox.set("Draw BBox", false);
	vResetFont.set("Reset Font");

	//--

	paramsDraw.setName("DRAW");
	paramsDraw.add(bDrawMeshes);

	paramsDebug.setName("DEBUG");
	paramsDebug.add(bDebug);
	paramsDebug.add(bDrawBBox);
	paramsDebug.add(bDrawBBoxBounds);
	paramsDraw.add(paramsDebug);

	parameters.setName("3D_TEXT");
	string n = parameters.getName();
	//n = "UI " + n;
	bGui.set(n, true);
	parameters.add(paramsDraw);
	//parameters.add(filesBrowserFonts.bGui);

	parameters.add(textMessage);
	listenerTextMessage = textMessage.newListener([this](string & s) {
		ofLogNotice("ofxSurfing3dText") << "text: " << s;
		bFlagSetupText = true;
	});

	paramsFont.setName("Font");
	paramsFont.add(color);
	paramsFont.add(sizeFont);
	paramsFont.add(extrusion);

#ifdef SURFING__USE_LINE_WIDTH_FOR_FONT_INTERLETTER
	paramsFont.add(lineWidth);
#endif

	paramsFont.add(letterSpacing);
	paramsFont.add(heightLine);
	paramsFont.add(bUppercase);
	paramsFont.add(rotateChars);

	paramsFont.add(paramsFile);
	paramsFont.add(vResetFont);
	parameters.add(paramsFont);

	paramsMeshDeform.setName("Mesh Style");
	paramsMeshDeform.add(indexModeDeform);
	paramsMeshDeform.add(powerDeform);
	paramsMeshDeform.add(control1);
	paramsMeshDeform.add(control2);
	paramsMeshDeform.add(bAnim);
	parameters.add(paramsMeshDeform);

	//--

	transformNode.setName("NODE");
	transformNode.setup();

	//--

	paramsInternal.setName("Internal");
	paramsInternal.add(bGui);
#ifdef SURFING__USE__IMGUI
	surfingImGuizmo.setup("NodeGizmo");
	paramsInternal.add(surfingImGuizmo.parameters);
#endif
	parameters.add(paramsInternal);

	parameters.add(bDebug);
	parameters.add(bKeys);
	parameters.add(bHelp);

	nameFont.setSerializable(false);

	//--

	ofAddListener(parameters.parameterChangedE(), this, &ofxSurfing3dText::Changed);
	ofAddListener(paramsFont.parameterChangedE(), this, &ofxSurfing3dText::ChangedFont);

	listenerResetFont = vResetFont.newListener([this](void) {
		doResetFont();
	});

	callback_t f = std::bind(&ofxSurfing3dText::save, this);
	autoSaver.setFunctionSaver(f);

	//--

	listenerIndex = filesBrowserFonts.indexFile.newListener([this](int & i) {
		ofLogNotice("ofxSurfing3dText") << "index: " << i;
		if (filesBrowserFonts.bModeAutoload) {
			string p = filesBrowserFonts.getPathFile(i);
			pathFont.set(p);
		}

		//filesBrowserFonts.fontsBook.indexFont.set(i);
	});

	//--

	setupParamsPreset();

	//--

	setupGui();

#ifdef SURFING__USE__IMGUI
	setupImGui();
	bGui_ofxGui = false;
#endif

	//--

	startup();
}

//--------------------------------------------------------------
void ofxSurfing3dText::setupParamsPreset() {
	ofLogNotice("ofxSurfing3dText") << "setupParamsPreset()";

	paramsPreset.setName(namePresetParams);

	//font/letters
	paramsPresetFont.setName("Font");
	paramsPresetFont.add(extrusion);
	paramsPresetFont.add(letterSpacing);
	paramsPresetFont.add(heightLine);
	paramsPresetFont.add(rotateChars);

	paramsPresetFont.add(getIndexParam());
	paramsPresetFont.add(nameFont);

	paramsPreset.add(paramsPresetFont);

	//transform
	paramsPreset.add(transformNode.paramsPreset);
}

//--------------------------------------------------------------
void ofxSurfing3dText::setupGui() {
	ofLogNotice("ofxSurfing3dText") << "setupGui()";

	gui.setup(parameters);
	ofxSurfing::setGuiPositionToLayout(gui, ofxSurfing::SURFING_LAYOUT_TOP_RIGHT);

#ifdef SURFING__USE__IMGUI
	guiManager.setAutoAddInternalParamasToMainPanel(false);
	guiManager.setAutoHideFirstToggleVisibleForAnchorPanel(false);
#endif

	guiManager.setup(&gui);
	guiManager.add(&gui, bGui);
	guiManager.add(&transformNode.gui, transformNode.bGui);
	guiManager.add(&filesBrowserFonts.gui, filesBrowserFonts.bGui);
	guiManager.startup();

	//refresh
	gui.getGroup(paramsInternal.getName()).minimize();
	gui.getGroup(paramsDraw.getName()).minimize();
	gui.getGroup(paramsFont.getName()).maximize();
	gui.getGroup(paramsFont.getName()).getGroup(paramsFile.getName()).minimize();
	gui.getGroup(paramsDraw.getName()).getGroup(paramsDebug.getName()).minimize();
	gui.getGroup(paramsMeshDeform.getName()).minimize();

	refreshGui();
}

#ifdef SURFING__USE__IMGUI
//--------------------------------------------------------------
void ofxSurfing3dText::setupImGui() {
	ofLogNotice("ofxSurfing3dText") << "setupImGui()";

	//ui.setEnableHelpInternal(true);

	ui.addWindowSpecial(bGui);
	ui.addWindowSpecial(transformNode.bGui);
	ui.addWindowSpecial(filesBrowserFonts.bGui);

	//ui.startup();
	//ui.setLinkedSpecialWindows(true);
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawImGui() {
	if (!bGui) return;

	ui.Begin();
	{
		//if (bGui) {
		//	IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL;
		//	//float w = 500.f;
		//	//ImVec2 size_min = ImVec2(w, w);
		//	//ImVec2 size_max = ImVec2(-1, -1);
		//	//ImGui::SetNextWindowSizeConstraints(size_min, size_max);
		//}
		if (ui.BeginWindowSpecial(bGui)) {
			SurfingGuiTypes t = OFX_IM_TOGGLE_MEDIUM;
			ui.Add(transformNode.bGui, t);
			ui.Add(filesBrowserFonts.bGui, t);
			ui.Add(bHelp, OFX_IM_TOGGLE_ROUNDED_SMALL);
			//ui.AddLinekdWindowsToggle();

			ui.AddSpacingSeparated();

			surfingImGuizmo.drawImGuiUser(ui);

			ui.AddSpacingSeparated();

			//SurfingGuiGroupStyle flags = SurfingGuiGroupStyle_None;
			SurfingGuiGroupStyle flags = SurfingGuiGroupStyle_Collapsed;
			ui.AddGroup(parameters, flags);

			ui.EndWindowSpecial();
		}

		if (ui.BeginWindowSpecial(transformNode.bGui)) {
			ui.AddGroup(transformNode.parameters);
			#if 0
			ui.AddSpacingBigSeparated();
			ui.AddGroup(transformNode.paramsPreset);
			#endif

			ui.EndWindowSpecial();
		}

		if (ui.BeginWindowSpecial(filesBrowserFonts.bGui)) {
			ui.AddGroup(filesBrowserFonts.parameters);

			ui.EndWindowSpecial();
		}

		//--

		surfingImGuizmo.drawImGuizmo(camera, &transformNode);
	}
	ui.End();
}
#endif

//--------------------------------------------------------------
void ofxSurfing3dText::startup() {
	ofLogNotice("ofxSurfing3dText") << "startup()";

	bool b = load();

	if (!b) {
		ofLogNotice("ofxSurfing3dText") << "No settings files found!";
		//doResetTransform();
	}
}

////--------------------------------------------------------------
//void ofxSurfing3dText::doResetTransform() {
//	ofLogNotice("ofxSurfing3dText") << "doResetTransform()";
//}

//--------------------------------------------------------------
void ofxSurfing3dText::setupText(string text) {
	ofLogNotice("ofxSurfing3dText") << "setupText() " << text;

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
	ofLogNotice("ofxSurfing3dText") << "setupFont() " << path;

	if (path != "") {
		pathFont.set(path);
	}

	// load(const of::filesystem::path& filename, int fontSize,
	// bool antialiased, bool fullCharacterSet, bool makeContours,
	// float simplifyAmt, int dpi)
	bool b = font.load(pathFont.get(), sizeFont, true, true, true);

	if (b) {
		ofLogNotice("ofxSurfing3dText") << "Font successfully loaded: " << pathFont;
		nameFont = ofFilePath::getBaseName(pathFont.get());
	} else {
		ofLogError("ofxSurfing3dText") << "Error loading font: " << pathFont;
		nameFont = "NONE";
	}

	bFlagSetupText = true;
}

//--------------------------------------------------------------
void ofxSurfing3dText::doResetFont() {
	ofLogNotice("ofxSurfing3dText") << "doResetFont()";

	sizeFont = 50;
	extrusion = sizeFont * 0.5;
	letterSpacing = 0.f;
	heightLine = 0.f;
	color = ofColor(128, 255);

	indexModeDeform = 0;
	bAnim = false;
	powerDeform = 0.1;
	control1 = .5;
	control2 = .5;

	rotateChars = 0;

	bUppercase = false;
	bDrawBBox = true;
	bDrawBBoxBounds = false;

#ifdef SURFING__USE_LINE_WIDTH_FOR_FONT_INTERLETTER
	lineWidth = 1000;
#endif
}

//--------------------------------------------------------------
void ofxSurfing3dText::setup() {
	ofLogNotice("ofxSurfing3dText") << "setup()";

	setupParams();

	string p = "assets\\fonts";
	setupFontsBowser(p);

	setupFont();
}

//--------------------------------------------------------------
void ofxSurfing3dText::setupFontsBowser(string path) {
	ofLogNotice("ofxSurfing3dText") << "setupFontsBowser()";

	filesBrowserFonts.setupFonts(path);
}

//--------------------------------------------------------------
void ofxSurfing3dText::update(ofEventArgs & args) {
	update();
}

//--------------------------------------------------------------
void ofxSurfing3dText::update() {

	if (bAnim && (indexModeDeform == 0)) updateAnimMode0();

	uint64_t t = ofGetElapsedTimeMillis() - timeFlagSetupFont;
	if (bFlagSetupFont && t >= timeFlagSetupFontGap) {
		bFlagSetupFont = false;
		setupFont();
	}

	if (bFlagSetupText) {
		bFlagSetupText = false;
		setupText();
	}

	if (bFlagBuildHelp) {
		bFlagBuildHelp = false;
		buildHelp();
	}

	//filesBrowserFonts.updateAutoSwitch();
}

//--------------------------------------------------------------
void ofxSurfing3dText::updateAnimMode0() {

	float t = ofGetElapsedTimef();
	float a = ofMap(powerDeform, 0, 1, 0.4, 1, true);
	a *= MAX(0.1, control1 * 10);

	float et = a * t;

	glm::vec3 axis(0, -1, 0);

	float dur = 3;
	//dur *= MAX(0.1,control2);

	int df = 60 * dur;
	if (ofGetFrameNum() % df < df / 2) {
		axis = glm::vec3(1, 0, 0);
	}
	for (int i = 0; i < meshNodes.size(); i++) {
		float sine = sinf((float)i + et);
		float d = 360;
		d *= control2;

		float spin = ofMap(sine, 0.4, 0.6, 0, d, true);
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

	if (bGui_ofxGui) {
		guiManager.draw();

		filesBrowserFonts.drawHelp(true);
		ofRectangle r = guiManager.getShapePanels();
		filesBrowserFonts.drawPreviewRightToRect(r);
	}

	//--

	drawHelp();
}

//--

//--------------------------------------------------------------
void ofxSurfing3dText::refreshGui() {
	ofLogNotice("ofxSurfing3dText") << "refreshGui()";

	transformNode.refreshGui();
}

//--------------------------------------------------------------
void ofxSurfing3dText::refreshGuiUserParams(ofxPanel & gui_, ofxGuiGroup & group_) {
	ofLogNotice("ofxSurfing3dText") << "refreshGuiUserParams(ofxPanel,ofxGuiGroup)";

	transformNode.refreshGuiUserParams(gui_, group_);
}

//--------------------------------------------------------------
void ofxSurfing3dText::refreshGuiUserParams(ofxPanel & gui_) {
	ofLogNotice("ofxSurfing3dText") << "refreshGuiUserParams(ofxPanel)";

	//TODO
	ofxGuiGroup & g = gui_.getGroup(paramsPreset.getName());
	//ofxGuiGroup & g = gui_.getGroup(this->getName()).getGroup(paramsPreset.getName());
	//ofxGuiGroup & g = gui_.getGroup(this->getName()).getGroup("TransformNode");
	//ofxGuiGroup & g = gui_.getGroup(this->getName()).getGroup("TEXT_PRESET");
	this->refreshGuiUserParams(gui_, g);
}

//--

//--------------------------------------------------------------
void ofxSurfing3dText::buildHelp() {

	ofLogNotice("ofxSurfing3dText") << "buildHelp()";

	sHelp = "HELP\n3D_TEXT";
	sHelp += "\n\n";
	sHelp += "TEXT:\n\n";
	sHelp += textMessage.get();
	sHelp += "\n\n";
	if (!bKeys) {
		sHelp += "KEYS DISABLED\n";
		return;
	}
	sHelp += "g: Gui\n";
	sHelp += "h: Help\n";
	sHelp += "d: Debug      " + string(bDebug ? "ON " : "OFF") + "\n";
	sHelp += "u: Uppercase  " + string(bUppercase ? "ON " : "OFF") + "\n";
	sHelp += "a: Animate    " + string(bAnim ? "ON " : "OFF") + "\n";
	sHelp += "\n";
	sHelp += "BACKSPACE:   \nReset\n\n";
	sHelp += "E/e:         \nExtrusion     " + ofToString(extrusion.get(), 0) + "\n\n";
	sHelp += "LEFT/RIGHT:  \nLetterSpacing " + ofToString(letterSpacing.get(), 2) + "\n\n";
	sHelp += "UP/DOWN:     \nHeightLine    " + ofToString(heightLine.get(), 2) + "\n\n";
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawHelp() {
	if (!bHelp) return;

//ofxSurfing::ofDrawBitmapStringBox(sHelp, &gui, ofxSurfing::SURFING_LAYOUT_TOP_RIGHT);
#if 0
	//left
	ofxSurfing::ofDrawBitmapStringBox(sHelp, &gui, ofxSurfing::SURFING_LAYOUT_TOP_LEFT);
#else
	//bottom
	//ofxSurfing::ofDrawBitmapStringBox(sHelp, ofxSurfing::SURFING_LAYOUT_BOTTOM_CENTER);
	ofxSurfing::ofDrawBitmapStringBox(sHelp, &gui, ofxSurfing::SURFING_LAYOUT_BOTTOM_LEFT);
#endif
}

//--------------------------------------------------------------
void ofxSurfing3dText::draw() {
	if (!transformNode.isEnabled()) return;

	drawMeshes();
	drawBounds();
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawMeshes() {
	if (!bDrawMeshes) return;

	if (transformNode.isDebug()) transformNode.draw();
	if (!transformNode.isEnabled()) return;

	transformNode.transformGL();
	{
		if (indexModeDeform == 0) {
			drawMeshesMode0();
		}

		else if (indexModeDeform == 1) {
			drawMeshesMode1();
		}

		else if (indexModeDeform == 2) {
			drawMeshesMode2();
		}
	}
	transformNode.restoreTransformGL();
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawMeshesMode0() {
	ofPushMatrix();
	for (auto & meshNode : meshNodes) {
		//rotate chars
		if (rotateChars.get() != 0) {
			auto axis = glm::vec3(0, 1, 0);
			const glm::quat q = glm::angleAxis(ofDegToRad(rotateChars), axis);
			meshNode.node.setOrientation(q);
		}

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

	float t = ofGetElapsedTimef();

	t *= control1 * 10;

	float ow = 50;
	ow *= control2;

	for (auto & meshNode : meshNodes) {
		ofPushMatrix();
		ofTranslate(meshNode.node.getPosition());

		//ofTranslate(0, 0, -1000 - extrusion * 2);
		ofTranslate(0, 0, -(extrusion * 3) / 2.f);

		//ofSetColor(184, 180, 176);
		ofColor c = (255, 255);
		ofSetColor(c.r, c.g, c.b, 176);
		for (auto & pline : meshNode.polylines) {
			pline.draw();
		}

		float offset;
		if (bAnim)
			offset = 1.f - ofClamp(sinf(t), 0, 1);
		else
			offset = .25f;

		offset *= powerDeform;

		ofSetColor(c.r, c.g, c.b, 59);
		ofTranslate(0, 0, 2 * ow * offset);
		for (auto & mesh : meshNode.srcMeshes) {
			mesh.draw();
		}

		ofSetColor(c.r, c.g, c.b, 116);
		ofTranslate(0, 0, ow * offset + extrusion);
		for (auto & mesh : meshNode.sideMeshes) {
			mesh.draw();
		}

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
void ofxSurfing3dText::drawMeshesMode2() {
	ofPushMatrix();
	ofPushStyle();

	//modify mesh with some noise
	float liquidness = 10 * control1;
	float speedDampen = 10 * control2;
	float amax = 20;
	float amplitude = ofMap(powerDeform, 0, 1, 0, amax, true);

	for (auto & meshNode : meshNodes) {
		ofVboMesh mesh = meshNode.mesh;
		auto & verts = mesh.getVertices();

		for (unsigned int i = 0; i < verts.size(); i++) {
			verts[i].x += ofSignedNoise(verts[i].x / liquidness, verts[i].y / liquidness, verts[i].z / liquidness, ofGetElapsedTimef() / speedDampen) * amplitude;
			verts[i].y += ofSignedNoise(verts[i].z / liquidness, verts[i].x / liquidness, verts[i].y / liquidness, ofGetElapsedTimef() / speedDampen) * amplitude;
			verts[i].z += ofSignedNoise(verts[i].y / liquidness, verts[i].z / liquidness, verts[i].x / liquidness, ofGetElapsedTimef() / speedDampen) * amplitude;
		}

		meshNode.node.transformGL();
		mesh.drawFaces();
		meshNode.node.restoreTransformGL();
	}

	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawBounds() {
	if (!bGui) return;
	if (!bDebug) return;
	if (!bDrawBBox && !bDrawBBoxBounds) return;

	transformNode.transformGL();
	{
		ofColor c;

		int a = 150;
		//int a = ofMap(glm::sin(ofGetElapsedTimef()), -1, 1, 0, 200);//blink

		c = ofColor(255, a); //white
		//c = ofColor(0, a); //black

		ofPushStyle();
		{
			if (bDrawBBox) {
				ofPushMatrix();
				{
					ofTranslate(-meshCentroid);

					ofSetColor(c);
					drawBounds(meshMin, meshMax, 100);
				}
				ofPopMatrix();
			}

			if (indexModeDeform != 1)
				if (bDrawBBoxBounds) {
					for (auto & meshNode : meshNodes) {
						meshNode.node.transformGL();

						ofSetColor(c);
						drawBounds(meshNode.min, meshNode.max, 30);

						meshNode.node.restoreTransformGL();
					}
				}
		}
		ofPopStyle();
	}
	transformNode.restoreTransformGL();
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

	ofLogVerbose("ofxSurfing3dText") << "keyPressed";

	keyPressed(key);
}

//--------------------------------------------------------------
void ofxSurfing3dText::keyPressed(int key) {
	if (!bKeys) return;
	ofLogNotice("ofxSurfing3dText") << "keyPressed: " << key;

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

#ifdef SURFING__USE__IMGUI
	surfingImGuizmo.keyPressed(key);
#endif
}
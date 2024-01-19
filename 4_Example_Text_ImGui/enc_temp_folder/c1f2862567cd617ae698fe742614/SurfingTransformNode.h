
/*
* SurfingTransformNode.h
* 
	A class derived from ofNode, 
	but adding ofParameters for 
	position, rotation, scale and other useful params (draw, debug...etc)
	Ready to be exposed to a UI.
	Handles the transforms for an object.
	To be applied easily and to store to JSON settings
*/

/*
	TODO
	- add refreshGui by passing transfromGroup. like presets groups
	- allow mode for linked or independent scale for xyz
		- fix lock all axis to x 
	- add bbox for selected model
	- reduce some feedback callbacks
	- add simple example?
*/

//--

#pragma once

#include "ofMain.h"

//TODO: add settings
#include "ofxSurfingHelpersLite.h"

//----

//OPTIONAL: comment the below line
// if you are using this class standalone,
// without the whole ofxSurfingPBR!
//#include "ofxSurfingPBRConstants.h"

#ifndef SURFING__PBR__SCENE_SIZE_UNIT
	#define SURFING__PBR__SCENE_SIZE_UNIT 1000.f
#endif

// Range for rotate
#define SURFING__MAX_DEGREE 360
//#define SURFING__MAX_DEGREE 180

//#define SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS

//----

class TransformNode : public ofNode {

	//--

	// Some constants
	const float scaleNormalizedRatio = 10.f;
	const int scaleNormalizedPowMax = 100;
	//const float scaleNormalizedUnit = scaleNormalizedPowMax;
	float scaleNormalizedUnit = scaleNormalizedRatio * scaleNormalizedPowMax;
	const float unitSize = SURFING__PBR__SCENE_SIZE_UNIT * 1.f;

	//--

	// Main ofParameter controls
	// Exposed to gui and "redirected" to ofNode's internals!
public:
	ofParameter<glm::vec3> position { "Position", glm::vec3(0),
		glm::vec3(-unitSize), glm::vec3(unitSize) };

	ofParameter<glm::vec3> scale { "Scale", glm::vec3(1),
		glm::vec3(1), glm::vec3(scaleNormalizedUnit) };

	ofParameter<glm::vec3> orientationEuler { "Orientation Euler", glm::vec3(0),
		glm::vec3(-(float)SURFING__MAX_DEGREE), glm::vec3((float)SURFING__MAX_DEGREE) };

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
	// Extra normalized controls
	// intended to adapt to our scene and objects dimensions.
	ofParameter<float> scaleNormalized { "Scale Norm", 0, -1.f, 1.f };
	ofParameter<int> scaleNormalizedPow { "Scale Pow", scaleNormalizedPowMax / 2, 1, scaleNormalizedPowMax };
	ofParameter<bool> bScaleLinkAxis { "ScaleLinkAxis", true }; //TODO
	ofParameter<glm::vec3> positionNormalized { "Position Normalized", glm::vec3(0), glm::vec3(-1), glm::vec3(1) };
	ofParameterGroup paramsScaleNormalized;
#endif

	ofParameterGroup parameters; //exposed to the gui and settings
	ofParameterGroup paramsOfNode;
	ofParameterGroup paramsResets;

private:
	bool bAttendingPosition = false;
	bool bAttendingScale = false;
	bool bAttendingOrientation = false;

public:
	ofParameter<void> vReset { "Reset" };
	ofParameter<void> vResetScale { "Reset Scale" };
	ofParameter<void> vResetPosition { "Reset Position" };
	ofParameter<void> vResetOrientation { "Reset Rotation" };

	ofParameter<bool> bDraw { "Draw", true };
	ofParameter<bool> bDebug { "Debug", false };

private:
	SurfingAutoSaver autoSaver;
	string name = "";
	string pathSuffix = "_Transform.json";
	string path = "";
	bool bDoneSetup = false;

public:
	//preset
	ofxPanel gui;
	ofParameter<bool> bGui;
	ofParameterGroup paramsPreset;

	void setName(string name_) { //caall before setup
		ofLogNotice("TransformNode") << "setName(" << name_ << ")";
		name = name_;
	}

private:
	void setupSettings() {
		ofLogNotice("TransformNode") << "setupSettings()";

		// build preset group
		if (name == "") name = "Transform";
		//name = "UI " + name;
		bGui.set(name, true);

		paramsPreset.setName(name);

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
		paramsPreset.add(positionNormalized);
		paramsPreset.add(paramsScaleNormalized);
#endif
		paramsPreset.add(paramsOfNode);
		paramsPreset.add(paramsResets);

		path = name + pathSuffix;

		gui.setup(paramsPreset);

		if (bEnableSettings) {
			callback_t f = std::bind(&TransformNode::save, this);
			autoSaver.setFunctionSaver(f);

			load();
		}
	}

	void save() {
		ofLogNotice("TransformNode") << "save -> " << path;
		ofxSurfing::saveSettings(parameters, path);
	}

	bool load() {
		ofLogNotice("TransformNode") << "load -> " << path;
		if (bEnableSettings) autoSaver.pause();
		bool b = ofxSurfing::loadSettings(parameters, path);
		if (bEnableSettings) autoSaver.start();
		return b;
	}

	void update(ofEventArgs & args) {
		update();
	}

	void update() {
		if (!bDoneSetup) {
			setup();
		}
	}

	void Changed(ofAbstractParameter & e) {
		string name = e.getName();

		ofLogVerbose("TransformNode") << "Changed: " << name << ": " << e;

		if (bEnableSettings)
			if (e.isSerializable()) {
				autoSaver.saveSoon();
			}
	}

	//----

public:
	TransformNode() {
		ofLogNotice("TransformNode") << "Constructor";

		//setup();

		ofAddListener(ofEvents().update, this, &TransformNode::update);
		ofAddListener(parameters.parameterChangedE(), this, &TransformNode::Changed);
	}

	TransformNode(const TransformNode & other) {
		ofLogNotice("TransformNode") << "Constructor";

		ofAddListener(ofEvents().update, this, &TransformNode::update);
		ofAddListener(parameters.parameterChangedE(), this, &TransformNode::Changed);
	}

	~TransformNode() {
		ofLogNotice("TransformNode") << "Destructor";

		ofRemoveListener(ofEvents().update, this, &TransformNode::update);
		ofRemoveListener(parameters.parameterChangedE(), this, &TransformNode::Changed);
	}

	//--

	void refreshGui() {
		ofLogNotice("TransformNode") << "refreshGui()";

		gui.getGroup(paramsOfNode.getName()).getGroup(scale.getName()).minimize();
		gui.getGroup(paramsOfNode.getName()).getGroup(position.getName()).minimize();
		gui.getGroup(paramsResets.getName()).minimize();
	}

	void refreshGuiUserParams(ofxPanel & gui_, ofxGuiGroup & group_) {
		ofLogNotice("TransformNode") << "refreshGui(ofxPanel,ofxGuiGroup)";

		group_.getGroup(paramsOfNode.getName()).getGroup(scale.getName()).minimize();
		group_.getGroup(paramsOfNode.getName()).getGroup(position.getName()).minimize();
		group_.getGroup(paramsResets.getName()).minimize();
	}

	//--

private:
	bool bEnableSettings = true;

public:
	void setEnableSettings(bool b) { //use to disable when settings are handled externaly. as when using a transform vector for multiple objects..
		bEnableSettings = b;
	}

private:
	std::unique_ptr<ofEventListener> e_vResetScale;
	std::unique_ptr<ofEventListener> e_vResetPosition;
	std::unique_ptr<ofEventListener> e_vResetOrientation;
	std::unique_ptr<ofEventListener> e_vReset;

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
	std::unique_ptr<ofEventListener> e_positionNormalized;
	std::unique_ptr<ofEventListener> e_scaleNormalized;
	std::unique_ptr<ofEventListener> e_scaleNormalizedPow;
#endif
	std::unique_ptr<ofEventListener> e_rotationEulerChanged; //TODO

	//--

public:
	void setup() {

		// parameters

		parameters.setName("TRANSFORM");

		paramsOfNode.setName("ofNode");
		paramsOfNode.add(position);
		paramsOfNode.add(orientationEuler);
		paramsOfNode.add(scale);

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
		paramsScaleNormalized.setName("Scale Normalized");
		paramsScaleNormalized.add(scaleNormalized);
		paramsScaleNormalized.add(scaleNormalizedPow);
		paramsScaleNormalized.add(bScaleLinkAxis); //TODO
#endif

		parameters.add(bDraw);
		parameters.add(bDebug);

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
		parameters.add(positionNormalized);
		parameters.add(paramsScaleNormalized);
#endif
		parameters.add(paramsOfNode);

		paramsResets.setName("Resets");
		paramsResets.add(vResetScale);
		paramsResets.add(vResetPosition);
		paramsResets.add(vResetOrientation);
		paramsResets.add(vReset);
		paramsResets.setSerializable(false);
		parameters.add(paramsResets);

		setupCallbacks();

		//--

		setupSettings();

		bDoneSetup = true;
	}

	//--

	// Get the transforms for each model
	// passing the model index as argument:

public:
	bool isEnabled() const {
		return bDraw;
	}
	bool isDebug() const {
		return bDebug;
	}

	//--

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
	float getScaleNormalized() const {
		return scaleNormalized;
	}

	float getScalePow() const {
		return scaleNormalizedPow;
	}

	glm::vec3 getPositionNormalized() const {
		return positionNormalized;
	}
#endif

	glm::vec3 getOrientationEuler() const {
		return orientationEuler;
	}

	//--

	void doReset() {
		doResetPosition();
		doResetOrientation();
		doResetScale();
	}

	void doResetPosition() {
		setPosition(0, 0, 0);

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
		positionNormalized = glm::vec3(0);
#endif
	}

	void doResetOrientation() {
		setOrientation(glm::vec3(0));
		//orientationEuler = glm::vec3(0);
	}

	void doResetScale() {
		setScale(1, 1, 1);

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
		//scaleNormalizedPow = scaleNormalizedPowMax/2;
		scaleNormalizedPow = 5;
		scaleNormalized = 0;
#endif
	}

	//--

	//https://forum.openframeworks.cc/t/ofnode-and-ofparameter/24436/2

public:
	void setupCallbacks() {
		position.addListener(this, &TransformNode::_setPosition);
		orientationEuler.addListener(this, &TransformNode::_setOrientation);
		scale.addListener(this, &TransformNode::_setScale);

		//--

		// Transform

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
		e_positionNormalized = std::make_unique<ofEventListener>(positionNormalized.newListener([this](glm::vec3) {
			ofLogNotice(__FUNCTION__);

			refreshPositionFromNormalized();
		}));

		e_scaleNormalized = std::make_unique<ofEventListener>(scaleNormalized.newListener([this](float) {
			ofLogNotice(__FUNCTION__);

			refreshScaleFromNormalized();
		}));

		e_scaleNormalizedPow = std::make_unique<ofEventListener>(scaleNormalizedPow.newListener([this](float) {
			ofLogNotice(__FUNCTION__);

			//adapt param range
			scaleNormalizedUnit = scaleNormalizedRatio * scaleNormalizedPow;
			scale.setMax(glm::vec3(scaleNormalizedUnit, scaleNormalizedUnit, scaleNormalizedUnit));

			refreshScaleFromNormalized();
		}));
#endif

		e_rotationEulerChanged = std::make_unique<ofEventListener>(orientationEuler.newListener([this](glm::vec3) {
		}));

		//--

		// Resets

		e_vReset = std::make_unique<ofEventListener>(vReset.newListener([this](void) {
			doReset();
		}));

		e_vResetPosition = std::make_unique<ofEventListener>(vResetPosition.newListener([this](void) {
			doResetPosition();
		}));


		e_vResetScale = std::make_unique<ofEventListener>(vResetScale.newListener([this](void) {
			doResetScale();
		}));
		e_vResetOrientation = std::make_unique<ofEventListener>(vResetOrientation.newListener([this](void) {
			doResetOrientation();
		}));
	}

	//--

	// Update ofNode

	void _setPosition(glm::vec3 & v) {
		ofLogNotice(__FUNCTION__);

		setPosition(v);

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
		refreshPositionToNormalized();
#endif
	}

	void _setScale(glm::vec3 & v) {
		if (bAttendingScale) return;
		ofLogNotice(__FUNCTION__) << v;

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
		if (bScaleLinkAxis)
#endif
		{
			bAttendingScale = true;

			//TODO: fix lock all axis to x
			glm::vec3 s = glm::vec3(scale.get().x);

			//glm::vec3 s = glm::vec3(v.x, v.x, v.x);
			scale.set(s);
//v = s;
#if 0
			ofLogNotice(__FUNCTION__) << "s:" << s;
			ofLogNotice(__FUNCTION__) << "v:" << v;
			ofLogNotice(__FUNCTION__) << "scale:" << scale;
#endif

			bAttendingScale = false;

			setScale(s);
			return;
		}

		setScale(v);
	}

	void _setOrientation(glm::vec3 & v) {
		ofLogNotice(__FUNCTION__);

		setOrientation(v);
	}

	//--

	// Update ofParams from ofNode

	void onPositionChanged() override {
		if (position.get() != getPosition()) {
			ofLogNotice(__FUNCTION__);

			position.set(getPosition());
			//position.setWithoutEventNotifications(getPosition());

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
			refreshPositionToNormalized();
#endif
		}
	}

	void onScaleChanged() override {
		if (bAttendingScale) return;

		if (scale.get() != getScale()) {
			ofLogNotice(__FUNCTION__);

			scale.set(getScale());
		}

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
		//TODO: apply to normalized scale
		refreshScaleToNormalized();
#endif
	}

	void onOrientationChanged() override {
		bool bDiff = orientationEuler.get() != getOrientationEuler();
		// compare the two objects with an epsilon of 0.001
		//bool bDiff = glm::all(glm::epsilonEqual(orientationEuler.get(), getOrientationEuler(), 0.001f));

		if (bDiff) {
			ofLogNotice(__FUNCTION__);

			//ofLogNotice() << "orientationEuler:" << orientationEuler;
			//ofLogNotice() << "getOrientationEuler():" << getOrientationEuler();

			//TODO
			refreshOrientationEulerFromOfNode();
		}
	}

	//--

#ifdef SURFING_TRANSFORM_NOSE___USE__NORMALIZED_CONTROLS
	void refreshPositionFromNormalized() {
		ofLogNotice(__FUNCTION__);

		const float u = unitSize; //distance unit
		float x = ofMap(positionNormalized.get().x, -1, 1, -u, u, true);
		float y = ofMap(positionNormalized.get().y, -1, 1, -u, u, true);
		float z = ofMap(positionNormalized.get().z, -1, 1, -u, u, true);
		setPosition(glm::vec3(x, y, z));
		_setPosition(glm::vec3(x, y, z));
	}

	void refreshPositionToNormalized() {
		ofLogNotice(__FUNCTION__);

		//apply to normalized
		const float u = position.getMax().x; // unit assumed the same for the three axis
		float x = ofMap(position.get().x, -u, u, -1, 1, true);
		float y = ofMap(position.get().y, -u, u, -1, 1, true);
		float z = ofMap(position.get().z, -u, u, -1, 1, true);

		float px = positionNormalized.get().x;
		float py = positionNormalized.get().y;
		float pz = positionNormalized.get().z;
		if (px != x || py != y || pz != z) //update only if changed/differs to reduce callbacks
		{
			positionNormalized.set(glm::vec3(x, y, z));
		}
	}

	void refreshScaleFromNormalized() {
		ofLogNotice(__FUNCTION__);

		float s = 1;
		if (scaleNormalized == 0) {
			s = scaleNormalizedPow;
		} else if (scaleNormalized > 0) {
			s = ofMap(scaleNormalized, 0, 1, scaleNormalizedPow, scaleNormalizedPow * scaleNormalizedRatio, true);
		} else if (scaleNormalized < 0) {
			s = ofMap(scaleNormalized, 0, -1, scaleNormalizedPow, scaleNormalizedPow / scaleNormalizedRatio, true);
		}
		setScale(s, s, s);
		_setScale(glm::vec3(s, s, s));
	}

	void refreshScaleToNormalized() {
		ofLogNotice(__FUNCTION__);

		//TODO
		float sx = 0;
		float s = scale.get().x;
		const float u = scale.getMax().x; // unit assumed the same for the three axis

		sx = ofMap(s, scale.getMin().x, scale.getMax().x, -1, 1, true);

		//if (s == 0) {
		//	sx = 0;
		//} else if (s < scaleNormalizedPow * scaleNormalizedRatio) {
		//	sx = ofMap(s, 0, scaleNormalizedPow / scaleNormalizedRatio, 0, -1, true);
		//}
		//if (s > 0) {
		//	sx = ofMap(s, 0, scaleNormalizedPow * scaleNormalizedRatio, 0, 1, true);
		//}

		//TODO: link
		if (bScaleLinkAxis) {
			scaleNormalized.set(sx);
		}
	}
#endif

	void refreshOrientationEulerFromOfNode() {
		ofLogNotice(__FUNCTION__);

		////TODO
		//return;

		//bAttendingOrientation = true;

		orientationEuler = getOrientationEuler();

		//glm::vec3 r = orientationEuler.get();
		//_setOrientation(r);

		//bAttendingOrientation = false;
	}

	//--

public:
	ofNode & getNode() { return *this; }
	// Exposed to use push/pop matrix:
	// myObject.getNode().transformGL();
	// draw your "transformed" scene
	// myObject.getNode().restoreTransformGL();
};
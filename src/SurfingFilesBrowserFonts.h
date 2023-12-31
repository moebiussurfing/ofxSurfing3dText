
/*
* SurfingFilesBrowserFonts.h
* 
	A class to help loading the many font files
	from a path directory (default bin/data/assets/fonts)
*/

//--

#pragma once

#include "ofMain.h"

#include "SurfingFilesBrowser.h"
#include "ofxFontBook.hpp"

//--

class SurfingFilesBrowserFonts : public SurfingFilesBrowser {
public:
	SurfingFilesBrowserFonts() {
		ofLogNotice("ofxSurfingPBR") << "SurfingFilesBrowserFonts:SurfingFilesBrowserFonts()";

		//paramsUser.setName("FONTS_BROWSER");
		////paramsUser.add(bGui);
		//paramsUser.add(bDraw);
		//paramsUser.add(bModeAutoload);
	}

	~SurfingFilesBrowserFonts() {
		ofLogNotice("ofxSurfingPBR") << "SurfingFilesBrowserFonts:~SurfingFilesBrowserFonts()";
	}

private:
	void setupParametersExtra() override {
		ofLogNotice("ofxSurfingPBR") << "SurfingFilesBrowserFonts:setupParametersExtra()";

		parameters.add(paramsExtra);
		//parameters.add(paramsUser);

		parameters.add(bDraw);
		parameters.add(bModeAutoload);

		parameters.add(bKeys);
		parameters.add(bHelp);
	}

	void setupGui() override {
		ofLogNotice("ofxSurfingPBR") << "SurfingFilesBrowserFonts:setupGui()";

		setupParametersExtra();

		gui.setup(parameters);

		gui.getGroup(paramsExtra.getName()).minimize();
	}

public:
	void doReset() override {
		ofLogNotice("ofxSurfingPBR") << "SurfingFilesBrowserFonts:doReset()";
	}

public:
	void save() override {
		ofLogNotice("ofxSurfingPBR") << "SurfingFilesBrowserFonts:save()";

		ofxSurfing::saveSettings(parameters, pathSettings);
	}

	void load() override {
		ofLogNotice("ofxSurfingPBR") << "SurfingFilesBrowserFonts:load()";

		autoSaver.pause();
		ofxSurfing::loadSettings(parameters, pathSettings);
		autoSaver.start();
	}

	//--

public:
	void refreshGui() override {
		ofLogNotice("ofxSurfingPBR") << "SurfingFilesBrowserFonts:refreshGui()";

		//gui.getGroup(paramsExtra.getName()).minimize();
	}

	//--

	ofxFontBook fontBook;
	//ofEventListener listenerIndex; //get class internal index changed

	ofParameter<string> pathFonts;

	ofParameter<bool> bDraw { "Draw Previews", true };
	ofParameter<bool> bModeAutoload { "Mode Autoload", false };

	//ofParameterGroup paramsUser;

	//else if (name == pathFonts.getName()) {
	//}

	void setupFonts(string path) {
		ofLogNotice("ofxSurfing3dText") << "setupFonts() " << path;

		setFileExtensions({ "ttf", "otf" });
		setTitle("FONTS_BROWSER");

		pathFonts.set("pathFonts", "");
		if (path != "") {
			pathFonts.set(path);
		}

		paramsExtra.add(pathFonts);

		fontBook.bDraw.makeReferenceTo(bDraw);

		//TODO: link
		//indexFile.makeReferenceTo(fontBook.indexFont);
		//fontBook.indexFont.makeReferenceTo(indexFile);

		fontBook.autoloadFontsFromDir(pathFonts.get());

		//listenerIndex= indexFile.newListener([this](int & i) {
		//	ofLogNotice("ofxSurfing3dText") << "index: " << i;
		//	fontBook.indexFont.set(i);
		//});

	setup(path);
	}

	void drawPreview(int x, int y) {
		fontBook.drawPreviews(x, y);
	}
};
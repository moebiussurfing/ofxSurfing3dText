/*
	Original code taken from 
	https://gitlab.artificiel.org/ofxaddons/ofxFontsBook/-/tree/master?ref_type=heads
	and modified by @moebiusSurfing.
	currently only used to draw a preview of all the font files.
	the class allows to directly draw fonts 
	and they will be auto uploaded if is not preloaded.
*/

#ifndef OFXFONTBOOK_HPP
#define OFXFONTBOOK_HPP

#include "ofMain.h"

/*
   ofxFontsBook est un static-friendly conteneur de ofTrueTypeFont qui
   cherche récursivement et charge paresseusement les fontes à la demande
 */

class ofTrueTypeFontPlus : public ofTrueTypeFont {
public:
	auto drawStringAlignedRight(std::string str, size_t x, size_t y) {
		auto w = stringWidth(str);
		return drawString(str, x - w, y);
	}
	auto drawStringCentered(std::string str, size_t x, size_t y) {
		auto w = stringWidth(str);
		return drawString(str, x - (w / 2), y);
	}
};

//--

// fallback font (should be provided as ADDON_DATA) -- FIXME
static std::string fontbook_fallback_ttf_ = "assets/fonts/mplus-1m-medium.ttf";

class ofxFontsBook {

public:
#if 0
	ofxFontsBook() = default;
#else
	ofxFontsBook() {
		//	parameters.setName("FONT BOOK");
		//	parameters.add(bDraw);
		//	parameters.add(indexFont);
		//	parameters.add(sizeFont);

		#if 0
		listenerSizeFont = sizeFont.newListener([this](float & v) {
			ofLogNotice("ofxFontsBook") << "sizeFont: " << v;

			float wmax = 0;
			float hmax = 0;
			fonts_.clear();

			autoloadFontsFromDir();
		});
		#endif
	}
#endif

	bool load(std::pair<std::string, std::size_t> font) {

		if (!fonts_.count(font)) {
			fonts_[font] = std::make_shared<ofTrueTypeFontPlus>();
			if (scan_dir_for_font(ofToDataPath("", true), font)) {
				return 1;
			}
		}
		ofLogWarning("ofxFontsBook") << "not found: " << font.first << "; using fallback " << fontbook_fallback_ttf_ << "; loading size " << font.second;
		fonts_[font]->load(fontbook_fallback_ttf_, int(font.second));

		return 0;
	}

	auto operator[](std::pair<string, std::size_t> font) {
		if (!fonts_.count(font)) {
			load(font);
		}
		return fonts_[font];
	}

	void listLoadedFonts() {
		for (const auto & font : fonts_) {
			ofLogNotice("ofxFontsBook") << "Loaded font: " << font.first.first << ", size: " << font.first.second;
		}
	}

	//void toggleAlignMode() {
	//	indexAlignMode++;
	//	if (indexAlignMode > (int)ALIGN_RIGHT)
	//		indexAlignMode = (int)ALIGN_LEFT;
	//	indexAlignMode = ofClamp(indexAlignMode, 0, ALIGN_RIGHT);
	//}

	void autoloadFontsFromDir(const std::string & path_ = "") {
		if (path_ != "") {
			path = path_;
		}

		ofDirectory fontDir(path);

		fontDir.allowExt("ttf");
		fontDir.allowExt("otf");

		fontDir.listDir();

		if (fontDir.size() == 0) ofLogError("ofxFontsBook") << "Path " << path_ << " not found!";

		#if 0
		size_t size = sizeFont;
		#else
		size_t size = 12;
		#endif

		for (const auto & file : fontDir.getFiles()) {
			std::string fontName = file.getBaseName();
			load({ fontName, size });
		}
	}

	void drawPreviews(int x, int y, int lineSpacing = -1) {
		if (!bDraw) return;

		//if (indexFont == -1 && fonts_.size() > 0) indexFont = 0;

		if (lineSpacing == -1)
			lineSpacing = sizeFont * 1.7;

		int y_ = y;

		y_ += 17; //offset

		// bg
		//static float wmax = 0;
		//static float hmax = 0;
		int pad = 5;
		ofRectangle rbb { (float)x - pad, (float)y_ - sizeFont - pad, wmax + 2 * pad, hmax + 2 * pad };
		ofPushStyle();
		ofSetColor(0, 255);
		ofFill();
		ofDrawRectangle(rbb);

		// text
		ofSetColor(255, 255);

		//#define SURFING__SHOW__FONT_SELECTED 0
		//#if (SURFING__SHOW__FONT_SELECTED)
		//		// selected font
		//		string s = "";
		//		s += getFontNameByIndex() + "\n\n";
		//		//s += "Index: " + ofToString(indexFont) + "\n";
		//		//s += "Size: " + ofToString(sizeFont) + "\n";
		//		//s += "\n";
		//		if (indexFont >= 0 && indexFont < fonts_.size()) {
		//			auto it = fonts_.begin();
		//			std::advance(it, indexFont);
		//
		//			it->second->drawString(s, x, y_);
		//
		//			auto bb = it->second->getStringBoundingBox(s, 0, 0);
		//			int h = bb.getHeight();
		//			y_ += h;
		//
		//			y_ += lineSpacing;
		//
		//			if (bb.getWidth() > wmax) wmax = bb.getWidth();
		//		}
		//#endif

		// all fonts
		size_t i = 0;
		for (const auto & font : fonts_) {
			std::string fontName = font.first.first;
			string s = fontName;
			//#if (SURFING__SHOW__FONT_SELECTED)
			if (i == indexFont) s += " <";
			//#endif
			font.second->drawString(s, x, y_);

			auto bb = font.second->getStringBoundingBox(s, 0, 0);
			if (bb.getWidth() > wmax) wmax = bb.getWidth();

			y_ += lineSpacing;
			i++;
		}

		int ydiff = y_ - y - lineSpacing - 3;
		if (ydiff > hmax) hmax = ydiff;

		ofPopStyle();
	}

	void drawStringWithName(const std::string & text, int x, int y, const std::string & fontName = "", std::size_t size = 12) {
		//if (size == -1) size = sizeFont;
		std::string useFontName = fontName.empty() ? fonts_.begin()->first.first : fontName;
		if (!fonts_.count({ useFontName, size })) {
			load({ useFontName, size });
		}
		fonts_[{ useFontName, size }]->drawString(text, x, y);
	}

	void drawStringWithIndex(const std::string & text, int x, int y, int index = -1, std::size_t size = 12) {
		//if (index == -1) index = indexFont;
		//if (size == -1) size = sizeFont;
		if (index < 0 || index >= fonts_.size()) {
			ofLogError("ofxFontsBook") << "Invalid font index: " << index;
			return;
		}
		auto it = fonts_.begin();
		std::advance(it, index);
		it->second->drawString(text, x, y);
		//indexFont = index; // Store the index of the selected font
	}

	std::string getFontNameByIndex(int index = -1) {
		//if (index == -1) index = indexFont;
		if (index < 0 || index >= fonts_.size()) {
			ofLogError("ofxFontsBook") << "Invalid font index: " << index;
			return "";
		}
		auto it = fonts_.begin();
		std::advance(it, index);
		return it->first.first; // Return the font name
	}

	//void previousFont() {
	//	indexFont--;
	//	indexFont = ofClamp(indexFont, 0, fonts_.size()-1);
	//}

	//void nextFont() {
	//	indexFont++;
	//	indexFont = ofClamp(indexFont, 0, fonts_.size()-1);
	//}

	//void sizeSmaller() {
	//	sizeFont--;
	//	sizeFont = ofClamp(sizeFont, 4, 200);
	//}
	//void sizeBigger() {
	//	sizeFont++;
	//	sizeFont = ofClamp(sizeFont, 4, 200);
	//}

private:
	bool scan_dir_for_font(std::string cwd, std::pair<std::string, size_t> font) {

		// look for TTF in CWD
		ofDirectory ttf_dir(cwd);
		ttf_dir.listDir();
		ttf_dir.allowExt("ttf");
		ttf_dir.allowExt("otf");

		for (const auto & ttf_file : ttf_dir.getFiles()) {
			if (ttf_file.getBaseName() == font.first) {
				fonts_[font]->load(ttf_file, int(font.second));
				return 1;
			}
		}

#if 1
		// recursively scan cwd until found
		ofDirectory subdirs(cwd);
		subdirs.listDir();
		for (const auto & subdir : subdirs.getFiles()) {
			if (subdir.isDirectory()) {

				// maybe a proper programmer would check for infinite recursion? (funny symlinks etc.)
				if (scan_dir_for_font(subdir.getAbsolutePath(), font)) {
					return 1;
				}
			}
		}
#endif

		return 0;
	}

	std::map<std::pair<string, int>, std::shared_ptr<ofTrueTypeFontPlus>> fonts_;

	//int indexAlignMode = 0;
	//enum alignMode {
	//	ALIGN_LEFT = 0,
	//	ALIGN_CENTER,
	//	ALIGN_RIGHT,
	//};

private:
	float wmax = 0;
	float hmax = 0;

public:
	ofParameter<bool> bDraw { "Draw Previews", true };
	ofParameter<int> indexFont;
	//ofParameter<int> indexFont { "index", -1, -1, -1 };
	//ofParameterGroup parameters;

#if 0
	ofEventListener listenerSizeFont; //get class internal index changed
	ofParameter<float> sizeFont { "Size", 12.f, 4, 30 };
#else
	float sizeFont = 12;
#endif
	string path = "";
};

#endif // OFXFONTBOOK_HPP

#include "ofxSurfing3dText.h"
//--------------------------------------------------------------
ofxSurfing3dText::ofxSurfing3dText() {
	ofAddListener(ofEvents().update, this, &ofxSurfing3dText::update);
	ofAddListener(ofEvents().draw, this, &ofxSurfing3dText::draw);
	ofAddListener(ofEvents().keyPressed, this, &ofxSurfing3dText::keyPressed);

	setup();
}

//--------------------------------------------------------------
ofxSurfing3dText::~ofxSurfing3dText() {
	ofRemoveListener(ofEvents().update, this, &ofxSurfing3dText::update);
	ofRemoveListener(ofEvents().draw, this, &ofxSurfing3dText::draw);
	ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfing3dText::keyPressed);
}

//--------------------------------------------------------------
void ofxSurfing3dText::setup() {
	font.load("frabk.ttf", 164, true, true, true);

	message = "eternteinment";
	//message = "openframeworks";
	stringToMeshNodes(message, 100.0);
}

//--------------------------------------------------------------
void ofxSurfing3dText::update(ofEventArgs & args) {
	update();
}

//--------------------------------------------------------------
void ofxSurfing3dText::draw(ofEventArgs & args) {
}

//--------------------------------------------------------------
void ofxSurfing3dText::update() {
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
void ofxSurfing3dText::drawHelp() {

	ofDisableDepthTest();

	ofSetColor(184, 180, 176);
	string outString = "-- Type input string --";
	outString += "\n" + message;
	outString += "\n-----------------------";
	outString += "\nExtrusion amount (up / down): " + ofToString(extrusion, 0);
	ofDrawBitmapString(outString, 20, 20);
}

//--------------------------------------------------------------
void ofxSurfing3dText::draw() {
	drawMeshes();
	drawBounds();

#if 0 
	ofPushStyle();
	ofPushMatrix();
		float elapsedTime = ofGetElapsedTimef();

		for (auto & meshNode : meshNodes) {
			ofPushMatrix();
			ofTranslate(meshNode.node.getPosition());
			ofTranslate(0, 0, -1000 - extrusion * 2);

			ofSetColor(184, 180, 176);
			for (auto & pline : meshNode.polylines) {
				pline.draw();
			}

			float offset = 1.f - ofClamp(sinf(elapsedTime), 0, 1);

			ofTranslate(0, 0, 200 * offset);
			ofSetColor(55, 206, 49);
			for (auto & mesh : meshNode.srcMeshes) {
				mesh.draw();
			}

			ofSetColor(124, 120, 116);
			ofTranslate(0, 0, 100.0 * offset + extrusion);
			for (auto & mesh : meshNode.sideMeshes) {
				mesh.draw();
			}

			ofTranslate(0, 0, 100 * offset);
			ofSetColor(55, 206, 49);
			for (auto & mesh : meshNode.srcMeshes) {
				mesh.draw();
			}

			ofPopMatrix();
		}
	ofPopMatrix();
	ofPopStyle();
#endif
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawMeshes() {
	ofPushStyle();

	for (auto & meshNode : meshNodes) {
		meshNode.node.transformGL();
		meshNode.mesh.draw();
		meshNode.node.restoreTransformGL();
	}

	ofPopStyle();
}

//--------------------------------------------------------------
void ofxSurfing3dText::drawBounds() {
	ofPushStyle();

	ofPushMatrix();
	ofTranslate(-meshCentroid);
	ofSetColor(184, 180, 176);
	drawBounds(meshMin, meshMax, 100);
	ofPopMatrix();

	for (auto & meshNode : meshNodes) {
		meshNode.node.transformGL();
		ofSetColor(92, 237, 184);
		drawBounds(meshNode.min, meshNode.max, 30);
		meshNode.node.restoreTransformGL();
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

	//--

	//if (false) {}

	//else if (key == 'G' && !mod_ALT)
	//{
	//}
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

	meshNodes.clear();

	if (astring == "") {
		// the string does not have any characters so do not proceed
		return;
	}
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

	bool bRebuildMeshes = false;

	if (key == OF_KEY_DEL || key == 8) {
		if (message.length() > 0) {
			message.pop_back();
			bRebuildMeshes = true;
		}
	} else if (key == OF_KEY_RETURN || key == 13) {
		message += "\n";
	} else if (key == OF_KEY_UP) {
		extrusion += 10;
		bRebuildMeshes = true;
	} else if (key == OF_KEY_DOWN) {
		extrusion -= 10;
		if (extrusion < 0) {
			extrusion = 0;
		}
		bRebuildMeshes = true;
	} else if (key < 300) {
		unsigned char letter = (unsigned char)key;
		message += (letter);
		bRebuildMeshes = true;
	}

	if (bRebuildMeshes) {
		stringToMeshNodes(message, extrusion);
	}
}
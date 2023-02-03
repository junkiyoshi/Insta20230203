#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofSetLineWidth(2);
	//ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 15, true, true, true);
	this->word = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	vector<glm::vec2> location_list;
	ofColor color;
	for (int x = -400 - 15; x <= 400 - 15; x += 15) {

		for (int y = -400 + 15; y <= 400 + 15; y += 20) {

			this->location_list.push_back(glm::vec2(x + 10, y - 10));
			color.setHsb(ofRandom(255), 255, 255);
			this->color_list.push_back(color);
			this->index_list.push_back(ofRandom(this->word.size()));
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(120);

	auto noise_location = glm::vec3(ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.01), 0, 1, -200, 200), ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.01), 0, 1, -200, 200), 30);
	ofSetColor(0);
	ofDrawSphere(noise_location - glm::vec3(0, 0, 30), 10);
	ofColor color;
	for (int i = 0; i < this->location_list.size(); i++) {

		auto distance = glm::distance(noise_location, glm::vec3(this->location_list[i], 0));
		auto deg = glm::vec3();
		auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));
		if (distance < 150) {

			auto gap = glm::vec3(this->location_list[i], 0) - noise_location;
			auto location = noise_location + glm::normalize(gap) * 150;

			deg.x = ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.02), 0, 1, -180, 180);
			deg.y = ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.02), 0, 1, -180, 180);
			deg.z = ofMap(ofNoise(noise_seed.z, ofGetFrameNum() * 0.02), 0, 1, -180, 180);

			ofPushMatrix();
			ofTranslate(location);
			ofRotateZ(deg.z);
			ofRotateY(deg.y);
			ofRotateX(deg.x);

			this->index_list[i] = (this->index_list[i] + 1) % this->word.size();

		}
		else {

			ofPushMatrix();
			ofTranslate(this->location_list[i]);
			ofRotateZ(deg.z);
			ofRotateY(deg.y);
			ofRotateX(deg.x);

		}

		ofPath chara_path = this->font.getCharacterAsPoints(this->word[this->index_list[i]], true, false);
		vector<ofPolyline> outline = chara_path.getOutline();


		ofFill();
		ofSetColor(this->color_list[i], 128);
		ofBeginShape();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			ofNextContour(true);

			auto vertices = outline[outline_index].getVertices();
			for (auto& vertex : vertices) {

				ofVertex(vertex + glm::vec2(-6, 8));
			}
		}
		ofEndShape(true);

		ofNoFill();
		ofSetColor(this->color_list[i]);
		ofBeginShape();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			ofNextContour(true);

			auto vertices = outline[outline_index].getVertices();
			for (auto& vertex : vertices) {

				ofVertex(vertex + glm::vec2(-6, 8));
			}
		}
		ofEndShape(true);

		ofPopMatrix();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
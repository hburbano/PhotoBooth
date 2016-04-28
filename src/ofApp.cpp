#include "ofApp.h"

void ofApp::setup() {
	
	// Check Lenovo AIO res ?
	// 480p -> 640 * 480
	// 720p -> 1280 * 720
	// 1080p -> 1920 * 1080
	camWidth = 640;
	camHeight = 480;
	gridSpace = 5;
	
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth, camHeight);

	ofSetVerticalSync(true);

	videoComposition.allocate(camWidth, camHeight, OF_PIXELS_RGB);
	videoTexture.allocate(videoComposition);

	gui.setup();
	gui.add(resetBackground.set("Reset (X)", false));
	gui.add(switchBackground.set("Switch (Z)", true));
	//gui.add(learningTime.set("Learning Time", 5, 0, 30));
	gui.add(thresholdValue.set("Threshold (+/-)", 30, 0, 255));
}

//--------------------------------------------------------------
void ofApp::update() {
	ofBackground(100, 100, 100);
	vidGrabber.update();
	if (resetBackground) {
		background.reset();
		resetBackground = false;
	}
	if (vidGrabber.isFrameNew()) {
		//background.setLearningTime(learningTime);
		background.setThresholdValue(thresholdValue);
		background.update(vidGrabber, thresholded);
		thresholded.update();
		
		ofPixels &viPixels = vidGrabber.getPixels();
		ofPixels &thPixels = thresholded.getPixels();
		
		for (int i = 0; i < thPixels.size(); i++) {
			if (thPixels[i] < 10) {
				videoComposition[(i * 3) + 0] = 100;
				videoComposition[(i * 3) + 1] = 255;
				videoComposition[(i * 3) + 2] = 100;
			}
			else {
				videoComposition[(i * 3) + 0] = viPixels[(i * 3) + 0];
				videoComposition[(i * 3) + 1] = viPixels[(i * 3) + 1];
				videoComposition[(i * 3) + 2] = viPixels[(i * 3) + 2];
			}
		}
		videoTexture.loadData(videoComposition);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetHexColor(0xffffff);
	vidGrabber.draw(gridSpace, gridSpace);
	if (thresholded.isAllocated()) {
		if (switchBackground) {
			videoTexture.draw(gridSpace + camWidth, gridSpace, camWidth, camHeight);
		}else{
			thresholded.draw(gridSpace + camWidth, gridSpace, camWidth, camHeight);
		}
	}
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 's':
		vidGrabber.videoSettings();
	case 'S':
		break;
	case 'x':
		resetBackground = true;
	case 'X':
		break;
	case 'z':
		switchBackground = !switchBackground;
	case 'Z':
		break;
	case '+':
		thresholdValue++;
		if (thresholdValue > 255) thresholdValue = 255;
		break;
	case '-':
		thresholdValue--;
		if (thresholdValue < 0) thresholdValue = 0;
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
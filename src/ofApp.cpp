#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
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

	videoComposition.allocate(camWidth, camHeight, OF_PIXELS_RGB);
	videoTexture.allocate(videoComposition);
	ofSetVerticalSync(true);
	//Learn BG at start;
	threshold = 70;
	learnBackground = true;

}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100, 100, 100);
	vidGrabber.update();

	
	if (vidGrabber.isFrameNew()) {
		colorImg.setFromPixels(vidGrabber.getPixels());
		if (learnBackground) {
			backGroundImg.setFromPixels(vidGrabber.getPixels());
			grayBg = backGroundImg;
			learnBackground = false;
		}
		
		grayImage = colorImg;
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);
		//contourFinder.findContours(grayDiff, 20, (camWidth * camHeight) / 3, 10, true);

		ofPixels &bgPixels = backGroundImg.getPixels();
		ofPixels &viPixels = vidGrabber.getPixels();
		ofPixels &grayDiffPixels = grayDiff.getPixels();

		//ofLogNotice() << grayDiffPixels.size();
		for (int i = 0; i < grayDiffPixels.size(); i++) {
			if (grayDiffPixels[i] < 10) {
				videoComposition[(i*3)+0] = 100;
				videoComposition[(i*3)+1] = 255;
				videoComposition[(i*3)+2] = 100;
			} else {
				videoComposition[(i * 3) + 0] = viPixels[(i * 3) + 0];
				videoComposition[(i * 3) + 1] = viPixels[(i * 3) + 1];
				videoComposition[(i * 3) + 2] = viPixels[(i * 3) + 2];
			}
		}
		videoTexture.loadData(videoComposition);
	}	

	/*
	if (vidGrabber.isFrameNew()) {
		ofPixels & pixels = vidGrabber.getPixels();
		for (int i = 0; i < pixels.size(); i++) {
			videoComposition[i] = 255 - pixels[i];
		}
		videoTexture.loadData(videoComposition);
	}*/
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xffffff);
	vidGrabber.draw(gridSpace, gridSpace);
	videoTexture.draw(gridSpace + camWidth, gridSpace, camWidth, camHeight);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
		case 's':
			vidGrabber.videoSettings();
		case 'S':
			break;
		case 'x':
			learnBackground = true;
		case 'X':
			break;
		case '+':
			threshold++;
			if (threshold > 255) threshold = 255;
			ofLogNotice() <<threshold;
			break;
		case '-':
			threshold--;
			if (threshold < 0) threshold = 0;
			ofLogNotice() <<threshold;
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

#include "ofApp.h"



void ofApp::setup() {

	// Check Lenovo AIO res ?
	// 480p -> 854//640 * 480
	// 720p -> 1280 * 720
	// 1080p -> 1920 * 1080
	camWidth = 854;
	camHeight = 480;
	gridSpace = 0;

	lowmargin = 10;
	buttonSize = 50;
	spacing = buttonSize * 7;

	//Printer config

	printerName = "PDFCreator";

	//Config Buttons
	picButton.setImage("buttons/camera.png");
	picButton.set((camWidth - buttonSize)/2, camHeight - buttonSize - lowmargin, buttonSize, buttonSize);
	nextButton.setImage("buttons/next.png");
	nextButton.set(camWidth - (buttonSize * 2), (camHeight - buttonSize) / 2, buttonSize, buttonSize);
	prevButton.setImage("buttons/prev.png");
	prevButton.set(buttonSize, (camHeight - buttonSize) / 2, buttonSize, buttonSize);
	backButton.setImage("buttons/back.png");
	backButton.set((camWidth - buttonSize - spacing) / 2, camHeight - buttonSize - lowmargin, buttonSize, buttonSize);
	facebookButton.setImage("buttons/facebook.jpg");
	facebookButton.set((camWidth - buttonSize) / 2, camHeight - buttonSize - lowmargin, buttonSize, buttonSize);
	printButton.setImage("buttons/printer.png");
	printButton.set((camWidth - buttonSize + spacing) / 2, camHeight - buttonSize - lowmargin, buttonSize, buttonSize);
	backButton.enabled = false;
	facebookButton.enabled = false;
	printButton.enabled = false;
	currentState = 1;
	
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth, camHeight);

	ofSetVerticalSync(true);

	videoComposition.allocate(camWidth, camHeight, OF_PIXELS_RGB);
	videoTexture.allocate(videoComposition);

	//Config GUI

	gui.setup();
	gui.add(resetBackground.set("Reset (X)", false));
	gui.add(switchBackground.set("Switch (Z)", true));
	gui.add(snapPhoto.set("Photo (P)", false));
	//gui.add(learningTime.set("Learning Time", 5, 0, 30));
	gui.add(thresholdValue.set("Threshold (+/-)", 30, 0, 255));

	//Background options
	currentBackground = 0;
	backgroundList.push_back("backgrounds/back01.jpg");
	backgroundList.push_back("backgrounds/back02.jpg");
	backgroundList.push_back("backgrounds/back03.jpg");
	photoBackground.load(backgroundList[currentBackground]);
	photoBackground.resize(camWidth, camHeight);	

}

void ofApp::snap() {
	imgComp.setFromPixels(videoComposition);
	fileName = "snap_" + ofToString(now.elapsed()) + ".png";
	//Clear for final
	ofImage temp;
	temp.setFromPixels(imgComp.getPixelsRef());
	temp.resize(camWidth*0.75,camHeight*0.75);
	temp.save(fileName);
}

void ofApp::imageUploadUpdate() {
	
}

void ofApp::imageUploadDraw() {
	imgComp.draw(0, 0);
}

void ofApp::videoCaptureUpdate() {
	vidGrabber.update();
	if (updateBackground) {
		photoBackground.load(backgroundList[currentBackground]);
		photoBackground.resize(camWidth, camHeight);
		updateBackground = false;
	}

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
		ofPixels &phPixels = photoBackground.getPixels();

		for (int i = 0; i < thPixels.size(); i++) {
			if (thPixels[i] < 10) {
				videoComposition[(i * 3) + 0] = phPixels[(i * 3) + 0];
				videoComposition[(i * 3) + 1] = phPixels[(i * 3) + 1];
				videoComposition[(i * 3) + 2] = phPixels[(i * 3) + 2];
			}
			else {
				videoComposition[(i * 3) + 0] = viPixels[(i * 3) + 0];
				videoComposition[(i * 3) + 1] = viPixels[(i * 3) + 1];
				videoComposition[(i * 3) + 2] = viPixels[(i * 3) + 2];
			}
		}
		videoTexture.loadData(videoComposition);
	}

	if (snapPhoto == true) {
		snap();
		snapPhoto = false;
	}
}
void ofApp::videoCaptureDraw() {
	
	ofSetHexColor(0xffffff);
	//vidGrabber.draw(gridSpace, gridSpace);
	if (thresholded.isAllocated()) {
		if (switchBackground) {
			videoTexture.draw(gridSpace, gridSpace, camWidth, camHeight);
		}
		else {
			thresholded.draw(gridSpace, gridSpace, camWidth, camHeight);
		}
	}
	gui.draw();
}
//--------------------------------------------------------------
void ofApp::update() {
	ofBackground(100, 100, 100);

	switch (currentState)
	{
	case ofApp::imageCapture:
		videoCaptureUpdate();
		break;
	case ofApp::imageUpload:
		imageUploadUpdate();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	switch (currentState)
	{
	case ofApp::imageCapture:
		videoCaptureDraw();
		break;
	case ofApp::imageUpload:
		imageUploadDraw();
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'q':
		currentBackground--;
		if (currentBackground < 0) {
			currentBackground = backgroundList.size()-1;
		}
		updateBackground = true;
	case 'Q':
		break;
	case 'w':
		currentBackground++;
		if (currentBackground >= backgroundList.size()) {
			currentBackground = 0;
		}
		updateBackground = true;
	case 'W':
		break;
	case 's':
		vidGrabber.videoSettings();
	case 'S':
		break;
	case 'p':
		snapPhoto = true;
	case 'P':
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
	bool backPressed = backButton.isMouseDown();
	bool picPressed = picButton.isMouseDown();
	bool prvPressed = prevButton.isMouseDown();
	bool nxtPressed = nextButton.isMouseDown();
	bool prtPressed = printButton.isMouseDown();

	if (prtPressed) {
		string path = ofToDataPath(fileName, true);
		//Fix error in ofToDataPath specific for Windows 10
		path.insert(2,"/");
		path.replace(2,2,"\\");
		string printcmd = "i_view64.exe " + path + " /print=\""+printerName+"\"";
		system(printcmd.c_str());
	}

	if (prvPressed)
	{
		currentBackground--;
		if (currentBackground < 0) {
			currentBackground = backgroundList.size() - 1;
		}
		updateBackground = true;
	}
	if (nxtPressed)
	{
		currentBackground++;
		if (currentBackground >= backgroundList.size()) {
			currentBackground = 0;
		}
		updateBackground = true;
	}
	if (picPressed) {
		currentState = 2;
		picButton.enabled = false;
		nextButton.enabled = false;
		prevButton.enabled = false;
		snap();
		backButton.enabled = true;
		backButton.enableAllEvents();
		facebookButton.enabled = true;
		facebookButton.enableAllEvents();
		printButton.enabled = true;
		printButton.enableAllEvents();
	}
	if (backPressed) {
		currentState = 1;
		picButton.enabled = true;
		picButton.enableAllEvents();
		nextButton.enabled = true;
		nextButton.enableAllEvents();
		prevButton.enabled = true;
		prevButton.enableAllEvents();

		backButton.enabled = false;
		facebookButton.enabled = false;
		printButton.enabled = false;
	}
	
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
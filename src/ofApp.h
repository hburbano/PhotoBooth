#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofUtils.h"
#include "ImageButton.h"
#include "Poco/Timestamp.h"
#include <vector>

using namespace ofxCv;
using namespace cv;
using Poco::Timestamp;

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();


	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void snap();

	ofVideoGrabber vidGrabber;
	ofPixels videoComposition;
	ofTexture videoTexture;
	RunningBackground background;
	ofImage thresholded;

	//Buttons

	ImageButton picButton;
	ImageButton nextButton;
	ImageButton prevButton;
	ImageButton backButton;
	ImageButton facebookButton;
	ImageButton printButton;
	int buttonSize;
	int spacing;
	int lowmargin;

	//Sate
	enum states { imageCapture = 1,  imageUpload= 2};
	int currentState;
	void videoCaptureUpdate();
	void videoCaptureDraw();
	void imageUploadUpdate();
	void imageUploadDraw();

	//Backgrounds
	ofImage photoBackground;
	std::vector<std::string> backgroundList;
	int currentBackground;
	bool updateBackground = true;


	//Capture snapshot
	ofImage imgComp;
	Timestamp now;

	//Print opcionts
	string fileName;
	string printerName;

	int camWidth;
	int camHeight;
	int gridSpace;

	ofxPanel gui;
	ofParameter<bool> resetBackground;
	ofParameter<bool> switchBackground;
	//ofParameter<float> learningTime; //Probar background learning en cabina...---
	ofParameter<float> thresholdValue;
	ofParameter<bool> snapPhoto;
	//ofParameter<enum> cas;


};

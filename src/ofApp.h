#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofVideoGrabber vidGrabber;
		ofPixels videoComposition;
		ofTexture videoTexture;
		int camWidth;
		int camHeight;
		int gridSpace;

		//Background learning
		bool learnBackground;
		int threshold;
		ofxCvColorImage colorImg;
		ofxCvColorImage backGroundImg;
		ofxCvGrayscaleImage grayImage;
		ofxCvGrayscaleImage grayBg;
		ofxCvGrayscaleImage grayDiff;
		ofxCvContourFinder contourFinder;

};

#pragma once

#include "ofxMSAInteractiveObject.h"

#define		IDLE_COLOR		0xFFFFFF
#define		OVER_COLOR		0x888888
#define		DOWN_COLOR		0x222222

class ImageButton : public ofxMSAInteractiveObject {
public:

	ofImage icon;
	void setup() {
		enableMouseEvents();
	}

	void setImage(string image) {
		icon.load(image);
	}

	void exit() {
	}


	void update() {
	}


	void draw() {
		if (isMouseDown()) ofSetHexColor(DOWN_COLOR);
		else if (isMouseOver()) ofSetHexColor(OVER_COLOR);
		else ofSetHexColor(IDLE_COLOR);
		icon.draw(x, y, width, height);
	}

	virtual void onRollOver(int x, int y) {		
	}

	virtual void onRollOut() {
	}

	virtual void onMouseMove(int x, int y) {
	}

	virtual void onDragOver(int x, int y, int button) {
	}

	virtual void onDragOutside(int x, int y, int button) {
	}

	virtual void onPress(int x, int y, int button) {
	}

	virtual void onRelease(int x, int y, int button) {
	}

	virtual void onReleaseOutside(int x, int y, int button) {
	}

};
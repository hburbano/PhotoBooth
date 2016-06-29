#include "ofApp.h"



void ofApp::setup() {
	FreeConsole();
	// Check Lenovo AIO res ?
	// 480p -> 854//640 * 480
	// 720p -> 1280 * 720
	// 1080p -> 1920 * 1080
	camWidth = 1280;
	camHeight = 720;
	gridSpace = 0;

	margin = 10;
	buttonSize = 50;
	spacing = buttonSize * 7;

	//Email config
	senderEmail = "pbcasamem tumaco <pbcasamentco@gmail.com>";
	ofx::SMTP::Settings settings = ofx::SMTP::Settings::loadFromXML("email.xml");
	smtp.setup(settings);
	ofAddListener(smtp.events.onSMTPDelivery, this, &ofApp::onSMTPDelivery);
	ofAddListener(smtp.events.onSMTPException, this, &ofApp::onSMTPException);

	//Config Buttons
	picButton.setImage("buttons/camera.png");
	picButton.set((ofGetWidth() - buttonSize)/2, ofGetHeight() - buttonSize - margin, buttonSize, buttonSize);
	nextButton.setImage("buttons/next.png");
	nextButton.set(ofGetWidth() - (buttonSize * 2), (ofGetHeight() - buttonSize) / 2, buttonSize, buttonSize);
	prevButton.setImage("buttons/prev.png");
	prevButton.set(buttonSize, (ofGetHeight() - buttonSize) / 2, buttonSize, buttonSize);
	backButton.setImage("buttons/back.png");
	backButton.set((ofGetWidth() - buttonSize - spacing) / 2, ofGetHeight() - buttonSize - margin, buttonSize, buttonSize);
	facebookButton.setImage("buttons/facebook.jpg");
	facebookButton.set((ofGetWidth() - buttonSize) / 2, ofGetHeight() - buttonSize - margin, buttonSize, buttonSize);
	printButton.setImage("buttons/printer.png");
	printButton.set((ofGetWidth() - buttonSize + spacing) / 2, ofGetHeight() - buttonSize - margin, buttonSize, buttonSize);
	closeButton.setImage("buttons/close.png");
	closeButton.set((ofGetWidth() - buttonSize - margin), margin, buttonSize, buttonSize);

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
	logo.loadImage("backgrounds/logo.jpg");
	int newWidth = ofGetWidth() * 0.15;
	int newHeigth = logo.getHeight() * (newWidth / logo.getWidth() );
	logo.resize(newWidth, newHeigth);
	lema.loadImage("backgrounds/lema.jpg");

	//Config GUI

	gui.setup();
	gui.setSize(400, 200);
	gui.add(resetBackground.set("Reset (X)", false));
	gui.add(switchBackground.set("Switch (Z)", true));
	gui.add(snapPhoto.set("Photo (P)", false));
	//gui.add(learningTime.set("Learning Time", 5, 0, 30));
	gui.add(thresholdValue.set("Thresh (E R)", 30, 0, 255));
	gui.add(showGUI.set("Config (C)", false));
	gui.add(pname.set("Printer", "PDFCreator"));
	gui.add(email.set("Email", "nu8guvofzqstr@tumblr.com"));
	//gui.saveToFile("settings.xml");
	gui.loadFromFile("settings.xml");

	
	//Background options
	currentBackground = 0;
	backgroundList.push_back("backgrounds/back01.jpg");
	backgroundList.push_back("backgrounds/back02.jpg");
	backgroundList.push_back("backgrounds/back03.jpg");
	backgroundList.push_back("backgrounds/back04.jpg");
	backgroundList.push_back("backgrounds/back05.jpg");
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

void ofApp::imageUploadDraw() {
	imgComp.draw(gridSpace, gridSpace, ofGetWidth(), ofGetHeight());
}

void ofApp::imageUploadUpdate() {
	//Do nothig
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
		ofPixels &lgPixels = logo.getPixels();
		ofPixels &lmPixels = lema.getPixels();

		for (int i = 0; i < thPixels.size(); i++) {
			if(thPixels[i] < 10) {
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
		lgPixels.pasteInto(videoComposition, 15, 15);
		lmPixels.pasteInto(videoComposition, videoComposition.getWidth() / 2 - lmPixels.getWidth() / 2, videoComposition.getHeight() - lmPixels.getHeight() - 15);
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
			videoTexture.draw(gridSpace, gridSpace, ofGetWidth(), ofGetHeight());
		}
		else {
			thresholded.draw(gridSpace, gridSpace, ofGetWidth(), ofGetHeight());
		}
	}

	if (showGUI) {
		gui.draw();
	}
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

void ofApp::sendEmail() {
	ofx::SMTP::Message::SharedPtr message = ofx::SMTP::Message::makeShared();
	message->setSender(Poco::Net::MailMessage::encodeWord(senderEmail, "UTF-8"));
	message->setSubject(Poco::Net::MailMessage::encodeWord("Constructor de PAZ", "UTF-8"));
	message->addRecipient(Poco::Net::MailRecipient(Poco::Net::MailRecipient::PRIMARY_RECIPIENT,
		email.toString()));
	string msgEmail = Poco::DateTimeFormatter::format(now,
		"Construyendo PAZ, %Y-%n-%e",
		-5000);
	message->addContent(new Poco::Net::StringPartSource(msgEmail));
	try
	{
		string path = ofToDataPath(fileName, true);
		//Fix error in ofToDataPath specific for Windows 10
		path.insert(2, "/");
		path.replace(2, 2, "\\");
		message->addAttachment(Poco::Net::MailMessage::encodeWord(fileName, "UTF-8"),
			new Poco::Net::FilePartSource(path, "image/png"));
	}
	catch (const Poco::OpenFileException& exc)
	{
		ofLogError("ofApp::keyPressed") << exc.name() << " : " << exc.displayText();
	}
	smtp.send(message);
}

void ofApp::saveSettings() {
	gui.saveToFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	
	case 'c':
	case 'C':
		showGUI = !showGUI;
		saveSettings();
		break;
	case 'q':
	case 'Q':
		currentBackground--;
		if (currentBackground < 0) {
			currentBackground = backgroundList.size()-1;
		}
		updateBackground = true;
		saveSettings();
		break;
	case 'w':
	case 'W':
		currentBackground++;
		if (currentBackground >= backgroundList.size()) {
			currentBackground = 0;
		}
		updateBackground = true;
		saveSettings();
		break;
	case 'p':
	case 'P':
		snapPhoto = true;
		break;
	case 'x':
	case 'X':
		resetBackground = true;
		break;
	case 'z':
	case 'Z':
		switchBackground = !switchBackground;
		break;
	case 'e':
	case 'E':
		thresholdValue++;
		if (thresholdValue > 255) thresholdValue = 255;
		saveSettings();
		break;
	case 'r':
	case 'R':
		thresholdValue--;
		if (thresholdValue < 0) thresholdValue = 0;
		saveSettings();
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
	bool fbkPressed = facebookButton.isMouseDown();
	bool clsPressed = closeButton.isMouseDown();
	
	if (clsPressed) {
		std::exit(1);
	}

	if (fbkPressed) {
		sendEmail();
	}

	if (prtPressed) {
		string path = ofToDataPath(fileName, true);
		//Fix error in ofToDataPath specific for Windows 10
		path.insert(2,"/");
		path.replace(2,2,"\\");
		string printcmd = "i_view64.exe " + path + " /print=\""+pname.toString()+"\"";
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



void ofApp::onSMTPDelivery(ofx::SMTP::Message::SharedPtr& message)
{
	ofLogNotice("ofApp::onSMTPDelivery") << "Message Sent: " << message->getSubject();
}


void ofApp::onSMTPException(const ofx::SMTP::ErrorArgs& evt)
{
	ofLogError("ofApp::onSMTPException") << evt.getError().displayText();

	if (evt.getMessage())
	{
		ofLogError("ofApp::onSMTPException") << evt.getMessage()->getSubject();
	}

}


void ofApp::onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args)
{
	ofLogNotice("ofApp::onClientVerificationError") << std::endl << ofToString(args);

	// If you want to proceed, you must allow the user to inspect the certificate
	// and set `args.setIgnoreError(true);` if they want to continue.

	args.setIgnoreError(true);

}

void ofApp::onSSLPrivateKeyPassphraseRequired(std::string& passphrase)
{
	// If you want to proceed, you must allow the user to input the assign the private key's
	// passphrase to the `passphrase` argument.  For example:

	//passphrase = ofSystemTextBoxDialog("Enter the Private Key Passphrase", "");

}

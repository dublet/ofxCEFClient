// Copyright (c) 2013 Intel Corporation

#include "ofxCEFClient.h"
#include "client_handler.h"
//#include "testApp.h"
#include <windows.h>

// The global ClientHandler reference (defined in client.cpp)
extern CefRefPtr<ClientHandler> myClientHandler;

ofxCEFClient::ofxCEFClient() {
	_initialized = false; 

	mLoadedTexture = false;

}

ofxCEFClient::~ofxCEFClient() {
}

void ofxCEFClient::enableEvents() {
	
	ofAddListener(ofEvents().mousePressed, this, &ofxCEFClient::_mousePressed);
	ofAddListener(ofEvents().mouseMoved, this, &ofxCEFClient::_mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &ofxCEFClient::_mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &ofxCEFClient::_mouseReleased);

	ofAddListener(ofEvents().keyPressed, this, &ofxCEFClient::_keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxCEFClient::_keyReleased);
	
	ofAddListener(ofEvents().windowResized, this, &ofxCEFClient::_windowResized);

	ofAddListener(ofEvents().exit, this, &ofxCEFClient::_exit);

}

// startupResource is presently not used, rather hard coded to an index.html below. 
void ofxCEFClient::init() {
	ClientAppInit(this); 
	_initialized = true;

}


void ofxCEFClient::createBrowser(std::string startupResource, int width, int height, string js) {
	if (!_initialized)
		init();

	this->width = (width == -1) ? ofGetWidth() : width;
	this->height = (height == -1) ? ofGetHeight() : height;
	mJavascript = js;

	ofFilePath pathUtil; 

	//string startResource = "file:\\\\" + pathUtil.getCurrentWorkingDirectory() + "interface\\index.html"; 
	if (startupResource.empty() || startupResource == "") {
		startupResource = ofFilePath::getAbsolutePath("tile-small.html", true);
	}
	startupResource = "file:\\\\" + startupResource;
	ofLogNotice() << "Using UI HTML Document: \n " << startupResource << std::endl; 

	mBrowser = ClientAppCreateBrowser(this, startupResource); 
	
}

void ofxCEFClient::loop() {
	
	// This doesn't work quite right. The idea was to set buffer, width, and height but they return junk values
	// so the ->doneLoading flag needs to work better.

	if (myClientHandler.get()->hasBrowser() == true && _initialized == false) {

		_initialized = true; 
		_browserHost = myClientHandler.get()->GetBrowser()->GetHost();
		enableEvents(); 

	} 

	if (!_initialized) {
		ofLogError() << "CEF Client Not Initialized \n";
		return; 
	}
	
	//  TOFIX: It's really not ideal to set these pointers constantly... 
	if (mLoadedTexture) {

		mLoadedTexture = false; 
		
		buffer = (unsigned char *) myClientHandler.get()->buffer; 
		//width = myClientHandler.get()->width; 
		//height = myClientHandler.get()->height; 

		_cef_buffer.loadData(buffer, width, height, GL_BGRA);

	}
#if 0
	testApp* myApp = (testApp*)ofGetAppPtr();
	CefRefPtr<CefProcessMessage> newMsg; 
	if (jsEventQueue.fetch(newMsg)) 
		myApp->cefMessageCallback(newMsg);
#endif
}

void ofxCEFClient::loadedTexture() {
	 _buffer.setFromPixels((unsigned char *)myClientHandler.get()->buffer, width, height, 4);
	 mLoadedTexture = true; 
}


void ofxCEFClient::loadTex(ofTexture * texture, ofPixels &pixels) {
	while (!mLoadedTexture) {
		CefDoMessageLoopWork();
	}

	pixels.swap(_buffer);
	texture->loadData(pixels); 
	myClientHandler.get()->buffer = NULL;

	mLoadedTexture = false;
	_buffer.clear();
}


void ofxCEFClient::paint() {

	_cef_buffer.draw(0, 0, width, height); 

}

//--------------------------------------------------------------
void ofxCEFClient::_mouseMoved(ofMouseEventArgs &e) {

	int x = e.x;
	int y = e.y;
	int button = e.button;

	CefMouseEvent mouse_event;
	mouse_event.x = x;
	mouse_event.y = y;

	_browserHost->SendMouseMoveEvent(mouse_event, false); 

}

void ofxCEFClient::_mousePressed(ofMouseEventArgs &e) {

	int x = e.x;
	int y = e.y;
	int button = e.button;

	CefBrowserHost::MouseButtonType btnType = (button == 0 ? MBT_LEFT : ( button == 2 ? MBT_RIGHT : MBT_MIDDLE));

	CefMouseEvent mouse_event;
	mouse_event.x = x;
	mouse_event.y = y;

	// Double click vs single click: 
	int clickCount = 1; 

	unsigned long currClick = ofGetElapsedTimeMillis();

	if(lastClick != 0 && currClick - lastClick < 333) {
		clickCount = 2; 
	}

    lastClick = currClick;

	// Mouse Down
	_browserHost->SendMouseClickEvent(mouse_event, btnType, false, clickCount); 
															
}

void ofxCEFClient::_mouseDragged(ofMouseEventArgs &e) {

	int x = e.x;
	int y = e.y;
	int button = e.button;

	CefMouseEvent mouse_event;
	mouse_event.x = x;
	mouse_event.y = y;

	_browserHost->SendMouseMoveEvent(mouse_event, false); 

}

void ofxCEFClient::_mouseReleased(ofMouseEventArgs &e) {

	int x = e.x;
	int y = e.y;
	int button = e.button;

	CefBrowserHost::MouseButtonType btnType = (button == 0 ? MBT_LEFT : ( button == 2 ? MBT_RIGHT : MBT_MIDDLE));

	CefMouseEvent mouse_event;
	mouse_event.x = x;
	mouse_event.y = y;

	// Mouse Up
	_browserHost->SendMouseClickEvent(mouse_event, btnType, true, 1); 

}

void ofxCEFClient::_keyPressed(ofKeyEventArgs &e) {

	int key = e.key;

	CefKeyEvent event;
	event.windows_key_code = key;
	event.native_key_code = key;
	event.type = KEYEVENT_CHAR;

	_browserHost->SendKeyEvent(event);

}

void ofxCEFClient::_keyReleased(ofKeyEventArgs &e) {

	int key = e.key;	

	CefKeyEvent event;

	// Not sure about the different key_codes here. Needs research.
	event.windows_key_code = key; 
	event.native_key_code = key;
	event.type = KEYEVENT_KEYUP;
	 
	_browserHost->SendKeyEvent(event);

}

void ofxCEFClient::_windowResized(ofResizeEventArgs &e) {

	// BUGBUG: Should trigger a render to the browser
	// _cef_buffer.clear();
	// _cef_buffer.allocate(e.width, e.height, GL_RGBA); 
	//_browserHost->WasResized();

}

void ofxCEFClient::_exit(ofEventArgs &e) {

	myClientHandler->CloseAllBrowsers(true);

	// Most clean to use CefShutdown(), but seems to throw some nasty hard-to-debug error for the time being. 
	// I mean, we're closing the application anyway... 
	// CefShutdown();

}

void ofxCEFClient::sendMessage(std::string name, CefRefPtr <CefListValue> message) {

	CefRefPtr <CefProcessMessage> response = CefProcessMessage::Create(name);

	response->GetArgumentList()->SetList(0, message); 

	_browserHost->GetBrowser()->SendProcessMessage(PID_RENDERER, response);

}

void ofxCEFClient::messageCallback(CefRefPtr<CefProcessMessage> message) {

	jsEventQueue.post(message); 

}

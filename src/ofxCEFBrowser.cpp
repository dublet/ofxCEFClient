// Copyright (c) 2013 Intel Corporation

#include "ofxCEFBrowser.h"

#include "client_handler.h"

// The global ClientHandler reference (defined in client.cpp)
extern CefRefPtr<ClientHandler> myClientHandler;


ofxCEFBrowser::ofxCEFBrowser() 
: mWidth(-1), mHeight(-1), mPixels(NULL), mTexture(NULL) {
}

ofxCEFBrowser::ofxCEFBrowser(int width, int height, string js)
: mPixels(NULL), mTexture(NULL), mJavascript(js) {
	mWidth = (width == -1) ? ofGetWidth() : width;
	mHeight = (height == -1) ? ofGetHeight() : height;
}

void ofxCEFBrowser::browseTo(std::string startupResource) {
	//string startResource = "file:\\\\" + pathUtil.getCurrentWorkingDirectory() + "interface\\index.html"; 
	if (startupResource.empty() || startupResource == "") {
		startupResource = ofFilePath::getAbsolutePath("tile-small.html", true);
	}
	if (startupResource.compare(0, 7, "http://") != 0) {
		startupResource = "file:\\\\" + startupResource;
		ofLogNotice() << "Using UI HTML Document: \n " << startupResource << std::endl; 
	} else {
		ofLogNotice() << "Using URL: \n " << startupResource << std::endl; 
	}

	mBrowser = ClientAppCreateBrowser(shared_from_this(), startupResource); 
	assert(mBrowser);
	_browserHost = mBrowser->GetHost();
}

ofxCEFBrowser::~ofxCEFBrowser() {
}


void ofxCEFBrowser::enableEvents() {
	
	ofAddListener(ofEvents().mousePressed, this, &ofxCEFBrowser::_mousePressed);
	ofAddListener(ofEvents().mouseMoved, this, &ofxCEFBrowser::_mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &ofxCEFBrowser::_mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &ofxCEFBrowser::_mouseReleased);

	ofAddListener(ofEvents().keyPressed, this, &ofxCEFBrowser::_keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &ofxCEFBrowser::_keyReleased);
	
	ofAddListener(ofEvents().windowResized, this, &ofxCEFBrowser::_windowResized);
}

void ofxCEFBrowser::close() {
	myClientHandler->DoClose(mBrowser);
}

void ofxCEFBrowser::loadedTexture(const void *buffer) {
	if (mLoadIntoPixels) {
		assert(mPixels);
	
		mPixels->setFromPixels((unsigned char *)buffer, mWidth, mHeight, 4);
	} else {
		assert(mTexture);
	
		// Load data will allocate but somehow allocating with GL_BGRA results in a 
		// all white texture.
		if (!mTexture->isAllocated()) 
			mTexture->allocate(mWidth, mHeight, GL_RGBA);
		else {
			assert(mTexture->getHeight() == mHeight);
			assert(mTexture->getWidth() == mWidth);
		}
		mTexture->loadData((unsigned char *)buffer, mWidth, mHeight, GL_BGRA);
	}
	mLoadedTexture = true;
}


void ofxCEFBrowser::loadTex(ofPixels * texture) {
	assert(mBrowser);
	if (!mBrowser)
		return;

	mPixels = texture;
	mLoadIntoPixels = true;

	while (mBrowser->IsLoading() || !mLoadedTexture || !mPixels->isAllocated()) {
		CefDoMessageLoopWork();
	}

	myClientHandler->DoClose(mBrowser);
}

void ofxCEFBrowser::loadTex(ofTexture * texture) {
	mTexture = texture;
	mLoadIntoPixels = false;

	while (mBrowser->IsLoading() || !mLoadedTexture || !mTexture->isAllocated()) {
		CefDoMessageLoopWork();
	}

	myClientHandler->DoClose(mBrowser);
}


void ofxCEFBrowser::scroll(int deltaX, int deltaY) {
	if (!_browserHost)
		return;
	CefMouseEvent mouse_event;
	mouse_event.x = 0;
	mouse_event.y = 0;
	_browserHost->SendMouseWheelEvent(mouse_event, deltaX, deltaY);
}

//--------------------------------------------------------------
void ofxCEFBrowser::_mouseMoved(ofMouseEventArgs &e) {

	int x = e.x;
	int y = e.y;
	int button = e.button;

	CefMouseEvent mouse_event;
	mouse_event.x = x;
	mouse_event.y = y;

	_browserHost->SendMouseMoveEvent(mouse_event, false); 

}

void ofxCEFBrowser::_mousePressed(ofMouseEventArgs &e) {

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

void ofxCEFBrowser::_mouseDragged(ofMouseEventArgs &e) {

	int x = e.x;
	int y = e.y;
	int button = e.button;

	CefMouseEvent mouse_event;
	mouse_event.x = x;
	mouse_event.y = y;

	_browserHost->SendMouseMoveEvent(mouse_event, false); 

}

void ofxCEFBrowser::_mouseReleased(ofMouseEventArgs &e) {

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

void ofxCEFBrowser::_keyPressed(ofKeyEventArgs &e) {

	int key = e.key;

	CefKeyEvent event;
	event.windows_key_code = key;
	event.native_key_code = key;
	event.type = KEYEVENT_CHAR;

	_browserHost->SendKeyEvent(event);

}

void ofxCEFBrowser::_keyReleased(ofKeyEventArgs &e) {

	int key = e.key;	

	CefKeyEvent event;

	// Not sure about the different key_codes here. Needs research.
	event.windows_key_code = key; 
	event.native_key_code = key;
	event.type = KEYEVENT_KEYUP;
	 
	_browserHost->SendKeyEvent(event);

}

void ofxCEFBrowser::_windowResized(ofResizeEventArgs &e) {

	// BUGBUG: Should trigger a render to the browser
	// _cef_buffer.clear();
	// _cef_buffer.allocate(e.width, e.height, GL_RGBA); 
	//_browserHost->WasResized();

}

void ofxCEFBrowser::processEvent() {
	CefDoMessageLoopWork();
}


void ofxCEFBrowser::sendMessage(std::string name, CefRefPtr <CefListValue> message) {
	CefRefPtr <CefProcessMessage> response = CefProcessMessage::Create(name);

	response->GetArgumentList()->SetList(0, message); 

	mBrowser->SendProcessMessage(PID_RENDERER, response);
}

void ofxCEFBrowser::messageCallback(CefRefPtr<CefProcessMessage> message) {

	jsEventQueue.post(message); 

}

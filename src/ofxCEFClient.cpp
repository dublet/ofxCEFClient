// Copyright (c) 2013 Intel Corporation

#include "ofxCEFClient.h"
#include "client_handler.h"
//#include "testApp.h"
#include <windows.h>

extern CefRefPtr<ClientApp> sClientApp;

ofxCEFClient *ofxCEFClient::mInstance = NULL;

ofxCEFClient::ofxCEFClient() {
	_initialized = false; 

	ofAddListener(ofEvents().exit, this, &ofxCEFClient::_exit);
}

ofxCEFClient::~ofxCEFClient() {
}

 ofxCEFClient &ofxCEFClient::getInstance() {
	 if (!mInstance)
		 mInstance = new ofxCEFClient();
	 return *mInstance;
}


// startupResource is presently not used, rather hard coded to an index.html below. 
void ofxCEFClient::init() {
	if (!_initialized) {
		ClientAppInit(); 
		_initialized = true;
	}
}


void ofxCEFClient::processEvent() {
	CefDoMessageLoopWork();
}

std::shared_ptr<ofxCEFBrowser> ofxCEFClient::createBrowser(int width, int height, string js) {
	assert (_initialized);

	return std::shared_ptr<ofxCEFBrowser>(new ofxCEFBrowser(width, height, js));
}

void ofxCEFClient::_exit(ofEventArgs &e) {

	shutdown();
	// Most clean to use CefShutdown(), but seems to throw some nasty hard-to-debug error for the time being. 
	// I mean, we're closing the application anyway... 
	// CefShutdown();

}


void ofxCEFClient::shutdown() {
	for (auto clientHandler : sClientApp->getClientHandlers()) {
		clientHandler->CloseAllBrowsers(true);
	}
	for (int i = 0; i < 1000; i++)
		processEvent();
	CefShutdown();
}
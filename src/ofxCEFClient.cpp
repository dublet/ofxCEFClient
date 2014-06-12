// Copyright (c) 2013 Intel Corporation

#include "ofxCEFClient.h"
#include "client_handler.h"
//#include "testApp.h"
#include <windows.h>

// The global ClientHandler reference (defined in client.cpp)
extern CefRefPtr<ClientHandler> myClientHandler;

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

std::shared_ptr<ofxCEFBrowser> ofxCEFClient::createBrowser(std::string startupResource, int width, int height, string js) {
	assert (_initialized);

	return std::shared_ptr<ofxCEFBrowser>(new ofxCEFBrowser(startupResource, width, height, js));
}

void ofxCEFClient::_exit(ofEventArgs &e) {

	myClientHandler->CloseAllBrowsers(true);

	// Most clean to use CefShutdown(), but seems to throw some nasty hard-to-debug error for the time being. 
	// I mean, we're closing the application anyway... 
	// CefShutdown();

}
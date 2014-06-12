// Copyright (c) 2013 Intel Corporation

#pragma once

#include "ofEvents.h"

#include <memory>

class ofxCEFBrowser;

class ofxCEFClient {

private:
	static ofxCEFClient mInstance;

	ofxCEFClient();
	ofxCEFClient(const ofxCEFClient &);

	~ofxCEFClient();

public:
	static ofxCEFClient &getInstance();
	
	void init();

	std::shared_ptr<ofxCEFBrowser> createBrowser(std::string startupResource, int width = -1, int height = -1, string js = "");
private:

	void _exit(ofEventArgs &e);
	bool _initialized; 

};

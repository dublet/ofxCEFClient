// Copyright (c) 2013 Intel Corporation

#pragma once

#include "ofMain.h"
#include "client.h"

class ofxCEFClient {

public:

	ofxCEFClient();

	~ofxCEFClient();

	void init(std::string startupResource);

	void loop(); 

	void paint(); 

	void sendMessage(std::string name, CefRefPtr<CefListValue> message); 

	void messageCallback(CefRefPtr<CefProcessMessage> message); 

private:

	void enableEvents(); 

	void _mouseMoved(ofMouseEventArgs &e);
	void _mousePressed(ofMouseEventArgs &e);
	void _mouseDragged(ofMouseEventArgs &e);
	void _mouseReleased(ofMouseEventArgs &e);

	void _keyPressed(ofKeyEventArgs &e);
	void _keyReleased(ofKeyEventArgs &e);

	void _windowResized(ofResizeEventArgs &e);

	void _exit(ofEventArgs &e);

	CefRefPtr<CefBrowserHost> _browserHost; 

	ofTexture _cef_buffer; 

	bool _initialized; 

};
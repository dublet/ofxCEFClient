// Copyright (c) 2013 Intel Corporation

#pragma once

#include "ofMain.h"
#include "client.h"
#include <thread>
#include <mutex>
#include <atomic>

#include "client\client.h"

class message_queue {

	public: 

	std::deque < CefRefPtr<CefProcessMessage> > fifo;
	std::mutex guard;            

	void post (const CefRefPtr<CefProcessMessage> &msg) {
		std::lock_guard<std::mutex> lock(guard);
		fifo.push_back(msg->Copy());
	}

	bool fetch (CefRefPtr<CefProcessMessage> &message) {

		bool hasMessage = false; 

		std::lock_guard<std::mutex> lock(guard);

		if (fifo.size()) {
			message = fifo.front();
			fifo.pop_front();
			hasMessage = true; 
		}

		return hasMessage;

	}

};

class ofxCEFClient {

public:

	ofxCEFClient();

	~ofxCEFClient();
	
	void init();

	void createBrowser(std::string startupResource, int width = -1, int height = -1, string js = "");

	void loop(); 

	void paint(); 

	void sendMessage(std::string name, CefRefPtr<CefListValue> message); 

	void messageCallback(CefRefPtr<CefProcessMessage> message); 

	
	void loadTex(ofTexture *, ofPixels &); 

	void loadedTexture();

	int getWidth() { return width; }
	int getHeight() { return height; }
	
	std::string &getJavascript() { return mJavascript; }

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
	ofPixels _buffer;

	bool _initialized; 

	message_queue jsEventQueue; 

	unsigned char *buffer; 
	int width;
	int height; 

	uint32_t lastClick; 
	std::atomic<bool> mLoadedTexture; 
	CefRefPtr<CefBrowser> mBrowser;

	string mJavascript;
	
};

#pragma once

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

class ofxCEFBrowser {


public:
	ofxCEFBrowser();
	ofxCEFBrowser(std::string startupResource, int width, int height, string js);
	~ofxCEFBrowser();
	
	void loadTex(ofPixels *); 
	void loadTex(ofTexture *); 

	void loadedTexture(const void *);

	int getWidth() { return width; }
	int getHeight() { return height; }
	
	std::string &getJavascript() { return mJavascript; }

	void processEvent();

	void close();

	
	void sendMessage(std::string name, CefRefPtr<CefListValue> message); 

	void messageCallback(CefRefPtr<CefProcessMessage> message); 
private:
	ofxCEFBrowser(const ofxCEFBrowser &);
	
	void enableEvents(); 

	void _mouseMoved(ofMouseEventArgs &e);
	void _mousePressed(ofMouseEventArgs &e);
	void _mouseDragged(ofMouseEventArgs &e);
	void _mouseReleased(ofMouseEventArgs &e);

	void _keyPressed(ofKeyEventArgs &e);
	void _keyReleased(ofKeyEventArgs &e);

	void _windowResized(ofResizeEventArgs &e);


	CefRefPtr<CefBrowserHost> _browserHost; 

	bool mLoadIntoPixels;
	ofTexture *mTexture; 
	ofPixels *mPixels;

	bool _initialized; 


	unsigned char *buffer; 
	int width;
	int height; 

	uint32_t lastClick; 
	std::atomic<bool> mLoadedTexture; 
	CefRefPtr<CefBrowser> mBrowser;

	string mJavascript;
	
	
	message_queue jsEventQueue; 
};
#pragma once

#include "client.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

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

class ofxCEFBrowser : public std::enable_shared_from_this<ofxCEFBrowser> {


public:
	ofxCEFBrowser();
	ofxCEFBrowser(int width, int height, string js);

	~ofxCEFBrowser();

	void browseTo(std::string url);

	void loadTex(std::shared_ptr<ofPixels>); 
	void loadTex(std::shared_ptr<ofTexture>); 

	void loadedTexture(const void *);

	int getWidth() { return mWidth; }
	int getHeight() { return mHeight; }
	
	std::string &getJavascript() { return mJavascript; }

	void processEvent();

	void close();

	void executeJavascript(std::string &js);
	std::string getUrl();
	
	void sendMessage(std::string name, CefRefPtr<CefListValue> message); 

	void messageCallback(CefRefPtr<CefProcessMessage> message); 

	void scroll(int deltaX, int deltaY);
	void click(int x, int y);
	void move(int x, int y);

	bool isLoading();
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
	std::shared_ptr<ofTexture> mTexture; 
	std::shared_ptr<ofPixels> mPixels;

	bool _initialized; 

	int mWidth;
	int mHeight; 

	uint32_t lastClick; 
	std::atomic<bool> mLoadedTexture; 
	CefRefPtr<CefBrowser> mBrowser;

	string mJavascript;
	
	
	message_queue jsEventQueue; 
};
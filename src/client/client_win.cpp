// Copyright (c) 2013 The Chromium Embedded Framework 
// Copyright (c) 2013 Intel Corporation

#include "ofMain.h"
#include "ofxCEFBrowser.h"

#include "WinBase.h"
#include "client.h"

#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <direct.h>
#include <sstream>
#include <string>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"

#include "client_handler.h"
#include "client_cli.h"
#include "util.h"
#include "string_util.h"

#define MAX_LOADSTRING 100
#define MAX_URL_LENGTH  255
#define BUTTON_WIDTH 72
#define URLBAR_HEIGHT  24

// Global Variables:
HINSTANCE hInst;   // current instance

char szWorkingDir[MAX_PATH];  // The current working directory

// Used for processing messages on the main application thread while running
// in multi-threaded message loop mode.
HWND hMessageWnd = NULL;
HWND CreateMessageWindow(HINSTANCE hInstance);
LRESULT CALLBACK MessageWndProc(HWND, UINT, WPARAM, LPARAM);

CefRefPtr<ClientApp> sClientApp;
// http://stackoverflow.com/questions/15462064/hinstance-in-createwindow

void ClientAppInit() {
	
	// Init =======================================================

	HINSTANCE hInstance = GetModuleHandle(nullptr); 
	hInst = hInstance;

	CefMainArgs main_args(hInstance);

	// Create the v8-binding app so we can communicate
	// between html/js, the cef client, and ofx
	sClientApp = new ClientApp();

	// Parse command line arguments. The passed in values are ignored on Windows.
	AppInitCommandLine(0, NULL);

	CefSettings appSettings;

	// The CEF client launches multiples threads from this single process
	appSettings.single_process = true; 

	// Populate the settings based on command line arguments.
	AppGetSettings(appSettings);

	CefInitialize(main_args, appSettings, sClientApp.get());
}

CefRefPtr< CefBrowser > ClientAppCreateBrowser(std::shared_ptr<ofxCEFBrowser> ofx, std::string startResource) {
	// OSR Browser =======================================================
	CefBrowserSettings settings;

	// Create the single static handler class instance
	HWND hWnd = WindowFromDC(wglGetCurrentDC());
	
	CefWindowInfo info;
	info.width = ofx->getWidth();
	info.height = ofx->getHeight();
	info.SetAsOffScreen(hWnd);
	info.SetTransparentPainting(true);
	//info.SetAsChild(hWnd, rect); 
	//info.SetAsPopup(hWnd, "hehe"); 

	// Create the new child browser window using an offscreen window
	auto browser =  CefBrowserHost::CreateBrowserSync(info, ofx->getClientHandler().get(), CefString(startResource), settings);

	return browser;
}

// Global functions
std::string AppGetWorkingDirectory() {

	return szWorkingDir;

}

void AppQuitMessageLoop() {

	CefRefPtr<CefCommandLine> command_line = AppGetCommandLine();

	//if (command_line->HasSwitch(cefclient::kMultiThreadedMessageLoop)) {
		// Running in multi-threaded message loop mode. Need to execute
		// PostQuitMessage on the main application thread.
		// ASSERT(hMessageWnd);
		PostMessage(hMessageWnd, WM_COMMAND, 12, 0);
	//} else {
	//	CefQuitMessageLoop();
	//}

}

LRESULT CALLBACK MessageWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
		case WM_COMMAND: {
				int wmId = wParam;
				switch (wmId) {
					case 12:
						PostQuitMessage(0);
						return 0;
          default:
            break; 
				}
			}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


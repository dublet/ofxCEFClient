// Copyright (c) 2013 The Chromium Embedded Framework 
// Copyright (c) 2013 Intel Corporation

#include "client.h"
#include <windows.h>
#include <commdlg.h>
#include <shellapi.h>
#include <direct.h>
#include <sstream>
#include <string>
#include "WinBase.h"
#include "ofMain.h"
#include "ofxCEFClient.h"

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

// The global ClientHandler reference (client.cpp)
extern CefRefPtr<ClientHandler> myClientHandler;

class MainBrowserProvider {
	virtual CefRefPtr<CefBrowser> GetBrowser() {
		if (myClientHandler.get())
			return myClientHandler->GetBrowser();

		return NULL;
	}
} g_main_browser_provider;


// http://stackoverflow.com/questions/15462064/hinstance-in-createwindow

void ClientAppInit(ofxCEFClient *ofx, std::string startResource) {

	// Init =======================================================

	HINSTANCE hInstance = GetModuleHandle(nullptr); 
	hInst = hInstance;

	CefMainArgs main_args(hInstance);

	// Create the v8-binding app so we can communicate
	// between html/js, the cef client, and ofx
	CefRefPtr<ClientApp> app(new ClientApp);

	// Parse command line arguments. The passed in values are ignored on Windows.
	AppInitCommandLine(0, NULL);

	CefSettings appSettings;

	// The CEF client launches multiples threads from this single process
	appSettings.single_process = true; 

	// Populate the settings based on command line arguments.
	AppGetSettings(appSettings);

	CefInitialize(main_args, appSettings, app.get());

	// OSR Browser =======================================================

	CefBrowserSettings settings;

	// Create the single static handler class instance
	HWND hWnd = WindowFromDC(wglGetCurrentDC());
	myClientHandler = new ClientHandler();
	myClientHandler->SetOfxPtr(ofx); 

	RECT rect;
	GetClientRect(hWnd, &rect);

	CefWindowInfo info;
	info.SetAsOffScreen(hWnd);
	info.SetTransparentPainting(true);

	// Create the new child browser window using an offscreen window
	CefBrowserHost::CreateBrowser(info, myClientHandler.get(), CefString(startResource), settings);

}

// Global functions
std::string AppGetWorkingDirectory() {

	return szWorkingDir;

}

void AppQuitMessageLoop() {

	CefRefPtr<CefCommandLine> command_line = AppGetCommandLine();

	if (command_line->HasSwitch(cefclient::kMultiThreadedMessageLoop)) {
		// Running in multi-threaded message loop mode. Need to execute
		// PostQuitMessage on the main application thread.
		ASSERT(hMessageWnd);
		//PostMessage(hMessageWnd, WM_COMMAND, ID_QUIT, 0);
	} else {
		CefQuitMessageLoop();
	}

}

LRESULT CALLBACK MessageWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message) {
		case WM_COMMAND: {
				int wmId = LOWORD(wParam);
				switch (wmId) {
					//case ID_QUIT:
					//	PostQuitMessage(0);
					//	return 0;
          default:
            break; 
				}
			}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}


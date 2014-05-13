// Copyright (c) 2013 Intel Corporation

#ifndef CEF_TESTS_CEFCLIENT_CEFCLIENT_H_
#define CEF_TESTS_CEFCLIENT_CEFCLIENT_H_
#pragma once

// Poco gives some crazy errors with CEF
// This makes poco not undefine anything... 
#define POCO_NO_UNWINDOWS

#include "ofMain.h"

#include "include/cef_base.h"
#include "client_app.h"

#include <string>
#include <strsafe.h>
#include <WinBase.h>

class CefApp;
class CefBrowser;
class CefCommandLine;
class ofxCEFClient; 

void ClientAppInit(ofxCEFClient *ofx);

CefRefPtr< CefBrowser > ClientAppCreateBrowser(ofxCEFClient *ofx, std::string startResource);


// Returns the main browser window instance.
CefRefPtr<CefBrowser> AppGetBrowser();

// Returns the main application window handle.
CefWindowHandle AppGetMainHwnd();

// Returns the application working directory.
std::string AppGetWorkingDirectory();

// Initialize the application command line.
void AppInitCommandLine(int argc, const char* const* argv);

// Returns the application command line object.
CefRefPtr<CefCommandLine> AppGetCommandLine();

// Returns the application settings based on command line arguments.
void AppGetSettings(CefSettings& settings);

// Quit the application message loop.
void AppQuitMessageLoop();

#endif  // CEF_TESTS_CEFCLIENT_CEFCLIENT_H_

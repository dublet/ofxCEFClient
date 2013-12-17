// Copyright (c) 2013 Intel Corporation

#include "client.h"
#include <stdio.h>
#include <cstdlib>
#include <sstream>
#include <string>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_web_plugin.h"

#include "client_handler.h"
#include "client_cli.h"
#include "util.h"
#include "string_util.h"

CefRefPtr<ClientHandler> myClientHandler;
CefRefPtr<CefCommandLine> g_command_line;

CefRefPtr<CefBrowser> AppGetBrowser() {
  if (!myClientHandler.get())
    return NULL;
  return myClientHandler->GetBrowser();
}

void AppInitCommandLine(int argc, const char* const* argv) {
  g_command_line = CefCommandLine::CreateCommandLine();
#if defined(OS_WIN)
  g_command_line->InitFromString(::GetCommandLineW());
#else
  g_command_line->InitFromArgv(argc, argv);
#endif
}

// Returns the application command line object.
CefRefPtr<CefCommandLine> AppGetCommandLine() {
  return g_command_line;
}

// Returns the application settings based on command line arguments.
void AppGetSettings(CefSettings& settings) {
  ASSERT(g_command_line.get());
  if (!g_command_line.get())
    return;

  CefString str;

#if defined(OS_WIN)
  // okay...
  settings.multi_threaded_message_loop = true;
#endif

  // Set cache path
  CefString(&settings.cache_path) = g_command_line->GetSwitchValue(cefclient::kCachePath);

  // Specify a port to enable DevTools if one isn't already specified.
  if (!g_command_line->HasSwitch("remote-debugging-port"))
    settings.remote_debugging_port = 8088;

}


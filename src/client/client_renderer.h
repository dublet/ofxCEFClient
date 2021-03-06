// Copyright (c) 2012 The Chromium Embedded Framework 
// Copyright (c) 2013 Intel Corporation

#ifndef CEF_TESTS_CEFCLIENT_CLIENT_RENDERER_H_
#define CEF_TESTS_CEFCLIENT_CLIENT_RENDERER_H_
#pragma once

#include "client_app.h"
#include "include/cef_base.h"

namespace client_renderer {

// Message sent when the focused node changes.
extern const char kFocusedNodeChangedMessage[];

// Create the render delegate.
void CreateRenderDelegates(ClientApp::RenderDelegateSet& delegates);

}  // namespace client_renderer

#endif  // CEF_TESTS_CEFCLIENT_CLIENT_RENDERER_H_

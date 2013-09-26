// Copyright (c) 2011 The Chromium Embedded Framework 
// Copyright (c) 2013 Intel Corporation

#include "client_app.h"
#include "client_renderer.h"

// static
void ClientApp::CreateBrowserDelegates(BrowserDelegateSet& delegates) {

}

// static
void ClientApp::CreateRenderDelegates(RenderDelegateSet& delegates) {

  client_renderer::CreateRenderDelegates(delegates);

}

// static
void ClientApp::RegisterCustomSchemes( CefRefPtr<CefSchemeRegistrar> registrar, std::vector<CefString>& cookiable_schemes) {

}

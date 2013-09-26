// Copyright (c) 2010 The Chromium Embedded Framework 
// Copyright (c) 2013 Intel Corporation

#ifndef CEF_TESTS_CEFCLIENT_STRING_UTIL_H_
#define CEF_TESTS_CEFCLIENT_STRING_UTIL_H_
#pragma once

#include <string>
#include "include/cef_base.h"

class CefRequest;

// Dump the contents of the request into a string.
void DumpRequestContents(CefRefPtr<CefRequest> request, std::string& str);

// Replace all instances of |from| with |to| in |str|.
std::string StringReplace(const std::string& str, const std::string& from,
                          const std::string& to);

#endif  // CEF_TESTS_CEFCLIENT_STRING_UTIL_H_

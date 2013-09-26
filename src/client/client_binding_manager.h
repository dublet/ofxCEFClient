// Copyright (c) 2013 Intel Corporation

#pragma once

#include "client_handler.h"

namespace client_binding_manager {

	// Delegate creation. Called from ClientHandler.
	void CreateProcessMessageDelegates(ClientHandler::ProcessMessageDelegateSet& delegates);

} 


// Copyright (c) 2013 Intel Corporation

#include "client_binding_manager.h"

#include <algorithm>
#include <string>

#include "ofMain.h"

namespace client_binding_manager
{

	namespace

	{

		const char *kMessageName = "client_binding_manager";

		// Handle messages in the browser process.
		class ProcessMessageDelegate : public ClientHandler::ProcessMessageDelegate {

			public:

				ProcessMessageDelegate() { }

				// From ClientHandler::ProcessMessageDelegate.
				virtual bool OnProcessMessageReceived( CefRefPtr<ClientHandler> handler,
				    CefRefPtr<CefBrowser> browser,
				    CefProcessId source_process,
				    CefRefPtr<CefProcessMessage> message) OVERRIDE {

					std::string message_name = message->GetName();

					handler->ForwardMessageToOfx(message); 

					return true;
				}

				IMPLEMENT_REFCOUNTING(ProcessMessageDelegate);

		};

	}  // namespace

	void CreateProcessMessageDelegates( ClientHandler::ProcessMessageDelegateSet &delegates) {

		delegates.insert(new ProcessMessageDelegate);

	}

}

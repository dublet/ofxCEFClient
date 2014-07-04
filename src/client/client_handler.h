// Copyright (c) 2011 The Chromium Embedded Framework 
// Copyright (c) 2013 Intel Corporation

#ifndef CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_
#pragma once

#include "ofxCEFBrowser.h"

#include <list>
#include <map>
#include <set>
#include <string>

#include "util.h"
#include "include/cef_client.h"

// ClientHandler implementation.
class ClientHandler : public CefClient,
	public CefContextMenuHandler,
	public CefDisplayHandler,
	public CefDownloadHandler,
	public CefDragHandler,
	public CefGeolocationHandler,
	public CefKeyboardHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefRenderHandler,
	public CefRequestHandler
{

	public:

		// Interface for process message delegates. Do not perform work in the
		// RenderDelegate constructor.
		class ProcessMessageDelegate : public virtual CefBase
		{
			public:
				// Called when a process message is received. Return true if the message was
				// handled and should not be passed on to other handlers.
				// ProcessMessageDelegates should check for unique message names to avoid
				// interfering with each other.
				virtual bool OnProcessMessageReceived(
				    CefRefPtr<ClientHandler> handler,
				    CefRefPtr<CefBrowser> browser,
				    CefProcessId source_process,
				    CefRefPtr<CefProcessMessage> message)
				{
					return false;
				}
		};

		typedef std::set<CefRefPtr<ProcessMessageDelegate> >
		ProcessMessageDelegateSet;

		// Interface implemented to handle off-screen rendering.
		class RenderHandler : public CefRenderHandler
		{
			public:
				virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) = 0;
		};

		ClientHandler();
		virtual ~ClientHandler();

		// CefClient methods
		virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE
		{
			return this;
		}

		virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE
		{
			return this;
		}

		virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE
		{
			return this;
		}

		virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE
		{
			return this;
		}

		virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() OVERRIDE
		{
			return this;
		}

		virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE
		{
			return this;
		}

		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
		{
			return this;
		}

		virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE
		{
			return this;
		}

		virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE
		{
			return this;
		}

		virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE
		{
			return this;
		}

		virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		                                      CefProcessId source_process,
		                                      CefRefPtr<CefProcessMessage> message)
		OVERRIDE;

		// CefContextMenuHandler methods
		virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
		                                 CefRefPtr<CefFrame> frame,
		                                 CefRefPtr<CefContextMenuParams> params,
		                                 CefRefPtr<CefMenuModel> model) OVERRIDE;
		virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
		                                  CefRefPtr<CefFrame> frame,
		                                  CefRefPtr<CefContextMenuParams> params,
		                                  int command_id,
		                                  EventFlags event_flags) OVERRIDE;

		// CefDisplayHandler methods
		virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
		                                  bool isLoading,
		                                  bool canGoBack,
		                                  bool canGoForward) OVERRIDE;
		virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
		                             CefRefPtr<CefFrame> frame,
		                             const CefString &url) OVERRIDE;
		virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
		                           const CefString &title) OVERRIDE;
		virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
		                              const CefString &message,
		                              const CefString &source,
		                              int line) OVERRIDE;

		// CefDownloadHandler methods
		virtual void OnBeforeDownload(
		    CefRefPtr<CefBrowser> browser,
		    CefRefPtr<CefDownloadItem> download_item,
		    const CefString &suggested_name,
		    CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;
		virtual void OnDownloadUpdated(
		    CefRefPtr<CefBrowser> browser,
		    CefRefPtr<CefDownloadItem> download_item,
		    CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

		// CefDragHandler methods
		virtual bool OnDragEnter(CefRefPtr<CefBrowser> browser,
		                         CefRefPtr<CefDragData> dragData,
		                         DragOperationsMask mask) OVERRIDE;

		// CefGeolocationHandler methods
		virtual void OnRequestGeolocationPermission(
		    CefRefPtr<CefBrowser> browser,
		    const CefString &requesting_url,
		    int request_id,
		    CefRefPtr<CefGeolocationCallback> callback) OVERRIDE;

		// CefKeyboardHandler methods
		virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
		                           const CefKeyEvent &event,
		                           CefEventHandle os_event,
		                           bool *is_keyboard_shortcut) OVERRIDE;

		// CefLifeSpanHandler methods
		virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
		                           CefRefPtr<CefFrame> frame,
		                           const CefString &target_url,
		                           const CefString &target_frame_name,
		                           const CefPopupFeatures &popupFeatures,
		                           CefWindowInfo &windowInfo,
		                           CefRefPtr<CefClient> &client,
		                           CefBrowserSettings &settings,
		                           bool *no_javascript_access) OVERRIDE;
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

		// CefLoadHandler methods
		virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
		                         CefRefPtr<CefFrame> frame) OVERRIDE;
		virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
		                       CefRefPtr<CefFrame> frame,
		                       int httpStatusCode) OVERRIDE;
		virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		                         CefRefPtr<CefFrame> frame,
		                         ErrorCode errorCode,
		                         const CefString &errorText,
		                         const CefString &failedUrl) OVERRIDE;
		virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
		                                       TerminationStatus status) OVERRIDE;

		// CefRequestHandler methods
		virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
		    CefRefPtr<CefBrowser> browser,
		    CefRefPtr<CefFrame> frame,
		    CefRefPtr<CefRequest> request) OVERRIDE;
		virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
		                            const CefString &origin_url,
		                            int64 new_size,
		                            CefRefPtr<CefQuotaCallback> callback) OVERRIDE;
		virtual void OnProtocolExecution(CefRefPtr<CefBrowser> browser,
		                                 const CefString &url,
		                                 bool &allow_os_execution) OVERRIDE;
		///
		// Called on the UI thread before browser navigation. Return true to cancel
		// the navigation or false to allow the navigation to proceed. The |request|
		// object cannot be modified in this callback.
		// CefLoadHandler::OnLoadingStateChange will be called twice in all cases.
		// If the navigation is allowed CefLoadHandler::OnLoadStart and
		// CefLoadHandler::OnLoadEnd will be called. If the navigation is canceled
		// CefLoadHandler::OnLoadError will be called with an |errorCode| value of
		// ERR_ABORTED.
		///
		/*--cef()--*/
		virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
									CefRefPtr<CefFrame> frame,
									CefRefPtr<CefRequest> request,
									bool is_redirect) OVERRIDE ;

		// CefRenderHandler methods
		virtual bool GetRootScreenRect(CefRefPtr<CefBrowser> browser,
		                               CefRect &rect) OVERRIDE;
		virtual bool GetViewRect(CefRefPtr<CefBrowser> browser,
		                         CefRect &rect) OVERRIDE;
		virtual bool GetScreenPoint(CefRefPtr<CefBrowser> browser,
		                            int viewX,
		                            int viewY,
		                            int &screenX,
		                            int &screenY) OVERRIDE;
		virtual bool GetScreenInfo(CefRefPtr<CefBrowser> browser,
		                           CefScreenInfo &screen_info) OVERRIDE;
		virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE;
		virtual void OnPopupSize(CefRefPtr<CefBrowser> browser,
		                         const CefRect &rect) OVERRIDE;
		virtual void OnPaint(CefRefPtr<CefBrowser> browser,
		                     PaintElementType type,
		                     const RectList &dirtyRects,
		                     const void *buffer,
		                     int width,
		                     int height) OVERRIDE;

		std::shared_ptr<ofxCEFBrowser> mBindToThisClient;

		void setBindToThisClient(std::shared_ptr<ofxCEFBrowser> client) {
			mBindToThisClient = client;
		}

		void bindToThisClient(CefRefPtr<CefBrowser> browser) {
			assert(mBindToThisClient);
			auto &it = ofxClientBrowserMap.find(mBindToThisClient);
			if (it != ofxClientBrowserMap.end()) {
				it->second.insert(browser);
			} else {
				set<CefRefPtr<CefBrowser>> browserset;
				browserset.insert(browser);
				ofxClientBrowserMap.insert(make_pair(mBindToThisClient, browserset));
			}
		}

		virtual void ForwardMessageToOfx(CefRefPtr<CefBrowser> browser, CefRefPtr<CefProcessMessage> message){
			auto client = getClient(browser);
			if (client)
				client->messageCallback(message); 
		}

		void unbindClient(CefRefPtr<CefBrowser> browser) {
			for (auto ofxClient : ofxClientBrowserMap) {
				ofxClient.second.erase(browser);
			}
		}

		std::shared_ptr<ofxCEFBrowser> getClient(CefRefPtr<CefBrowser> browser) {
			for (auto ofxClient : ofxClientBrowserMap) {
				for (auto cefbrowser : ofxClient.second) {
					if (cefbrowser == browser || cefbrowser->IsSame(browser) /*|| browser->GetMainFrame() == cefbrowser->GetMainFrame()*/
						|| cefbrowser->GetIdentifier() == browser->GetIdentifier()) {
						return ofxClient.first;
					}
				}
			}
			

			//if (ofxClientBrowserMap.size() == 1)
			//	return ofxClientBrowserMap.begin()->second;
			//assert(false);

			
			return std::shared_ptr<ofxCEFBrowser>(); 	
		}

		void closeClient(std::shared_ptr<ofxCEFBrowser> browser) {
			auto it = ofxClientBrowserMap.find(browser);
			if (it != ofxClientBrowserMap.end()) {
				for (auto &cefbrowser : it->second) {
					DoClose(cefbrowser);
				}
				ofxClientBrowserMap.erase(it);
			}
		}

		std::map<std::shared_ptr<ofxCEFBrowser>, std::set<CefRefPtr<CefBrowser>>> ofxClientBrowserMap; 

		virtual void OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor) OVERRIDE;

		// Request that all existing browser windows close.
		void CloseAllBrowsers(bool force_close);

		std::string GetLogFile();

		void SetLastDownloadFile(const std::string &fileName);
		std::string GetLastDownloadFile();

		// Send a notification to the application. Notifications should not block the
		// caller.
		enum NotificationType
		{
		    NOTIFY_CONSOLE_MESSAGE,
		    NOTIFY_DOWNLOAD_COMPLETE,
		    NOTIFY_DOWNLOAD_ERROR,
		};
		void SendNotification(NotificationType type);

		void ShowDevTools(CefRefPtr<CefBrowser> browser);

		// Returns the startup URL.
		std::string GetStartupURL()
		{
			return m_StartupURL;
		}

		// Create an external browser window that loads the specified URL.
		static void LaunchExternalBrowser(const std::string &url);

		void BeginTracing();
		void EndTracing();

		bool Save(const std::string &path, const std::string &data);

		bool hasBrowser(); 

	protected:

		void SetLoading(bool isLoading);
		void SetNavState(bool canGoBack, bool canGoForward);

		// Create all of ProcessMessageDelegate objects.
		static void CreateProcessMessageDelegates(
		    ProcessMessageDelegateSet &delegates);

		// Returns the full download path for the specified file, or an empty path to
		// use the default temp directory.
		std::string GetDownloadPath(const std::string &file_name);

		// List of any popup browser windows. Only accessed on the CEF UI thread.
		typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
		BrowserList m_PopupBrowsers;

		HWND ofxWindowHandle; 

		// Support for logging.
		std::string m_LogFile;

		// Support for downloading files.
		std::string m_LastDownloadFile;

		// True if an editable field currently has focus.
		bool m_bFocusOnEditableField;

		// Registered delegates.
		ProcessMessageDelegateSet process_message_delegates_;

		// If true DevTools will be opened in an external browser window.
		bool m_bExternalDevTools;

		// List of open DevTools URLs if not using an external browser window.
		std::set<std::string> m_OpenDevToolsURLs;

		// The startup URL.
		std::string m_StartupURL;

		// True if mouse cursor change is disabled.
		bool m_bMouseCursorChangeDisabled;

		// Number of currently existing browser windows. The application will exit
		// when the number of windows reaches 0.
		static int m_BrowserCount;

		// Include the default reference counting implementation.
		IMPLEMENT_REFCOUNTING(ClientHandler);

		// Include the default locking implementation.
		IMPLEMENT_LOCKING(ClientHandler);

};

#endif  // CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_

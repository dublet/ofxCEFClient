#ofxCEFClient

ofxCEFClient is a means of loading locally-available HTML/JS files as UI layer on top of an openFrameworks app. This means you can develop a UI in standard HTML5/CSS3/JS and use it within C++. This is accomplished by wrapping [Chromium Embedded Framework (CEF)](http://code.google.com/p/chromiumembedded/). CEF is a wrapper itself around [Chromium](http://www.chromium.org/), the open-source project behind the Google Chrome browser.

In short, CEF can render a browser context to an offscreen window. This window is moved back into the ofx GL context via an ofTexture with transparency. Interaction is handled by simply forwarding the ofx keyboard & mouse events to CEF. Offscreen rendering (OSR) in general is a tad slower (20-30%) over a native on-screen window, however the performance impact in reality is negligible and everything is a solid 30 FPS within CEF.

ofxCEF is currently Windows only (VS2012, ofx0.8.0, pull requests welcome!)

## Events in C++

Chromium uses Google's [V8 Javascript engine](http://code.google.com/p/v8/) and is the method by which Chromium and openFrameworks can message back and forth asynchronously. It's up to the user to develop a system to pass UI events between ofx and HTML interface elements bound to JS events. 

The included example projects shows how the event callback works. CEF provides a nice structure, `CefProcessMessage`, that wraps JS arguments and lets you retrieve them as any native-c++ type you prefer. This structure has a name (the key for the message) and an indexed array of arguments. This system assumes you know the key and structure of arguments for that key (since you're the one sending them from your JS).

```c++
// This would be in the callbackHandler in testApp
std::string name = message.get()->GetName();

CefRefPtr<CefListValue> args = message->GetArgumentList();

int anInteger = args->GetInt(0);
double aDouble = args->GetDouble(1);
std::string aString = args->getString(2);
```

The ofxCEFClient object has a method for sending events back to JS. The `sendMessage` method takes a string key and a smart-pointer-wrapped CefListValue as args.

```c++
// Create the CefListValue
CefRefPtr <CefListValue> myNewMessage = CefListValue::Create();

myNewMessage->SetBool(0, TRUE);
myNewMessage->SetNull(1);

myCEFClient.sendMessage("a_new_message", myNewMessage);
```

## Events in JS

CEF exposes a global JS object with several methods called `app`. `app` is created by CEF and injected into the user's JS.

```javascript
// Register a callback a single message from openFrameworks
app.setMessageCallback('my_message_test', function(messageKey, argumentArray) {
  var someType = argumentArray[0]; // we need to remember what we're sending from c++
});
```

```javascript
// Send a message to openFrameworks
function sendMessage() {
  var intArgument = 1;
  app.sendMessage('my_message_test_send_int', [intArgument]);
}
```

## UI Development

The example project comes with an HTML5-Boilerlate based template.

## Project Setup
The example project needs to be placed in $(OFX_ROOT)/apps/myApps

Right now, the best place to start is by modifying the included example. If setting up from scratch, make sure you do the following:
* Add libcef_dll_wrapper.vcxproj to your existing Visaul Studio solution
* In Visual Studio, copy the CEF folder from libcef_dll_wrapper to your own project
* Link against libcef.lib
* Make sure the addon files and all sub-folders in src/ and cef/include are in your include path
* Add the files from src/ and src/client into your project so they get compiled
* Ensure libcef_dll_wrapper is a dependency of your own project (Project Name >> Project Dependencies)
* Ensure libcef_dll_wrapper is referenced by your project (Project Name >> References >> Add New Reference) 

libcef.lib is configured for release, using a 39mb DLL at runtime. 

## HTML/JS in C++?
[Awesomium](http://www.awesomium.com/) is another implementation of this idea (ofxAwesomium), and while free for apps making under 100k in revenue, isn't OSS. librocket is another variation but approaches it differently, instead using a custom HTML/CSS parser to draw interface elements.

## Linkage
Applications using CEF normally link with the /MT setting. Since openFrameworks is all /MD, this app statically links the libcef_dll_wrapper which dynamically loads the /MT version.

## Known Bugs
* Resizing a window isn't currently supported. 
* Text fields in HTML don't like particular keys other than standard characters
* Text fields in HTML don't have a blinking cursor when focused
* App doesn't shut down cleanly

## License
This project is licensed under the New BSD License (BSDv3), the same as CEF and the majority of Chromium. This wrapper code is copyright 2013 Intel Corporation, Perceptual Computing Lab. 

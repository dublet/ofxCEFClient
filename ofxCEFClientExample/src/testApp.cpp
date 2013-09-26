#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{

	counter = 0;

	ofSetFrameRate(60);

	myCEFClient.init("");

}

//--------------------------------------------------------------
void testApp::update()
{
	counter = counter + 0.10f; 

	myCEFClient.loop(); 

}

//--------------------------------------------------------------
void testApp::draw()
{
		
	ofSetColor(255); 
	myCEFClient.paint();

	ofSetColor(100);
	float radius = 50 + 10 * sin(counter);
	ofFill(); 
	ofCircle((ofGetWidth() / 2), 400, radius);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}

/* Some Available Methods: 
		GetString(int index)
		GetDouble(int index)
		GetBool(int index)
		GetBinary(int index)
		GetInt(int index)
*/ 
void testApp::cefMessageCallback(CefRefPtr<CefProcessMessage> message) {

	ofLogNotice() << "Got Message from: " << (std::string) message.get()->GetName() << std::endl; 

	CefRefPtr<CefListValue> args = message->GetArgumentList();

	if (args->GetSize() > 0) {

		std::string hello = args->GetString(0);
		ofLogNotice() << "String Argument 1: " << hello << std::endl; 

		CefRefPtr <CefListValue> myRandomNumberMsg = CefListValue::Create();
		myRandomNumberMsg->SetDouble(0, ofRandom(0, 1));
		myCEFClient.sendMessage("random_ofx", myRandomNumberMsg);

	}

}
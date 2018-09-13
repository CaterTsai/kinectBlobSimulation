#include "ofApp.h"

#pragma region KinectUnit
//--------------------------------------------------------------
void kinectUnit::setup(int id, ofRectangle rect, int port)
{
	_kinectId = id;
	_range = rect;
	_package._kid = id;

	_udpSender.Create();
	_udpSender.Connect("127.0.0.1", port);
	_udpSender.SetNonBlocking(true);
}

//--------------------------------------------------------------
void kinectUnit::update(array<testBlobData, cTestBlobSize>& blobList)
{
	int index = 0;

	for (int i = 0; i < cKMaxBlobNumEach; i++)
	{
		_package._blobData[i].x = 0;
		_package._blobData[i].y = 0;
		_package._blobData[i].width = 0;
		_package._blobData[i].height = 0;
	}

	for (auto& iter : blobList)
	{
		ofRectangle rect(iter.pos, iter.width, iter.height);
		if (_range.intersects(rect))
		{
			auto intersectRect = getIntersectRect(rect);

			_package._blobData[index].x = intersectRect.x - _range.x;
			_package._blobData[index].y = intersectRect.y - _range.y;
			_package._blobData[index].width = intersectRect.width;
			_package._blobData[index].height = intersectRect.height;

			index++;
		}

		if (index >= cKMaxBlobNumEach)
		{
			break;
		}
	}
	send();
}

//--------------------------------------------------------------
void kinectUnit::draw()
{
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(_range.getTopLeft());
	ofDrawRectangle(0, 0, _range.width, _range.height);

	for (int i = 0; i < cKMaxBlobNumEach; i++)
	{
		ofDrawRectangle(
			_package._blobData[i].x,
			_package._blobData[i].y,
			_package._blobData[i].width,
			_package._blobData[i].height
		);
		
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
ofRectangle kinectUnit::getIntersectRect(ofRectangle& rect)
{
	ofVec2f posLT, posRB;
	posLT.x = MAX(rect.getLeft(), _range.getLeft());
	posLT.y = MAX(rect.getTop(), _range.getTop());

	posRB.x = MIN(rect.getRight(), _range.getRight());
	posRB.y = MIN(rect.getBottom(), _range.getBottom());

	return ofRectangle(posLT, posRB);
}

//--------------------------------------------------------------
void kinectUnit::send()
{
	char msg[cUDPSendMsgLength] = { 0 };
	memcpy(msg, &_package, sizeof(kinectPackage));

	_udpSender.Send(msg, cUDPSendMsgLength);
}
#pragma endregion


//--------------------------------------------------------------
void ofApp::setup() {

	_kinectsList[0].setup(1, ofRectangle(10, 10, 640, 480), 2233);
	_kinectsList[1].setup(2, ofRectangle(630, 10, 640, 480), 2244);
	_kinectsList[2].setup(3, ofRectangle(10, 470, 640, 480), 2255);
	_kinectsList[3].setup(4, ofRectangle(630, 470, 640, 480), 2266);



	_sendTimer = cSendTime;
	ofBackground(0);

	ofSetFrameRate(60);
	_timer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update() {
	float delta = ofGetElapsedTimef() - _timer;
	_timer += delta;

	_sendTimer -= delta;
	if (_sendTimer <= 0.0f)
	{
		_sendTimer = cSendTime;
		for (int i = 0; i < 4; i++)
		{
			_kinectsList[i].update(_blobDataList);
		}
	}
	updateBlob(delta);

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofPushStyle();
	{
		ofNoFill();
		ofSetColor(255, 0, 0);
		_kinectsList[0].draw();
		ofSetColor(255, 255, 0);
		_kinectsList[1].draw();
		ofSetColor(255, 0, 255);
		_kinectsList[2].draw();
		ofSetColor(0, 255, 0);
		_kinectsList[3].draw();
	}

	ofPopStyle();

	drawBlob();
}


//--------------------------------------------------------------
void ofApp::updateBlob(float delta)
{
	for (auto& iter : _blobDataList)
	{
		iter.update(delta);
	}
}

//--------------------------------------------------------------
void ofApp::drawBlob()
{
	ofPushStyle();
	ofNoFill();
	ofSetColor(255);
	for (auto& iter : _blobDataList)
	{
		ofDrawRectangle(iter.pos, iter.width, iter.height);
	}
	ofPopStyle();
}



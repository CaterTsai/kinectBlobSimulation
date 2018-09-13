#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"

static const int cKMaxBlobNumEach = 5;
static const int cTestBlobSize = 5;
static const int cUDPSendMsgLength = 50;
static const float cSendTime = 0.5;

struct blobData
{
	blobData()
		:x(0)
		, y(0)
		, width(0)
		, height(0)
	{}

	uint16_t x, y;
	uint16_t width, height;
};
struct kinectPackage
{
	kinectPackage()
		:_kid(0)
	{}
	uint8_t _kid;
	blobData _blobData[cKMaxBlobNumEach];
};


class testBlobData
{
public:
	testBlobData()
	{
		pos.set(ofRandomWidth(), ofRandomHeight());
		vec.set(1.0, 0.0);
		vec.rotate(ofRandom(0, 360));
		vec *= ofRandom(100, 200);

		width = ofRandom(50, 200);
		height = ofRandom(50, 200);
	}

	void update(float delta)
	{
		pos += delta * vec;
		if (pos.x < 0)
		{
			pos.x = 0;
			vec.x *= -1;
		}
		else if(pos.x >= ofGetWidth() - width)
		{
			pos.x = ofGetWidth() - width;
			vec.x *= -1;
		}

		if (pos.y < 0)
		{
			pos.y = 0;
			vec.y *= -1;
		}
		else if (pos.y >= ofGetHeight() - height)
		{
			pos.y = ofGetHeight() - height;
			vec.y *= -1;
		}
	}
	ofVec2f pos, vec;
	int width, height;
};

class kinectUnit
{
public:
	void setup(int id, ofRectangle rect, int port);
	void update(array<testBlobData, cTestBlobSize>& blobList);
	void draw();
	ofRectangle getIntersectRect(ofRectangle& rect);
	void send();

public:
	int _kinectId;
	ofRectangle _range;
private:
	kinectPackage _package;
	ofxUDPManager _udpSender;
};

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

private:
	float _timer;
	float _sendTimer;
	kinectUnit _kinectsList[4];

private:
	void updateBlob(float delta);
	void drawBlob();
private:
	array<testBlobData, cTestBlobSize> _blobDataList;

		
};

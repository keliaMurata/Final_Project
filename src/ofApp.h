#pragma once

#include "ofMain.h"
#include "ofxLeapMotion2.h"

class ofApp : public ofBaseApp
{
    
public:
    void setup();
    void update();
    void draw();
    void exit();
    
	ofxLeapMotion leap;
	vector<ofxLeapMotionSimpleHand> simpleHands;
	vector <vector <ofxLeapMotionSimpleHand>> delayHands; 
	
	bool trigger; 
	int delayedTime; 
	int hands; 

	ofEasyCam cam;
};

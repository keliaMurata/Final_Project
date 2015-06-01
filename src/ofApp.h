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
	 void keyPressed  (int key);
    
	ofxLeapMotion leap;
	vector<ofxLeapMotionSimpleHand> simpleHands;
	vector <vector <ofxLeapMotionSimpleHand>> delayHands; 
	
	bool trigger; 
	int triggerCount; 
	bool timeDelay; 
	int delayCount; 
	int temp; 

	ofEasyCam cam;
};

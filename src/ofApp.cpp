#include "ofApp.h"

/* 
Basic leap set up and controls curtousy of the Leap ofx addon example library. 

Time delay and fame storing code written by Kelia Murata
Project: 
In oder to empower one must first educate. I wrote this program in th hopes to help others understand some of the struggles faced by
individuals that have suffered strokes and lost the ability to completely control their limbs.  Brain damage can occur quickly when 
blood flow is restricted or cut off from different areas in the brain, causing irreversable damage to outer body functions such as 
motor, speech and concentration abilities.  This program will simulate the disjunct, delay and struggle between the sitmuli in the brain
(the input to the leap controler) and the movement in the limb itself (the displayed output) 

 */

//--------------------------------------------------------------


void ofApp::setup()
{
    //how often the frame refreshes per second 
    ofSetFrameRate(60);
	//syncs the refresh with your computers settings 
    ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
    
	//opens up the leap to listen 
	leap.open();
    
	
	trigger = false; //bool variable that turns delay of movement on and off 
	delayedTime = 0; //new type of timing system that delayed hands updates accordingly with 
	hands = 0; //the first hand that appeared on the screen 


	//sets where in 3d space our camera sits 
	cam.setOrientation(ofPoint(-30, 0, 0));
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}


//--------------------------------------------------------------
void ofApp::update()
{
	//fills vector with a single frame containing the position of all present hands 
	simpleHands = leap.getSimpleHands();

	//fills a temporary vector with a single frame containing the position of all present hands
	vector<ofxLeapMotionSimpleHand> tempHand = leap.getSimpleHands(); 
	for(int i = 0; i < tempHand.size(); ++i)
	{
		//add the new frame (temphand) to the end of delayHands 
		delayHands.push_back(tempHand);		
	}
		
	//when the vector starts to get too long cut off stored frames at the begining of vector 
	if(delayHands.size() > 600)
	{
		delayHands.erase(delayHands.begin());
		//delayedTime--; //shorten both together?
	}

	//rest delayedTime iterator so it doesn't go outside the scope of the vector
	//clear the delay vector and switch back to realtime hand 
	if(delayedTime == 597)
	{
		delayedTime = 0; 
		trigger = false;
		delayHands.clear();
	}

	//if the hand is moving realtime wait 500 frames and switch it to delay 
	if(trigger == false && delayedTime == 500)
	{
		trigger = true; 
		delayedTime = 0; 
	}

	//mapping the shape of the hand 
	if( leap.isFrameNew() && simpleHands.size() && delayHands.size())
	{  
		leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
		leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);			
		leap.setMappingZ(-150, 150, -200, 200);
	}
	
	//IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
	leap.markFrameAsOld();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	//adds a gradiated light spot to the background coloration 
    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
	
	//sets the color of the string 
	ofSetColor(200);
	ofDrawBitmapString("ofxLeapMotion - Example App\nLeap Connected? " + ofToString(leap.isConnected()), 20, 20);
    
	//starts camera 
	cam.begin();
    
	//creates an array of finger types 
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    

	//how to draw for the realtime hand 
	if(trigger == false)
	{

		for(int i = 0; i < simpleHands.size(); i++)
		{
			//incrememnt the time 
			delayedTime++; 
			bool isLeft        = simpleHands[i].isLeft;
			ofPoint handPos    = simpleHands[i].handPos;
			ofPoint handNormal = simpleHands[i].handNormal;
        

			ofSetColor(255);
			ofDrawSphere(handPos.x, handPos.y, handPos.z, 20);
			ofDrawArrow(handPos, handPos + 100*handNormal);
        
			//to gain access into each of the fingers by their index (ie thumb = 0, index = 1 etc)
			for (int f = 0; f < 5; f++) 
			{
				//create a point with the name of each joint so we can more easily call them 
				ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
				ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
				ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
				ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
				//draw circles for each of the joints 
				ofDrawSphere(mcp.x, mcp.y, mcp.z, 10);
				ofDrawSphere(pip.x, pip.y, pip.z, 10);
				ofDrawSphere(dip.x, dip.y, dip.z, 10);
				ofDrawSphere(tip.x, tip.y, tip.z, 10);
            
				//draw lines bvetween joints for fingers 
				ofSetLineWidth(300);
				ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
				ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
				ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
			}
		}
	}

//-----------------------------------------------------------------------------------------------------------------------------------------

	//time delay hand 
	if(trigger == true && delayHands.size() > 0)
	{
		//only updates the delay hand every 3 frames 
		if(ofGetFrameNum() % 3 == 0) delayedTime++; // if the frame rate counter is equally divisable by 3
		
		bool isLeft        = delayHands[delayedTime][hands].isLeft;
		ofPoint handPos    = delayHands[delayedTime][hands].handPos;
		ofPoint handNormal = delayHands[delayedTime][hands].handNormal;
        
		ofSetColor(255);
		ofDrawSphere(handPos.x, handPos.y, handPos.z, 20);
		ofDrawArrow(handPos, handPos + 100*handNormal);
        
			
		//to gain access into each of the fingers by their index (ie thumb = 0, index = 1 etc)
		for (int f = 0; f < 5; f++) 
		{
			//create a point with the name of each joint so we can more easily call them 
			ofPoint mcp = delayHands[delayedTime][hands].fingers[ fingerTypes[f] ].mcp;  // metacarpal
			ofPoint pip = delayHands[delayedTime][hands].fingers[ fingerTypes[f] ].pip;  // proximal
			ofPoint dip = delayHands[delayedTime][hands].fingers[ fingerTypes[f] ].dip;  // distal
			ofPoint tip = delayHands[delayedTime][hands].fingers[ fingerTypes[f] ].tip;  // fingertip
        
			//draw a sphere for each joint 
			ofDrawSphere(mcp.x, mcp.y, mcp.z, 10);
			ofDrawSphere(pip.x, pip.y, pip.z, 10);
			ofDrawSphere(dip.x, dip.y, dip.z, 10);
			ofDrawSphere(tip.x, tip.y, tip.z, 10);
            
			//draw lines to connect the joints together for fingers 
			ofSetLineWidth(300);
			ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
			ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
			ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
		}
	}
	//stop the camera 
	cam.end();
}

//--------------------------------------------------------------
void ofApp::exit()
{
    // let's close down Leap and kill the controller
    leap.close();
}

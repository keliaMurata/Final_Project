#include "ofApp.h"

/* 
Basic leap set up and controls curtousy of the Leap ofx addon example library. 

Time delay and fame storing code written by Kelia Murata
Project: 
In oder to empower one must first educate. I wrote this program in th hopes to help others understand some of the struggles faced by
individuals that have suffered strokes and lost the ability to completely control their limbs.  Brain damage can occur quickly when 
blood flow is restricted or cut off from different areas in the brain, causing irreversable damage to outer body functions such as 
motor, speech and concentration abilities.  This program will simulate the disjunct and struggle between the sitmuli in the brain
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
    
	trigger = false; 
	timeDelay = false;
	triggerCount = 1;
	delayCount = 1; 
	temp = 0; 

	//sets where in 3d space our camera sits 
	cam.setOrientation(ofPoint(-30, 0, 0));
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}


//--------------------------------------------------------------
void ofApp::update()
{
	//fills vectors with a single frame containing the position of all present hands 
	simpleHands = leap.getSimpleHands();

	vector<ofxLeapMotionSimpleHand> tempHand = leap.getSimpleHands(); //one frame multihand
	for(int i = 0; i < tempHand.size(); ++i)
	{
		if(delayHands.size() <= i) 
		{
			delayHands.push_back(vector <ofxLeapMotionSimpleHand> ()); //for first hand note: index will change without program knowledge 
		}
		delayHands[i].push_back(tempHand[i]);	
	}



	if(trigger == false)
	{
		if( leap.isFrameNew() && simpleHands.size() )
		{  
			leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
			leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
			leap.setMappingZ(-150, 150, -200, 200);
		}
	}

	if(trigger == true) 
	{
		if( leap.isFrameNew() && delayHands.size() )
		{  
			leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
			leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
			leap.setMappingZ(-150, 150, -200, 200);
		}
	}
	
	if(trigger == true)
	{
		cout << "x" << endl;
	}
	
	if(timeDelay == true) 
	{
		cout << "delaying time" + ofToString(delayCount) << endl; 

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
    

	if(trigger == false)
	{

		for(int i = 0; i < simpleHands.size(); i++)
		{
			bool isLeft        = simpleHands[i].isLeft;
			ofPoint handPos    = simpleHands[i].handPos;
			ofPoint handNormal = simpleHands[i].handNormal;
        

			ofSetColor(255);
			ofDrawSphere(handPos.x, handPos.y, handPos.z, 20);
			//ofSetColor(255, 255, 0);
			ofDrawArrow(handPos, handPos + 100*handNormal);
        
			//to gain access into each of the fingers by their index (ie thumb = 0, index = 1 etc)
			for (int f = 0; f < 5; f++) 
			{
				//create a point with the name of each joint so we can more easily call them 
				ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
				ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
				ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
				ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
				//ofSetColor(0, 255, 0);
				ofDrawSphere(mcp.x, mcp.y, mcp.z, 10);
				ofDrawSphere(pip.x, pip.y, pip.z, 10);
				ofDrawSphere(dip.x, dip.y, dip.z, 10);
				ofDrawSphere(tip.x, tip.y, tip.z, 10);
            
				//ofSetColor(255, 0, 0);
				ofSetLineWidth(300);
				ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
				ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
				ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
			}
		}
	}

//-----------------------------------------------------------------------------------------------------------------------------------------

	//time delay hand 
	if(trigger == true)
	{
		for(int i = 0; i < delayHands.size(); i++)
		{
			for(int j = 0; j < delayHands[i].size(); ++j)
			{
				if(timeDelay == false) 
				{
					temp = i; 
				}
				if(timeDelay == true) 
				{ 
					temp = temp;  
				}

				

				bool isLeft        = delayHands[temp][j].isLeft;
				ofPoint handPos    = delayHands[temp][j].handPos;
				ofPoint handNormal = delayHands[temp][j].handNormal;
        

				ofSetColor(255);
				ofDrawSphere(handPos.x, handPos.y, handPos.z, 20);
				//ofSetColor(255, 255, 0);
				ofDrawArrow(handPos, handPos + 100*handNormal);
        
			
				//to gain access into each of the fingers by their index (ie thumb = 0, index = 1 etc)
				for (int f = 0; f < 5; f++) 
				{
					//create a point with the name of each joint so we can more easily call them 
					ofPoint mcp = delayHands[temp][j].fingers[ fingerTypes[f] ].mcp;  // metacarpal
					ofPoint pip = delayHands[temp][j].fingers[ fingerTypes[f] ].pip;  // proximal
					ofPoint dip = delayHands[temp][j].fingers[ fingerTypes[f] ].dip;  // distal
					ofPoint tip = delayHands[temp][j].fingers[ fingerTypes[f] ].tip;  // fingertip
           
					//ofSetColor(0, 255, 0);
					ofDrawSphere(mcp.x, mcp.y, mcp.z, 10);
					ofDrawSphere(pip.x, pip.y, pip.z, 10);
					ofDrawSphere(dip.x, dip.y, dip.z, 10);
					ofDrawSphere(tip.x, tip.y, tip.z, 10);
            
					//ofSetColor(255, 0, 0);
					ofSetLineWidth(300);
					ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
					ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
					ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
				}
			}
		}
	}
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if(key == OF_KEY_TAB)
	{
		triggerCount++; 
		if(triggerCount % 2 == 0)
		{
			trigger = true; 
		}
		if(triggerCount % 2 == 1)
		{
			trigger = false; 
		}
	}
	if(key == 'd')
	{
		delayCount++; 
		if(delayCount % 2 == 0)
		{
			timeDelay = true; 
		}
		if(delayCount % 2 == 1)
		{
			timeDelay = false; 
		}
	}
}


//--------------------------------------------------------------
void ofApp::exit()
{
    // let's close down Leap and kill the controller
    leap.close();
}

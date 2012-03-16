#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(ofColor::gray);
	ofSetLineWidth(2);
	
	for(int i =0; i<500; i++)
	{
		ofPoint point(ofRandom(50, ofGetWidth()-50), ofRandom(50, ofGetHeight()-50));
		points.push_back(point);
	}
	
	solvedPoints = scanner.solve(points);
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){

	ofPushStyle();
	
	ofSetColor(ofColor::black);
	ofNoFill();
	ofBeginShape();
	for (int i = 0; i<solvedPoints.size(); i++) 
	{
		ofVertex(solvedPoints[i].x, solvedPoints[i].y);
	}
	ofEndShape();
	
	ofSetColor(ofColor::white);
	for (int i = 0; i<points.size(); i++) 
	{
		ofFill();
			ofCircle(points[i].x, points[i].y, 4);
		ofNoFill();
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	bpm=40;
	ofSetCircleResolution(360);

	stream.setup(this, 2, 0, 44100, 512, 4);
	soundBuffer = new float[512];
	type = oscillator::sineWave;

	show=true;
	instructions=true;

}

//--------------------------------------------------------------
void testApp::update(){
	linePos=ofGetWidth()*(ofGetElapsedTimeMillis()%(60000/bpm)/(60000.0/bpm));
	for (int i=0; i<circles.size(); i++) {
		if(abs(linePos-circles[i].second.x)<20){
			growCircles[i]=true;
			waves[i].setVolume(.5);
		}
	}
	for (int i=0; i<growCircles.size(); i++) {
		if(growCircles[i]){
			if(circles[i].first>255){
				growCircles[i]=false;
				circles[i].first=20;
			} else {
				circles[i].first+=5;
			}
		}
	}
	for (int i=0; i<waves.size(); i++) {
		float vol=waves[i].volume;
		if (vol>.01) {
			waves[i].setVolume(vol-.01);
		}

	}

}

//--------------------------------------------------------------
void testApp::draw(){
	if(instructions){
		string displayMessage;
		displayMessage="SEQUENCER SYNTH\n\n";
		displayMessage+="press anywhere to place a dot that will play when the beam hits it\nthe range is from 50Hz at the top to 15000Hz at the bottom of the screen\n\npress 1 for a sine wave [white dot]\npress 2 for a square wave [blue dot]\npress 3 for a triangle wave [green dot]\npress 4 for a saw wave [red dot]\npress 5 for a reverse saw wave [magenta dot]\n\npressing Q will clear the screen, \nW will show or hide the waveform, \nand the spacebar will bring this message back.\n\nHAVE FUN =)\n\n\n\n\nWritten by Dom Amato - 4/18/2014\nMade with openFrameworks";
		ofDrawBitmapString(displayMessage, 250,200);
	}else{

		if(show){
			ofPushStyle();
			ofSetLineWidth(2);
			ofFill();
			ofSetColor(255);
			ofRect(ofGetWidth()-512,ofGetHeight()-200,512, 200);

			ofNoFill();
			ofSetColor(0);
			ofBeginShape();
			for (int i = 0; i < 512; i++){
				ofVertex(ofGetWidth()-512 + i, ofGetHeight()-100 + 100 * soundBuffer[i]);
			}
			ofEndShape();
			ofPopStyle();
		}
		ofSetLineWidth(10);
		for(int i=0;i<10;i++){
			ofSetColor(0,128,255,180-(20*i));
			ofLine(linePos-10*i, 0, linePos-10*i, ofGetHeight());
		}
		for(int i=0;i<circles.size();i++){
			if(waves[i].type==oscillator::sineWave)
				ofSetColor(255,255,255,255-circles[i].first);
			if(waves[i].type==oscillator::squareWave)
				ofSetColor(0,128,180,255-circles[i].first);
			if(waves[i].type==oscillator::triangleWave)
				ofSetColor(128,180,0,255-circles[i].first);
			if(waves[i].type==oscillator::sawWave)
				ofSetColor(180,0,0,255-circles[i].first);
			if(waves[i].type==oscillator::sawWaveReverse)
				ofSetColor(180,0,180,255-circles[i].first);
			ofEllipse(circles[i].second, circles[i].first, circles[i].first);
		}
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key=='q'){
		waves.clear();
		soundBuffers.clear();
		circles.clear();
	}
	if(key=='1'){
		type=oscillator::sineWave;
	}
	if(key=='2'){
		type=oscillator::squareWave;
	}
	if(key=='3'){
		type=oscillator::triangleWave;
	}
	if(key=='4'){
		type=oscillator::sawWave;
	}
	if(key=='5'){
		type=oscillator::sawWaveReverse;
	}
	if(key=='w'){
		show= !show;
	}
	if(key==' '){
		instructions= !instructions;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	circles.push_back(pair<int,ofPoint>(20,ofPoint(x,y)));
	growCircles.push_back(false);
	oscillator tempWaves;
	tempWaves.setup(44100);
	tempWaves.setFrequency(ofMap(y, 0, ofGetHeight(), 50, 15000));
	tempWaves.setVolume(0);
	tempWaves.type=type;
	soundBuffers.push_back(new float[512]);
	waves.push_back(tempWaves);
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

void testApp::audioOut(float * output, int bufferSize, int nChannels){

	for (int i = 0; i < bufferSize; i++){

		double sample = 0;
		int it = waves.size();
		for (int j = 0; j < it; j++){
			float val = waves[j].getSample();
			soundBuffers[j][i] = val;
			sample += val;
		}

		ofClamp(sample, -1, 1);
		output[i*nChannels    ] = sample;
		output[i*nChannels + 1] = sample;

		soundBuffer[i] = sample;
	}


}
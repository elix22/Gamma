/*	Gamma - Generic processing library
	See COPYRIGHT file for authors and license information
	
	Tutorial:		IO / AudioDevice
	Description:	How to start an audio stream and send line-input to output.
*/

#include "tutorial.h"

// A user defined class that can be accessed from the audio callback
struct UserData{
	float ampL, ampR;
};


// create a callback for generating a block of samples
void audioCB(AudioIOData& io){

	UserData& user = *(UserData *)io.user;
	float ampL = user.ampL;
	float ampR = user.ampR;

	// loop through the number of samples in the block
	for(uint32_t i=0; i<io.numFrames(); i++){
		
		float s = io.in(0)[i];		// get the line-in or microphone sample
		
		io.out(0)[i] = s * ampL;	// set left and right output channel samples
		io.out(1)[i] = s * ampR;
	}
}


int main(int argc, char* argv[]){

	// set parameters of audio stream
	int blockSize = 64;				// how many samples per block?
	float sampleRate = 44100;		// sampling rate (samples/second)
	int outputChannels = 2;			// how many output channels to open
	int inputChannels = 1;			// how many input channels to open
	UserData user = {-0.5, 0.5};	// external data to be passed into callback

	// create an audio i/o object using default input and output devices
	AudioIO io(blockSize, sampleRate, audioCB, &user, outputChannels, inputChannels);
	
	// set the global sample rate "subject"
	Sync::master().spu(io.fps());
	
	// start the audio stream
	io.start();
	
	// print some information about the i/o streams
	io.print();
	
	printf("\nPress 'enter' to quit...\n"); getchar();
	return 0;
}

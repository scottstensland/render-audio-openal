/*
 * Audio_Utility.h
 *
 *  Created on: Dec 11, 2013
 *      Author: stens
 */

#ifndef AUDIO_UTILITY_H_
#define AUDIO_UTILITY_H_




//#define MAX_NUMBER_CIRCULAR_BUFFERS 8 // OK nice

static const int MAX_NUMBER_CIRCULAR_BUFFERS = 8; // OK nice
//static const int MAX_NUMBER_CIRCULAR_BUFFERS = 33;


//static const int MAX_SIZE_CIRCULAR_BUFFER = 262144;
//static const int MAX_SIZE_CIRCULAR_BUFFER = 131072;
//static const int MAX_SIZE_CIRCULAR_BUFFER = 65536;
//static const int MAX_SIZE_CIRCULAR_BUFFER = 32768;
static const int MAX_SIZE_CIRCULAR_BUFFER = 16384;
//static const int MAX_SIZE_CIRCULAR_BUFFER = 8192;
//static const int MAX_SIZE_CIRCULAR_BUFFER = 4096;
//static const int MAX_SIZE_CIRCULAR_BUFFER = 2048;



enum BUFFER_CONDITION {

    production_eligible = 0,
    stage_production,
    production_complete,
    stage_consumption
};


typedef struct {	// component data items for a given audio sample session

	double frequency;
	int sample_rate;
//	int sample_length_seconds;
	double sample_length_seconds;
	int buffer_size;
	int aac_index;
	unsigned short num_channels;

} struct_sample_specs;




typedef struct {        // this struct contains LPCM data, both num bytes and matching array storing lpcm audio format data

//    unsigned long buffer_size;
//    unsigned long sample_rate;

	struct_sample_specs * ptr_struct_sample_specs;

//    unsigned char * ptr_to_array;
    short * ptr_to_array;
    enum BUFFER_CONDITION array_condition;
    int aac_index;         // shows index of input aac file
    int flag_last_buffer;

} struct_audio_array;



class Audio_Utility {
public:
	Audio_Utility();
	virtual ~Audio_Utility();
};

#endif /* AUDIO_UTILITY_H_ */

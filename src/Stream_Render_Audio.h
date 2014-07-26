/*
 * Stream_Render_Audio.h
 *
 *  Created on: Jun 3, 2013
 *      Author: stens
 */

#ifndef STREAM_RENDER_AUDIO_H_
#define STREAM_RENDER_AUDIO_H_

#include <cstddef>

#include <iostream>
#include <vector>
//#include "Synthesize_Sound.h"

#include "Consumer.h"
//#include "Utility.h"

#include "Audio_Model.h"


extern "C" {

	#include  <AL/al.h>
	#include  <AL/alc.h>
	#include <stdlib.h>     /* malloc, free, rand */
	#include <math.h>       /* sin */
	#include <unistd.h>
}

//#define MAX_OPENAL_QUEUE_BUFFERS 3
static const int MAX_OPENAL_QUEUE_BUFFERS = 2;

class Stream_Render_Audio {

public:

	Stream_Render_Audio();
	virtual ~Stream_Render_Audio();

	// ------------

	void orig_populate_buffer();

	// ------------

	void SRA_exit_al();

//	void SRA_openal(Circular_Buffer * given_circular_buffer);


	void SRA_openal(Circular_Buffer * given_circular_buffer,
											Audio_Model * given_audio_model);

private:

	Audio_Model * audio_model;

	bool get_next_available_buffer();

//	bool flag_continue_running;

	ALCdevice * openal_output_device;
	ALCcontext * openal_output_context;

//	ALuint internal_buffer;
//	ALuint streaming_source;
	ALuint streaming_source[1];

	ALuint available_AL_buffer_array[MAX_OPENAL_QUEUE_BUFFERS];
	ALuint available_AL_buffer_array_curr_index;

	ALsizei buffer_size;
	ALsizei sample_rate;
//	ALenum data_format;
	short num_channels;

//	short * samples;
	int sample_length_seconds;
	double sample_frequency;
	int aac_index;
	int flag_last_buffer;
	int last_aac_index;

//	unsigned int curr_vector_index;

	short * previous_local_lpcm_buffer, * local_lpcm_buffer;

	bool flag_continue_processing;
	bool flag_received_last_buffer;
	bool flag_reached_end_of_input_data;
	int  buffer_count_down;

	bool get_next_consumable();
	bool SRA_stream_render_buffers();

	void SRA_check_error(std::string given_label);

	void populate_buffer(short * given_samples);

	void get_next_buffer_to_stream();

	void SRA_stream_render_init_openal();

	Circular_Buffer * sra_circular_buffer;
};

#endif /* STREAM_RENDER_AUDIO_H_ */

/*
 * Producer.h
 *
 *  Created on: Jun 5, 2013
 *      Author: stens
 */

#ifndef PRODUCER_H_
#define PRODUCER_H_


#include "Circular_Buffer.h"
#include "Synthesize_Sound.h"



static const int SYNTH_MODE_SIN = 0;
static const int SYNTH_MODE_PCM_FILE = 1;
static const int SYNTH_MODE_DIY = 2;


class Producer {

public:

	Producer();
	virtual ~Producer();

	// ----------------

	void outnum(std::string given_label, int n);

	bool launch_loop(int flag_mode);

//	void wrap_process(
//			Circular_Buffer * given_circular_buffer,
//			int * audio_buffer,
//			unsigned int buffer_size);

//	void wrap_process(Circular_Buffer * given_circular_buffer,
//			Synthesize_Sound * given_synthesize_sound);

	void wrap_process(Circular_Buffer * given_circular_buffer,
			Synthesize_Sound * given_synthesize_sound, short int * given_audio_buffer,
			unsigned int given_buffer_size, unsigned int sample_rate, unsigned short num_channels,
			int flag_mode);


	static const int SLEEP_PRODUCE_01 = 100;

private:







	// ----------------------

	int loop_counter;

	bool flag_continue_processing_producer;

	Circular_Buffer * circular_buffer;

	Synthesize_Sound * synthesize_sound;

	void inner_wrap_process(int flag_mode);

//	void pop_audio_spec(struct_sample_specs * ptr_struct_sample_specs);

	void pop_audio_spec(struct_sample_specs * ptr_struct_sample_specs,
			int given_buffer_size);

	// --------------------------------

	short int * audio_buffer;
	short int * curr_audio_buffer;
	unsigned int source_buffer_size;
	unsigned int buffer_size_available;
	unsigned short num_channels;

	unsigned int sample_rate;

};

#endif /* PRODUCER_H_ */

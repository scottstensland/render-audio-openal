/*
 * Producer.cpp
 *
 *  Created on: Jun 5, 2013
 *      Author: stens
 */

#include <iostream>
#include <thread>
#include <exception>
#include <mutex>
#include "Producer.h"
//#include <string.h>		// memcpy

void Producer::outnum(std::string given_label, int n) {

	static std::mutex curr_mutex;

	// lock_guard   constructor/destructor   creates/destroys lock
	std::lock_guard <std::mutex> g( curr_mutex );

	std::cout << given_label << "[" << n << "]" << std::endl;
}

void Producer::pop_audio_spec(struct_sample_specs * ptr_struct_sample_specs,
		int given_buffer_size) {

	ptr_struct_sample_specs->frequency = 560;
	ptr_struct_sample_specs->sample_rate = 44100;

//	ptr_struct_sample_specs->sample_length_seconds = 3;
//	ptr_struct_sample_specs->buffer_size =	ptr_struct_sample_specs->sample_length_seconds *
//											ptr_struct_sample_specs->sample_rate;

	ptr_struct_sample_specs->buffer_size = given_buffer_size;
	ptr_struct_sample_specs->sample_length_seconds = double(given_buffer_size) /
			double(ptr_struct_sample_specs->sample_rate);
}

bool Producer::launch_loop(int flag_mode) {

	std::cout << " Producer::launch_loop   tooooooop" << std::endl;

	while (flag_continue_processing_producer) {

    	if (! circular_buffer->is_production_possible()) {

    		std::cout << "Producer launch_loop is_production_possible NO so sleep" << std::endl;

    		return false;
    	}

		outnum("Producer production IS POSSIBLE ", loop_counter);


		struct_audio_array * ptr_curr_audio_struct = circular_buffer->do_stage_production();

		if (! ptr_curr_audio_struct) {

			std::cout << "Producer NOTICE - do_stage_production returned NULL pointer "
					<< " sleep N retry" << std::endl;

			return false;
		}

		// ----------- OK now populate buffer ----------------

		ptr_curr_audio_struct->aac_index = loop_counter;
		ptr_curr_audio_struct->ptr_struct_sample_specs = synthesize_sound->get_sample_specs();
		ptr_curr_audio_struct->flag_last_buffer = NO;

		if (SYNTH_MODE_SIN == flag_mode) {

			std::cout << "Producer launch_loop OK now populate buffer SYNTH_MODE_SIN"
					<< std::endl;

			pop_audio_spec(ptr_curr_audio_struct->ptr_struct_sample_specs,
					circular_buffer->get_max_size_circular_buffer());

			synthesize_sound->populate_buffer(ptr_curr_audio_struct->ptr_to_array);

		} else if (SYNTH_MODE_PCM_FILE == flag_mode) {

			std::cout << "Producer launch_loop OK now populate buffer SYNTH_MODE_PCM_FILE"
					<< std::endl;

			std::cout << "Producer::launch_loop   source_buffer_size " << source_buffer_size << std::endl;

//			ptr_curr_audio_struct->ptr_to_array = audio_buffer;

//			std::cout << "PRODUCER NOTICE take out following memcpy ... "
//					<< "its needed to satisify Render need to see new buff address on subsequent buffs"
//					<< std::endl;

//			memcpy(ptr_curr_audio_struct->ptr_to_array, curr_audio_buffer, circular_buffer->get_max_size_circular_buffer());

			unsigned int this_buffer_size = circular_buffer->get_max_size_circular_buffer();

			// stens TODO - commented out belo IF ELSE 20131122

/*
			if (loop_counter == 26)
//			if (loop_counter == 27)

//			if (loop_counter == 44)

			{

				exit(8);
			}
*/

			if (buffer_size_available >= this_buffer_size) {

				// OK we have more buffer in reserve for next iteration

				buffer_size_available -= this_buffer_size;

				std::cout << "Producer - OK we have more buffer in reserve for next iteration "
						<< "this_buffer_size " << this_buffer_size
						<< std::endl;

			} else {

//				this_buffer_size = buffer_size_available;

				// stens TODO - since input data does NOT fill desired buffer size may fill with zeros

				// we have reached END of source input buffer - signal
				// stens TODO insert logic to communicate this last buffer

				std::cout << "Producer - we have reached END of source input buffer "
						<< "this_buffer_size " << this_buffer_size
						<< std::endl;

				flag_continue_processing_producer = false;

				ptr_curr_audio_struct->flag_last_buffer = YES;
			}

			ptr_curr_audio_struct->ptr_to_array = curr_audio_buffer;
			ptr_curr_audio_struct->ptr_struct_sample_specs->buffer_size = this_buffer_size;
			ptr_curr_audio_struct->ptr_struct_sample_specs->frequency = 0;	// define 0 since from file not synth
			ptr_curr_audio_struct->ptr_struct_sample_specs->num_channels = num_channels;

			// following division by 2 (i.e. XXX >> 1) works for mono - I believe stereo wants >> 2
			curr_audio_buffer += (circular_buffer->get_max_size_circular_buffer() >> 1);

			ptr_curr_audio_struct->ptr_struct_sample_specs->sample_rate = sample_rate;

		} else if (SYNTH_MODE_DIY == flag_mode) {

			std::cout << "Producer cool Corinde we are doing DIY audio" << std::endl;

		} else {

			std::cout << "Producer ERROR - unrecognized SYNTH_MODE_xxxxxx" << std::endl;
			exit(8);
		}

		// ----------- commit production -----------------

		if(! circular_buffer->execute_production(ptr_curr_audio_struct)) {

			std::cout << "Producer ERROR - failed call to execute_production" << std::endl;

			return false;
		}
//		std::cout << "In launch_loop just after execute_production NOW show_dict" << std::endl;

//		circular_buffer->show_dict();

		loop_counter++;
	}
	return true;

}	//	launch_loop

// -----------------

void Producer::inner_wrap_process(int flag_mode) {

	std::cout << "Producer inner_wrap_process start" << std::endl;

	while (flag_continue_processing_producer) {

		if (launch_loop(flag_mode)) {

			std::cout << "Producer wrap_process OK successful production" << std::endl;

		} else {

			std::cout
			<< "Producer NOTICE popped back to wrap_process some hiccup so sleep"
			<< std::endl;

//	    	std::this_thread::sleep_for (std::chrono::milliseconds( Producer::SLEEP_PRODUCE_01));
	    	std::this_thread::sleep_for (std::chrono::milliseconds( 200));
		}
	}

	std::cout << "Producer inner_wrap_process ended" << std::endl;

	circular_buffer->set_is_production_complete(true); // have reached end of production now permit
													   // consumer to deplete all buffers
}

void Producer::wrap_process(Circular_Buffer * given_circular_buffer,
		Synthesize_Sound * given_synthesize_sound, short int * given_audio_buffer,
		unsigned int given_source_buffer_size, unsigned int given_sample_rate,
		unsigned short given_num_channels, int flag_mode) {

	circular_buffer = given_circular_buffer;

	synthesize_sound = given_synthesize_sound;

	audio_buffer = given_audio_buffer;

	curr_audio_buffer = audio_buffer;

	source_buffer_size = given_source_buffer_size;

	sample_rate = given_sample_rate;

	buffer_size_available = given_source_buffer_size;

	num_channels = given_num_channels;

	inner_wrap_process(flag_mode);
}

Producer::Producer() {

	loop_counter = 0;
	flag_continue_processing_producer = true;
}

Producer::~Producer() {

}


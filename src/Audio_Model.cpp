/*
 * Audio_Model.cpp
 *
 *  Created on: Dec 12, 2013
 *      Author: stens
 */

#include "Audio_Model.h"
#include <string.h>


void Audio_Model::set_circular_buffer(Circular_Buffer * given_circular_buffer) {

	circular_buffer = given_circular_buffer;
}





void Audio_Model::set_audio_buffer(int given_aac_index, int given_buffer_size, int given_sample_rate, short * given_buffer) {


	std::cout << "set_audio_buffer TOP with aac_index " << aac_index << std::endl;

	if (previous_aac_index == aac_index) {

		std::cout << "seeing same aac_index " << aac_index << " so shortcircuit early" << std::endl;

		return;
	}

	if (given_buffer_size > max_size_circular_buffer) {

		std::cerr << "ERROR - given buffer size -->" << given_buffer_size
				<< "<-- exceeds MAX_SIZE_CIRCULAR_BUFFER -->"
				<< max_size_circular_buffer
				<< "<-- " << std::endl;

		exit(8);
	}

	previous_aac_index = aac_index;	// roll previous value to place holder for change validation

	aac_index = given_aac_index;
	buffer_size = given_buffer_size;
	sample_rate = given_sample_rate;

	// ---------- now need to do a memory copy from supplied buffer into local buffer

	memcpy((short *)local_lpcm_buffer, (short *)given_buffer, given_buffer_size * sizeof(short));

	// ------ //

	float tmp_max_value = 0;
	float tmp_min_value = 0;

	for (int curr_byte = 0; curr_byte < given_buffer_size; curr_byte++) {

//		fprintf(stdout, "aac_index %4d  curr_byte  %5d orig %6d local copy %6d\n",
//				aac_index,
//				curr_byte,
//				given_buffer[curr_byte],
//				local_lpcm_buffer[curr_byte]);

		if (local_lpcm_buffer[curr_byte] > tmp_max_value) tmp_max_value = local_lpcm_buffer[curr_byte];
		if (local_lpcm_buffer[curr_byte] < tmp_min_value) tmp_min_value = local_lpcm_buffer[curr_byte];
	}

	max_value = tmp_max_value;
	min_value = tmp_min_value;

//	std::stringstream some_string;
//	some_string << "max_value " << max_value;
//	std::cout << some_string << std::endl;

//	std::cout << "max_value " << max_value
//			<< " min_value " << min_value << std::endl;

//	fprintf(stdout, "\nmax_value %8f   min_value %8f\n",
//			max_value,
//			min_value);

	// -------------

//	std::cout << "prior output_file_counter " << output_file_counter << std::endl;
//
//	std::string given_output_filename_orig =
//			common_utilities->create_string_from_string_N_int("/usr/tmp/output_orig_", output_file_counter);
//
//	std::string given_output_filename_copy =
//			common_utilities->create_string_from_string_N_int("/usr/tmp/output_copy_", output_file_counter);

//	output_file_counter++;

//	std::cout << "given_output_filename_orig " << given_output_filename_orig << std::endl;
//	std::cout << "given_output_filename_copy " << given_output_filename_copy << std::endl;


//	common_utilities->write_output_file_ascii(given_output_filename_orig, given_buffer,      given_buffer_size);
//	common_utilities->write_output_file_ascii(given_output_filename_copy, local_lpcm_buffer, given_buffer_size);
}










//void Audio_Model::set_stream_render_audio(Stream_Render_Audio * given_stream_render_audio) {
//
//	stream_render_audio = given_stream_render_audio;
//}



// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------

void * Audio_Model::get_address_paddle_location() {

	return (void *) ptr_to_array_paddle_location;
}

int Audio_Model::get_sizeof_paddle_location() {

	return sizeof(paddle_location);
}

int Audio_Model::get_sizeof_paddle_neighbor_indices() {

	return sizeof(paddle_neighbor_indices);
}

void * Audio_Model::get_address_paddle_neighbor_indices() {

	return (void *) ptr_to_array_paddle_neighbor_indices;
}

int Audio_Model::get_curr_paddle_neighbor_index() {

	return curr_paddle_neighbor_index;
}

// --------------------



int Audio_Model::get_sizeof_audio_buffer() {

	return sizeof(audio_buffer);
}




void * Audio_Model::get_address_audio_buffer() {

	return (void *) ptr_to_array_audio_buffer;
}




int Audio_Model::get_sizeof_audio_buffer_indices() {

	return sizeof(audio_buffer_indices);
}





void * Audio_Model::get_address_audio_buffer_indices() {

	return (void *) ptr_to_array_audio_buffer_indices;
}





int Audio_Model::get_curr_audio_buffer_indices_index() {

	return curr_audio_buffer_indices_index;
}

// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------

bool Audio_Model::get_flag_continue_processing() {

	return flag_continue_processing;
}



void Audio_Model::update_location_molecules(int count_num_molecular_update_chronos) {

//	std::cout << "just inside update_location_molecules ... aac_index "
//			<< aac_index << std::endl;

//	previous_aac_index

	if (aac_index > most_recently_visualized_aac_index) {


		std::cout << "about to show aac_index " << aac_index << std::endl;



		most_recently_visualized_aac_index = aac_index;

	} else {

		// std::cout << "Already did show for aac_index " << aac_index << std::endl;
		return;
	}

	// ----------------------

	if (aac_index <= 0) {

		std::cerr << "NOTICE - attempting to visualize audio buffer not yet defined"
				<< std::endl;
		return;
	}

//	local_lpcm_buffer = (short *) malloc(sizeof(short) * max_size_circular_buffer);

//	short * local_lpcm_buffer;


//	float audio_buffer[max_size_audio_buffer][num_dimensions_3D];
//	int curr_audio_buffer_index;
//
//	int audio_buffer_indices[max_size_audio_buffer * 2];
//	int curr_audio_buffer_indices_index;

	float min_displayed_x = -1.0;
	float max_displayed_x = 1.0;
	float incr_x = (max_displayed_x - min_displayed_x) / max_size_circular_buffer;
	float curr_displayed_x = min_displayed_x;

//	float max_amplitude = 32768 - 1;
//	float min_amplitude = -32768;

//	float max_value;
//	float min_value;


	std::cout << "incr_x " << incr_x << std::endl;

	curr_audio_buffer_indices_index = 0;

//	float tmp_y;

	for (int buffer_index = 0; buffer_index < max_size_circular_buffer; buffer_index++) {

		audio_buffer[buffer_index][X] = curr_displayed_x;
		audio_buffer[buffer_index][Y] = local_lpcm_buffer[buffer_index] / max_value;
		audio_buffer[buffer_index][Z] = 0;

//		fprintf(stdout, "orig %6d   x %4.8f  y %4.8f  z %4.8f\n",
//				local_lpcm_buffer[buffer_index],
//				audio_buffer[buffer_index][X],
//				audio_buffer[buffer_index][Y],
//				audio_buffer[buffer_index][Z]
//				                  );

		curr_displayed_x += incr_x;

		// -------------

		if (buffer_index > 0) {

			audio_buffer_indices[curr_audio_buffer_indices_index++] = buffer_index - 1;
			audio_buffer_indices[curr_audio_buffer_indices_index++] = buffer_index;
		}
	}
}




void Audio_Model::pop_paddle() {

	float min_range = -1.0;
	float max_range =  1.0;

	for (; curr_paddle_location_index < max_paddle_locations; curr_paddle_location_index++) {

		paddle_location[curr_paddle_location_index][X] = common_utilities->get_random_in_range(min_range, max_range);
		paddle_location[curr_paddle_location_index][Y] = common_utilities->get_random_in_range(min_range, max_range);
		paddle_location[curr_paddle_location_index][Z] = common_utilities->get_random_in_range(min_range, max_range);

		/*
		std::cout << std::setw(4) << curr_paddle_location_index
				<< std::fixed << std::setw( 10 ) << std::setprecision( 3 ) << std::setfill( ' ' )
				  << " " << paddle_location[curr_paddle_location_index][X]
//				  << std::fixed << std::setw( 10 ) << std::setprecision( 3 ) << std::setfill( ' ' )
		          << " " << paddle_location[curr_paddle_location_index][Y]
//		          << std::fixed << std::setw( 10 ) << std::setprecision( 3 ) << std::setfill( ' ' )
		          << " " << paddle_location[curr_paddle_location_index][Z]
		          << std::endl;
		*/


		fprintf(stdout, "%5d %8.4f  %8.4f  %8.4f\n",
				curr_paddle_location_index,
				paddle_location[curr_paddle_location_index][X],
				paddle_location[curr_paddle_location_index][Y],
				paddle_location[curr_paddle_location_index][Z]);

		if (0 < curr_paddle_location_index) {

			paddle_neighbor_indices[curr_paddle_neighbor_index++] = curr_paddle_location_index;	// create new line from
			paddle_neighbor_indices[curr_paddle_neighbor_index++] = curr_paddle_location_index - 1;
		}
	}
}




void Audio_Model::initialize() {

	curr_paddle_location_index = 0;
	curr_paddle_neighbor_index = 0;

	pop_paddle();

//	stream_render_audio->SRA_openal(circular_buffer);	// this launches audio playback

}

// --------------------------------------------------------------------

void Audio_Model::initialize_from_constructor() {

	common_utilities = new Common_Utilities();
	common_utilities->seed_random();

	flag_continue_processing = true;

	ptr_to_array_paddle_location         = (void *) & paddle_location;	// these 3 needed for display
	ptr_to_array_paddle_neighbor_indices = (void *) & paddle_neighbor_indices;
	ptr_to_array_audio_buffer            = (void *) & audio_buffer;
	ptr_to_array_audio_buffer_indices    = (void *) & audio_buffer_indices;

	circular_buffer = NULL;

	aac_index = -2;

	previous_aac_index = -1;	// its 1 based so starts from 1 upwards

//	output_file_counter = 1;	// stens TODO - remove troubleshooting only

//	std::cout << "inside constructor Audio_Model with output_file_counter "
//			<< output_file_counter << std::endl;

	// ----------------------------------------------------------------------------------

	local_lpcm_buffer = (short *) malloc(sizeof(short) * max_size_circular_buffer);

	curr_audio_buffer_indices_index = 0;
}

Audio_Model::Audio_Model(int given_max_size_buffer) {	// constructor

	max_size_circular_buffer = given_max_size_buffer;

	if (max_size_audio_buffer < max_size_circular_buffer) {

		std::cerr << "ERROR - supplied max_size_circular_buffer -->"
				<< max_size_circular_buffer << "<-- "
				<< "cannot exceed size of max_size_audio_buffer -->"
				<< max_size_audio_buffer << "<--"
				<< std::endl;
		exit(8);
	}

	initialize_from_constructor();
}

Audio_Model::Audio_Model() {	// constructor

	max_size_circular_buffer = 8192;	// give value since not using constructor which has parm

	initialize_from_constructor();
}

Audio_Model::~Audio_Model() {
	// TODO Auto-generated destructor stub
}


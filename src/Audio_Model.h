/*
 * Audio_Model.h
 *
 *  Created on: Dec 12, 2013
 *      Author: stens
 */

#ifndef AUDIO_MODEL_H_
#define AUDIO_MODEL_H_

#include "Common_Utilities.h"
#include "Circular_Buffer.h"

#include <iostream>		// std::cout
#include <map>		// std::map
#include <fstream>	// std::string
#include <climits>
#include <math.h>	// sin cos tan pow   M_E     M_PI

// -------------

static const int X = 0;		// XYZ index
static const int Y = 1;
static const int Z = 2;


class Audio_Model {

public:

	Audio_Model();
	Audio_Model(int MAX_SIZE_CIRCULAR_BUFFER);

	virtual ~Audio_Model();

	// -----------

	void initialize();


	bool get_flag_continue_processing();


	int get_sizeof_paddle_location();

	void * get_address_paddle_location();

	void update_location_molecules(int count_num_molecular_update_chronos);

	int get_sizeof_paddle_neighbor_indices();

	void * get_address_paddle_neighbor_indices();

	int get_curr_paddle_neighbor_index();

	void set_circular_buffer(Circular_Buffer * given_circular_buffer);

	void set_audio_buffer(int given_aac_index, int given_buffer_size, int given_sample_rate, short * given_buffer);

	// ---

	int get_sizeof_audio_buffer();

	void * get_address_audio_buffer();

	int get_sizeof_audio_buffer_indices();

	void * get_address_audio_buffer_indices();

	int get_curr_audio_buffer_indices_index();

private:

	float max_value;
	float min_value;

	void initialize_from_constructor();

	int buffer_size;
	int sample_rate;

	int aac_index;
	int previous_aac_index;

	int max_size_circular_buffer;

	short * local_lpcm_buffer;

	// ------------------

	Circular_Buffer * circular_buffer;

	Common_Utilities * common_utilities;

	void pop_paddle();

	static const int max_paddle_locations = 10;

	float paddle_location[max_paddle_locations][num_dimensions_3D];
	int curr_paddle_location_index;

	static const int max_paddle_neighbors = 100;

	int paddle_neighbor_indices[max_paddle_locations * max_paddle_neighbors * 2];
	int curr_paddle_neighbor_index;

	// -------------

	int most_recently_visualized_aac_index;

//  chart of powers of 2
//
//				2^9  512
//				2^10 1024
//				2^11 2048
//				2^12 4096
//				2^13 8192
//				2^14 16384
//				2^15 32768
//				2^16 65536
//				2^17 131072
//				2^18 262144
//				2^19 524288
//				2^20 1048576
//				2^21 2097152
//				2^22 4194304
//				2^23 8388608
//				2^24 16777216

	static const int max_size_audio_buffer = 65536;	// increase if necessary see above chart

	float audio_buffer[max_size_audio_buffer][num_dimensions_3D];
//	int curr_audio_buffer_index;

	int audio_buffer_indices[max_size_audio_buffer * 2];
	int curr_audio_buffer_indices_index;

	// -------------

	bool flag_continue_processing;

	// -----------------------------------------------------------------------------

	void * ptr_to_array_paddle_location;

	void * ptr_to_array_paddle_neighbor_indices;

	void * ptr_to_array_audio_buffer;

	void * ptr_to_array_audio_buffer_indices;
};

#endif /* AUDIO_MODEL_H_ */

/*
 * Circular_Buffer.h
 *
 *  Created on: Jun 5, 2013
 *      Author: stens
 */

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <iostream>
#include <map>
#include <string>

//#include "Utility.h"	// was originally in    cpp_openal_20130819_06_10
#include "Audio_Utility.h"	// stens TODO - use c++ templates instead of this class
#include "Common_Utilities.h"



static const int currently_unavailable_circ_ring = -1;

static const int MINIMUM_CIRCULAR_BUFFERS_OFF_LIMITS = 2;


class Circular_Buffer {


public:

	Circular_Buffer();
	virtual ~Circular_Buffer();

	// -------------------------------------------

    static std::map <int, std::string> create_map() {   // used for visualization in print logging ONLY

        std::map <int, std::string> m;
        m[0] = "------ production_eligible ";
        m[1] = "////// stage_production    ";
        m[2] = "|||||| production_complete ";
        m[3] = "\\\\\\ stage_consumption   ";

        return m;
    }

    std::map<int, std::string> b_n;

	void show_dict();



	int get_max_size_circular_buffer();

	// -----------  consumption  -----------------

	bool execute_consumption(struct_audio_array * given_ptr_curr_audio_struct);

	struct_audio_array * do_stage_consumption();

	bool is_consumption_possible();

	// -----------  production   -----------------

	bool execute_production(struct_audio_array * given_ptr_curr_audio_struct);

	int get_next_avail_circular_buffer_production_index();

	struct_audio_array * do_stage_production();

	bool is_production_possible();

	void show_buffer();

//	void init(int);

	void init_allocate(int given_max_number_circular_buffers, int given_max_size_circular_buffer);

	bool get_did_queue_ever_fill();

	void set_is_production_complete(bool given_is_production_complete);

private:

    int max_size_circular_buffer;
    int max_number_circular_buffers;

    int avail_circular_buffers_production;
    int next_avail_circular_buffer_production_index;

    int avail_circular_buffers_consumption;
    int next_avail_circular_buffer_consumption_index;

    std::map <int, struct_audio_array * > dict_index_to_circular_ring_buffer;

    bool has_any_production_happened;
    bool did_queue_ever_fill;
    bool is_production_complete;
};

#endif /* CIRCULAR_BUFFER_H_ */

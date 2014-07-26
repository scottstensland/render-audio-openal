/*
 * Circular_Buffer.cpp
 *
 *  Created on: Jun 5, 2013
 *      Author: stens
 */

#include "Circular_Buffer.h"
#include <stdlib.h>     // malloc and free

//#include <stdio.h>
 #include <iomanip>  // setw(5)


void Circular_Buffer::set_is_production_complete(bool given_is_production_complete) {

	is_production_complete = given_is_production_complete;
}

bool Circular_Buffer::get_did_queue_ever_fill() {

	return did_queue_ever_fill;
}

int Circular_Buffer::get_max_size_circular_buffer() {

	return max_size_circular_buffer;
}

bool Circular_Buffer::execute_consumption(struct_audio_array * given_ptr_curr_audio_struct) {

	show_dict();

	// ------------

    struct_audio_array * ptr_curr_audio_struct = NULL;

    if (avail_circular_buffers_consumption == currently_unavailable_circ_ring ||
        avail_circular_buffers_consumption < 1) {

        printf("Circular_Ring execute_consumption NOTICE - avail_circular_buffers_consumption < 1     so EXIT EARLY\n");

        return false;   // return NULL pointer in this warning conditon
    }

    // -------------

    std::map<int, struct_audio_array *>::iterator curr_iter =
    		dict_index_to_circular_ring_buffer.find(next_avail_circular_buffer_consumption_index);

    if (curr_iter == dict_index_to_circular_ring_buffer.end()) {

        printf("Circular_Ring::execute_consumption  did NOT find key of %d so return NULL     so EXIT EARLY\n",
               next_avail_circular_buffer_consumption_index);

        return false;   // did NOT find key so return NULL
    }

    ptr_curr_audio_struct = curr_iter->second;

    // -------------

    if (ptr_curr_audio_struct->array_condition != stage_consumption) {

        printf("Circular_Ring execute_consumption NOTICE - array_condition was NOT stage_consumption, instead was %d     so EXIT EARLY\n",
               ptr_curr_audio_struct->array_condition);

        return false;
    }

    ptr_curr_audio_struct->array_condition = production_eligible;

    // -------------

    next_avail_circular_buffer_consumption_index++;

    if (next_avail_circular_buffer_consumption_index == max_number_circular_buffers) {

    	std::cout << "next_avail_circular_buffer_consumption_index reached MAXXX to set to 00000000000"
    			<< std::endl;

        next_avail_circular_buffer_consumption_index = 0;
    }

    avail_circular_buffers_consumption--;

    if (avail_circular_buffers_production == currently_unavailable_circ_ring) {

        avail_circular_buffers_production = 1;
        next_avail_circular_buffer_production_index = 0;

    } else {

        avail_circular_buffers_production++;
    }

    // ---------------

    return true;

}   //  execute_consumption







struct_audio_array * Circular_Buffer::do_stage_consumption() {

    struct_audio_array * ptr_curr_audio_struct = NULL;

    if (avail_circular_buffers_consumption == currently_unavailable_circ_ring ||
        avail_circular_buffers_consumption < 1) {

        printf("Circular_Ring do_stage_consumption NOTICE - avail_circular_buffers_consumption < 1    so EXIT EARLY\n");

        return ptr_curr_audio_struct;   // return NULL pointer in this warning conditon
    }

    // -------------

    std::cout << "next_avail_circular_buffer_consumption_index == "
    		<< next_avail_circular_buffer_consumption_index
    		<< std::endl;

    std::map<int, struct_audio_array *>::iterator curr_iter =
    		dict_index_to_circular_ring_buffer.find(next_avail_circular_buffer_consumption_index);

    if (curr_iter == dict_index_to_circular_ring_buffer.end()) {

        printf("Circular_Ring::do_stage_consumption  did NOT find key of %d so return NULL     so EXIT EARLY\n",
               next_avail_circular_buffer_consumption_index);

        std::cout << "about to show contents of dict_index_to_circular_ring_buffer"
        		<< std::endl;

        show_dict();

        std::cout << "end of show_dict" << std::endl;

//        dict_index_to_circular_ring_buffer.

        return ptr_curr_audio_struct;   // did NOT find key so return NULL
    }

    ptr_curr_audio_struct = curr_iter->second;

    // ------------------

    if (ptr_curr_audio_struct->array_condition != production_complete) {

        printf("Circular_Ring do_stage_consumption NOTICE - array_condition was NOT production_available, instead was %d     so EXIT EARLY\n",
               ptr_curr_audio_struct->array_condition);

        return NULL;
    }

    ptr_curr_audio_struct->array_condition = stage_consumption;

    // ------------------

    return ptr_curr_audio_struct;

}   //  do_stage_consumption

bool Circular_Buffer::is_consumption_possible() {

	std::cout << "avail_circular_buffers_consumption "
			<< avail_circular_buffers_consumption
			<< std::endl;


	std::cout << "MINIMUM_CIRCULAR_BUFFERS_OFF_LIMITS "
			<< MINIMUM_CIRCULAR_BUFFERS_OFF_LIMITS
			<< std::endl;

    bool answer;

    if (is_production_complete) {

    	answer = (avail_circular_buffers_consumption > 0);

    	std::cout << "Since is_production_complete is true ... permit depletion of all buffers"
    			<< std::endl; // no need to retain cache of unused buffers since production
    						  // is complete
    } else {

    	answer = (avail_circular_buffers_consumption > MINIMUM_CIRCULAR_BUFFERS_OFF_LIMITS);
    }

// ------------

    printf("Circular_Ring::is_consumption_possible post min off limit check with answer %s\n", answer ? "YES" : "NO");

//    if (answer == false) show_dict();

    show_dict();

    return (answer);
}

// -----------------------------------------------------------------

void Circular_Buffer::show_dict() {

	std::cout << "Circular_Buffer::show_dict TOP" << std::endl;

    std::map<int, struct_audio_array *>::iterator curr_iter;

    for (curr_iter = dict_index_to_circular_ring_buffer.begin();
         curr_iter != dict_index_to_circular_ring_buffer.end(); curr_iter++) {

        std::cout << "Circular_Buffer " << curr_iter->first
//        		<< " address " << curr_iter->second->ptr_to_array
		<< "_pppossible";

        std::cout << b_n[curr_iter->second->array_condition];  // retrieves condition from map

        std::cout << " aac_index -->" << std::setw(3)
        		<<  curr_iter->second->aac_index;


        std::cout << " last -->" <<  (YES==curr_iter->second->flag_last_buffer?"YES":"NO");

        if (curr_iter->first == next_avail_circular_buffer_production_index) {

        	std::cout << "<-- next production";
        }

        if (curr_iter->first == next_avail_circular_buffer_consumption_index) {

        	std::cout << "<-- next consumption";
        }

        std::cout << "<--  ";
        std::cout << std::endl;
    }
}

// -------------------------  consumption above ... production below -------------------

bool Circular_Buffer::execute_production(struct_audio_array * given_ptr_curr_audio_struct) {

	show_dict();

	// ---------------

    struct_audio_array * ptr_curr_audio_struct = NULL;

    if (avail_circular_buffers_production == currently_unavailable_circ_ring ||
        avail_circular_buffers_production < 1) {

        printf("Circular_Ring::execute_production NOTICE - avail_circular_buffers_production < 1 : %d    so EXIT EARLY\n",
               avail_circular_buffers_production);

        return false;

    } else {

        std::map<int, struct_audio_array *>::iterator curr_iter =
        		dict_index_to_circular_ring_buffer.find(get_next_avail_circular_buffer_production_index());

        if (curr_iter == dict_index_to_circular_ring_buffer.end()) {

            return false;   // did NOT find key
        }

        ptr_curr_audio_struct = curr_iter->second;

        if (given_ptr_curr_audio_struct->ptr_to_array != ptr_curr_audio_struct->ptr_to_array) {

            printf("Circular_Ring::execute_production ERROR - failed to see a match on array addresses     so EXIT EARLY\n");
            return false;
        }

        if (ptr_curr_audio_struct->array_condition != stage_production) {

            printf("Circular_Ring::execute_production NOTICE - array_condition is NOT staging_to_populate, instead is %d     so EXIT EARLY\n",
                   ptr_curr_audio_struct->array_condition);

            return false;
        }

        ptr_curr_audio_struct->array_condition         = production_complete;
        ptr_curr_audio_struct->ptr_struct_sample_specs = given_ptr_curr_audio_struct->ptr_struct_sample_specs;
        ptr_curr_audio_struct->aac_index               = given_ptr_curr_audio_struct->aac_index;

        std::cout << "\nCB ep      flag_last_buffer " << given_ptr_curr_audio_struct->flag_last_buffer;
        std::cout << "\nCB ep             aac_index " << given_ptr_curr_audio_struct->aac_index;
        std::cout << "\nCB ep           buffer_size " << ptr_curr_audio_struct->ptr_struct_sample_specs->buffer_size;
        std::cout << "\nCB ep             frequency " << ptr_curr_audio_struct->ptr_struct_sample_specs->frequency;
        std::cout << "\nCB ep sample_length_seconds " << ptr_curr_audio_struct->ptr_struct_sample_specs->sample_length_seconds;
        std::cout << "\nCB ep           sample_rate " << ptr_curr_audio_struct->ptr_struct_sample_specs->sample_rate;
        std::cout << "\nCB ep          num_channels " << ptr_curr_audio_struct->ptr_struct_sample_specs->num_channels;

        std::cout << std::endl;

        next_avail_circular_buffer_production_index++;

        if (next_avail_circular_buffer_production_index == max_number_circular_buffers) {

            next_avail_circular_buffer_production_index = 0;
        }

        avail_circular_buffers_production--;

        if (avail_circular_buffers_consumption == currently_unavailable_circ_ring) {

            avail_circular_buffers_consumption = 1;
            next_avail_circular_buffer_consumption_index = 0;

            std::cout << "just set next_avail_circular_buffer_consumption_index to 0000000000000"
            		<< std::endl;

        } else {

            avail_circular_buffers_consumption++;
        }
    }

    has_any_production_happened = true;

    return true;

}	//	execute_production

int Circular_Buffer::get_next_avail_circular_buffer_production_index() {

    return next_avail_circular_buffer_production_index;
}

struct_audio_array * Circular_Buffer::do_stage_production() {

	struct_audio_array * ptr_curr_audio_struct = NULL;

    if (avail_circular_buffers_production == currently_unavailable_circ_ring ||
        avail_circular_buffers_production < 1) {

        printf("Circular_Ring do_stage_production NOTICE - avail_circular_buffers_production < 1 : %d     so EXIT EARLY\n",
               avail_circular_buffers_production);

        return ptr_curr_audio_struct;   // error condition just return NULL pointer

    } else {

        std::map<int, struct_audio_array *>::iterator curr_iter =
        		dict_index_to_circular_ring_buffer.find(get_next_avail_circular_buffer_production_index());

        if (curr_iter == dict_index_to_circular_ring_buffer.end()) {

            printf("Circular_Ring::do_stage_production  did NOT find key of %d so return NULL     so EXIT EARLY\n",
                   get_next_avail_circular_buffer_production_index());

            return ptr_curr_audio_struct;   // did NOT find key so return NULL
        }

        ptr_curr_audio_struct = curr_iter->second;

        if (ptr_curr_audio_struct->array_condition != production_eligible) {

            printf("Circular_Ring do_stage_production NOTICE - array_condition was NOT available_to_populate, instead was %d     so EXIT EARLY\n",
                   ptr_curr_audio_struct->array_condition);

            return NULL;
        }

        ptr_curr_audio_struct->array_condition = stage_production;
    }

    return ptr_curr_audio_struct;
}

bool Circular_Buffer::is_production_possible() {

	std::cout << "avail_circular_buffers_production " << avail_circular_buffers_production
			<< std::endl;

    bool answer = avail_circular_buffers_production > MINIMUM_CIRCULAR_BUFFERS_OFF_LIMITS;

    // ------------  maintain number of buffers populated yet off limits to production

    printf("Circular_Ring::is_production_possible   answer is %s\n", answer ? "YES" : "NO");

    if (has_any_production_happened) {

    	did_queue_ever_fill = true;
    }

    return (answer);
}

void Circular_Buffer::show_buffer() {

	std::map<int, struct_audio_array *>::iterator curr_iter;
}

void Circular_Buffer::init_allocate(int given_max_number_circular_buffers, int given_max_size_circular_buffer) {

	max_number_circular_buffers = given_max_number_circular_buffers;

	max_size_circular_buffer = given_max_size_circular_buffer;

	std::cout << "init_allocate max_number_circular_buffers " << max_number_circular_buffers << std::endl;
	std::cout << "init_allocate    max_size_circular_buffer " << max_size_circular_buffer << std::endl;

	avail_circular_buffers_production = 0;
	avail_circular_buffers_consumption = 0;

	std::cout << "CB init" << std::endl;

    for (int curr_buffer = 0; curr_buffer < max_number_circular_buffers; curr_buffer++) {

    	std::cout << "CB adding curr_buffer " << curr_buffer << std::endl;

		struct_audio_array * ptr_curr_audio_struct =
				(struct_audio_array *) malloc(sizeof (struct_audio_array));

		short * curr_lpcm_buffer =
				(short *) malloc(sizeof(short) * given_max_size_circular_buffer);

//        ptr_curr_audio_struct->buffer_size = given_max_size_circular_buffer;
//        ptr_curr_audio_struct->sample_rate = 0;					// stub value to be overridden later
		ptr_curr_audio_struct->ptr_to_array = curr_lpcm_buffer;
        ptr_curr_audio_struct->array_condition = production_eligible;
		ptr_curr_audio_struct->aac_index = curr_buffer;			// bogus just for TS
		ptr_curr_audio_struct->ptr_struct_sample_specs = NULL;	// place holder

		dict_index_to_circular_ring_buffer.insert(
						std::pair<int, struct_audio_array *>(curr_buffer, ptr_curr_audio_struct));

        std::cout << "Circular_Buffer inserted dict_index_to_circular_ring_buffer pair key "
        		<< curr_buffer << " value " << curr_lpcm_buffer << std::endl;


		avail_circular_buffers_production++;
    }

    next_avail_circular_buffer_production_index = 0;
	next_avail_circular_buffer_consumption_index = 0;

    has_any_production_happened = false;
    did_queue_ever_fill = false;
    is_production_complete = false;	// when true allow consumer to deplete all buffers
}


Circular_Buffer::Circular_Buffer() {

	std::cout << "CB constructor" << std::endl;


}

Circular_Buffer::~Circular_Buffer() {

	std::cout << "CB destructor" << std::endl;
}


/*
 * Consumer.cpp
 *
 *  Created on: Jun 5, 2013
 *      Author: stens
 */

#include "Consumer.h"

#include <iostream>
#include <thread>
#include <exception>
#include <mutex>

void Consumer::set_circular_buffer(Circular_Buffer * given_circular_buffer) {

	std::cout << "Consumer set_circular_buffer" << std::endl;

	circular_buffer = given_circular_buffer;
}


bool Consumer::get_next_available_memory_buffer(struct_audio_array * given_ptr_curr_audio_struct) {

//-(struct_audio_array *) get_next_available_memory_buffer {

    // stens TODO make following constant 1 into configurable input parameter JSON as cushion of # of processed aac to LPCM files

    std::cout << "Consumer  just inside get_next_available_file_path   vvvvvvvvvvvvv" << std::endl;

    if (NULL == circular_buffer) {

    	std::cout << "Consumer get_next_available_memory_buffer seeing NULL circular_buffer"
    			<< std::endl;

    	return false;
    }

    std::cout << "Consumer get_next_available_file_path  just after NULL check" << std::endl;


    //    printf("Consumer at TOP of get_next_available_memory_buffer ... about to call show\n");
    circular_buffer->show_dict();


    struct_audio_array * local_ptr_curr_audio_struct = NULL;

    if (! circular_buffer->is_consumption_possible()) {

    	std::cout << "Consumer NOTICE - NNNNNNNNNNNNNNNOOOO bad call to is_consumption_possible\n";

        return false;
    }

    std::cout << "Consumer  IIIIIIIINNN circular_ring->is_consumption_possible YES YES YES\n";

    local_ptr_curr_audio_struct = circular_buffer->do_stage_consumption();

    if (! local_ptr_curr_audio_struct) {

    	std::cout << "Consumer NOTICE - seeing NULL ptr_curr_audio_struct returned"
    			<< " from call to do_stage_consumption" << std::endl;

        return false;
    }

    if (! circular_buffer->execute_consumption(local_ptr_curr_audio_struct)) {

    	std::cout << "Consumer NOTICE - bad call to execute_consumption" << std::endl;

        return false;
    }

    given_ptr_curr_audio_struct->aac_index = local_ptr_curr_audio_struct->aac_index;
    given_ptr_curr_audio_struct->array_condition = local_ptr_curr_audio_struct->array_condition;
    given_ptr_curr_audio_struct->ptr_to_array = local_ptr_curr_audio_struct->ptr_to_array;

    given_ptr_curr_audio_struct->ptr_struct_sample_specs = local_ptr_curr_audio_struct->ptr_struct_sample_specs;

//    given_ptr_curr_audio_struct->buffer_size = local_ptr_curr_audio_struct->buffer_size;

    return true;

}   //  get_next_available_memory_buffer




void Consumer::outnum() {

	static std::mutex curr_mutex;

	// lock_guard   constructor/destructor   creates/destroys lock
	std::lock_guard <std::mutex> g( curr_mutex );

	std::string given_label = "Consumer do_consumption";

	std::cout << given_label << "[" << loop_counter << "]" << std::endl;
}

bool Consumer::get_next_consumable() {

//    	std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));

		if (! circular_buffer->is_consumption_possible()) {

			std::cout << "Consumer consume is NOT possible ... sleep N loop" << std::endl;

			std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));

			return false;
		}

//		std::cout << "Producer launch_loop ";

//		outnum("Producer launch_loop ", loop_counter++);

		loop_counter++;

//    	outnum();

		// ------------------------

		struct_audio_array * ptr_curr_audio_struct = NULL;

		ptr_curr_audio_struct = circular_buffer->do_stage_consumption();

		if (! ptr_curr_audio_struct) {

			std::cout << "Consumer NOTICE - seeing NULL ptr_curr_audio_struct "
					<< "returned from call to do_stage_consumption" << std::endl;

			return false;
		}

		// ------------- OK now just consume it ------------

		if (! circular_buffer->execute_consumption(ptr_curr_audio_struct)) {

			std::cout << "Consumer NOTICE - bad call to execute_consumption" << std::endl;

			return false;
		}

		// ------------ OK do something with the booty !!!!!!!!!!

		std::cout << "Consumer::do_consumption aac_index "
				<< ptr_curr_audio_struct->aac_index << std::endl;


		return true;

}

bool Consumer::do_consumption() {

	while (flag_continue_processing) {

		if (! get_next_consumable()) {

			return false;
		}
	}
	return true;
}

void Consumer::consume(Circular_Buffer * given_circular_buffer) {

	std::cout << "Consumer consume" << std::endl;

	circular_buffer = given_circular_buffer;

//    std::thread t2( &Consumer::do_consumption, this);

	while (flag_continue_processing) {

		if (do_consumption()) {

			std::cout << "OK just consumed item" << std::endl;

		} else {

			std::cout << "Consumer NOTICE  failed to consume ... sleep N loop" << std::endl;

	    	std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));
		}
	}
}


Consumer::Consumer() {

	loop_counter = 0;
	flag_continue_processing = true;
}

Consumer::~Consumer() {
	// TODO Auto-generated destructor stub
}


/*
 * Audio_Controller.cpp
 *
 *  Created on: Dec 12, 2013
 *      Author: stens
 */

#include "Audio_Controller.h"


void Audio_Controller::do_value_handovers_to_model() {

}

void Audio_Controller::do_value_handovers_to_view() {


	audio_view->set_point_size(size_point);
}

void Audio_Controller::entry_point() {

	std::cout << "TOP of void Audio_Controller::entry_point" << std::endl;

	audio_model->initialize();

	// --------------------------------------

	if (true == do_we_want_opengl) {

		std::cout << "YES we want OpenGL" << std::endl;

		// --------

		audio_view->entry_point();

	} else {	// running with NO OpenGL - headless

		std::cout << "running with NO OpenGL - headless" << std::endl;


		int count_num_molecular_update_chronos = 0;
		int curr_speed_counter = 0;

		// -------

		while (continue_processing) {

			// ---------------------------------------------

			audio_model->update_location_molecules(count_num_molecular_update_chronos);

			count_num_molecular_update_chronos++;
			curr_speed_counter++;

			continue_processing = audio_model->get_flag_continue_processing();

			// ----------- show chronos per second ---------

			if (curr_speed_counter >= 1000) {

				curr_speed_counter = 0;
				std::cout << "count_num_molecular_update_chronos " << count_num_molecular_update_chronos
						<< std::endl;
			}
		}
	}

	// ------
}

Audio_Controller::Audio_Controller(Audio_Model * given_audio_model, Audio_View * given_audio_view) {  // constructor

//	flag_do_entire_universe_or_just_paddle = true;	// true == only run paddle - ignore universe of other visuals
//	flag_do_entire_universe_or_just_paddle = false; // false == do everything in entire universe

	audio_model = given_audio_model;

	audio_view = given_audio_view;

	do_we_want_opengl = true;

	continue_processing = true;

	size_point = 1.0;
}

//Audio_Controller::Audio_Controller() {}

Audio_Controller::~Audio_Controller() {
	// TODO Auto-generated destructor stub
}


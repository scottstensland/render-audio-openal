/*
 * cpp_openal_opengl_dna.cpp
 *
 *  Created on: Dec 12, 2013
 *      Author: stens
 */

#include "cpp_openal_opengl_dna.h"


#include "Circular_Buffer.h"
#include "Stream_Render_Audio.h"


#include "Binary_File_IO.h"
#include <climits>

#include <thread>
#include <exception>
#include <mutex>

#include "Audio_Model.h"		// MVC
#include "Audio_View.h"			// MVC
#include "Audio_Controller.h"	// MVC

bool do_we_want_opengl = true;
//	bool do_we_want_opengl = false;


// ------------------------------------------


void pop_input_buffer(struct_input_file_read_pop_buffer * ptr_struct_input_file_read_pop_buffer,
		std::string given_chosen_file) {

	Binary_File_IO * read_binary_audio = new Binary_File_IO();

	read_binary_audio->init_file_read(given_chosen_file);

	std::cout << "cppopenalprod just did    init file" << std::endl;

	read_binary_audio->parse_wav_header_adaptive(
			& ptr_struct_input_file_read_pop_buffer->sample_rate,
			& ptr_struct_input_file_read_pop_buffer->bits_per_samp,
			& ptr_struct_input_file_read_pop_buffer->source_buffer_size,
			& ptr_struct_input_file_read_pop_buffer->num_channels);

//	read_binary_audio->parse_wav_header(
//			& ptr_struct_input_file_read_pop_buffer->sample_rate,
//			& ptr_struct_input_file_read_pop_buffer->bits_per_samp,
//			& ptr_struct_input_file_read_pop_buffer->source_buffer_size);

//	exit(8);

//	read_binary_audio->read_wav_header(
//			& ptr_struct_input_file_read_pop_buffer->sample_rate,
//			& ptr_struct_input_file_read_pop_buffer->bits_per_samp,
//			& ptr_struct_input_file_read_pop_buffer->source_buffer_size);

	std::cout << "cppopenal - pop_input_buffer   just did read_wav_header " << std::endl;

	std::cout << "coming out of header parse with  "
			<< " source_buffer_size " << ptr_struct_input_file_read_pop_buffer->source_buffer_size
			<< " num_channels " << ptr_struct_input_file_read_pop_buffer->num_channels
			<< std::endl;

	std::cout << "about to malloc memory for buffer" << std::endl;

	ptr_struct_input_file_read_pop_buffer->audio_buffer =
			(short int *) malloc((ptr_struct_input_file_read_pop_buffer->source_buffer_size) * sizeof(short int));

	/*
	read_binary_audio->read_wav_data(
			ptr_struct_input_file_read_pop_buffer->audio_buffer,
			ptr_struct_input_file_read_pop_buffer->sample_rate,
			ptr_struct_input_file_read_pop_buffer->bits_per_samp,
			ptr_struct_input_file_read_pop_buffer->source_buffer_size,
			PRINT_MODE);
	*/


	std::cout << "about to read_wav_data ...." << std::endl;

	read_binary_audio->read_wav_data(
			ptr_struct_input_file_read_pop_buffer->audio_buffer,
			ptr_struct_input_file_read_pop_buffer->sample_rate,
			ptr_struct_input_file_read_pop_buffer->bits_per_samp,
			ptr_struct_input_file_read_pop_buffer->source_buffer_size,
			PRINT_MODE);


	std::cout << ".... just resurfaced from call to  read_wav_data" << std::endl;

	/*
	if (true == ptr_struct_input_file_read_pop_buffer->do_we_want_RMS_calculated) {

		std::cout << "OK about to calculate root mean square of buffer" << std::endl;

		ptr_struct_input_file_read_pop_buffer->root_mean_square = Utility::calc_rms(
				ptr_struct_input_file_read_pop_buffer->audio_buffer,
				ptr_struct_input_file_read_pop_buffer->source_buffer_size,
				"raw input buffer"
				);

		std::cout << "calculated RMS " << ptr_struct_input_file_read_pop_buffer->root_mean_square << std::endl;
	}
	*/

}	//	pop_input_buffer



void launch_producer(Circular_Buffer * given_circular_buffer,
		struct_sample_specs * ptr_struct_sample_specs, std::string chosen_file) {

	std::cout << "cppopenal launch_producer  - here is chosen file " << chosen_file << std::endl;

	// -------------------

	struct_input_file_read_pop_buffer * ptr_struct_input_file_read_pop_buffer =
			(struct_input_file_read_pop_buffer *) malloc(sizeof (struct_input_file_read_pop_buffer));

	pop_input_buffer(ptr_struct_input_file_read_pop_buffer, chosen_file);

	// ------------

	Producer * producer = new Producer();

	Synthesize_Sound * synthesize_sound = new Synthesize_Sound();
	synthesize_sound->set_sample_specs(ptr_struct_sample_specs);

	std::cout << "minimum short int is " << SHRT_MIN << std::endl;
	std::cout << "maximum short int is " << SHRT_MAX << std::endl;

	std::cout << "launch_producer        audio_buffer  is " << (* ptr_struct_input_file_read_pop_buffer->audio_buffer) << std::endl;
	std::cout << "launch_producer source_buffer_size   is " << ptr_struct_input_file_read_pop_buffer->source_buffer_size << std::endl;
	std::cout << "launch_producer        sample_rate   is " << ptr_struct_input_file_read_pop_buffer->sample_rate << std::endl;
	std::cout << "launch_producer       num_channels   is " << ptr_struct_input_file_read_pop_buffer->num_channels << std::endl;

	producer->wrap_process(given_circular_buffer,
			synthesize_sound,
			ptr_struct_input_file_read_pop_buffer->audio_buffer,
			ptr_struct_input_file_read_pop_buffer->source_buffer_size,
			ptr_struct_input_file_read_pop_buffer->sample_rate,
			ptr_struct_input_file_read_pop_buffer->num_channels,
			WHICH_MODE_SIN_OR_PCM);

	std::cout << "Producer is Complete" << std::endl;

}	//	launch_producer

// ------------------------------

//void launch_mvc_visualization(Circular_Buffer * given_circular_buffer) {
void launch_mvc_visualization(Audio_Model * given_audio_model) {

//	Audio_Model * audio_model = new Audio_Model();

//	given_audio_model->set_circular_buffer(given_circular_buffer);

	Audio_View * audio_view = NULL;
	Audio_Controller * audio_controller = NULL;


	if (true == do_we_want_opengl) {	// show OpenGL graphics

		audio_view = new Audio_View(given_audio_model);

		audio_controller = new Audio_Controller(given_audio_model, audio_view);

//		audio_controller->set_flag_do_entire_universe_or_just_paddle(flag_do_entire_universe_or_just_paddle);

//		audio_controller->set_opengl_usage(true);	// enable OpenGL

//		audio_controller->set_full_screen(full_screen);

//		audio_controller->set_size_point(size_point);

//		audio_controller->set_do_air_logic(do_air_logic);

		audio_controller->do_value_handovers_to_view();

	} else {	// go headless - no OpenGL graphics - command line mode

		audio_controller = new Audio_Controller(given_audio_model, NULL);

//		audio_controller->set_flag_do_entire_universe_or_just_paddle(flag_do_entire_universe_or_just_paddle);

//		audio_controller->set_opengl_usage(false);	// disable OpenGL - going headless

//		audio_controller->set_do_air_logic(do_air_logic);

	}

	// --------------

//	audio_controller->set_output_sound_buffer_size(output_sound_sample_buffer);

//	audio_controller->set_flag_do_we_want_output_sound_file(do_we_want_output_sound_file);

	// -------------------------------------------------------------------
/*
	if (do_we_want_ear) {

		// -------

		audio_model->set_ear(ear);

		audio_model->set_ear_listening(do_we_want_ear);	// enable model to calculate whether each molecule intersects ear

		audio_model->set_output_sound_buffer_size(output_sound_sample_buffer);

		// ---

//		std::map<int, std::string> all_file_names = Common_Utilities::create_map();
//
//		audio_controller->set_sound_output_filename(all_file_names[WHICH_FILE]);


//		std::map<int, std::string> all_file_names = Common_Utilities::create_map();

//			audio_controller->set_sound_output_filename(Common_Utilities::create_map()[WHICH_FILE]);
		audio_controller->set_sound_output_filename(create_map()[WHICH_FILE_OUTPUT]);

		// ---

		audio_controller->set_normalize_flag(desire_to_normalize_sample_range);

		audio_model->set_desired_sound_amplitude_range(min_desired_sound_sample_value, max_desired_sound_sample_value);
	}
	*/

	audio_controller->do_value_handovers_to_model();

	audio_controller->entry_point();	// stens TODO put this into thread t1
}

void launch_audio_playback(Circular_Buffer * given_circular_buffer, Audio_Model * given_audio_model) {

	if (! given_circular_buffer) {

		std::cout << "ERROR main launch_consumer seeing NULL given_circular_buffer" << std::endl;

		exit(6);
	}

	// ----------   probe to see if circular buffer queue is full

	while (! given_circular_buffer->get_did_queue_ever_fill()) {

		std::cout << "cpp_openal_prod  consumer sees producer has not yet filled queue so sleep ..." << std::endl;

    	std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));
	}

	// ----------   above is tight loop OK --- below is new OpenAL -------------- //

	Stream_Render_Audio * stream_render_audio = new Stream_Render_Audio();

	stream_render_audio->SRA_openal(given_circular_buffer, given_audio_model);


//	std::cout << "cpp_openal_prod_cons_threaded end of launch_consumer" << std::endl;
}

int main() {

	std::cout << "hello Corinde" << std::endl; // prints hello Corinde

	all_file_names = create_map();

	Circular_Buffer * circular_buffer = new Circular_Buffer();

	circular_buffer->init_allocate(MAX_NUMBER_CIRCULAR_BUFFERS, MAX_SIZE_CIRCULAR_BUFFER);

	// -----------------------

	struct_sample_specs * ptr_struct_sample_specs =
			(struct_sample_specs *) malloc(sizeof (struct_sample_specs));

	// ------------------------

	std::thread t1(launch_producer, circular_buffer, ptr_struct_sample_specs,
			all_file_names[WHICH_FILE_INPUT]);

	Audio_Model * audio_model = new Audio_Model(MAX_SIZE_CIRCULAR_BUFFER);


	std::thread t2(launch_mvc_visualization, audio_model);	// OpenGL graphics visualization

	std::thread t3(launch_audio_playback, circular_buffer, audio_model);

	// -------------------------

	std::cout << "Corinde in main before join CASE 0" << std::endl;

	t1.join();
	t2.join();
	t3.join();

	std::cout << "at point Middleberg" << std::endl;

	// ----------

	return 0;
}

/*
cpp_openal_opengl_dna::cpp_openal_opengl_dna() {

}

cpp_openal_opengl_dna::~cpp_openal_opengl_dna() {

}
*/


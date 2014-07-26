/*
 * Stream_Render_Audio.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: stens
 */

#include "Stream_Render_Audio.h"

#include <thread>
#include <exception>
#include <mutex>

void Stream_Render_Audio::SRA_check_error(std::string given_label) {

	ALenum al_error;
	al_error = alGetError();
	std::string str_curr_error = alGetString(al_error);

	if(AL_NO_ERROR != al_error) {

		std::cout << "SRA ERROR in call to " << given_label <<
				" : " <<  str_curr_error << std::endl;
        exit(9);
	}

}	//	SRA_check_error

void Stream_Render_Audio::SRA_exit_al() {

	std::cout << "top of SRA_exit_al " << std::endl;

	// ALenum errorCode = 0;

/*
    ALCdevice *dev = NULL;
    ALCcontext *ctx = NULL;
    ctx = alcGetCurrentContext();
    dev = alcGetContextsDevice(ctx);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(ctx);
    alcCloseDevice(dev);
*/
    // -------------



	// Stop capture
//	alcCaptureStop(inputDevice);
//	alcCaptureCloseDevice(inputDevice);

	// Stop the sources
	alSourceStopv(1, & streaming_source[0]);		//		streaming_source

	for (int ii = 0; ii < 1; ++ii) {
		alSourcei(streaming_source[ii], AL_BUFFER, 0);
	}
	// Clean-up
	alDeleteSources(1, &streaming_source[0]);
	alDeleteBuffers(16, &streaming_source[0]);
	// errorCode = alGetError();
	alcMakeContextCurrent(NULL);
	// errorCode = alGetError();
	alcDestroyContext(openal_output_context);
	alcCloseDevice(openal_output_device);

}

bool Stream_Render_Audio::get_next_consumable() {

	std::cout << "SRA get_next_consumable TOP" << std::endl;

	bool flag1 = true;
//	bool flag1 = false;

	if (flag1 && flag_received_last_buffer) {

		std::cout << "already saw last buffer so sleep N return" << std::endl;

		std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));

		return true;
	}

	sra_circular_buffer->show_dict();

	std::cout << "about to ask if IS consumption possible ??????????????????"
			<< std::endl;

	if (! sra_circular_buffer->is_consumption_possible()) {

		std::cout << "SRA get_next_consumable   is_consumption_possible NO NO NO  ... sleep N loop" << std::endl;

		std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));

		return false;
	}

	std::cout << "so YES consumption IS possible"
		<< std::endl;

	// ------------------------

	struct_audio_array * ptr_curr_audio_struct = NULL;

	ptr_curr_audio_struct = sra_circular_buffer->do_stage_consumption();

	if (! ptr_curr_audio_struct) {

		std::cout << "SRA NOTICE -   get_next_consumable  seeing NULL ptr_curr_audio_struct "
				<< "returned from call to do_stage_consumption" << std::endl;

		return false;
	}

	// ------------- OK do something with the booty !!!!!!!!!!

	buffer_size = ptr_curr_audio_struct->ptr_struct_sample_specs->buffer_size;
	sample_rate = ptr_curr_audio_struct->ptr_struct_sample_specs->sample_rate;
	num_channels = ptr_curr_audio_struct->ptr_struct_sample_specs->num_channels;
	aac_index = ptr_curr_audio_struct->aac_index;

	local_lpcm_buffer = ptr_curr_audio_struct->ptr_to_array;

	flag_last_buffer = ptr_curr_audio_struct->flag_last_buffer;

	if (YES == flag_last_buffer && false == flag_received_last_buffer) {

		last_aac_index = aac_index;

		flag_received_last_buffer = true;

		std::cout << "seeing flag_last_buffer    last_aac_index "
				<< last_aac_index << std::endl;
	}

	if (! sra_circular_buffer->execute_consumption(ptr_curr_audio_struct)) {

		std::cout << "SRA NOTICE - get_next_consumable bad call to execute_consumption" << std::endl;

		return false;
	}

	// ------------ OK

	std::cout << "SRA::get_next_consumable OK at END aac_index -->"
			<< ptr_curr_audio_struct->aac_index
			<< "<-- "
			<< std::endl;

	return true;

}	//	get_next_consumable

void Stream_Render_Audio::get_next_buffer_to_stream() {

	std::cout << "SRA top get_next_buffer_to_stream " << std::endl;

	if (false == flag_continue_processing) {

		std::cout << "SRA get_next_buffer_to_stream seeing FALSE "
				<< "flag_continue_processing so return" << std::endl;
		return;
	}

	if (flag_received_last_buffer) {

		buffer_count_down--;

		std::cout << "TRUE flag_received_last_buffer so sleep N jump over buffer_count_down "
				<< buffer_count_down
				<< std::endl;

		if (-7 == buffer_count_down) {

			std::cout << "buffer_count_down is 0000 so stop" << std::endl;

			exit(8);
		}

//    	std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));

//		return;		// stens TODO - corinde1
	}

	if (! get_next_consumable()) {

		std::cerr << "\nSRA ERROR in Stream_Render_Audio::get_next_buffer_to_stream  ... sleep "
				<< std::endl << std::endl;

    	std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));
	}
}

void Stream_Render_Audio::SRA_stream_render_init_openal() {

//	curr_vector_index = 0;

	flag_continue_processing = true;
	flag_received_last_buffer = false;
	flag_reached_end_of_input_data = false;

	buffer_count_down = MAX_OPENAL_QUEUE_BUFFERS;

    openal_output_device = alcOpenDevice(NULL);

    if (openal_output_device != NULL) {

        // create context

        openal_output_context = alcCreateContext(openal_output_device, 0);

        if (openal_output_context != NULL) {

            // activate this new context

            alcMakeContextCurrent(openal_output_context);

        } else {

            printf("SRA ERROR - failed to create context\n");
            exit(8);
        }

    } else {

    	printf("SRA ERROR - failed to get audio device\n");
    	exit(8);
    }

    // allocate the buffer array to given number of buffers
    alGenBuffers(MAX_OPENAL_QUEUE_BUFFERS, available_AL_buffer_array);

//    alGenSources(1, & streaming_source);
    alGenSources(1, & streaming_source[0]);

//    printf("SRA  streaming_source starts with %u\n", streaming_source);
    printf("SRA  streaming_source starts with %u\n", streaming_source[0]);

    available_AL_buffer_array_curr_index = 0;

    std::cout << "SRA  initialization of available_AL_buffer_array_curr_index to "
    		<< available_AL_buffer_array_curr_index << std::endl;

    previous_local_lpcm_buffer = 0;

    get_next_buffer_to_stream();

}	//	SRA_stream_render_init_openal

bool Stream_Render_Audio::SRA_stream_render_buffers() {

    std::cout << "\n----------------- SRA   inside SRA_stream_render_buffers" << std::endl;


    std::cout << "available_AL_buffer_array_curr_index "
    		<< available_AL_buffer_array_curr_index << std::endl;
    // -----------------------------
/*
	ALint early_buffers_queued = 0;


	alGetSourcei(streaming_source, AL_BUFFERS_QUEUED, & early_buffers_queued);
	SRA_check_error("alGetSourcei AL_BUFFERS_QUEUED");

	std::cout << "SRA early_buffers_queued is " << early_buffers_queued << std::endl;
*/


    // -----------------------------

    // UN queue used buffers

    ALint buffers_processed = 0;

    // get source parameter num used buffs
//    alGetSourcei(streaming_source, AL_BUFFERS_PROCESSED, & buffers_processed);
    alGetSourcei(streaming_source[0], AL_BUFFERS_PROCESSED, & buffers_processed);


    std::cout << "SRA fresh buffers_processed of " << buffers_processed << std::endl;

    while (buffers_processed > 0) {     // we have a consumed buffer so we need to replenish

        printf("SRA    inner_run seeing consumed buffer\n");

        ALuint unqueued_buffer;

//        alSourceUnqueueBuffers(streaming_source, 1, & unqueued_buffer);
        alSourceUnqueueBuffers(streaming_source[0], 1, & unqueued_buffer);

        printf("SRA     about to decrement available_AL_buffer_array_curr_index from OLD %d unqueued_buffer %d  ",
               available_AL_buffer_array_curr_index,
               unqueued_buffer);

        // corinde2

        available_AL_buffer_array_curr_index--;

        printf("SRA      to NEW %d  with unqueued_buffer %d\n",
               available_AL_buffer_array_curr_index,
               unqueued_buffer);

        available_AL_buffer_array[available_AL_buffer_array_curr_index] = unqueued_buffer;

        buffers_processed--;
    }

    // queue UP fresh buffers

    if (available_AL_buffer_array_curr_index >= MAX_OPENAL_QUEUE_BUFFERS) {

    	std::cout << "SRA internal openal queue is full ... so sleep" << std::endl;

		std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));

    } else {

		ALuint curr_audio_buffer = available_AL_buffer_array[available_AL_buffer_array_curr_index];

		std::cout << "SRA curr_audio_buffer of " << curr_audio_buffer << std::endl;

		if (local_lpcm_buffer == previous_local_lpcm_buffer) {

			std::cout << "SRA need to throttle up openal sleep duration seeing same value "
					<< "for local_lpcm_buffer %d - so will skip loading into alBufferData" << std::endl;
		} else {

			// upload buffer to OpenAL

			std::cout << "SRA about to upload into OpenAL another buffer "
					<< " buffer_size " << buffer_size
					<< " sample_rate " << sample_rate
					<< " aac_index " << aac_index
					<< std::endl;

			if (NULL != audio_model) {

//				std::cout << "about to transfer audio data from openal into opengl ..." << std::endl;
//				for (int curr_byte = 0; curr_byte < buffer_size; curr_byte++) {
//
//					fprintf(stdout, "%d pree copy orig %d\n",
//							curr_byte,
//							local_lpcm_buffer[curr_byte]);
//				}

				std::cout << "about to call set audio buffer   with buffer_size "
						<< buffer_size << std::endl;

				audio_model->set_audio_buffer(aac_index, buffer_size, sample_rate, local_lpcm_buffer);

				std::cout << "finished transfer of audio data from openal into opengl ..." << std::endl;
			}

			if (1 == num_channels) {

				std::cout << "SRA OpenAL mono" << std::endl;

				alBufferData( curr_audio_buffer, AL_FORMAT_MONO16, local_lpcm_buffer, buffer_size, sample_rate);// OK

			} else if (2 == num_channels) {

				std::cout << "SRA OpenAL stereo" << std::endl;

				alBufferData( curr_audio_buffer, AL_FORMAT_STEREO16, local_lpcm_buffer, buffer_size, sample_rate);
			}

//			alBufferData( curr_audio_buffer, AL_FORMAT_STEREO8, local_lpcm_buffer, buffer_size, sample_rate);
			SRA_check_error("alBufferData");

			std::cout << "SRA post alBufferData" << std::endl;

/*
			//   Sound samples: format specifier
			#define AL_FORMAT_MONO8                           0x1100
			#define AL_FORMAT_MONO16                          0x1101
			#define AL_FORMAT_STEREO8                         0x1102
			#define AL_FORMAT_STEREO16                        0x1103
*/

            previous_local_lpcm_buffer = local_lpcm_buffer;

//			alSourceQueueBuffers(streaming_source, 1, & curr_audio_buffer);
			alSourceQueueBuffers(streaming_source[0], 1, & curr_audio_buffer);
			SRA_check_error("alSourceQueueBuffers");

			available_AL_buffer_array_curr_index++;
		}

		ALenum current_playing_state;

		// get source parameter STATE

//		alGetSourcei(streaming_source, AL_SOURCE_STATE, & current_playing_state);
		alGetSourcei(streaming_source[0], AL_SOURCE_STATE, & current_playing_state);
		SRA_check_error("alGetSourcei AL_SOURCE_STATE");


		if (AL_PLAYING != current_playing_state) {

			std::cout << "SRA AL_PLAYING is NOT playing" << std::endl;

			if (flag_received_last_buffer) {

				std::cout << "currently NOT playing and saw last buffer so stop"
						<< std::endl;



				std::cout << "about to set flag_continue_processing to FALSE" << std::endl;

				flag_continue_processing = false;

//				uuuuuu

				return true;
			}

			ALint buffers_queued = 0;

			// get source parameter num queued buffs

//			alGetSourcei(streaming_source, AL_BUFFERS_QUEUED, & buffers_queued);
			alGetSourcei(streaming_source[0], AL_BUFFERS_QUEUED, & buffers_queued);
			SRA_check_error("alGetSourcei AL_BUFFERS_QUEUED");

			std::cout << "SRA buffers_queued is " << buffers_queued << std::endl;

			if (buffers_queued > 0) {

				// restart play

				printf("SRA  about to restart play\n");

//				alSourcePlay(streaming_source);
				alSourcePlay(streaming_source[0]);
				SRA_check_error("alSourcePlay");
			}
		}

	//	about to add call to OK   about to progress streaming by calling get_next_buffer_to_stream
	//    at bottom of render method

		std::cout << "SRA   about to probe for stop conditions with "
				<< "available_AL_buffer_array_curr_index "
				<<  available_AL_buffer_array_curr_index << std::endl;

//		if (available_AL_buffer_array_curr_index == 0) {

//		if (last_aac_index == aac_index && available_AL_buffer_array_curr_index == 0) {
		if (last_aac_index <= aac_index && available_AL_buffer_array_curr_index == 0) {

			printf("SRA    reached end of event tell parent\n");

	//        [self send_running_condition_message_to_parent: rendered_last_buffer];

//			flag_continue_processing = false; // terminate since all rendering work is done
			flag_reached_end_of_input_data = true;

			std::cout << "\n\n\tSRA NOTICE - about to exit - remove this exit once proper end logic is worked in\n\n"
					<< std::endl;

			flag_continue_processing = false;

//			exit(8);	// stens TODO remove this exit its stub until proper end logic is worked in

		} else {

			printf("SRA    about to call get_next_buffer_to_stream\n");

//			if (false == flag_received_last_buffer) {

			get_next_buffer_to_stream();
//			}
		}

		printf("SRA    SR_stream_render_buffers about to sleep for %d seconds ...\n",
				SLEEP_CONSUME_01);


//    sleep(seconds);

//    usleep(1000);
//    sleep(1);

//		std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));
		std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));
    }

	return true;

}   //  SR_stream_render_buffers


//void Stream_Render_Audio::SRA_openal(Circular_Buffer * given_circular_buffer) {
void Stream_Render_Audio::SRA_openal(Circular_Buffer * given_circular_buffer,
										Audio_Model * given_audio_model) {

	std::cout << "SRA just inside SRA_openal" << std::endl;

	audio_model = given_audio_model;

	// assert producer has filled queue or at least has some items in queue prior to
	// reaching this point - or openal logic will starve - below sleep now has logic upstream in calling logic

//	std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));
//	std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));
//	std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));


	if (! given_circular_buffer) {

		std::cout << "ERROR SRA given_circular_buffer seeing NULL given_circular_buffer" << std::endl;

	}

	sra_circular_buffer = given_circular_buffer;

    SRA_stream_render_init_openal();

	std::cout << "SRA SRA_openal about to enter LOOP " << std::endl;


//	int given_loop_limit = 5;
//    do {
//
//		SRA_stream_render_buffers();
//
//	} while (given_loop_limit-- > 0);


	while (flag_continue_processing) {

		if (SRA_stream_render_buffers()) {

			std::cout << "SRA OK just rendered an item" << std::endl;

		} else {

			std::cout << "SRA NOTICE  failed to consume ... sleep N loop" << std::endl;

	    	std::this_thread::sleep_for (std::chrono::milliseconds( SLEEP_CONSUME_01));
		}
	}

	// Dealloc OpenAL

	std::cout << "about to stop openal" << std::endl;

	SRA_exit_al();		// only needed for SR_stream_render_buffers

}	//	SRA_openal

Stream_Render_Audio::Stream_Render_Audio() {

	audio_model = NULL;
}

Stream_Render_Audio::~Stream_Render_Audio() {

}


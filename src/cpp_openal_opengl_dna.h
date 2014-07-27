/*
 * cpp_openal_opengl_dna.h
 *
 *  Created on: Dec 12, 2013
 *      Author: stens
 */

#ifndef CPP_OPENAL_OPENGL_DNA_H_
#define CPP_OPENAL_OPENGL_DNA_H_


#include "Common_Utilities.h"
#include "Producer.h"
//#include <unistd.h>

#include <map>

static const int PRINT_MODE = NO;

static const int WHICH_MODE_SIN_OR_PCM = SYNTH_MODE_PCM_FILE;

static const int WHICH_FILE_OUTPUT = 14; //  sound_buffer_output.pcm


static const int WHICH_FILE_INPUT  = 2; // audio/binaural_stereo_2_ch.wav
// static const int WHICH_FILE_INPUT  = 18; // audio/binaural_stereo_2_ch.wav
//static const int WHICH_FILE_INPUT  = 17; // Die_Antwoord_11_doong_doong_1_sec
//static const int WHICH_FILE_INPUT  = 4; // /home/stens/Justice_Genesis_chewy_chocolate_cookies_gtZunGHG0ls_mono.wav
//static const int WHICH_FILE_INPUT  = 15; //   /home/stens/Lee_Smolin_Physics_Envy_and_Economic_Theory-cWn86ESze6M_mono_1st_few_seconds.wav


static std::map <int, std::string> create_map() {   // used for visualization in print logging ONLY

		std::map <int, std::string> m;

		m[0] = "media/Justice_Genesis.wav";
		m[1] = "media/Justice_Genesis_first_30_seconds.wav";
		m[2] = "media/binaural_stereo_2_ch.wav";

		return m;
}

std::map<int, std::string> all_file_names;

class cpp_openal_opengl_dna {

public:

	cpp_openal_opengl_dna();
	virtual ~cpp_openal_opengl_dna();

	// -------------------------------------

private:

//	unsigned short num_channels;

	// ------------
};

#endif /* CPP_OPENAL_OPENGL_DNA_H_ */

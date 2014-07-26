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



static const int WHICH_FILE_INPUT  = 18; // audio/binaural_stereo_2_ch.wav
//static const int WHICH_FILE_INPUT  = 17; // Die_Antwoord_11_doong_doong_1_sec
//static const int WHICH_FILE_INPUT  = 4; // /home/stens/Justice_Genesis_chewy_chocolate_cookies_gtZunGHG0ls_mono.wav
//static const int WHICH_FILE_INPUT  = 15; //   /home/stens/Lee_Smolin_Physics_Envy_and_Economic_Theory-cWn86ESze6M_mono_1st_few_seconds.wav




static std::map <int, std::string> create_map() {   // used for visualization in print logging ONLY

		std::map <int, std::string> m;

		m[0] = "/home/stens/Justice_Genesis_first_tiny_bit.pcm";
		m[1] = "/home/stens/Justice_Genesis_first_10_seconds.pcm";
//		m[2] = "/home/stens/Poem_of_Ecstasy_Alexander_Scriabin-7OEzPSPjbyk_mono.wav"; //  OK
//		m[2] = "/home/stens/Lee_Smolin_Physics_Envy_and_Economic_Theory_cWn86ESze6M_mono.wav"; //  OK
		m[2] = "/home/stens/Dropbox/Documents/data/audio/Lee_Smolin_Physics_Envy_and_Economic_Theory_cWn86ESze6M_mono.wav"; //  OK
		m[3] = "/home/stens/Justice_Genesis_mono.wav";	// OK using MAX_SIZE_CIRCULAR_BUFFER 12733734
		m[4] = "/home/stens/Justice_Genesis_chewy_chocolate_cookies_gtZunGHG0ls_mono.wav";
		m[5] = "/home/stens/Justice_Genesis_first_third_second.pcm";
		m[6] = "/home/stens/Moons_of_Uranus_vol_2_Sir_William_Herschel_minimix--SRuqwj2EgM_clean_deep.pcm";
//		m[7] = "/home/stens/bach_three_lute_pieces_andres_segovia_mono.wav";
		m[7] = "/home/stens/bach_three_lute_pieces_andres_segovia_mono_first_pluck.wav";
		m[8] = "/home/stens/bach_three_lute_pieces_andres_segovia_mono_simple_flow.wav";
		m[9] = "/home/stens/bach_three_lute_pieces_andres_segovia_mono_simple_tiny.wav";
		m[10] = "/home/stens/bach_three_lute_pieces_andres_segovia_mono_simple_tiny_shifted.wav";
		m[11] = "/home/stens/source_sin_wave_440_hz_0point8_amplitude_0point1_seconds.wav";	// generated in audacity
		m[12] = "/home/stens/source_440_phase_shifted.pcm";	// generated in audacity
//		m[13] = "/home/stens/Dropbox/Documents/data/audio/a_csharp_and_e.wav";
		m[13] = "/home/stens/Dropbox/Documents/data/audio/e_660_Hz.wav";
//		m[14] = "/home/stens/sound_buffer_output.pcm";
		m[14] = "sound_buffer_output.pcm";
		m[15] = "/home/stens/Lee_Smolin_Physics_Envy_and_Economic_Theory-cWn86ESze6M_mono_1st_few_seconds.wav";
//		m[15] = "/home/stens/Dropbox/Documents/data/audio/Lee_Smolin_Physics_Envy_and_Economic_Theory_cWn86ESze6M_mono.wav";
		m[16] = "/home/stens/Dropbox/Documents/data/audio/Die_Antwoord_11_doong_doong_30_sec_audacity.wav";
		m[17] = "/home/stens/Dropbox/Documents/data/audio/Die_Antwoord_11_doong_doong_1_sec.wav";
		m[18] = "/home/stens/Dropbox/Documents/data/audio/binaural_stereo_2_ch.wav";




//		a_csharp_and_e.wav
//		a_440_Hz.wav
//		csharp_550_Hz.wav
//		e_660_Hz.wav

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

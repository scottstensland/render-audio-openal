/*
 * Synthesize_Sound.h
 *
 *  Created on: May 30, 2013
 *      Author: stens
 */

#ifndef SYNTHESIZE_SOUND_H_
#define SYNTHESIZE_SOUND_H_


#include <iostream>
#include <vector>
#include <map>


//#include "Audio_Sample_Info.h"

#include "Audio_Utility.h"



class Synthesize_Sound {

public:

	Synthesize_Sound();
	virtual ~Synthesize_Sound();

	// ------------------------------

	void fold_wavelet_into_existing_proposals(
			const short int * wavelet_audio_buffer,
			short int * target_audio_buffer,
			const unsigned int buffer_size);

	void create_wavelet(
			short int * wavelet_audio_buffer,
			const double * gaussian_buffer,
			const unsigned int buffer_size);

	struct_sample_specs * get_sample_specs();
	void set_sample_specs(struct_sample_specs * given_ptr_struct_sample_specs);

	short * get_next_buffer();

	void populate_buffer(short * given_samples);

	void populate_buffer(
			short * given_audio_buffer,
			double given_frequency,
			int given_sample_rate,
//			double given_sample_length_seconds,
			int given_buffer_size,
			int position_offset
//			double amplitude_factor
			);

/*
	static void generate_wavelet(
			double mean,
			double standard_deviation,
			std::map<int, double> * ptr_given_dict_wavelet_gaussian);
*/


	int get_vector_size();

	void setup_audio_buffers();

	void sin_wave_signed_16bit_pcm(std::string given_output_filename,
			double given_sample_rate, double given_frequency, double period_in_seconds);

	void init();

	void read_binary_file(std::string given_filename);

	void read_binary_file_16_bit(std::string given_filename);

	std::vector <short *> get_samples();

private:

	double frequency;
	int sample_rate;
	int sample_length_seconds;
	int buffer_size;


	std::vector <short *> vector_samples;
	unsigned int curr_vector_index;

	struct_sample_specs * ss_ptr_struct_sample_specs;

};

#endif /* SYNTHESIZE_SOUND_H_ */

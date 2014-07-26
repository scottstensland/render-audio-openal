/*
 * Common_Utilities.h
 *
 *  Created on: Sep 9, 2013
 *      Author: stens
 */

#ifndef COMMON_UTILITIES_H_
#define COMMON_UTILITIES_H_


#include <iomanip>  // setw(5)

#include <fstream>	// std::string
#include <sstream>	// std::ostringstream

#include <map>

static const int YES = 1;
static const int NO = 0;

#include <random>



static const int DEBUG = YES;
//static const int DEBUG = NO;

//#define DEBUG YES
//#define KDTREE_DEBUG YES

//#define DEBUG NO
//#define KDTREE_DEBUG NO


static const int SLEEP_CONSUME_01 = 50;
//static const int SLEEP_CONSUME_01 = 250;

static const int num_dimensions_3D = 3;

/*
#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO 0
#endif
*/



typedef struct {

	unsigned int sample_rate;
	unsigned int source_buffer_size;
	unsigned int bits_per_samp;
	unsigned short num_channels;

//	std::string chosen_file;

	short int * audio_buffer;

	bool do_we_want_RMS_calculated = false;
	double root_mean_square = 0;

} struct_input_file_read_pop_buffer;




class Common_Utilities {

public:

	Common_Utilities();
	virtual ~Common_Utilities();

	// ----------


	/*
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

	//		a_csharp_and_e.wav
	//		a_440_Hz.wav
	//		csharp_550_Hz.wav
	//		e_660_Hz.wav

			return m;
		}
		*/

//		std::map<int, std::string> all_file_names;

	// ----------

	static void delete_all_files_in_dir(const char * dir_name_to_empty);

	static std::string string_format(int size_string, const std::string &fmt, ...);

	static double hermite_interpolate(
	        double y0, double y1, double y2, double y3,
	        double mu, double tension, double bias);

	static std::ifstream::pos_type filesize(std::string given_filename);

	static void write_output_file_ascii(
	        std::string given_output_filename,
	        double * data_buffer,
	        unsigned int buffer_size);

	static void write_output_file_ascii(
	        std::string given_output_filename,
	        short int * data_buffer,
	        unsigned int buffer_size);

	static std::string int_to_std_string(int given_integer);

//	float get_gaussian_random_in_range(
//			float given_mean, float given_standard_deviation,float minimum, float maximum);

	float get_gaussian_random_in_range(
			float given_mean, float given_standard_deviation);

	float get_random_in_range(float minimum, float maximum);

	int get_random_in_range(int minimum, int maximum);

	static void write_output_binary_file(
			std::string given_output_filename,
			const void * data_buffer,
			unsigned int buffer_size);

//	static void seed_random();
	void seed_random();

	static const char * insert_commas_into_int(int given_integer);


	std::default_random_engine random_generator;


	static std::string create_string_from_string_N_int(std::string given_string, int given_int);



};

#endif /* COMMON_UTILITIES_H_ */

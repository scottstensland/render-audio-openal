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

	float get_gaussian_random_in_range(
			float given_mean, float given_standard_deviation);

	float get_random_in_range(float minimum, float maximum);

	int get_random_in_range(int minimum, int maximum);

	static void write_output_binary_file(
			std::string given_output_filename,
			const void * data_buffer,
			unsigned int buffer_size);

	void seed_random();

	static const char * insert_commas_into_int(int given_integer);

	std::default_random_engine random_generator;

	static std::string create_string_from_string_N_int(std::string given_string, int given_int);
};

#endif /* COMMON_UTILITIES_H_ */

/*
 * Common_Utilities.cpp
 *
 *  Created on: Sep 9, 2013
 *      Author: stens
 */

#include "Common_Utilities.h"


#include <stdlib.h>     // malloc, free, rand

#include <sstream>		// ostringstream str

#include <iostream>
//#include <cstdlib>

#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include <dirent.h>     // opendir


double Common_Utilities::hermite_interpolate(
        double y0, double y1, double y2, double y3,
        double mu, double tension, double bias) {

//  see     http://paulbourke.net/miscellaneous/interpolation/

//  The parameter mu defines where to estimate the value on the interpolated line,
//  it is 0 at the first point and 1 and the second point. For interpolated values
//  between the two points mu ranges between 0 and 1. Values of mu outside this
//  range result in extrapolation.

//  Tension can be used to tighten up the curvature at the known points.
//  The bias is used to twist the curve about the known points.
//  default tension and bias values of 0

//   Tension: 1 is high, 0 normal, -1 is low
//   Bias: 0 is even,
//         positive is towards first segment,
//         negative towards the other



    double m0, m1, mu2, mu3;
    double a0, a1, a2, a3;

    mu2 = mu * mu;
    mu3 = mu2 * mu;
    m0 = (y1 - y0) * (1 + bias) * (1 - tension) / 2;
    m0 += (y2 - y1) * (1 - bias) * (1 - tension) / 2;
    m1 = (y2 - y1) * (1 + bias) * (1 - tension) / 2;
    m1 += (y3 - y2) * (1 - bias) * (1 - tension) / 2;
    a0 = 2 * mu3 - 3 * mu2 + 1;
    a1 = mu3 - 2 * mu2 + mu;
    a2 = mu3 - mu2;
    a3 = -2 * mu3 + 3 * mu2;

    return (a0 * y1 + a1 * m0 + a2 * m1 + a3 * y2);
}




std::ifstream::pos_type Common_Utilities::filesize(std::string given_filename) {

    std::ifstream input_filehd(given_filename, std::ifstream::in | std::ifstream::binary);
    input_filehd.seekg(0, std::ifstream::end);

    return input_filehd.tellg();
}





std::string Common_Utilities::string_format(int size_string, const std::string &fmt, ...) {

	//      http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf

    if (10000 < size_string) {

        std::cerr << "ERROR - seeing HUGE string size given to function Utility::string_format so fail for now"
                << std::endl;

        std::cout << "ERROR - seeing HUGE string size given to function Utility::string_format so fail for now"
                << std::endl;
        exit(8);
    }

    std::vector<char> str(size_string, '\0');
    va_list ap;
    while (1) {
        va_start(ap, fmt);
        auto n = vsnprintf(str.data(), str.size(), fmt.c_str(), ap);
        va_end(ap);
        if ((n > -1) && (size_t(n) < str.size())) {
            return str.data();
        }
        if (n > -1)
            str.resize( n + 1 );
        else
            str.resize( str.size() * 2);
    }
    return str.data();
}





void Common_Utilities::delete_all_files_in_dir(const char * dir_name_to_empty) {

     // These are data types defined in the "dirent.h" header

//  char * dir_name_to_empty = given_dir.c_str();

    struct dirent * next_file;

    DIR * theFolder;

    char filepath[256];

    theFolder = opendir(dir_name_to_empty);

    while ( (next_file = readdir(theFolder)) ) {

        // build the full path for each file in the folder

        sprintf(filepath, "%s/%s", dir_name_to_empty, next_file->d_name);

        remove(filepath);
    }
}





const char * Common_Utilities::insert_commas_into_int(int given_integer) {

	std::ostringstream stm ;
	stm << given_integer;
	std::string str_num(stm.str());

	int size_str = str_num.size() ;

//	std::cout << " num digits in " << given_integer << " is " << size_str << std::endl;
	for( int i = size_str ; i > 0 ; i -= 3 ) {

		if (i != size_str)
			str_num.insert( i, 1, ',' ) ;
	}
//	std::cout << str_num << '\n' ;

	return str_num.c_str();
}



void Common_Utilities::seed_random() {

	// initialize random seed

	std::cout << "its tuesday" << std::endl;

//	srand ( time(NULL) );	// stens TODO - may need to put both this and get_random_in_range as normal non

	srand ( 7 );	// stens TODO - may need to put both this and get_random_in_range as normal non

//	srand ( 0 );	// stens TODO - may need to put both this and get_random_in_range as normal non
							// static methods - lets see

//    std::random_device rd;
//    std::srand(rd());


}


void Common_Utilities::write_output_binary_file(
		std::string given_output_filename,
		const void * data_buffer,
		unsigned int buffer_size) {

//	FILE * output_filehd = fopen(given_output_filename.c_str(), "w");
	FILE * output_filehd = fopen(given_output_filename.c_str(), "wb");

	fwrite( data_buffer, sizeof(short int), buffer_size, output_filehd);

	fclose(output_filehd);
}


void Common_Utilities::write_output_file_ascii(
        std::string given_output_filename,
        double * data_buffer,
        unsigned int buffer_size) {

    FILE * output_filehd = fopen(given_output_filename.c_str(), "w");

    unsigned int i;
    for (i = 0; i < buffer_size; i++) {

        fprintf(output_filehd, "%f\n", data_buffer[i]);
    }

    fclose(output_filehd);
}

void Common_Utilities::write_output_file_ascii(
        std::string given_output_filename,
        short int * data_buffer,
        unsigned int buffer_size) {

    FILE * output_filehd = fopen(given_output_filename.c_str(), "w");

    unsigned int i;
    for (i = 0; i < buffer_size; i++) {

        fprintf(output_filehd, "%d\n", data_buffer[i]);
    }

    fclose(output_filehd);
}





std::string Common_Utilities::int_to_std_string(int given_integer) {


	std::ostringstream stm ;
	stm << given_integer;
	std::string str_num(stm.str());

	return str_num;
}



float Common_Utilities::get_gaussian_random_in_range(
		float given_mean, float given_standard_deviation) {

	std::normal_distribution<double> gaussian_distribution(given_mean, given_standard_deviation);

//	float generated_number;

//	bool did_we_need_to_calc_more_than_once = false;
//
//	do {

//		if (did_we_need_to_calc_more_than_once) {
//
//			std::cerr << "NOTICE - seeing loop in get_gaussian_random_in_range" << std::endl;
//		}

//	generated_number = gaussian_distribution(random_generator);
	return gaussian_distribution(random_generator);

//		did_we_need_to_calc_more_than_once = true;
//
//	} while(generated_number < minimum || generated_number > maximum);

//	return generated_number;
}

float Common_Utilities::get_random_in_range(float minimum, float maximum) {

//	srand ( 0 );	// put the seed in each call like this to generate repeatable sequence

	return(minimum + (float)rand()/((float)RAND_MAX/(maximum - minimum)));
}

int Common_Utilities::get_random_in_range(int minimum, int maximum) { // stens TODO - is this inclusive

	return(minimum + (float)rand()/((float)RAND_MAX/(maximum - minimum)));
}

std::string Common_Utilities::create_string_from_string_N_int(std::string given_string, int given_int) {

	std::ostringstream temp_string;
	temp_string << given_string << given_int;
	return (temp_string.str());

}









Common_Utilities::Common_Utilities() {		// constructor

}

Common_Utilities::~Common_Utilities() {		// deconstructor

}


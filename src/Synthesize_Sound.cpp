/*
 * Synthesize_Sound.cpp
 *
 *  Created on: May 30, 2013
 *      Author: stens
 */

#include "Common_Utilities.h"


#include "Synthesize_Sound.h"

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>


#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

//#include "Utility.h"

void Synthesize_Sound::fold_wavelet_into_existing_proposals(
		const short int * wavelet_audio_buffer,
		short int * target_audio_buffer,
		const unsigned int buffer_size) {

	unsigned int i = 0;
	for (; i < buffer_size; i++) {

//		std::cout << wavelet_audio_buffer[i] << " times " << gaussian_buffer[i];

		// add together both wave forms then divide by 2 to normalize output wave

		target_audio_buffer[i] = (target_audio_buffer[i] + wavelet_audio_buffer[i]) >> 1;

//		std::cout << " == " << wavelet_audio_buffer[i] << std::endl;
	}
}

void Synthesize_Sound::create_wavelet(
		short int * wavelet_audio_buffer,
		const double * gaussian_buffer,
		const unsigned int buffer_size) {

	unsigned int i = 0;
	for (; i < buffer_size; i++) {

//		std::cout << wavelet_audio_buffer[i] << " times " << gaussian_buffer[i];

		wavelet_audio_buffer[i] = ((double)  wavelet_audio_buffer[i]) * gaussian_buffer[i];

//		std::cout << " == " << wavelet_audio_buffer[i] << std::endl;
	}
}

std::vector <short *> Synthesize_Sound::get_samples() {

		return vector_samples;
}

struct_sample_specs * Synthesize_Sound::get_sample_specs() {

	return ss_ptr_struct_sample_specs;
}

void Synthesize_Sound::set_sample_specs(struct_sample_specs * given_ptr_struct_sample_specs) {

	ss_ptr_struct_sample_specs = given_ptr_struct_sample_specs;
}

void Synthesize_Sound::populate_buffer(
		short * given_audio_buffer,
		double given_frequency,
		int given_sample_rate,
//		double given_sample_length_seconds,
		int given_buffer_size,
		int position_offset
//		double desired_rms_peak_amplitude		// from 0.0 to 1.0 which maps to 0.0 to +/- 32760 ... 2^16 == 65536
		) {

    int i=0;
    int offset_adjusted_index = position_offset;

//    std::cout << "new synth wave offset_adjusted_index " << offset_adjusted_index << std::endl;

    for(; i < given_buffer_size; ++i) {

    	if (offset_adjusted_index == given_buffer_size) {

    		offset_adjusted_index = 0;
    	}

//    	given_audio_buffer[i] = (short)(32760 * sin( ((2.f * M_PI * given_frequency)/given_sample_rate) * i));
    	given_audio_buffer[offset_adjusted_index] = (short)(32760 * sin( ((2.f * M_PI * given_frequency)/given_sample_rate) * i));
//    	given_audio_buffer[offset_adjusted_index] = (short)(peak_amplitude * sin( (2.f * M_PI * given_frequency)/given_sample_rate * i));

//    	std::cout << " freq " << given_frequency
//    			<< " offset " << position_offset
//    			<< " sin value for i " << offset_adjusted_index
//    			<< " is " << given_audio_buffer[offset_adjusted_index] << std::endl;

    	offset_adjusted_index++;
    }

}	//	populate_buffer


void Synthesize_Sound::populate_buffer(short * given_samples) {

	std::cout << "SS populate_buffer TOP freq "
			<< ss_ptr_struct_sample_specs->frequency
			<< std::endl;

//	printf("just inside populate_buffer with buffer_size %d   freq %f  sample_rate %d\n",
//			buffer_size, sample_frequency, sample_rate);

    int i=0;

    for(; i < ss_ptr_struct_sample_specs->buffer_size; ++i) {
    	given_samples[i] = (short)(32760 *
    			sin( (2.f * M_PI *
    					ss_ptr_struct_sample_specs->frequency)/ss_ptr_struct_sample_specs->sample_rate * i));

//    	std::cout << given_samples[i] << std::endl;
    }

    ss_ptr_struct_sample_specs->frequency += 50;

}	//	populate_buffer



// ------------------------------------------------------------------- //


/*
void Synthesize_Sound::generate_wavelet(
		double mean,
		double standard_deviation,
		std::map<int, double> * ptr_given_dict_wavelet_gaussian) {


//	 how to synthesize wavelet
//
//	 * 		http://paos.colorado.edu/research/wavelets/wavelet2.html
//
//	 * 		http://rocketscienceofwallstreet.blogspot.com/2012/05/alternatives-to-gaussian-curve.html
//	 *
//	 * 		how to synthesize gaussian curve :
//	 *
//	 * 			http://en.cppreference.com/w/cpp/numeric/random/normal_distribution
//	 *
//	 * 			http://stackoverflow.com/questions/9606458/looking-for-a-good-c-c-wavelet-library-for-signal-processing
//	 *
//	 *
//	 *		http://www.cplusplus.com/reference/random/normal_distribution/
//	 *
//	 *		http://polyvalens.pagesperso-orange.fr/clemens/wavelets/wavelets.html
//	 *
//	 *
//	 * 		basically :  superimpose sine wave with Gaussian




//	std::cout << "SS populate_buffer TOP freq "
//			<< ss_ptr_struct_sample_specs->frequency
//			<< std::endl;
//
//    int i=0;
//    for(; i < ss_ptr_struct_sample_specs->buffer_size; ++i) {
//    	given_samples[i] = (short)(32760 *
//    			sin( (2.f * M_PI *
//    					ss_ptr_struct_sample_specs->frequency)/ss_ptr_struct_sample_specs->sample_rate * i));
//    }




//    std::random_device rd;
//    std::mt19937 gen(rd());
//
//    // values near the mean are the most likely
//    // standard deviation affects the dispersion of generated values from the mean
//    std::normal_distribution<> d(5,2);
//
//    std::map<int, int> hist;
//    for(int n=0; n<10000; ++n) {
//        ++hist[std::round(d(gen))];
//    }
//    for(auto p : hist) {
//        std::cout << std::fixed << std::setprecision(1) << std::setw(2)
//                  << p.first << ' ' << std::string(p.second/200, '*') << '\n';
//    }



	std::cout << "SS               mean " << mean << std::endl;
	std::cout << "SS standard_deviation " << standard_deviation << std::endl;


	double compression_factor = 20.0;

    std::random_device rd;
    std::mt19937 gen(rd());

    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<> d(mean,  standard_deviation);

    std::map<int, int> hist;
    for(int n=0; n<100000; ++n) {
        ++hist[std::round(d(gen))];
    }

//    int index_hist = 0;

    for(auto p : hist) {

//        std::cout << std::fixed << std::setprecision(1) << std::setw(2)
//                  << p.first << ' ' << std::string(p.second/200, '*') << '\n';


//        std::cout << std::fixed << std::setprecision(1) << std::setw(2)
//                  << p.first << ' ' << std::string(p.second/compression_factor, '*') << '\n';


    	double temp_height = p.second/compression_factor ;

        std::cout << "    " << p.first << "    " << temp_height << '\n';

//        given_dict_wavelet_gaussian[index_hist++] = temp_height;


        ptr_given_dict_wavelet_gaussian->insert(
						std::pair<int, double>(p.first, temp_height));
    }

    std::cout << " here is hist size " << hist.size() << std::endl;



    std::cout << " here is given_dict_wavelet_gaussian size " << ptr_given_dict_wavelet_gaussian->size() << std::endl;





//	const int nrolls=10000;  // number of experiments
//	const int nstars=500;    // maximum number of stars to distribute
//
//
//	std::default_random_engine generator;
//
//	std::normal_distribution<double> distribution(15.0,2.0);
//
//#define size_breakdown 100
//
//	int p[size_breakdown]={};
//	for (int i=0; i<nrolls; ++i) {
//
//		double number = distribution(generator);
//	    if ((number>=0.0)&&(number<(1.0 * size_breakdown))) ++p[int(number)];
//	}
//
//	  std::cout << "normal_distribution (5.0,2.0):" << std::endl;
//
//	  for (int i=0; i< size_breakdown; ++i) {
//	    std::cout << i << "-" << (i+1) << ": ";
//	    std::cout << std::string(p[i]*nstars/nrolls,'*') << std::endl;
//	  }


}	//	generate_wavelet
*/


// ------------------------------------------------------------------- //





short * Synthesize_Sound::get_next_buffer() {

	std::cout << "SS TOP get_next_buffer" << std::endl;

//	std::cout << "SS just inside get_next_buffer with curr_vector_index "
//			<< curr_vector_index << std::endl;




//	if (vector_samples.empty()) {
//
//		std::cout << "SS seeing empty vector" << std::endl;
//
//		exit(8);
//
//	} else {
//
//		std::cout << "SS vector is NOT empty" << std::endl;
//
//	}
//
//
//
//	if (vector_samples.size() == 0) {
//
//		std::cout << "ERROR - seeing empty vector" << std::endl;
//	}


	std::cout << "SS MID get_next_buffer point Cairo" << std::endl;

	if (curr_vector_index == vector_samples.size()) {

		curr_vector_index = 0;
	}

	std::cout << "SS MID get_next_buffer point Ethiopia" << std::endl;



	std::cout << "SS about to return vector index "
			<< curr_vector_index << std::endl;

	std::cout << "SS MID get_next_buffer point Sudan" << std::endl;


//
//	return vector_samples.at(curr_vector_index++);


	return vector_samples[curr_vector_index++];
}



int Synthesize_Sound::get_vector_size() {

	if (vector_samples.empty()) {

		std::cout << "SS vector_samples is EMPTY"  << std::endl;

		return 0;
	}

	return (int)vector_samples.size();
}

void Synthesize_Sound::setup_audio_buffers() {

	std::cout << "SS inside setup_audio_buffers "  << std::endl;

	frequency = 1840.0;
	sample_rate = 22050;
	sample_length_seconds = 3;

	buffer_size = sample_length_seconds * sample_rate;

	std::cout << "spot BBB with set_sample_rate " << sample_rate << std::endl;

	int count_num_mem_buffers = 3;
	int index_buffer = 0;
	for (; index_buffer < count_num_mem_buffers; index_buffer++) {

		std::cout << "SS  buffer size in AAA is " << buffer_size
				<< "   freq=" << frequency
				<< "   sample_rate=" << sample_rate
				<< "   sample_length=" << sample_length_seconds;


		short * curr_sample = (short *) malloc(sizeof(short) * buffer_size);

		std::cout << "  address " << curr_sample << std::endl;

		populate_buffer(curr_sample);

		vector_samples.push_back(curr_sample);

		frequency += 800;
	}

}	//	setup_audio_buffers


void Synthesize_Sound::sin_wave_signed_16bit_pcm(std::string given_output_filename,
		double given_sample_rate, double given_frequency, double period_in_seconds) {

//	using namespace std;

//	std::ios_base::fmtflags old_flags = std::cout::flags();

	int desired_prints = 7;
	int curr_prints = 0;

	FILE * output_filehd = fopen(given_output_filename.c_str(), "wb");

	double time_point;
	double time_interval = 1.0/given_sample_rate;	// sample rate is cycles per second interval is reciprical

//	for (time_point = 0; time_point < period_in_seconds; time_point += time_interval) {

	for (time_point = 0; time_point < period_in_seconds && curr_prints < desired_prints; time_point += time_interval, curr_prints++) {






		std::cout << "__________________" << std::endl;

		double sampled_amplitude = 15000 * sin(2 * M_PI * given_frequency * time_point);

		short sample_16_bit = (short) sampled_amplitude;

		unsigned char c_itself;

		c_itself = (unsigned) sample_16_bit % 256;	// remainder after dividing by 256

		std::cout << " sample_16_bit " << std::dec << sample_16_bit << std::endl;
//		std::cout << " c_itself " << c_itself << std::endl;
//		std::cout << " c_itself hex " << std::hex << std::setw(4) << c_itself << std::endl;
//		std::cout << " c_itself hex " << std::hex << c_itself << std::endl;






//	    cout << hex << setw(4) << c_itself << dec << " = " << setw(3) << c_itself;


		fwrite( & c_itself, 1, 1, output_filehd);

		c_itself = (unsigned) sample_16_bit / 256 % 256;

//		std::cout << " after divide " << hex << setw(4) << c_itself << std::endl;

		fwrite( & c_itself, 1, 1, output_filehd);

//		curr_prints++
	}



	//	std::ostream &stream, T const &data)

//		Utils::writeRaw();


	fclose(output_filehd);





//	std::cout.flags(old_flags);
}


//void Synthesize_Sound::init(std::string given_filename) {

void Synthesize_Sound::init() {

	curr_vector_index = 0;

	std::cout << "SS init just set curr_vector_index to zero" << std::endl;
}






void Synthesize_Sound::read_binary_file_16_bit(std::string given_filename) {

//	std::cout << "kicking off with init in synth sound file " <<
//			std::endl <<  given_filename << std::endl;


	/*
	 *
	 * 		signed 16 bit PCM
	 * 		little endian
	 * 		1 channel mono
	 * 		sample rate :  44100 Hz
	 *
	 *
	 * 		file name :    ~/Justice_Genesis_first_tiny_bit.pcm
	 *
	 */

//    fstream input_filehd (given_filename, ios::in|ios::binary);

	std::ifstream input_filehd (given_filename, std::ios::in | std::ios::binary);

	if ( ! input_filehd.is_open()) {

		std::cerr << "ERROR - failed to open binary file : " <<
				std::endl << given_filename << std::endl;

		exit(5);
	}

	// --------- file is open now read contents



//	std::ifstream::pos_type file_size = Utility::filesize(given_filename);
	std::ifstream::pos_type file_size = Common_Utilities::filesize(given_filename);




	std::cout << "file_size is " << file_size << std::endl;

	char * memory_block = new char[file_size];

	input_filehd.seekg(0, std::ios::beg);

	input_filehd.read(memory_block, file_size);

	input_filehd.close();


	// ------------ entire file contents now in memory block

	std::cout << std::endl << "start of showing file contents" << std::endl;



	for (int i = 0; i < file_size; i++) {

		std::cout << memory_block[i] << std::endl;
	}

	std::cout << std::endl << "end of showing file contents" << std::endl;

	exit(8);

	std::cout << "end of call to xxxxx \n";

}	//	read_binary_file_16_bit




/*
void Synthesize_Sound::read_binary_file(std::string given_filename) {

//	std::cout << "kicking off with init in synth sound file " <<
//			std::endl <<  given_filename << std::endl;



//
//	  		signed 16 bit PCM
//	  		little endian
//	  		1 channel mono
//	  		sample rate :  44100 Hz
//
//
//	  		file name :    ~/Justice_Genesis_first_tiny_bit.pcm
//


//    fstream input_filehd (given_filename, ios::in|ios::binary);

	std::ifstream input_filehd (given_filename, std::ios::in | std::ios::binary);

	if ( ! input_filehd.is_open()) {

		std::cerr << "ERROR - failed to open binary file : " <<
				std::endl << given_filename << std::endl;

		exit(5);
	}

	// --------- file is open now read contents



	std::ifstream::pos_type file_size = Utility::filesize(given_filename);

	std::cout << "file_size is " << file_size << std::endl;

	char * memory_block = new char[file_size];

	input_filehd.seekg(0, std::ios::beg);

	input_filehd.read(memory_block, file_size);

	input_filehd.close();


	// ------------ entire file contents now in memory block

	std::cout << std::endl << "start of showing file contents" << std::endl;



	for (int i = 0; i < file_size; i++) {

		std::cout << memory_block[i] << std::endl;
	}

	std::cout << std::endl << "end of showing file contents" << std::endl;

	exit(8);

	std::cout << "end of call to xxxxx \n";

}
*/


Synthesize_Sound::Synthesize_Sound() {

}

Synthesize_Sound::~Synthesize_Sound() {

}


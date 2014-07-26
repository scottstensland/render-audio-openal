/*
 * Binary_File_IO.h
 *
 *  Created on: Jun 13, 2013
 *      Author: stens
 */

#ifndef BINARY_FILE_IO_H_
#define BINARY_FILE_IO_H_

#include <stdio.h>	// fread

#include <string>


class Binary_File_IO {

public:

	Binary_File_IO();
	virtual ~Binary_File_IO();

	// ------------

	FILE * get_file_handle();

	void read_wav_data(short int *data, unsigned int samp_rate,
			unsigned int bits_per_samp, unsigned int num_samp, bool flag_want_print);

	void read_wav_data_corinde(short int * buff_data, unsigned int samp_rate,
					unsigned int bits_per_samp, unsigned int source_buffer_size, bool flag_want_print);

//	void init_file_read(char * filename);

	void init_file_read(std::string filename);


	void read_wav_header(unsigned int * samp_rate, unsigned int * bits_per_samp,
			unsigned int * num_samp);

	static void write_binary_file(
			std::string filename,
			short int * buff_data,
			unsigned int source_buffer_size);

	void parse_wav_header(unsigned int * samp_rate, unsigned int * bits_per_samp, unsigned int * buffer_size);

	void parse_wav_header_adaptive(unsigned int * samp_rate, unsigned int * bits_per_samp,
			unsigned int * buffer_size, unsigned short * num_channels);

	// ----------------------------------------------


private:

//	unsigned int samp_rate, bits_per_samp, num_samp;
//	unsigned int samp_rate, bits_per_samp;


	FILE * file_hd;		// file handle


//	int conv_bit_size(unsigned int in, int bps); // converts unsigned into signed
	short int conv_bit_size(unsigned int in, int bps); // converts unsigned into signed



};

#endif /* BINARY_FILE_IO_H_ */

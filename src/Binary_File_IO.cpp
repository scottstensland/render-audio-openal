/*
 * Read_Binary_Audio.cpp
 *
 *  Created on: Jun 13, 2013
 *      Author: stens
 */

//   WAV and PCM format details :
//   https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
//   http://unusedino.de/ec64/technical/formats/wav.html
//   http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
//   http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/Samples.html
//   http://www.gamedev.net/page/resources/_/technical/game-programming/loading-a-wave-file-r709
//   http://www.topherlee.com/software/pcm-tut-wavformat.html
//   http://www.drdobbs.com/database/inside-the-riff-specification/184409308

#include "Binary_File_IO.h"

#include <iostream>	// std::cout

#include <stdlib.h>	// exit

#include <string.h>		// remove when I cut over to strncmp
                        // http://stackoverflow.com/questions/1623769/is-there-any-safe-strcmp
#include <sstream>

void Binary_File_IO::parse_wav_header_adaptive(unsigned int * samp_rate, unsigned int * bits_per_samp,
		unsigned int * buffer_size, unsigned short * num_channels) {

	struct wave_header
	{
	    char chunk_id[4];
	    int chunk_size;
	    char format[4];
	};

	struct riff_chunk_header
	{
	    char id[4];
	    int size;
	};

	struct wave_fmt_chunk
	{
	    short audio_format;
	    short num_channels;
	    int sample_rate;
	    int byte_rate;
	    short block_align;
	    short bits_per_sample;
	};

	// ---------------

	wave_header wavHeader;

	fread( & wavHeader, sizeof(wave_header), 1, file_hd);

	std::cout << "  chunk_id : " << wavHeader.chunk_id[0] << wavHeader.chunk_id[1] << wavHeader.chunk_id[2] << wavHeader.chunk_id[3] << std::endl;
	std::cout << "chunk_size : " << wavHeader.chunk_size << std::endl;
	std::cout << "    format : " << wavHeader.format[0] << wavHeader.format[1] << wavHeader.format[2] << wavHeader.format[3] << std::endl;

	// ---

	riff_chunk_header chunkHeader;

	fread( & chunkHeader, sizeof(riff_chunk_header), 1, file_hd);

//	std::cout << "chunkHeader ID " << chunkHeader.id << std::endl;

	// ---

//	unsigned char buf_5[5];

	bool is_ID_fmt = false;

	if (chunkHeader.id[0] == 'f' &&
		chunkHeader.id[1] == 'm' &&
		chunkHeader.id[2] == 't' &&
		chunkHeader.id[3] == ' ') {

		// OK ... seeing  'fmt '

		is_ID_fmt = true;

		std::cout << "OK chunkHeader.id == 'fmt '" << std::endl;

		std::cout << "  ID : " << chunkHeader.id[0] << chunkHeader.id[1] << chunkHeader.id[2] << chunkHeader.id[3] << std::endl;
		std::cout << "size : " << chunkHeader.size << std::endl;

	} else {

		std::cout << "well ... NOT seeing 'fmt ' so skip the chunk and re-read" << std::endl;
	}

	// ---

	wave_fmt_chunk fmtChunk;

	fread( & fmtChunk, sizeof(wave_fmt_chunk), 1, file_hd);

	if (1 == fmtChunk.audio_format) {

		std::cout << "OK seeing audio_format == 1 so PCM" << std::endl;
		std::cout << "   audio_format " << fmtChunk.audio_format << std::endl;
		std::cout << "   num_channels " << fmtChunk.num_channels << std::endl;
		std::cout << "    sample_rate " << fmtChunk.sample_rate << std::endl;
		std::cout << "      byte_rate " << fmtChunk.byte_rate << std::endl;
		std::cout << "    block_align " << fmtChunk.block_align << std::endl;
		std::cout << "bits_per_sample " << fmtChunk.bits_per_sample << std::endl;

	} else {

		std::cout << "well ... NOT seeing 1 for audio_format instead seeing "
				<< fmtChunk.audio_format
				<< std::endl;
	}

	// ---

	struct wave_final_chunk
	{
		char  subchunk2ID[4];
//		int  subchunk2Size;
		char  subchunk2Size[4];
	};

	struct wave_final_chunk_extended
	{
		short filler_01;
		char  subchunk2ID[4];
//		int   subchunk2Size;
//		long   subchunk2Size;
		char  subchunk2Size[4];
	};

	* num_channels  = fmtChunk.num_channels;
	* samp_rate     = fmtChunk.sample_rate;
	* bits_per_samp = fmtChunk.bits_per_sample;

	if (1 == fmtChunk.audio_format && is_ID_fmt && 18 == chunkHeader.size) {

		wave_final_chunk_extended chunkExtended;

		fread( & chunkExtended, sizeof(wave_final_chunk_extended), 1, file_hd);

		std::cout << "seeing PCM with fmt and size 18 ..." << std::endl;

		std::cout << "subchunk2ID : "
				<< chunkExtended.subchunk2ID[0]
				<< chunkExtended.subchunk2ID[1]
				<< chunkExtended.subchunk2ID[2]
				<< chunkExtended.subchunk2ID[3] << std::endl;

		* buffer_size   = chunkExtended.subchunk2Size[0] + (chunkExtended.subchunk2Size[1] << 8) + (chunkExtended.subchunk2Size[2] << 16)
						+ (chunkExtended.subchunk2Size[3] << 24);

//		* buffer_size   = chunkExtended.subchunk2Size;

		std::cout << "* buffer_size : " << * buffer_size << std::endl;


//		  chunk_id : RIFF
//		chunk_size : 851882
//		    format : WAVE
//		chunkHeader.id == 'fmt '
//		  ID : fmt
//		size : 18
//		OK seeing audio_format == 1 so PCM
//		   audio_format 1
//		   num_channels 2
//		    sample_rate 44100
//		      byte_rate 176400
//		    block_align 4
//		bits_per_sample 16
//		seeing PCM with fmt and size 18 ...
//		subchunk2ID : data
//		* buffer_size : 780928


	} else {

		std::cout << "NOT seeing size 18 so try normal size" << std::endl;

		wave_final_chunk chunkFinal;

		fread( & chunkFinal, sizeof(wave_final_chunk), 1, file_hd);

		std::cout << "subchunk2ID : "
				<< chunkFinal.subchunk2ID[0]
				<< chunkFinal.subchunk2ID[1]
				<< chunkFinal.subchunk2ID[2]
				<< chunkFinal.subchunk2ID[3] << std::endl;

		* buffer_size   = chunkFinal.subchunk2Size[0] + (chunkFinal.subchunk2Size[1] << 8) + (chunkFinal.subchunk2Size[2] << 16)
						+ (chunkFinal.subchunk2Size[3] << 24);

//		* buffer_size   = chunkFinal.subchunk2Size;

		std::cout << "* buffer_size : " << * buffer_size << std::endl;

//		* buffer_size : 140890

	}

	//  34			2     8 bits = 8, 16 bits = 16, etc.
//	    unsigned char  subchunk2ID[4];	//  36			4     Contains the letters "data"  (0x64617461 big-endian form)
//	    unsigned int   subchunk2Size;



}

void Binary_File_IO::parse_wav_header(unsigned int * samp_rate, unsigned int * bits_per_samp,
		unsigned int * buffer_size) {

//	typedef struct wavHeader {
//										// offset     size    notes
//	    unsigned char  chunckID[4];		//   0			4
//	    unsigned int   chunckSize;		//   4			4
//	    unsigned char  format[4];		//   8			4
//	    unsigned char  subchunk1ID[4];	//  12			4
//	    unsigned int   subchunk1Size;	//  16			4
//	    unsigned short audioFormat;		//  20			2
//	    unsigned short numChannels;		//  22			2
//	    unsigned int   sampleRate;		//  24			4
//	    unsigned int   byteRate;		//  28			4
//	    unsigned short blockAlign;		//  32			2     NumChannels * BitsPerSample/8
//	    unsigned short bitsPerSample;	//  34			2     8 bits = 8, 16 bits = 16, etc.
//	    unsigned char  subchunk2ID[4];	//  36			4     Contains the letters "data"  (0x64617461 big-endian form)
//	    unsigned int   subchunk2Size;	//  40			4     NumSamples * NumChannels * BitsPerSample/8
//	    								//					  This is the number of bytes in the data
//	} wav_header;

/*
	typedef struct wavHeader {
										// offset     size    notes
	    char  chunckID[4];		//   0			4
	    int   chunckSize;		//   4			4
	    char  format[4];		//   8			4
	    char  subchunk1ID[4];	//  12			4
	    int   subchunk1Size;	//  16			4
	    short audioFormat;		//  20			2
	    short numChannels;		//  22			2
	    int   sampleRate;		//  24			4
	    int   byteRate;		//  28			4
	    short blockAlign;		//  32			2     NumChannels * BitsPerSample/8
	    short bitsPerSample;	//  34			2     8 bits = 8, 16 bits = 16, etc.
	    char  subchunk2ID[4];	//  36			4     Contains the letters "data"  (0x64617461 big-endian form)
	    int   subchunk2Size;	//  40			4     NumSamples * NumChannels * BitsPerSample/8
	    						//					  This is the number of bytes in the data
	} wav_header;

	wav_header wavHeader;

*/



	typedef struct wavHeader {
										// offset     size    notes
	    char  chunckID[4];		//   0			4
	    int   chunckSize;		//   4			4
	    char  format[4];		//   8			4
	    char  subchunk1ID[4];	//  12			4
	    int   subchunk1Size;	//  16			4
	    short audioFormat;		//  20			2
	    short numChannels;		//  22			2
	    int   sampleRate;		//  24			4
	    int   byteRate;		//  28			4
	    short blockAlign;		//  32			2     NumChannels * BitsPerSample/8
	    short bitsPerSample;	//  34			2     8 bits = 8, 16 bits = 16, etc.
	    short filler_01;		//  36          2
	    char  subchunk2ID[4];	//  38			4     Contains the letters "data"  (0x64617461 big-endian form)
//	    int   subchunk2Size;	//  42			4     NumSamples * NumChannels * BitsPerSample/8
	    char  subchunk2Size[4];	//  42			4     NumSamples * NumChannels * BitsPerSample/8
	    						//					  This is the number of bytes in the data
	} wav_header;

	wav_header wavHeader;



//	std::cout << "cccc sizeof(wav_header) == " << sizeof(wav_header) << std::endl;

//	fread( & wavHeader, sizeof(wav_header), 1, file_hd);  // both of these work == size * number of
//	fread( & wavHeader, 1, sizeof(wav_header), file_hd);  // both of these work

	std::cout << "cccc sizeof(wav_header) == " << sizeof(wav_header) << std::endl;

	fread( & wavHeader, sizeof(wav_header), 1, file_hd);  // both of these work == size * number of


	* samp_rate     = wavHeader.sampleRate;
	* bits_per_samp = wavHeader.bitsPerSample;
//	* buffer_size   = wavHeader.subchunk2Size;
	* buffer_size   = wavHeader.subchunk2Size[0] + (wavHeader.subchunk2Size[1] << 8) + (wavHeader.subchunk2Size[2] << 16)
			+ (wavHeader.subchunk2Size[3] << 24);

	printf("      ChunkID: %c%c%c%c\n",  wavHeader.chunckID[0],wavHeader.chunckID[1],wavHeader.chunckID[2],wavHeader.chunckID[3]);
	printf("    ChunkSize: %d\n",        wavHeader.chunckSize);
	printf("       Format: %c%c%c%c\n",     wavHeader.format[0],wavHeader.format[1],wavHeader.format[2],wavHeader.format[3]);
	printf("  SubChunk1ID: %c%c%c%c\n",wavHeader.subchunk1ID[0],wavHeader.subchunk1ID[1],wavHeader.subchunk1ID[2],wavHeader.subchunk1ID[3]);
	printf("Subchunk1Size: %d\n",    wavHeader.subchunk1Size);
	printf("  AudioFormat: %d\n",      wavHeader.audioFormat);
	printf("  NumChannels: %d\n",      wavHeader.numChannels);
	printf("   SampleRate: %d\n",       wavHeader.sampleRate);
	printf("     ByteRate: %d\n",         wavHeader.byteRate);
	printf("   BlockAlign: %d\n",       wavHeader.blockAlign);
	printf("BitsPerSample: %d\n",    wavHeader.bitsPerSample);
	printf("    filler_01: %d\n",        wavHeader.filler_01);
	printf("  Subchunk2ID: %c%c%c%c\n", wavHeader.subchunk2ID[0],wavHeader.subchunk2ID[1],wavHeader.subchunk2ID[2],wavHeader.subchunk2ID[3]);
	printf("Subchunk2Size: %s\n",    wavHeader.subchunk2Size);
}

void Binary_File_IO::read_wav_header(unsigned int * samp_rate, unsigned int * bits_per_samp,
		unsigned int * buffer_size) {

	int ChunkSize;

	std::cout << "RBA about to parse header" << std::endl;

	unsigned char buf[5];

	/* ChunkID (RIFF for little-endian (lo/hi), RIFX for big-endian hi/lo) */
	fread(buf, 1, 4, file_hd);
	buf[4] = '\0';

//    $0000-0003: Chunk ID  "RIFF"  in  ASCII  ("RIFX"  files  identify  the
//                samples audio data in hi/lo format instead of  the  normal
//                lo/hi format)

	if (strcmp((char*) buf, "RIFF")) {

//		std::cout << "RBA ERROR did NOT see RIFF ... instead saw -->" << buf << "<--"
//				<< std::endl;

		std::stringstream local_string;
		local_string << "RBA ERROR did NOT see RIFF ... instead saw -->" << buf << "<--"
					<< std::endl;
		std::cout << local_string.str();

		exit(EXIT_FAILURE);

	} else {

		printf("ChunkID is little-endian since seeing RIFF (lo/hi) not RIFX\n");
	}

	// ------------------------------------------------- //

//    0004-0007: Chunk size (lo/hi) $01CC45D4=30,164,452 is the total  size
//               minus 8 (these first 8  bytes  are  not  included  in  the
//               overall size)

	/* ChunkSize */
	fread(buf, 1, 4, file_hd);

	ChunkSize = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);

	std::cout << "(RBA ChunkSize)  RIFF " << ChunkSize << std::endl;

	// ------------------------------------------------- //

//    0008-000B: Chunk format "WAVE". This format requires two subchunks to
//               exist, "fmt " and "data".

	/* Format */
	fread(buf, 1, 4, file_hd);
	buf[4] = '\0';
	if (strcmp((char*) buf, "WAVE")) {

		std::cout << "RBA ERROR did NOT see WAVE ... instead saw -->" << buf << "<--"
				<< std::endl;

		exit(EXIT_FAILURE);
	}

	printf("RBA format is %s\n", buf);

	// ------------------------------------------------- //

//    000C-000F: Subchunk1 ID "fmt ". This describes the format of the next
//               DATA subchunk.

	/* Subchunk1ID */
	fread(buf, 1, 4, file_hd);
	buf[4] = '\0';
	if (strcmp((char*) buf, "fmt ")) {

		std::cout << "RBA ERROR did NOT see fmt ... instead saw -->" << buf << "<--"
				<< std::endl;

		exit(EXIT_FAILURE);
	}

	printf("OK    000C-000F: Subchunk1 ID must say fmt as in ... %s\n", buf);

	// ------------------------------------------------- //


//    0010-0013: Subchunk size (lo/hi) $00000010 (usually  this  value  for
//               PCM audio)
//	16 for PCM.  This is the size of the rest of the Subchunk which follows this number.

	/* Subchunk1Size (16 for PCM) */
	fread(buf, 1, 4, file_hd);
	buf[4] = '\0';

	if (buf[0] != 16 || buf[1] || buf[2] || buf[3]) {

		std::cout << "RBA ERROR did NOT see Subchunk1Size ... instead saw -->" << buf << "<--"
				<< std::endl;

		std::cout << "buf[0]  ==  " << buf[0] << std::endl;

		std::stringstream local_string;
		local_string << "RBA ERROR did NOT see Subchunk1Size ... instead saw -->" << buf << "<--"
				<< std::endl;
		std::cout << local_string.str();


		if (18 == buf[0]) {

			std::cout << "seeing buf[0] == 18 \n" << std::endl;
		}

		exit(EXIT_FAILURE);
	}

	printf("%d bits\n", buf[0]);

	// ------------------------------------------------- //

//    0014-0015: Audio format (1=PCM, 2 and higher are custom) ($0001)
//               - $0001 = standard PCM
//               - $0101 = IBM mu-law (custom)
//               - $0102 = IBM a-law (custom)
//               - $0103 = IBM AVC ADPCM (custom)

	/* AudioFormat (PCM = 1, other values indicate compression) */
	fread(buf, 1, 2, file_hd);
	if (buf[0] != 1 || buf[1]) {

		std::cout << "RBA ERROR did NOT see AudioFormat ... instead saw -->" << buf << "<--"
				<< std::endl;

		exit(EXIT_FAILURE);
	}

	if (buf[0] == 1) {

		printf("Seeing PCM AudioFormat so no compression\n");

	}

	// ------------------------------------------------- //

//    0016-0017: Number of channels (1=mono, 2=stereo, etc) ($0002)

	/* NumChannels (Mono = 1, Stereo = 2, etc) */
	fread(buf, 1, 2, file_hd);
	unsigned int num_ch = buf[0] + (buf[1] << 8);
	if (num_ch != 1) {

		std::cout << "RBA ERROR did NOT see NumChannels ... instead saw -->" << buf << "<--"
				<< std::endl;

		exit(EXIT_FAILURE);
	}

	if (num_ch == 1) {

		printf("seeing Mono since num_ch is %d\n", num_ch);
	}

	// ------------------------------------------------- //

//    0018-001B: Sample rate per second (lo/hi) $0000AC44 = 44100

	/* SampleRate (8000, 44100, etc) */
	fread(buf, 1, 4, file_hd);
	*samp_rate = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);

	printf("BinaryIO file parser sample rate %d\n", * samp_rate);

	// ------------------------------------------------- //

//    001C-001F: Byte rate per second (=sample rate * number of channels  *
//               (bits per channel/8)) $0002B110 = 176400

	/* ByteRate (SampleRate * NumChannels * BitsPerSample / 8) */
	fread(buf, 1, 4, file_hd);
	const unsigned int byte_rate = buf[0] + (buf[1] << 8) + (buf[2] << 16)
			+ (buf[3] << 24);

	// ------------------------------------------------- //

//    0020-0021: Block Align (=number of  channels  *  bits  per  sample/8)
//               ($0004).

	/* BlockAlign (NumChannels * BitsPerSample / 8) */
	fread(buf, 1, 2, file_hd);
	const unsigned int block_align = buf[0] + (buf[1] << 8);

	// ------------------------------------------------- //

//    0022-0023: Bits per sample (8=8 bits, 16=16  bits)  ($0010).  Samples
//               not using the entire bit range allocated  should  set  the
//               unused bits off.

	/* BitsPerSample */
	fread(buf, 1, 2, file_hd);
	*bits_per_samp = buf[0] + (buf[1] << 8);

	if (byte_rate != ((*samp_rate * num_ch * *bits_per_samp) >> 3)) {

		std::cout << "RBA ERROR did NOT see BitsPerSample ... instead saw -->" << buf << "<--"
				<< std::endl;

		exit(EXIT_FAILURE);
	}

	if (block_align != ((num_ch * (* bits_per_samp)) >> 3))
		exit(EXIT_FAILURE);

	printf("(bits per sample) Bit Width %d\n", * bits_per_samp);

	// ------------------------------------------------- //

//    0024-0027:  Subchunk2  ID  "data".  This  chunk  contains  the  audio
//               samples. There can be more than one in a WAV file.

	/* Subchunk2ID */
	fread(buf, 1, 4, file_hd);
	buf[4] = '\0';
	if (strcmp((char*) buf, "data")) {

		std::cout << "RBA ERROR in Subchunk2ID did NOT see 'data' ... instead saw -->" << buf << "<--"
				<< std::endl;

		exit(EXIT_FAILURE);
	}

	// ------------------------------------------------- //

//    0028-002B: Subchunk2 size (lo/hi) ($01CC45B0=30,164,400)

	/* Subchunk2Size (NumSamples * NumChannels * BitsPerSample / 8) */
	fread(buf, 1, 4, file_hd);
	const unsigned int subchunk2_size = buf[0] + (buf[1] << 8) + (buf[2] << 16)
			+ (buf[3] << 24);

	printf("subchunk2_size raw prior to shift 3 to the left %d\n", subchunk2_size);
	printf("subchunk2_size raw POST    shift 3 to the left %d\n", (subchunk2_size << 3));
	std::cout << "RBA num_ch " << num_ch << std::endl;
	std::cout << "RBA bits_per_samp " << (* bits_per_samp) << std::endl;

//	* buffer_size = (subchunk2_size << 3) / (num_ch * (* bits_per_samp));

	* buffer_size = subchunk2_size;

	printf("number of bytes in data payload post this header %d\n", subchunk2_size);

	printf("byte_rate == (SampleRate * NumChannels * BitsPerSample / 8) %d\n", byte_rate);
	printf("RBA buffer_size %d\n", (* buffer_size));

}	//	read_wav_header


void Binary_File_IO::read_wav_data(short int * buff_data, unsigned int samp_rate,
		unsigned int bits_per_samp, unsigned int source_buffer_size, bool flag_want_print) {

//    002C-xxxx: Audio data (lo/hi),  stored  as  2's  complimented  signed
//               integers in the following order:
//
//                 Sample 1: Channel1 (left)   ($0095=+149)
//                 Sample 1: Channel2 (right)  ($FF6D=-)
//                 Sample 2: Channel1 (left)   ($0097=+151)
//                 Sample 2: Channel2 (right)  ($FF77=-)
//                 Sample 3: Channel1 (left)   ($006E=+110)
//                 Sample 3: Channel2 (right)  ($FF25=-)


	file_hd = get_file_handle();

	printf("source_buffer_size %d\n", source_buffer_size);

	unsigned char buf;
	unsigned int i, j;
	for (i = 0; i < source_buffer_size; ++i) {
		unsigned int tmp = 0;
		for (j = 0; j != bits_per_samp; j += 8) {
			fread(&buf, 1, 1, file_hd);
			tmp += buf << j;
		}
		buff_data[i] = conv_bit_size(tmp, bits_per_samp);

		if (flag_want_print) {

			printf("%d     orig  %d    new %d\n", i, tmp,  buff_data[i]);

		}
	}

//	Notes: 8-bit samples are stored as  unsigned  chars,  with  values  ranging
//	       ranging from 0-255. 16-bit samples are stored  as  signed  integers,
//	       with values ranging from -32768 to 32767

}


// -----------------------------


FILE * Binary_File_IO::get_file_handle() {

	return file_hd;
}

void Binary_File_IO::write_binary_file(
		std::string filename,
		short int * buff_data,
		unsigned int source_buffer_size) {

//#include <stdio.h>
//const unsigned long long size = 8ULL*1024ULL*1024ULL;
//unsigned long long a[size];

    FILE * pFile;
//    pFile = fopen("file.binary", "wb");
    pFile = fopen(filename.c_str(), "wb");

//    for (unsigned long long j = 0; j < 1024; ++j){
        //Some calculations to fill a[]
//    fwrite(buff_data, 1, source_buffer_size * sizeof(unsigned long long), pFile);
    fwrite(buff_data, 1, source_buffer_size * sizeof(short int), pFile);
//    }
    fclose(pFile);

}

void Binary_File_IO::init_file_read(std::string filename) {

//	if (file_hd) {
//
//		printf("RBA ERROR - seeing non NULL file handle\n");
//		exit(9);
//	}

	std::cout << "filename is " <<  filename << std::endl;

	file_hd = fopen(filename.c_str(), "rb");

	if (! file_hd) {

		std::cout << "ERROR - failed to open file " <<  filename << std::endl;
		exit(8);
	}

	std::cout << "OK open file" << std::endl;
}

short int Binary_File_IO::conv_bit_size(unsigned int in, int bps) { // converts unsigned into signed

	const unsigned int max = (1 << (bps - 1)) - 1;
//	return in > max ? in - (max << 1) : in;
	return in > max ? in - ((max << 1) + 2) : in;
}




// -----------------------------

Binary_File_IO::Binary_File_IO() {}

Binary_File_IO::~Binary_File_IO() {}


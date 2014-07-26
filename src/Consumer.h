/*
 * Consumer.h
 *
 *  Created on: Jun 5, 2013
 *      Author: stens
 */

#ifndef CONSUMER_H_
#define CONSUMER_H_

#include "Circular_Buffer.h"

class Consumer {

public:

	Consumer();
	virtual ~Consumer();

	// -----------------------

	void set_circular_buffer(Circular_Buffer * given_circular_buffer);

//	bool get_next_available_memory_buffer(Data_Transfer * given_data_transfer);

	bool get_next_available_memory_buffer(struct_audio_array *);

	bool get_next_consumable();



//	void outnum(std::string given_label, int n);



//	void outnum(std::string given_label, int n);

//	void outnum(std::string given_label, int n);
	void outnum();


	bool do_consumption();
//	void do_consumption();
//	void consume();

	void consume(Circular_Buffer * given_circular_buffer);

private:

	int loop_counter;
	bool flag_continue_processing;

	Circular_Buffer * circular_buffer;
};

#endif /* CONSUMER_H_ */

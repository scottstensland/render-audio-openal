/*
 * Audio_Controller.h
 *
 *  Created on: Dec 12, 2013
 *      Author: stens
 */

#ifndef AUDIO_CONTROLLER_H_
#define AUDIO_CONTROLLER_H_


#include "Audio_Model.h"
#include "Audio_View.h"


class Audio_Controller {
public:
//	Audio_Controller();

	Audio_Controller(Audio_Model * audio_model, Audio_View * audio_view);
	virtual ~Audio_Controller();

	// --------------------

	void do_value_handovers_to_view();

	void do_value_handovers_to_model();

	void entry_point();

private:


	Audio_Model * audio_model;
	Audio_View * audio_view;

	bool do_we_want_opengl;

	bool continue_processing;

	float size_point;

};

#endif /* AUDIO_CONTROLLER_H_ */

/*
 * Audio_View.h
 *
 *  Created on: Dec 12, 2013
 *      Author: stens
 */

#ifndef AUDIO_VIEW_H_
#define AUDIO_VIEW_H_

#define GLM_FORCE_RADIANS

#include "Audio_Model.h"

class Audio_View {

public:

//	Audio_View();
	Audio_View(Audio_Model * given_audio_model);

	virtual ~Audio_View();

	void entry_point();

	void set_point_size(float given_point_size);

	// ------------

private:

	Audio_Model * audio_model;

	bool do_fullscreen;

	bool flag_do_entire_universe_or_just_paddle;

	float size_point_controllable;

	std::string WINDOW_TITLE_PREFIX = "hello Corinde";

	bool continue_processing;

	int CurrentWidth, CurrentHeight;

};

#endif /* AUDIO_VIEW_H_ */

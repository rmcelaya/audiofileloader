#pragma once

#include "audio_signal.h"



enum error_code {
	E_AUDIOFILES_SUCCESS,
	E_AUDIOFILES_SYSTEM_MALLOC,
	E_AUDIOFILES_SYSTEM_ACCESS,
	E_AUDIOFILES_FORMAT,
	E_AUDIOFILES_SUPPORTED,
	E_AUDIOFILES_BAD_PARAMETER,
};

/*
struct error_data{
	int error_code;
	char* error_msg;
};


struct error_data error_list[] = {
	{E_AUDIOFILES_SUCCESS,""},
	{E_AUDIOFILES_SYSTEM_MALLOC,"Malloc failed"},
	{E_AUDIOFILES_SYSTEM_ACCESS,"Error while accessing the file"},
	{E_AUDIOFILES_FORMAT,"The file type could not be recognized or is not supported by this program" },
	{E_AUDIOFILES_SUPPORTED,"The files has characteristics not supported by this program yet" },
	{E_AUDIOFILES_BAD_PARAMETER,"Incorrect parameter" }
};
*/



enum audio_file_code{
    WAV
};



int read_audio_file (const char* path, audio_signal_t* audio);

int save_audio_file (const char* path, audio_signal_t audio, enum audio_file_code code, void* options);


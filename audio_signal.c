
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "audio_signal.h"
#include "audio_files.h"
#include "cross_compat.h"




void initialize_audio(audio_signal_t *audio)
{
    audio->info.alloc_channels_array = 0;
    audio->info.channels = 0;
	audio->info.num_samples = 0;

    //Frequency is one to avoid division by 0 problems
    audio->info.sampling_frequency = 1;
    audio->samples = NULL;
}

void delete_channel(audio_signal_t *audio, unsigned int channel_number, boolean_t free_data)
{
	if (channel_number > audio->info.channels - 1) return;

    if (free_data)
    {
        if(audio->samples[channel_number]){
            free(audio->samples[channel_number]);
        }
    }
    for (int i = channel_number; i < audio->info.channels - 1; i++)
    {
        audio->samples[i] = audio->samples[i + 1];
		audio->samples[i + 1] = NULL;
    }
	audio->samples[audio->info.channels - 1] = NULL;
	audio->info.channels--;
}

 int add_channel(audio_signal_t *audio, float32_t *channel)
{

    if (audio->info.alloc_channels_array == 0 || audio->samples == NULL)
    {
        audio->samples = malloc(sizeof(float32_t *));
		if (!audio->samples) {
			return E_AUDIOFILES_SYSTEM_MALLOC;
		}
        audio->samples[0] = channel;
		audio->info.channels++;
		audio->info.alloc_channels_array = 1;
		return 0;
    }
    if (audio->info.alloc_channels_array == audio->info.channels)
    {
		size_t osize = audio->info.alloc_channels_array;
		size_t nsize = osize * CHANNELS_ARRAY_GROWH_FACTOR;
		
		float32_t** tmp = realloc(audio->samples, sizeof(float32_t *) * nsize);
		if (tmp == NULL) {
			return E_AUDIOFILES_SYSTEM_MALLOC;
		}
		audio->samples = tmp;
		for (size_t i = osize; i < nsize; i++) {
			audio->samples[i] = NULL;
		}
		audio->info.alloc_channels_array = nsize;
    }
    audio->info.channels++;
	audio->samples[audio->info.channels - 1] = channel;
	return 0;
}

void swap_channels(audio_signal_t *audio, unsigned int channel1, unsigned int channel2)
{
	if (channel1 > audio->info.channels - 1 || channel2 > audio->info.channels - 1) return;

    float *tmp = audio->samples[channel1];
    audio->samples[channel1] = audio->samples[channel2];
    audio->samples[channel2] = tmp;
}

void delete_audio(audio_signal_t *audio)
{
    if (audio->samples)
    {
        for (int i = 0; i < audio->info.channels; i++)
        {
            if (audio->samples[i])
            {
                free(audio->samples[i]);
                audio->samples[i] = NULL;
            }
        }
        free(audio->samples);
        audio->samples = NULL;
    }
    audio->info.channels = 0;
    audio->info.alloc_channels_array = 0;
}

int copy_audio(audio_signal_t *copy, audio_signal_t *original)
{
    copy->info.channels = original->info.channels;
    copy->info.num_samples = original->info.num_samples;
    copy->info.sampling_frequency = original->info.sampling_frequency;
    int e = allocate_channels(copy,copy->info.channels,copy->info.num_samples);
    if (e)
    {
        return e;
    }
    for (int i = 0; i < copy->info.channels; i++)
    {
        memcpy(copy->samples[i], original->samples[i], original->info.num_samples * sizeof(float32_t));
    }
    return E_AUDIOFILES_SUCCESS;
}

int emancipate_channel(audio_signal_t *audio, audio_signal_t* new_audio, int channel_number)
{
	if (channel_number > audio->info.channels - 1) return E_AUDIOFILES_BAD_PARAMETER;
	initialize_audio(new_audio);
    new_audio->info.num_samples = audio->info.num_samples;
    new_audio->info.sampling_frequency = audio->info.sampling_frequency;
    float32_t* c = audio->samples[channel_number];
	int err = add_channel(new_audio, c);
	if (err == 0) {
		delete_channel(audio, channel_number, FALSE);
	}
	return err;
}


int allocate_channels(audio_signal_t *audio, uint16_t num_chann, size_t num_samples)
{

	size_t ns = num_chann * sizeof(float32_t*);
    audio->samples = malloc(ns);
    if (!audio->samples)
    {
        return E_AUDIOFILES_SYSTEM_MALLOC;
    }

	for (int i = 0; i < num_chann; i++) {
		audio->samples[i] = NULL;
	}

	int err;
    for (int i = 0; i < num_chann; i++)
    {

        float* tmp = malloc(num_samples * sizeof(float32_t));
        if (tmp == NULL){
            delete_audio(audio);
            return E_AUDIOFILES_SYSTEM_MALLOC;
        }
        audio->samples[i] = tmp;
    }

	audio->info.channels = num_chann;
	audio->info.alloc_channels_array = num_chann;
	audio->info.num_samples = num_samples;
    return 0;
}
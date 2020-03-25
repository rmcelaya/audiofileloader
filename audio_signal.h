#pragma once

#include "audio_files.h"


#define TRUE 1
#define FALSE 0

typedef int boolean_t;



#define DEFAULT_CHANNEL 0
#define LEFT_CHANNEL 0
#define RIGHT_CHANNEL 1



#define CHANNELS_ARRAY_GROWH_FACTOR 3


#define AUDIO_DURATION(audio) ((audio).info.num_samples)/((audio).info.sampling_frequency)


struct audio_info
{
  uint32_t sampling_frequency;
  uint16_t channels;
  unsigned long num_samples;
  unsigned int alloc_channels_array;
};


struct audio_signal_8bit
{
  struct audio_info info;
  int8_t** samples;
} ;

struct audio_signal_16bit
{
  struct audio_info info;
  int16_t** samples;
} ;

struct audio_signal_32bit
{
  struct audio_info info;
  float32_t** samples;
} ;


typedef struct audio_signal_32bit audio_signal_t;



/*
Function: initialize audio

Description: it initializes the main values of the audio signal so
that no garbage is stored

Remarks: 
*/
void initialize_audio(audio_signal_t *audio);




/*
Function: add_channel

Description: it adds a new channel to the audio signal. float32_t* channel is the signal
to be added.

Remarks: it allocates memory in the heap for the pointer of the new channel (not much memory),
This function doesn't check whether the number of samples in the new channel is the same as
the number of samples in the audio signal, because it doesn't even know that datum. It only copies
the pointer to the first element of the array in the audio's array of channels.
*/

int add_channel(audio_signal_t *audio, float32_t* channel);


/*
Function: delete_channel

Description: it deletes the channel given its index (channel_number). If free_data
is set to false the pointer is lost but the data is not freed. All the channels shift down
in the array of channels when another channel is deleted (their channel number change)

Remarks: set free_data to TRUE unless you have the signal pointer stored in another place. 
Nothing is done if the channel index is out of range
*/
void delete_channel(audio_signal_t *audio, int channel_number, boolean_t free_data);

/*
Function: swap_channels

Description: swaps two channels in the channel list

Remarks: nothing is done if the channel indexes are out of range
*/

void swap_channels (audio_signal_t* audio, int channel1,int channel2);


/*
Function: emancipate_channel

Description: deletes the channel in "audio" with the index "channel_number"
and creates a new whole audio ("new_audio") that contains only that channel. 
It creates a new audio from a single channel and deletes it from the the old audio.

Remarks: add_channel is used, hence memory is allocated for the array of channels in 
the new audio
*/
int emancipate_channel(audio_signal_t* audio, audio_signal_t* new_audio, int channel_number);


/*
Function: delete_audio

Description: it frees all the memory allocated at the audio and puts al the pointers to NULL
to avoid problems

Remarks:
*/
void delete_audio (audio_signal_t* audio);



/*
Function: allocate_channels

Description: Given an audio signal and some number of channels and samples,
it allocates heap memory for the channels (both for the channel array and for
the channels themselves. It assumes the audio didn't have any channels
allocated before.

Remarks: memory is allocated in the heap for the channel array and for the channels themselves.
*/
int allocate_channels(audio_signal_t *audio, uint16_t num_chann, size_t num_samples);


/*
Function: copy_audio

Description: makes a copy of the original audio into the copy

Remarks: memory is allocated in the heap for the copy, the only field 
that is not copied is the audio id
*/
int copy_audio(audio_signal_t *copy, audio_signal_t *original);
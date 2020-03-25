
#pragma once


#include "audio_signal.h"
#include "cross_compat.h"

#define RIFF_ID 0x46464952
#define WAVE_TYPE 0x45564157
#define DATA_ID 0x61746164
#define FMP_ID 0x20746D66

PACK(struct riff_header
{
    uint32_t chunk_id;
    uint32_t chunk_size;
    uint32_t format;
});

PACK(struct format_chunk
{
    uint32_t subchunk_1_id;
    uint32_t subchunk_1_size;
    uint16_t audio_format;
    uint16_t number_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
});

PACK(struct data_chunk_header
{
    uint32_t data_id;
    uint32_t data_size;
});



boolean_t check_wav (FILE* f);

int load_wav (FILE* f,audio_signal_t* audio);

int save_wav (FILE* f, audio_signal_t audio, void* options);

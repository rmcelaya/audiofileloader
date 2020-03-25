
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "file_wav.h"
#include "audio_signal.h"
#include "audio_files.h"


int check_wav(FILE *f,boolean_t* result)
{

    struct riff_header riff_h;

    size_t n = fread(&riff_h, 12, 1, f);
    if (n != 1)
    {
        return E_AUDIOFILES_SYSTEM_ACCESS;
    }

    //RIFF and WAVE signatures
    if (riff_h.chunk_id != RIFF_ID || riff_h.format != WAVE_TYPE)
    {
        *result = FALSE;
    }else{

        *result = TRUE;
    }

    return 0;
}


int load_wav(FILE *f, audio_signal_t *audio)
{

    fseek(f, 12, SEEK_SET);
    struct format_chunk fmp_h;
    size_t n;
    n = fread(&fmp_h, 24, 1, f);
    if (n != 1)
    {
        return E_AUDIOFILES_SYSTEM_ACCESS;
    }

    if (fmp_h.subchunk_1_id != FMP_ID)
    {
        return E_AUDIOFILES_FORMAT;
    }
    if (!((fmp_h.audio_format == 1) || ((fmp_h.audio_format == 3) && (fmp_h.bits_per_sample == 32))))
    {
        return E_AUDIOFILES_SUPPORTED;
    }
    if ((audio->info.channels = fmp_h.number_channels) > 2)
    {
        return E_AUDIOFILES_SUPPORTED;
    }
    audio->info.sampling_frequency = fmp_h.sample_rate;
    struct data_chunk_header data_h;
    n = fread(&data_h, 8, 1, f);
    if (n != 1)
    {
        return E_AUDIOFILES_SYSTEM_ACCESS;
    }

    if (data_h.data_id != DATA_ID)
    {
        return E_AUDIOFILES_FORMAT;
    }

    switch (fmp_h.bits_per_sample)
    {
    case 8:
    case 16:
        return E_AUDIOFILES_SUPPORTED;
    case 32:
        audio->info.num_samples = data_h.data_size / ((fmp_h.bits_per_sample / 8) * audio->info.channels);
        n = allocate_channels (audio,audio->info.channels,audio->info.num_samples);
        if (n){
            return (int)n;
        }
        for (unsigned int i = 0; i < audio->info.num_samples; i++)
        {
            for (int j = 0; j < audio->info.channels; j++)
            {
                n = fread(&audio->samples[j][i], sizeof(float), 1, f);
                if (n != 1)
                {
                    delete_audio (audio);
                    return E_AUDIOFILES_SYSTEM_ACCESS;
                }
            }
        }
        break;
    }
    return 0;
}

int save_wav(FILE *f, audio_signal_t audio, void *options)
{
    struct riff_header header;
    struct format_chunk format;
    struct data_chunk_header data_head;
    header.chunk_id = RIFF_ID;
    header.chunk_size = 36 + audio.info.num_samples * audio.info.channels * 4;
    header.format = WAVE_TYPE;
    format.audio_format = 3;
    format.bits_per_sample = 32;
    format.number_channels = audio.info.channels;
    format.subchunk_1_id = FMP_ID;
    format.subchunk_1_size = 16;
    format.sample_rate = audio.info.sampling_frequency;
    format.byte_rate = format.sample_rate * format.number_channels * (format.bits_per_sample / 8);
    format.block_align = (format.bits_per_sample / 8) * format.number_channels;
    data_head.data_id = DATA_ID;
    data_head.data_size = audio.info.num_samples * 4 * audio.info.channels;

    size_t n = fwrite(&header, sizeof(struct riff_header), 1, f);
    if (n != 1)
    {
        return E_AUDIOFILES_SYSTEM_ACCESS;
    }
    n = fwrite(&format, sizeof(struct format_chunk), 1, f);
    if (n != 1)
    {
        return E_AUDIOFILES_SYSTEM_ACCESS;
    }
    n = fwrite(&data_head, sizeof(struct data_chunk_header), 1, f);
    if (n != 1)
    {
        return E_AUDIOFILES_SYSTEM_ACCESS;
    }

    for (unsigned int i = 0 ;i< audio.info.num_samples;i++){
        for (int j = 0;j<audio.info.channels;j++){
            n = fwrite (&audio.samples[j][i],sizeof (float32_t),1,f);
            if (n != 1){
                return E_AUDIOFILES_SYSTEM_ACCESS;
            }
        }

    }
   
    return 0;
}

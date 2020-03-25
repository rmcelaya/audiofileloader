
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "audio_files.h"
#include "file_wav.h"
#include "cross_compat.h"


struct audio_file_type{
    enum audio_file_code  type;
    char* file_extension;
    int (*check) (FILE* fd,boolean_t* result);
    int (*load) (FILE* fd, audio_signal_t* audio);
    int (*save) (FILE* fd, audio_signal_t audio, void* options);
};


struct audio_file_type aft[]={
    {WAV,"wav",check_wav,load_wav,save_wav}
};


#define NUM_FILE_TYPES (sizeof aft /sizeof aft[0])


int read_audio_file (const char* path, audio_signal_t* audio){

    FILE *f = fopen(path, "rb");
    if (!f)
    {
        return E_AUDIOFILES_SYSTEM_ACCESS;
    }
    struct audio_file_type ft;
    initialize_audio(&ft);
    boolean_t r = FALSE;
    int e;
    for (int i = 0;i<NUM_FILE_TYPES;i++){
        ft = aft[i];
        e = ft.check(f,&r);
        if (e){
            return e;
        }
        if(r){
            break;
        }
    }

    if (!r){
        return E_AUDIOFILES_FORMAT;
    }
    rewind(f);
    int error = ft.load (f,audio);
    fclose (f);
    return error;
}

int save_audio_file (const char* path, audio_signal_t audio, enum audio_file_code code, void* options){

    struct audio_file_type ft;
    ft = aft[code];
    FILE* f = fopen (path, "wb");
    if (!f){
        return E_AUDIOFILES_SYSTEM_ACCESS;
    }
    int error = ft.save (f,audio,options);
    fclose(f);
    return error;
}
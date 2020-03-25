#include "audio_files.h"



int main(int argc, char** argv){

    char* filename = "test.wav";
    char* save_filename = "test_saved.wav";
    audio_signal_t a;
    initialize_audio(&a);
    int e = read_audio_file(filename,&a);
    if (e){
        printf("Error when reading the file %d\n",e);
        return e;
    }

    e = save_audio_file(save_filename,a,WAV,NULL);
    if (e){
        printf("Error when writin the file %d\n",e);
        return e;
    }
    return 0;
}
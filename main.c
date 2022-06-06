#include "audio.h"
#include "video.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    PaData data;
    data.coords = malloc(10 * sizeof(*data.coords));
    data.num_coords = 0;
    data.coords_cap = 10;
    Pa_Initialize();
    PaError e = audio_run(&data);
    printf("%s\n", Pa_GetErrorText(e));
    
    video_run(&data);
    
    e = audio_stop();
    printf("%s\n", Pa_GetErrorText(e));
    Pa_Terminate();
    return 0;
}

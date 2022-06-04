#include <portaudio.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "audio.h"
#define FORMAT paInt16

uint32_t idx = 0;
uint32_t lastidx = 0;
int64_t ti = 0;

int64_t a = 0;

#define samples 5

int lerp(int a, int b, double c) {
    return a + c * (b - a);
}

static int pa_write_callback(const void *, void *output,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *user_data) {
    int16_t *out = (int16_t*)output;
    (void)timeInfo;
    (void)statusFlags;
    PaData *data = (PaData*)user_data;
    
    for (unsigned int i = 0; i < framesPerBuffer; i++) {
        int lx = data->coords[lastidx];
        int ly = data->coords[lastidx + 1];
        
        int x = lerp(lx, data->coords[idx], (double)(ti % samples + 1) / (double)samples);
        int y = lerp(ly, data->coords[idx + 1], (double)(ti % samples + 1) / (double)samples);

        *out++ = x * 65535 / 1024 - 32767;
        *out++ = -y * 65535 / 1024 - 32767;
        if (idx >= data->num_coords) {
            idx = 0;
            lastidx = 0;
        }
        if (ti % samples == 0) {
            idx += 2;
            lastidx = idx;
        }
        ti++;
    }

    return 0;
}

#define ERRHANDLE(err) if (err != paNoError) return err;

PaStream *outstream;

PaError audio_run(PaData *data) {
    PaError err;    
    err = Pa_OpenDefaultStream(&outstream, 0, 2, FORMAT, 48000, paFramesPerBufferUnspecified, pa_write_callback, (void*)data); ERRHANDLE(err);
    err = Pa_StartStream(outstream); ERRHANDLE(err);
    return paNoError;
}

PaError audio_stop() {
    PaError err;    
    err = Pa_StopStream(outstream);  ERRHANDLE(err);
    err = Pa_CloseStream(outstream); ERRHANDLE(err);
}

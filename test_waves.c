#include <portaudio.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define FORMAT paInt16

double n = 0;
uint32_t idx = 0;
uint32_t lastidx = 0;
int64_t ti = 0;

int64_t a = 0;

#define samples 5

int lerp(int a, int b, double c) {
    return a + c * (b - a);
}

double saw(int freq) {
    double na = n / 48000 * freq;
    return (na - (long)na) * 2 - 1;
}

double sinfr(int freq) {
    return sin(n / 48000 * freq * M_PI * 2);
}

double cosfr(int freq) {
    return cos(n / 48000 * freq * M_PI * 2);
}

static int pa_write_callback(const void *, void *output,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *) {
    int16_t *out = (int16_t*)output;
    (void)timeInfo;
    (void)statusFlags;
    for (unsigned int i = 0; i < framesPerBuffer; i++) {
        int lx = coords[lastidx];
        int ly = coords[lastidx + 1];
        int x = lerp(lx, coords[idx], (double)(ti % samples + 1) / (double)samples);
        int y = lerp(ly, coords[idx + 1], (double)(ti % samples + 1) / (double)samples);

        *out++ = x * 60;
        *out++ = -y * 60;
        if (idx >= sizeof(coords) / sizeof(coords[0])) {
            idx = 0;
            lastidx = 0;
        }
        if (ti % samples == 0) {
            idx += 2;
            lastidx = idx;
        }
        ti++;
        /*const int fsaw = 200;
        const int fsin = 1600;
            
        *out++ = saw(fsaw) * 32767;//sinfr(fsin) * 32767 / 5 * (fmod(n / 48000 * fsaw * M_PI * 2, M_PI * 2) > (M_PI * 3.0/2.0) ? sinfr(fsaw) * 1.5 : 0.16) + saw(fsin) * cosfr(fsin + 1) * 8000;
        *out++ = cosfr(fsin) * 32767 / 5 + saw(fsaw) * 20000;
        n += 1;*/
    }

    return 0;
}

#define ERRHANDLE(err) if (err != paNoError) return err;

PaStream *outstream;

PaError audio_run() {
    PaError err;    
    //err = Pa_OpenDefaultStream(&instream,  1, 0, FORMAT, SAMPLE_RATE, paFramesPerBufferUnspecified, pa_read_callback,  this); ERRHANDLE(err);
    err = Pa_OpenDefaultStream(&outstream, 0, 2, FORMAT, 48000, paFramesPerBufferUnspecified, pa_write_callback, NULL); ERRHANDLE(err);
    err = Pa_StartStream(outstream); ERRHANDLE(err);
    while (1) {
        Pa_Sleep(500);
    }
    err = Pa_StopStream(outstream);  ERRHANDLE(err);
    err = Pa_CloseStream(outstream); ERRHANDLE(err);
    return paNoError;
}

int main() {
    Pa_Initialize();
    PaError e = audio_run();
    printf("%s\n", Pa_GetErrorText(e));
    Pa_Terminate();
    return 0;
}

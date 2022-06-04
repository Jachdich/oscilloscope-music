#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <portaudio.h>

struct PaData {
    int32_t *coords;
    uint32_t num_coords;
    uint32_t coords_cap;
};
typedef struct PaData PaData;

PaError audio_run(PaData *data);
PaError audio_stop();

#ifdef __cplusplus
}
#endif

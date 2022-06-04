#include "audio.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stdlib.h>

void add_coord(PaData *data, int32_t x, int32_t y) {
    if (data->coords_cap <= (data->num_coords + 2)) {
        data->coords = realloc(data->coords, sizeof(*data->coords) * (data->coords_cap *= 2));
    }   
    data->coords[data->num_coords++] = x;
    data->coords[data->num_coords++] = y;
}

void video_run(PaData *data) {
    SDL_Window *screen = NULL;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(1024, 1024, SDL_WINDOW_SHOWN, &screen, &renderer);
	if (!screen) {
		fprintf(stderr, "Failed to init screen\n");
	}

    int running = 1;
    while (running) {
        SDL_Event event;
    	while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_MOUSEBUTTONDOWN: {
			    SDL_MouseButtonEvent *ev = (SDL_MouseButtonEvent*)&event;
			    add_coord(data, ev->x, ev->y);
			    break;
			}
			default:
				break;
			}

		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		for (uint32_t i = 2; i < data->num_coords; i += 2) {
            SDL_RenderDrawLine(renderer, data->coords[i - 2], data->coords[i - 1], data->coords[i], data->coords[i + 1]);
		}
		uint32_t l = data->num_coords;
		if (l > 0) {
    		int mx, my;
    		SDL_GetMouseState(&mx, &my);
    		SDL_RenderDrawLine(renderer, data->coords[l - 2], data->coords[l - 1], mx, my);
		}
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(screen);
    SDL_Quit();
}

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

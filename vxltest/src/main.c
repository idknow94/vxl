#include "vxl.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int main(void) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Proper SDL3 window + renderer creation check
    if (!SDL_CreateWindowAndRenderer("vxltest", WIDTH, HEIGHT, 0, &window,
                                     &renderer)) {
        printf("SDL_CreateWindowAndRenderer failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Texture *texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                          SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    if (!texture) {
        printf("SDL_CreateTexture failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    VxlFrame frame = vxlCreateFrame(WIDTH, HEIGHT);
    vxlSetFrame(&frame);

    VxlVoxels voxels = vxlCreateVoxels(100, 10, 100);
    vxlSetVoxels(&voxels);

    VxlCamera camera = vxlCreateCamera(5, 5, 5, -0, -30, 80, false);
    vxlSetCamera(&camera);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        camera.yaw += 10.0f;

        vxlRender();

        SDL_UpdateTexture(texture, NULL, frame.pixels, WIDTH * 3);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

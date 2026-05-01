#include "vxl.h"
#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define FPS 120

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int main(void) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (!SDL_CreateWindowAndRenderer("vxltest", WIDTH, HEIGHT, 0, &window,
                                     &renderer)) {
        printf("SDL_CreateWindowAndRenderer failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderLogicalPresentation(renderer, WIDTH, HEIGHT,
                                     SDL_LOGICAL_PRESENTATION_STRETCH);
    uint32_t frameWidth = WIDTH / 2, frameHeight = HEIGHT / 2;


    SDL_Texture *texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                          SDL_TEXTUREACCESS_STREAMING, frameWidth, frameHeight);

    if (!texture) {
        printf("SDL_CreateTexture failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    VxlFrame frame = vxlCreateFrame(frameWidth, frameHeight);
    vxlSetFrame(&frame);

    VxlVoxels voxels = vxlCreateVoxels(10, 1, 10);
    vxlSetVoxels(&voxels);

    VxlCamera camera = vxlCreateCamera(5, -1, 5, -0, -0, 80, true);
    vxlSetCamera(&camera);

    bool running = true;
    SDL_Event event;

    Uint32 frameDelay = 1000 / FPS;
    Uint32 ct = SDL_GetTicks();

    while (running) {
        Uint32 nt = SDL_GetTicks();
        Uint32 frameTime = nt - ct;
        ct = nt;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        float deltaTime = frameTime / 1000.0f;

        camera.yaw += 60.0f * deltaTime;

        vxlRender();

        SDL_UpdateTexture(texture, NULL, frame.pixels, frameWidth * 3);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

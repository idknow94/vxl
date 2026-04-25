#include "vxl.h"
#include <math.h>
#include <stdlib.h>

VxlFrame vxlInitFrame(uint32_t width, uint32_t height) {
    uint8_t *pixels = malloc(width * height * 3 * sizeof(uint8_t));
    VxlFrame frame = {.width = width, .height = height, .pixels = pixels};
    return frame;
}

void vxlSetFrame(VxlFrame *frame) { s_frame = frame; }

void vxlSetCamera(VxlCamera *camera) { s_camera = camera; }

static inline void setColor(size_t idx, uint8_t r, uint8_t g, uint8_t b) {
    s_frame->pixels[idx] = r;
    s_frame->pixels[idx + 1] = g;
    s_frame->pixels[idx + 2] = b;
}

void vxlRender() {
    if (!vxlIsConfigured())
        return;

    float aspect = (float)s_frame->width / (float)s_frame->height;
    float fov = tanf(s_camera->fov * 0.5f * M_PI / 180.0f);
    bool fisheye = s_camera->fisheye;
    float ox = s_camera->x;
    float oy = s_camera->y;
    float oz = s_camera->z;
    float vx = (uint32_t)ox;
    float vy = (uint32_t)oy;
    float vz = (uint32_t)oz;
    for (uint32_t y = 0; y < s_frame->height; y++) {
        for (uint32_t x = 0; x < s_frame->width; x++) {
            float ndc_x = (2.0f * x / s_frame->width) - 1.0f;
            float ndc_y = (2.0f * y / s_frame->height) - 1.0f;

            float dx, dy, dz;

            if (fisheye) {
                dx = tanf(ndc_x * s_camera->fov * 0.5f * M_PI / 180.0f);
                dy = tanf(ndc_y * s_camera->fov * 0.5f * M_PI / 180.0f);
            } else {
                dx = ndc_x * aspect * fov;
                dy = ndc_y * fov;
            }

            dz = 1.0f;

            float len = sqrtf(dx * dx + dy * dy + dz * dz);
            dx /= len;
            dy /= len;
            dz /= len;
            int step_x = dx > 0 ? 1 : -1;
            int step_y = dy > 0 ? 1 : -1;
            int step_z = dz > 0 ? 1 : -1;
            float t_dx = fabsf(1.0f / dx);
            float t_dy = fabsf(1.0f / dy);
            float t_dz = fabsf(1.0f / dz);
            float t_max_x = (dx > 0 ? (vx + 1 - ox) : (ox - vx)) * t_dx;
            float t_max_y = (dy > 0 ? (vy + 1 - oy) : (oy - vy)) * t_dy;
            float t_max_z = (dz > 0 ? (vz + 1 - oz) : (oz - vz)) * t_dz;
            while (vx < s_voxels->xSize && vx >= 0 && vy < s_voxels->ySize &&
                   vx >= 0 && vz < s_voxels->zSize && vx >= 0) {
                if (t_max_x < t_max_y && t_max_x < t_max_z) {
                    vx += step_x;
                    t_max_x += t_dx;
                } else if (t_max_y < t_max_z) {
                    vy += step_y;
                    t_max_y += t_dy;
                } else {
                    vz += step_z;
                    t_max_z += t_dz;
                }
                if (vxlIsColliding(vx, vy, vz)) {
                    VxlVoxelType type = vxlGetVoxel(vx, vy, vz);
                    size_t idx = x + y * s_frame->width;
                    switch (type) {
                    case VXL_VOXEL_GRASS:
                        setColor(idx, 0, 255, 0);
                        break;
                    default:
                        setColor(idx, 0, 0, 255);
                        break;
                    }
                    break;
                }
            }
        }
    }
}

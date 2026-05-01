#include "globals.h"
#include "vxl.h"
#include <math.h>
#include <stdlib.h>

VxlFrame vxlCreateFrame(uint32_t width, uint32_t height) {
    uint8_t *pixels = malloc(width * height * 3 * sizeof(uint8_t));
    VxlFrame f = {.width = width, .height = height, .pixels = pixels};
    return f;
}

VxlCamera vxlCreateCamera(float x, float y, float z, float pitch, float yaw,
                          float fov, bool fisheye) {
    return (VxlCamera){.x = x,
                       .y = y,
                       .z = z,
                       .pitch = pitch,
                       .yaw = yaw,
                       .fov = fov,
                       .fisheye = fisheye};
}

void vxlSetFrame(VxlFrame *f) { frame = f; }
void vxlSetCamera(VxlCamera *c) { camera = c; }

static inline void setColor(size_t idx, uint8_t r, uint8_t g, uint8_t b) {
    frame->pixels[idx] = r;
    frame->pixels[idx + 1] = g;
    frame->pixels[idx + 2] = b;
}

void vxlRender() {
    if (!vxlIsConfigured())
        return;

    float aspect = (float)frame->width / (float)frame->height;
    float fov = tanf(camera->fov * 0.5f * M_PI / 180.0f);
    bool fisheye = camera->fisheye;
    float ox = camera->x;
    float oy = camera->y;
    float oz = camera->z;

    float yawR   = camera->yaw   * M_PI / 180.0f;
    float pitchR = camera->pitch * M_PI / 180.0f;
    float cosYaw   = cosf(yawR),   sinYaw   = sinf(yawR);
    float cosPitch = cosf(pitchR), sinPitch = sinf(pitchR);

    int32_t vx, vy, vz;

    for (uint32_t y = 0; y < frame->height; y++) {
        for (uint32_t x = 0; x < frame->width; x++) {
            float ndc_x = (2.0f * x / frame->width) - 1.0f;
            float ndc_y = (2.0f * y / frame->height) - 1.0f;

            vx = (int32_t)ox;
            vy = (int32_t)oy;
            vz = (int32_t)oz;

            float dx, dy, dz;
            if (fisheye) {
                dx = tanf(ndc_x * camera->fov * 0.5f * M_PI / 180.0f);
                dy = tanf(ndc_y * camera->fov * 0.5f * M_PI / 180.0f);
            } else {
                dx = ndc_x * aspect * fov;
                dy = ndc_y * fov;
            }
            dz = 1.0f;

            float len = sqrtf(dx * dx + dy * dy + dz * dz);
            dx /= len;
            dy /= len;
            dz /= len;

            float rx = dx * cosYaw + dz * sinYaw;
            float ry = dx * sinYaw * sinPitch + dy * cosPitch - dz * cosYaw * sinPitch;
            float rz = -dx * sinYaw * cosPitch + dy * sinPitch + dz * cosYaw * cosPitch;
            dx = rx; dy = ry; dz = rz;

            int step_x = dx > 0 ? 1 : -1;
            int step_y = dy > 0 ? 1 : -1;
            int step_z = dz > 0 ? 1 : -1;

            float t_dx = dx != 0 ? fabsf(1.0f / dx) : INFINITY;
            float t_dy = dy != 0 ? fabsf(1.0f / dy) : INFINITY;
            float t_dz = dz != 0 ? fabsf(1.0f / dz) : INFINITY;

            float t_max_x = (dx > 0 ? (vx + 1 - ox) : (ox - vx)) * t_dx;
            float t_max_y = (dy > 0 ? (vy + 1 - oy) : (oy - vy)) * t_dy;
            float t_max_z = (dz > 0 ? (vz + 1 - oz) : (oz - vz)) * t_dz;

            size_t idx = (x + y * frame->width) * 3;
            setColor(idx, 0, 0, 255);

            while (vx < (int32_t)voxels->xSize && vx >= 0 &&
                   vy < (int32_t)voxels->ySize && vy >= 0 &&
                   vz < (int32_t)voxels->zSize && vz >= 0) {
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
                    VxlVoxelType type = vxlGetVoxel((uint32_t)vx, (uint32_t)vy, (uint32_t)vz);
                    switch (type) {
                    case VXL_VOXEL_GRASS:
                        setColor(idx, 0, 255, 0);
                        break;
                    default:
                        break;
                    }
                    break;
                }
            }
        }
    }
}

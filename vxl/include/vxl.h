#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct VxlFrame {
    uint32_t width;
    uint32_t height;
    uint8_t *pixels; /// RGB format
} VxlFrame;

typedef struct VxlCamera {
    float x, y, z;
    float pitch, yaw;
    float fov;
    bool fisheye;
} VxlCamera;

typedef enum VxlVoxelType { VXL_VOXEL_AIR, VXL_VOXEL_GRASS } VxlVoxelType;

typedef struct VxlVoxels {
    uint32_t xSize;
    uint32_t ySize;
    uint32_t zSize;
    VxlVoxelType *voxels;
} VxlVoxels;

static VxlVoxels *s_voxels;
static VxlFrame *s_frame;
static VxlCamera *s_camera;

bool vxlIsConfigured();
VxlVoxels vxlCreateVoxels(uint32_t xSize, uint32_t ySize, uint32_t zSize);
VxlFrame vxlCreateFrame(uint32_t width, uint32_t height);
void vxlSetVoxels(VxlVoxels *voxels);
void vxlSetFrame(VxlFrame *frame);
void vxlSetCamera(VxlCamera *camera);
VxlVoxelType vxlGetVoxel(uint32_t x, uint32_t y, uint32_t z);
bool vxlIsColliding(float x, float y, float z);
void vxlUpdate();
void vxlRender();

#ifdef __cplusplus
}
#endif

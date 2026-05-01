#include "globals.h"
#include "vxl.h"
#include <stdlib.h>

#define VXL_VOXELS_IDX(x, y, z)                                                \
    ((y) * (voxels->zSize * voxels->xSize) + (z) * voxels->xSize + (x))

VxlVoxels vxlCreateVoxels(uint32_t xSize, uint32_t ySize, uint32_t zSize) {
    size_t size = xSize * ySize * zSize;
    VxlVoxelType *voxel_arr = malloc(size * sizeof(VxlVoxelType));
    for (int i = 0; i < size; i++) {
        voxel_arr[i] = VXL_VOXEL_GRASS;
    }
    VxlVoxels voxels = {
        .xSize = xSize, .ySize = ySize, .zSize = zSize, voxel_arr};
    return voxels;
}

void vxlSetVoxels(VxlVoxels *v) { voxels = v; }

VxlVoxelType vxlGetVoxel(uint32_t x, uint32_t y, uint32_t z) {
    if (x >= voxels->xSize || y >= voxels->ySize || z >= voxels->zSize)
        return VXL_VOXEL_AIR;
    return voxels->voxels[VXL_VOXELS_IDX(x, y, z)];
}

bool vxlIsColliding(float x, float y, float z) {
    if (x < 0 || y < 0 || z < 0)
        return false;
    uint32_t ix = (uint32_t)x;
    uint32_t iy = (uint32_t)y;
    uint32_t iz = (uint32_t)z;
    if (ix >= voxels->xSize || iy >= voxels->ySize || iz >= voxels->zSize)
        return false;
    return voxels->voxels[VXL_VOXELS_IDX(ix, iy, iz)] != VXL_VOXEL_AIR;
}

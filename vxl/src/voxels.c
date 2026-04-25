#include "vxl.h"
#include <stdlib.h>

#define VXL_VOXELS_IDX(x, y, z) ((y) * (s_voxels->zSize * s_voxels->xSize) + (z) * s_voxels->xSize + (x))

VxlVoxels vxlInitVoxels(uint32_t xSize, uint32_t ySize, uint32_t zSize) {
    size_t size = xSize * ySize * zSize;
    VxlVoxelType *voxel_arr = malloc(size * sizeof(VxlVoxelType));
    for (int i = 0; i < size; i++) {
        voxel_arr[i] = VXL_VOXEL_GRASS;
    }
    VxlVoxels voxels = { .xSize = xSize, .ySize = ySize, .zSize = zSize, voxel_arr};
    return voxels;
}

void vxlSetVoxels(VxlVoxels *voxels) {
    s_voxels = voxels;
}

VxlVoxelType vxlGetVoxel(uint32_t x, uint32_t y, uint32_t z) {
    return s_voxels->voxels[VXL_VOXELS_IDX(x, y, z)];
}

bool vxlIsColliding(float x, float y, float z) {
    return vxlGetVoxel((uint32_t)x, (uint32_t)y, (uint32_t)z) != VXL_VOXEL_AIR;
}

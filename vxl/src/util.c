#include "vxl.h"
#include <stdbool.h>
bool vxlIsConfigured() {
    return s_frame && s_camera && s_voxels;
}

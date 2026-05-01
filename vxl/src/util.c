#include "globals.h"
#include "vxl.h"
#include <stdbool.h>
#include <stdlib.h>
bool vxlIsConfigured() {
    return frame != NULL && camera != NULL && voxels != NULL;
}

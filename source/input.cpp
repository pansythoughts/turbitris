
#include "input.h"

void scanInput()
{
    scanKeys();
    keys_down = keysDown();
    keys_up = keysUp();
    keys_held = keysHeld();
}

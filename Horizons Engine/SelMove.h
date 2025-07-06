#pragma once

namespace hz {

    // Call once per frame to move the currently-selected shape
    // with Arrow keys (X/Y) and + / – (Z).  Uses GetDeltaTime()
    // from Frame.h for smooth movement.
    void UpdateSelectedMovement();

} // namespace hz
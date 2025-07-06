#pragma once

namespace hz {

    // Call once per frame to rotate the currently-selected shape.
    // 1 = rotate about X, 2 = rotate about Y, 3 = rotate about Z.
    void UpdateSelectedRotation();

} // namespace hz

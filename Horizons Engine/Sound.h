// Sound.h
#pragma once

namespace hz {
    bool InitSound();                         // Call once on startup
    void PlaySoundFile(const char* path);    // Play a .wav, .mp3, .ogg file
    void ShutdownSound();                     // Call once on shutdown
}
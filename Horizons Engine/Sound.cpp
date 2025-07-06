#define MINIAUDIO_IMPLEMENTATION
#include <windows.h>
#include "miniaudio.h"

ma_engine engine;

namespace hz {

    bool InitSound() {
        ma_result result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            char msg[128];
            sprintf_s(msg, "Init failed: %d", result);
            MessageBoxA(NULL, msg, "Sound Init Error", MB_OK);
            return false;
        }

        ma_engine_set_volume(&engine, 1.0f); // Ensure full volume
        return true;
    }

    void PlaySoundFile(const char* path) {
        ma_result result = ma_engine_play_sound(&engine, path, NULL);
        if (result != MA_SUCCESS) {
            char msg[256];
            sprintf_s(msg, "Failed to play sound: %d\n%s", result, path);
            MessageBoxA(NULL, msg, "PlaySoundFile Error", MB_OK);
        }
    }

    void ShutdownSound() {
        ma_engine_uninit(&engine);
    }

}
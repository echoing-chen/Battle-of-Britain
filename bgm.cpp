#include <SDL.h>

SDL_AudioSpec bgm_audio_spec[5];
Uint8* bgm_audio_buf[5];
Uint32 bgm_audio_length[5];
Uint32 bgm_audio_pos = 0;
SDL_AudioDeviceID bgm_device_id;
const char* music_path[5] = { "music/1.WAV","music/2.WAV","music/3.WAV","music/4.WAV","music/5.WAV" };
int bgm_index = 0;

void bgm_callback(void* userdata, Uint8* stream, int len);

void start_bgm() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_Log("SDL init failed! %s", SDL_GetError());
        return;
    }
    for (int i = 0; i < 5; i++) {
        if (SDL_LoadWAV(music_path[i], &bgm_audio_spec[i], &bgm_audio_buf[i], &bgm_audio_length[i]) == NULL) {
            SDL_Log("SDL_LoadWAV failed! %s", SDL_GetError());
            return;
        }
    }
    bgm_audio_spec[0].callback = bgm_callback;
    bgm_device_id = SDL_OpenAudioDevice(NULL, 0, &bgm_audio_spec[0], NULL, 0);
    SDL_PauseAudioDevice(bgm_device_id, 0);
}

void end_bgm() {
     for (int i = 0; i < 5; i++) {
         SDL_FreeWAV(bgm_audio_buf[i]);
     }
     SDL_CloseAudioDevice(bgm_device_id);
     SDL_Quit();
}



void bgm_callback(void* userdata, Uint8* stream, int len) {
    SDL_memset(stream, 0, len);
    int remain = bgm_audio_length[bgm_index] - bgm_audio_pos;
    if (remain > len) {
        SDL_memcpy(stream, bgm_audio_buf[bgm_index] + bgm_audio_pos, len);
        bgm_audio_pos += len;
    }
    else {
        SDL_memcpy(stream, bgm_audio_buf[bgm_index] + bgm_audio_pos, remain);
        bgm_index = (bgm_index + 1) % 5;
        bgm_audio_pos = 0;//Ñ­»·£¡£¡£¡
    }
}
#include "audio.h"

#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_state.h"

GameAudio* audio_initialization() {
    GameAudio* audio = NULL;

    // Инициализация аудиосистемы
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == 0) {
        audio = load_audio();
        if (audio) {
            audio->audio_initialized = 1;
        }
    }

    if (!audio->audio_initialized) {
        printf("Audio system unavailable - continuing without sound\n");
    }
    return audio;
}

GameAudio* load_audio() {
    GameAudio* audio = malloc(sizeof(GameAudio));
    if (!audio) return NULL;

    audio->cannon_shot = Mix_LoadWAV("../sounds/cannon_shot.mp3");
    if (!audio->cannon_shot) {
        printf("Failed to load cannon_shot.mp3: %s\n", Mix_GetError());
    }
    audio->victory = Mix_LoadWAV("../sounds/victory.mp3");
    if (!audio->victory) {
        printf("Failed to load victory.mp3: %s\n", Mix_GetError());
    }
    audio->water_splash = Mix_LoadWAV("../sounds/water_splash.mp3");
    if (!audio->water_splash) {
        printf("Failed to load water_splash.mp3: %s\n", Mix_GetError());
    }
    audio->ship_hit = Mix_LoadWAV("../sounds/ship_hit.mp3");
    if (!audio->ship_hit) {
        printf("Failed to load ship_hit.mp3: %s\n", Mix_GetError());
    }
    audio->background = Mix_LoadMUS("../sounds/background.mp3");
    if (!audio->background) {
        printf("Failed to load background.mp3: %s\n", Mix_GetError());
    }

    return audio;
}

void play_cannon_shot(GameState* game) {
    if (game->audio && game->audio->cannon_shot && game->all_sound_on) {
        Mix_PlayChannel(-1, game->audio->cannon_shot, 0);  // автовыбор потока, ,количество повторений
    }
}

void play_water_splash(GameState* game) {
    if (game->audio && game->audio->water_splash && game->all_sound_on) {
        Mix_PlayChannel(-1, game->audio->water_splash, 0);
    }
}

void play_ship_hit(GameState* game) {
    if (game->audio && game->audio->ship_hit && game->all_sound_on) {
        Mix_PlayChannel(-1, game->audio->ship_hit, 0);
    }
}

void play_background(GameState* game) {
    if (game->audio && game->audio->background && game->background_music_on) {
        Mix_PlayMusic(game->audio->background, -1);
    }
}

void fade_out_background_music(int fade_time_ms) {
    Mix_FadeOutMusic(fade_time_ms);  // 🎚️ Плавное затухание (например 500ms)
}

/*void play_victory(GameAudio* audio) {// !!!!!!!!!!!!!!!!!!!!!!!!!  пока никуда не вставляли
    if (audio && audio->victory) {
        Mix_PlayChannel(-1, audio->victory, 0);
    }
}*/

void cleanup_audio(GameAudio* audio) {
    if (audio->audio_initialized) {
        Mix_CloseAudio();  // 🆕 закрываем аудиосистему
    }
    Mix_FreeChunk(audio->cannon_shot);
    free(audio);
}
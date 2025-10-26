#ifndef AUDIO_H
#define AUDIO_H

typedef struct Mix_Chunk Mix_Chunk;
typedef struct Mix_Music Mix_Music;

typedef struct GameAudio{
    Mix_Chunk* cannon_shot;    // выстрел пушки
    Mix_Chunk* water_splash;   // промах 
    Mix_Chunk* ship_hit;       // попадание в корабль*/
    Mix_Chunk* victory;        // победа
    Mix_Music* background;     // фоновая музыка
    char audio_initialized;
} GameAudio;

GameAudio* audio_initialization();
GameAudio* load_audio();
void play_cannon_shot(GameAudio* audio);
void play_water_splash(GameAudio* audio);
void play_ship_hit(GameAudio* audio);
void play_background(GameAudio* audio);
//void play_victory(GameAudio* audio);
void cleanup_audio(GameAudio* audio);
#endif
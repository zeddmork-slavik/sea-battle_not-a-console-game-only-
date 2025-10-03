/*#ifndef GAME_H
#define GAME_H

//#include standart libraries and other headers
#include "graphics.h"

typedef struct { // схема к основному окну 
    WindowConfig config;       
    int grid_size;         // Размер сетки (10 для морского боя)
    int fps;               // Кадры в секунду
} GameConfig;  

typedef struct { // центральный "мозг"
    GameConfig config;           
    GraphicsContext graphics;    
    int game_is_running;         // Флаг: 1 - игра работает, 0 - пора выходить
    // TODO: Здесь позже добавим игровые поля, меню, статус и т.д.
} Game;


// ни одна из функций ещё не реализована
Game init_game(void);            // Создает и настраивает структуру Game "с нуля"
void handle_events(Game* game);  // Обрабатывает все события (ввод пользователя)
void update_game(Game* game);    // Обновляет логику игры
void render_game(Game* game);    // Отрисовывает кадр (вызывает функции из graphics.c)
void cleanup_game(Game* game);   // Корректно освобождает ресурсы (память, SDL)


#endif
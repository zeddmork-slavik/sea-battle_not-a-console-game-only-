#ifndef GAME_H
#define GAME_H

//#include standart libraries and other headers

typedef struct { //используем typedef чтобы при упоминании не писать каждый раз ключевое слово struct
    int window_width;      // ??? какого окна? Ширина окна
    int window_height;     // Высота окна  
    const char* title;     // Заголовок окна
    int cell_size;         // Размер клетки поля
    int grid_size;         // Размер сетки (10 для морского боя)
    int fps;               // Кадры в секунду
} GameConfig;  // ????????????? точнее про что !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



Game init_game(void);   // ни одна из них ещё не писалась
void update_game(Game* game);


#endif
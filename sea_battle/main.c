#define SDL_MAIN_HANDLED // иначе при компиляции ошибка "undefined reference to `WinMain'"
#include <SDL2/SDL.h>
#include <stdio.h>
#include "board.h"
#include "game.h"
#include "graphics.h"

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) { // подключение модулей SDL можно будет подключить аудио, Video (даёт доступ к SDL_CreateWindow и SDL_Renderer)
        printf("SDL Error: %s\n", SDL_GetError()); // при ошибке SDL_Init возвращает отрицательное число
        return 1; //SDL_GetError - функция для получения текстового человекочитаемого описания ошибки
    }
    
    // 2. Создание окна
    SDL_Window* window = SDL_CreateWindow( // SDL_Window - непрозрачная структура из SDL, 
        //SDL_CreateWindow создаёт HWND (идентификатор окна для ОС)
    "SEA BATTLE",              // Заголовок окна
    SDL_WINDOWPOS_CENTERED,  // Позиция нашего окна в рамках экрана от левого края X (CENTERED - центр)
    SDL_WINDOWPOS_CENTERED,  // Позиция Y от верха 
    1024, 768,                // Ширина и высота главного окна
    SDL_WINDOW_SHOWN         // здесь флаги отображения, конкретно в этом случае - показать окно сразу
    );
    
    if (!window) {
        printf("Window Error: %s\n", SDL_GetError());// отсутсвует драйвер видеокарты, нехватает памяти видюхи, неадекватные размеры 
        SDL_Quit(); 
        return 1;
    }
    
    // 3. Создание рендерера - т.е. рисовальщика (перевод - средство визуализации)
    SDL_Renderer* renderer = SDL_CreateRenderer(  
    window,              // сцена (где будет рисовать), создали в 2.
    -1,                  // Индекс драйвера (-1 = авто) OpenGL - старенький работяга для всех, Direct3D - специально для винды, Vulkan - крутой но не робит на маках
    SDL_RENDERER_ACCELERATED  // определяется аппаратное ускорение - здесь используй видюху(1000 FPS) а не проц(60FPS) или есть VSync - он заставляет видюху ждать обновления кадра на мониторе, но говорят для обучения это too mach но он нужен будет на анимацию
    )
    
    // 4. игровой код здесь
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //выбор краски: рисовальщик(всё равно всегда он один, просто чтоб связаться с ним),   R,   G,  B,   A - прозрачность
    SDL_RenderClear(renderer); // заливаем весь холст - одна из функций, есть так же прямоугольники, линии, точки (но данные остаются в видюхе)
    SDL_RenderPresent(renderer); // показываем - забираем у видюхи и выводит на экран
    SDL_Delay(2000);  // задержка между обновлениями кадра 2 сек.
    
    // 5. Очистка
    SDL_DestroyRenderer(renderer); // если напишем после SDL_DestroyWindow - то ничего не будет удалено - не найдут его при отсутсвии окна
    SDL_DestroyWindow(window); // закрывает окно в ОС, освобождает всю связанную память, аннулирует указатель
    SDL_Quit(); // освобождает только СИСТЕМНЫЕ ресурсы: инициализированные модули но не наши созданные объекты
    
    return 0;
}
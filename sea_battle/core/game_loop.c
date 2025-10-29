#include "game_loop.h"

#include <SDL2/SDL.h>

#include "audio.h"
#include "combat/cannons_logic.h"
#include "game_state.h"
#include "graphics/graphics_state.h"
#include "graphics/rendering.h"
#include "menu.h"

void run_game(const GraphicsContext* ctx,
              const GameLandmarks* landmarks) {  // зарефакторить её нужно будет скоро
    GameState game = {0};  // не трогать: убираю в инициализации поля в которых нужны нули
    game.running = GAME_RUNNING;
    game.current_turn = IS_PLAYER;  // поменяю как у компа будет ИИ
    GameBoard player_board_obj = {0};
    GameBoard computer_board_obj = {0};
    game.player_board = &player_board_obj;  // указатель на статический объект
    game.computer_board = &computer_board_obj;
    init_board(game.player_board);  // заполняем их
    init_board(game.computer_board);

    init_cannon(&game.player_cannon, IS_PLAYER, landmarks->player_x + ctx->field_size + OFFSET_X_FROM_BOARD,
                landmarks->offset_y + OFFSET_Y_FROM_BOARD, ctx->renderer);
    init_cannon(&game.computer_cannon, IS_COMPUTER,
                landmarks->player_x + ctx->field_size + OFFSET_X_FROM_BOARD,
                landmarks->offset_y + OFFSET_Y_FROM_COMPUTER_CANON, ctx->renderer);

    game.cannonball.texture = load_texture_from_file(ctx->renderer, "../images/cannonball.png");
    game.cannonball.rotation_speed = 720.0;  // 720°/секунду = 2 оборота в секунду

    game.audio = audio_initialization();

    play_background(game.audio);

    event_processing(&game, ctx, landmarks, game.audio);

    cleanup_game(&game);
}

void event_processing(GameState* game, const GraphicsContext* ctx, const GameLandmarks* landmarks,
                      GameAudio* audio) {
    double delta_time = BARREL_OR_CORE_ROTATION_SPEED_SECOND_PER_FRAME;
    SDL_Event event;
    while (game->running) {
        Uint32 current_time = SDL_GetTicks();  // текущее время каждого кадра
        // Обработка событий
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game->running = DONT_RUNNING;
            }
            if (game->game_state == STATE_MENU) {
                handle_menu_input(game, &event, ctx);
            } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                       event.button.button ==
                           SDL_BUTTON_LEFT) {  // так то здесь дублирование с предыдущей функцией - надо будет
                                               // когда-нибудь подумать
                int mouse_x = event.button.x;
                int mouse_y = event.button.y;

                // Проверяем клик по полю компьютера
                if (mouse_x >= landmarks->computer_x && mouse_x <= landmarks->computer_x + ctx->field_size &&
                    mouse_y >= landmarks->offset_y && mouse_y <= landmarks->offset_y + ctx->field_size) {
                    aim_cannon_at(&game->player_cannon, mouse_x, mouse_y, ctx, landmarks, &game->cannonball);
                }
            }
        }
        if (game->game_state == STATE_MENU) {
            render_main_menu(ctx, game);
        } else {
            update_fire_animations(game, current_time);
            compose_frame(game, delta_time, current_time, ctx, landmarks, audio);
            if (game->game_state == STATE_PLAYING) {
                const Uint8* keys = SDL_GetKeyboardState(NULL);
                if (keys[SDL_SCANCODE_ESCAPE]) {
                    game->game_state = STATE_MENU;
                    reset_game_for_new_match(game, ctx, landmarks);
                }
            }
        }
        SDL_Delay(33);  // ~30 FPS для пошаговой игры
    }
}

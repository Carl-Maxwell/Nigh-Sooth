#pragma once

#include <lapse_scalar.h>

#include <platform_olc_pixel.h>

#include "ephrais_next_map_settings.h"

namespace ephrais{

enum class GameState_Enum{
  lost,
  in_progress,
  won
};

struct Run{
  lapse::f32     game_zoom = 1.0f;
  lapse::vec2<>  panning_offset = {10, 10};
  GameState_Enum game_state{GameState_Enum::in_progress};
  lapse::vec2<>  player_position = {0, 0};

  ~Run() {
    std::cout << "~ephrais Run()\n";
    platform::finalize_shutdown();
  }

  bool start_main_loop();
  static void main_loop(lapse::f32 delta);
  void initialize_run(NextMapSettings next_map = {});
  bool is_game_won();
  void win_game();
  void lose_game();

  lapse::vec2<lapse::i32> next_window_size = {1920/10*9, 1080/10*9};
};

} // end namespace

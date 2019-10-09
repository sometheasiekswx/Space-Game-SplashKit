#include "splashkit.h"
#include "game_data.h"

/**
 * The main sequence for the program. This will:
 *
 *  - Load in the resources required
 *  - Plays background music
 *  - Declare planets, enemies, and the player
 *  - Draw planets in random locations
 *  - Draw and move enemies move around the screen in a pattern
 *  - When the enemy hit the player, the player takes damage
 *  - Draw the player
 *  - Draw text displaying what controls are avialable
 *  - Handles input from the user that controls the player to move or attack
 *  - Loops until the user chooses to exit or dies
 *  - After death or exit, the screen shows the final score with options to replay or quit
 *  - If the user chooses to replay, loop through the game again.
 *  - If the user chooses to quit, end loop, turn off music, and end the program.
 *
 * @returns 0   to indicate success to the operating system
 */
int main()
{
    open_window("Space Game", 1300, 700);
    load_resources();

    open_audio();
    play_music("tomorrow");

    bool replay_game = true;

    while (replay_game == true)
    {
        play_game(replay_game);
    }

    stop_music();
    close_audio();

    return 0;
}

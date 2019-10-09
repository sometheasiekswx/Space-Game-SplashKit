#ifndef GAME_DATA_H
#define GAME_DATA_H

#define PLAYER_SPEED 15

enum ship_type
{
    AQUARII,
    GLIESE,
    PEGASI
};

enum planet_type
{
    EARTH,
    JUPITER,
    MARS,
    MERCURY,
    NEPTUNE,
    PLUTO,
    SATURN,
    URANUS,
    VENUS
};

struct location
{
    double x;
    double y;
};

struct player_data
{
    int level;
    int health;
    ship_type type;
    location location;
    int score;
};

struct planet_data
{
    planet_type type;
    location location;
};

struct enemy_data
{
    int health;
    int max_health;
    ship_type type;
    location location;
    double speed;
};

/**
 * Load in the resources requiured using a resource bundle.
 */
void load_resources();

/**
 * Determines the bitmap to return based on the type of planet the object is.
 *
 * @param       type    The type of planet the object is.
 * @returns             The bitmap of that planet type.
 */
bitmap planet_bitmap(planet_type type);

/**
 * Determines the bitmap to return based on the type of ship the object is.
 *
 * @param       type    The type of ship the object is.
 * @returns             The bitmap of that ship type.
 */
bitmap ship_bitmap(ship_type type);

/**
 * Returns a random integer from given minimum and maximum number.
 *
 * @param   min     Minimum integer wanted.
 * @param   max     Maximum integer wanted.
 */
int rand_num(const int &min, const int &max);

/**
 * Returns a random value from given minimum and maximum number.
 *
 * @param   min     Minimum value wanted.
 * @param   max     Maximum value wanted.
 */
float rand_num(const float &min, const float &max);

/**
 * Randomized a number to dertermine the planet type to return.
 *
 * @returns     A random planet type.
 */
planet_type random_planet_type();

/**
 * Sets the stats for a new planet and return those stats.
 *
 * @returns     The stats for a new planet.
 */
planet_data new_planet();

/**
 * Randomized a number to dertermine the ship type to return.
 *
 * @returns     A random ship type.
 */
ship_type random_ship_type();

/**
 * Sets the stats for a new planet and return those stats.
 *
 * @returns     The stats for a new planet.
 */
enemy_data new_enemy();

/**
 * Sets the stats for a new player and return those stats.
 *
 * @returns     The stats for a new player.
 */
player_data new_player();

/**
 * Displays the planet on the screen.
 *
 * @param   planet_to_draw  Data of the planet used it draw it.
 */
void draw_planet(const planet_data &planet_to_draw);

/**
 * Displays the enemy on the screen with its health on top of it.
 *
 * @param   enemy_to_draw   Data of the enemy used it draw it.
 */
void draw_enemy(const enemy_data &enemy_to_draw);

/**
 * Displays the player on the screen with its level on top of it.
 *
 * @param   player_to_draw  Data of the enemy used it draw it.
 */
void draw_player(const player_data &player_to_draw);

/**
 * Displays a black screen with text congratulating the player for leveling up and showing the new level. Pauses the game abit. Also, this plays a victory and explosion sound effect.
 *
 * @param   player_level    Level of the player to display.
 */
void draw_level_up_screen(const int &player_level);

/**
 * Levels up the player and gives new stats to the enemy destroyed. The enemy is given more health and speed. increases the player score.
 *
 * @param   player_level    Level of the player to update.
 * @param   player_score    Score of the player to update.
 * @param   enemy           Data of the enemy used to create a new enemy.
 */
void enemy_destroyed(int &player_level, int &player_score, enemy_data &enemy);

/**
 * Lets the player shoot a laser that knocks back the user abit. If any of the two enemies are hit, their health is resduced until its 0 or lower. Afterwhich, the enemy resets with more health and speed. Also,  this plays a laser sound effect. increases the player score.
 *
 * @param   player_location     Location of the player used as origin for projectile.
 * @param   player_level        Level of the player that is used as damage.
 * @param   player_score        Score of the player to update.
 * @param   enemy_1             Data of the enemy to update when hit.
 * @param   enemy_2             Data of the enemy to update when hit.
 */
void shoot_laser(location &player_location, int &player_level, int &player_score, enemy_data &enemy_1, enemy_data &enemy_2);

/**
 * Pushes the player back when going over the edge of the screen to prevent the player from moving through walls.
 * @param   location    Location of the player to update.
 */
void player_cannot_move_through_walls(location &location);

/**
 * Accepts inputs from the user like changing ship type, moving the player around the screen, and firing the laser.
 *
 * @param   player_data     Data of the player used for performing actions.
 * @param   enemy_1         Data of the enemy to update.
 * @param   enemy_2         Data of the enemy to update.
 */
void handle_user_input(player_data &player, enemy_data &enemy_1, enemy_data &enemy_2);

/**
 * Makes the enemy move in a specific pattern. The enemy can move through walls. When the enemy's health is too low, it moves twice as fast. When the enemy hits the player, the player and the enemy loses health. An explosion sound is made to show the two ships crushed.
 *
 * @param   enemy_data          Data of enemy used to perform actions.
 * @param   player_location     Location of the player used as target.
 * @param   player_health       Health of the player to update.
 * @param   player_score        Score of the player to update.
 */
void enemy_movement(enemy_data &enemy, location &player_location, int &player_health, int &player_score);

/**
 * Displays the actions and what the user needs to input to perform that action.
 */
void draw_controls();

/**
 * Checks if the exit button has been clicked on or not. If not, this funtion returns false. If the button is clicked, this function returns true.
 *
 * @returns             A boolean value of whether or not the exit button is clicked.
 */
bool exit_clicked();

/**
 * Checks if the player's health is too low. If so, return true. If not, return false.
 *
 * @param       player_health   The amount of health the player has.
 * @returns                     True if health is below or is zero and false if otherwise.
 */
bool lose_game(const int &player_health);

/**
 * After exit, the screen shows the final score and allows the user to choose whether to quit or replay.
 *
 * @param   player_score    Final score of the player before gameover.
 * @param   replay_game     Choice to replay the game.
 */
void game_over_screen(const int &player_score, bool &replay_game);

/**
 * Starts a new game by declaring and drawing random planets, 2 enemies of random health and location, and the player. The user can move and shoot while the enemy can only move. Both enemies have a unique movement pattern and different speeds depending on thier health levels. The player loses health when hit and gains points when shooting the enemy or killing the enemy. Also, shows the controls available on the screen. At the end of the game when the player either quits or dies, the player can decide whether to play the game again or not.
 * 
 * @param   replay_game     The status of whether to replay the game or not. 
 */
void play_game(bool &replay_game);

#endif

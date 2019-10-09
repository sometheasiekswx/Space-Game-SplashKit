#include "splashkit.h"
#include "game_data.h"

void load_resources()
{
    load_resource_bundle("game_bundle", "planet_play.txt");
}

bitmap planet_bitmap(planet_type type)
{
    switch (type)
    {
    case EARTH:
        return bitmap_named("earth");

    case JUPITER:
        return bitmap_named("jupiter");

    case MARS:
        return bitmap_named("mars");

    case MERCURY:
        return bitmap_named("mercury");

    case NEPTUNE:
        return bitmap_named("neptune");

    case PLUTO:
        return bitmap_named("pluto");

    case SATURN:
        return bitmap_named("saturn");

    case URANUS:
        return bitmap_named("uranus");

    default:
        return bitmap_named("venus");
    }
}

bitmap ship_bitmap(ship_type type)
{
    switch (type)
    {
    case AQUARII:
        return bitmap_named("aquarii");

    case GLIESE:
        return bitmap_named("gliese");

    default:
        return bitmap_named("pegasi");
    }
}

int rand_num(const int &min, const int &max)
{
    int range = max - min + 1;
    int result = rand() % range + min;
    return result;
}

float rand_num(const float &min, const float &max)
{
    float range = (float)rand() / (float)RAND_MAX;
    float result = min + range * (max - min);
    return result;
}

planet_type random_planet_type()
{
    return static_cast<planet_type>(rand_num(0, 9));
}

planet_data new_planet()
{
    planet_data result;

    result.type = random_planet_type();
    result.location.x = rand_num(0, screen_width()) - 50;
    result.location.y = rand_num(0, screen_height()) - 50;

    return result;
}

ship_type random_ship_type()
{
    return static_cast<ship_type>(rand_num(0, 3));
}

enemy_data new_enemy()
{
    enemy_data result;

    result.health = rand_num(1000, 5000);
    result.max_health = result.health;
    result.type = random_ship_type();
    result.location.x = rand_num(100, screen_width() - 100);
    result.location.y = rand_num(100, screen_height() - 100);
    result.speed = 2;

    return result;
}

player_data new_player()
{
    player_data result;

    result.level = 1;
    result.health = 5000;
    result.type = AQUARII;
    result.location.x = 25;
    result.location.y = 250;
    result.score = 0;

    return result;
}

void draw_planet(const planet_data &planet_to_draw)
{
    bitmap to_draw = planet_bitmap(planet_to_draw.type);
    draw_bitmap(to_draw, planet_to_draw.location.x, planet_to_draw.location.y);
}

void draw_enemy(const enemy_data &enemy_to_draw)
{
    bitmap to_draw = ship_bitmap(enemy_to_draw.type);
    draw_bitmap(to_draw, enemy_to_draw.location.x, enemy_to_draw.location.y);
    draw_text("Hp: " + std::to_string(enemy_to_draw.health), COLOR_RED, "cnr", 30, enemy_to_draw.location.x, enemy_to_draw.location.y - 50);
}

void draw_player(const player_data &player_to_draw)
{
    bitmap to_draw = ship_bitmap(player_to_draw.type);
    draw_bitmap(to_draw, player_to_draw.location.x, player_to_draw.location.y);
    draw_text("Level: " + std::to_string(player_to_draw.level), COLOR_GREEN, "cnr", 35, player_to_draw.location.x, player_to_draw.location.y - 50);
    draw_text("Hp: " + std::to_string(player_to_draw.health), COLOR_GREEN, "cnr", 35, player_to_draw.location.x, player_to_draw.location.y - 25);
    draw_text("Score: " + std::to_string(player_to_draw.score), COLOR_GREEN, "cnr", 40, 500, 25);
}

void draw_level_up_screen(const int &player_level)
{
    play_sound_effect("short_triumphal_fanfare", 1, 0.30);
    play_sound_effect("explosion_4");

    clear_screen(COLOR_BLACK);

    int x = 50;
    int y = 50;

    draw_text("Congratulations!", COLOR_GREEN, "cnr", 100, x, y);
    y += 150;
    draw_text("You Leveled Up!", COLOR_CRIMSON, "cnr", 100, x, y);
    y += 150;
    draw_text("You are now Level " + std::to_string(player_level) + "!", COLOR_RED, "cnr", 100, x, y);
    y += 150;
    draw_text("+ " + std::to_string(player_level * 10000) + " Bonus Score!", COLOR_TEAL, "cnr", 100, x, y);

    refresh_screen(60);
    delay(2500);

    fade_all_sound_effects_out(500);
}

void enemy_destroyed(int &player_level, int &player_score, enemy_data &enemy)
{
    player_level++;
    player_score += player_level * 10000;

    enemy.health = rand_num(player_level * 10000, player_level * 50000);
    enemy.max_health = enemy.health;
    enemy.type = random_ship_type();
    enemy.location.x = rand_num(100, screen_width() - 100);
    enemy.location.y = rand_num(100, screen_height() - 100);
    enemy.speed += 0.5;
}

void shoot_laser(location &player_location, int &player_level, int &player_score, enemy_data &enemy_1, enemy_data &enemy_2)
{
    play_sound_effect("alien_machine_gun", 1, 0.15);

    player_cannot_move_through_walls(player_location);

    location projectile;
    projectile.x = player_location.x + 100;
    projectile.y = player_location.y + 10;

    player_location.x -= 4;
    player_location.y -= 1;

    while (projectile.x < screen_width() + 125)
    {
        draw_bitmap("laser", projectile.x, projectile.y);

        projectile.x += 1;

        int enemy_size_x = 80;
        int enemy_size_y = 25;

        // Enemy_1 Hit
        if (
            projectile.x > enemy_1.location.x - enemy_size_x &&
            projectile.x < enemy_1.location.x + enemy_size_x &&
            projectile.y > enemy_1.location.y - enemy_size_y &&
            projectile.y < enemy_1.location.y + enemy_size_y + 10)
        {
            enemy_1.health -= player_level;

            player_score += player_level;

            if (enemy_1.health <= 0)
            {
                enemy_destroyed(player_level, player_score, enemy_1);
                draw_level_up_screen(player_level);
            }
        }

        // Enemy_2 Hit
        if (
            projectile.x > enemy_2.location.x - enemy_size_x &&
            projectile.x < enemy_2.location.x + enemy_size_x &&
            projectile.y > enemy_2.location.y - enemy_size_y &&
            projectile.y < enemy_2.location.y + enemy_size_y + 10)
        {
            enemy_2.health -= player_level;

            player_score += player_level;

            if (enemy_2.health <= 0)
            {
                enemy_destroyed(player_level, player_score, enemy_2);
                draw_level_up_screen(player_level);
            }
        }
    }

    fade_all_sound_effects_out(35);
}

void player_cannot_move_through_walls(location &location)
{
    int player_size_x = 50;
    int player_size_y = 20;

    if (location.y < 0 - player_size_y)
    {
        location.y = 0 + player_size_y;
    }
    if (location.y > 0 + screen_height() + player_size_y)
    {
        location.y = 0 + screen_height() - player_size_y;
    }
    if (location.x > 0 + screen_width() + player_size_x)
    {
        location.x = 0 + screen_width() - player_size_x;
    }
    if (location.x < 0 - player_size_x)
    {
        location.x = 0 + player_size_x;
    }
}

void handle_user_input(player_data &player, enemy_data &enemy_1, enemy_data &enemy_2)
{
    player_cannot_move_through_walls(player.location);

    if (key_typed(NUM_1_KEY))
    {
        player.type = AQUARII;
    }
    if (key_typed(NUM_2_KEY))
    {
        player.type = GLIESE;
    }
    if (key_typed(NUM_3_KEY))
    {
        player.type = PEGASI;
    }

    if (key_down(W_KEY))
    {
        player.location.y -= PLAYER_SPEED;
    }
    if (key_down(S_KEY))
    {
        player.location.y += PLAYER_SPEED;
    }
    if (key_down(D_KEY))
    {
        player.location.x += PLAYER_SPEED;
    }
    if (key_down(A_KEY))
    {
        player.location.x -= PLAYER_SPEED;
    }

    if (key_down(SPACE_KEY) || mouse_down(LEFT_BUTTON))
    {
        shoot_laser(player.location, player.level, player.score, enemy_1, enemy_2);
    }
}

void enemy_can_move_through_walls(location &location)
{
    if (location.x < 0)
    {
        location.x = screen_width();
    }
    if (location.x > screen_width())
    {
        location.x = 0;
    }
    if (location.y < 0)
    {
        location.y = screen_height();
    }
    if (location.y > screen_height())
    {
        location.y = 0;
    }
}

void enemy_movement(enemy_data &enemy, location &player_location, int &player_health, int &player_score)
{
    enemy_can_move_through_walls(enemy.location);

    switch (enemy.type)
    {
    case AQUARII:

        if (enemy.health > enemy.max_health / 2)
        {
            enemy.location.x += enemy.speed;
            enemy.location.y += enemy.speed;
        }
        else
        {
            enemy.location.x -= (enemy.speed * 2);
            enemy.location.y -= (enemy.speed * 2);
        }
        break;

    case GLIESE:

        if (enemy.health > enemy.max_health / 2)
        {
            enemy.location.x += enemy.speed;
            enemy.location.y -= enemy.speed;
        }
        else
        {
            enemy.location.x -= (enemy.speed * 2);
            enemy.location.y += (enemy.speed * 2);
        }

        break;

    default:

        if (enemy.health > enemy.max_health / 2)
        {
            enemy.location.x += enemy.speed;
        }
        else
        {
            enemy.location.y += (enemy.speed * 2);
        }
        break;
    }

    int player_size_x = 50;
    int player_size_y = 20;

    if (
        enemy.location.x > player_location.x - player_size_x &&
        enemy.location.x < player_location.x + player_size_x &&
        enemy.location.y > player_location.y - player_size_y &&
        enemy.location.y < player_location.y + player_size_y + 10)
    {
        play_sound_effect("explosion_4");
        player_health -= 500;
        enemy.health -= 5000;
        player_score -= 5000;
    }

    fade_all_sound_effects_out(800);
}

void draw_controls()
{
    int x = 1300 - bitmap_width("exit_button");
    int y = 0;
    draw_bitmap("exit_button", x, y);

    x = 25;
    y = 25;
    draw_text("{Player 1 Controls}", COLOR_WHITE_SMOKE, "cnr", 25, x, y);

    y += 100;
    draw_text("    Movement:", COLOR_TEAL, "cnr", 25, x, y);

    y += 50;
    draw_text("       [W]", COLOR_TEAL, "cnr", 25, x, y);

    y += 25;
    draw_text("     [A S D]", COLOR_TEAL, "cnr", 25, x, y);

    y += 100;
    draw_text("      Fire:", COLOR_RED, "cnr", 25, x, y);

    y += 50;
    draw_text("    [SPACEBAR]", COLOR_RED, "cnr", 25, x, y);
}

bool exit_clicked()
{
    int x1 = 1300 - bitmap_width("exit_button");
    int y1 = 0;
    int x2 = x1 + bitmap_width("exit_button");
    int y2 = y1 + bitmap_height("exit_button");

    if (mouse_clicked(LEFT_BUTTON) &&
        mouse_x() > x1 &&
        mouse_x() < x2 &&
        mouse_y() > y1 &&
        mouse_y() < y2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool lose_game(const int &player_health)
{
    if (player_health <= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void game_over_screen(const int &player_score, bool &replay_game)
{
    while (!key_down(ESCAPE_KEY) && !quit_requested() && replay_game == false)
    {
        process_events();

        clear_screen(COLOR_BLACK);

        int x = 50;
        int y = 50;
        draw_text("GAME OVER!", COLOR_RED, "cnr", 125, x, y);

        y += 250;
        draw_text("Final Score: " + std::to_string(player_score), COLOR_TEAL, "cnr", 75, x, y);

        if (key_typed(R_KEY))
        {
            replay_game = true;
        }

        y += 225;
        draw_text("Press R to Replay", COLOR_GREEN, "cnr", 25, x, y);

        y += 75;
        draw_text("Press ESC to QUIT", COLOR_GREEN, "cnr", 25, x, y);

        refresh_screen(60);
    }
}

void play_game(bool &replay_game)
{
    replay_game = false;

    int planet_amount = rand_num(1, 5);
    planet_data planet[planet_amount];
    for (int i = 0; i < planet_amount; i++)
    {
        planet[i] = new_planet();
    }

    enemy_data enemy_1 = new_enemy();
    enemy_data enemy_2 = new_enemy();

    player_data player = new_player();

    while (
        !quit_requested() &&
        !key_down(ESCAPE_KEY) &&
        !exit_clicked() &&
        !lose_game(player.health))
    {
        process_events();

        clear_screen(COLOR_PURPLE);

        for (int i = 0; i < planet_amount; i++)
        {
            draw_planet(planet[i]);
        }

        draw_enemy(enemy_1);
        draw_enemy(enemy_2);

        enemy_movement(enemy_1, player.location, player.health, player.score);
        enemy_movement(enemy_2, player.location, player.health, player.score);

        draw_player(player);

        draw_controls();

        handle_user_input(player, enemy_1, enemy_2);

        refresh_screen(60);
    }

    game_over_screen(player.score, replay_game);
}

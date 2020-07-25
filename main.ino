void setup() {
    set_up_hardware();
    
    display_start_screen();
    wait_for_player_to_start_new_game();
    set_up_new_game();
}

void loop() {
    read_buttons();

    update_paddle_positions();

    update_ball_position();
    if (ball_collided()) {
        play_sound();
        change_ball_trajectory();
    }

    update_screen();

    if (scored_a_point()) {
        blink_LEDs();
        update_score();
        reset_events();
        reset_ball();
    }

    if (reached_point_goal()) {
        display_win_lose_message();

        display_start_screen();
        wait_for_player_to_start_new_game();
        set_up_new_game();
    }
}
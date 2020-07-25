/**
 * This file includes all of the variables and instructions (functions) for the game.
 */

/**
 * DEFINING THE TYPES OF VARIABLES
 */

struct Ball {
    int x;
    int y;

    uint radius;

    uint speed;
    double direction;  // angle in radians
};

struct Paddle {
    int x;
    int y;

    uint height;
    uint width;

    uint speed;
    int direction;
};

struct Score {
    uint player;
    uint opponent;
    uint target;
};

struct Events {
    bool ball_collided_with_wall;
    bool ball_collided_with_paddle;

    bool player_scored_a_point;
    bool opponent_scored_a_point;
};

/**
 * VARIABLES USED BY PONG GAME
 */
// ball is an instance of Ball
Ball ball;

// player_paddle and opponent_paddle are instances of Paddle
Paddle player_paddle;
Paddle opponent_paddle;

// score is an instance of Score
Score score;

// events is an instance of Events
Events events;

/**
 * FUNCTIONS
 */

/**
 * This function will
 * give the ball a random direction
 * put the paddles in their starting position
 * reset the score
 * reset the game events
 */
void set_up_new_game() {
    SerialUSB.println("Starting new game");

    reset_ball();

    reset_paddles();

    reset_score();

    reset_events();
}

void reset_ball() {
    SerialUSB.println("Resetting ball");

    ball.radius = 3;

    // center the ball vertically and horizontally
    ball.x = screen.getDisplayHeight() / 2;
    ball.y = screen.getDisplayWidth() / 2;

    ball.speed = 5;

    ball.direction = PI - 2.0 * PI / 7;
}

void reset_paddles() {
    SerialUSB.println("Resetting paddles");

    player_paddle.x = 2;
    player_paddle.y = screen.getDisplayHeight() / 2;
    player_paddle.width = 2;
    player_paddle.height = 20;
    player_paddle.speed = 5;
    player_paddle.direction = -1;

    opponent_paddle.x = screen.getDisplayWidth() - 2;
    opponent_paddle.y = screen.getDisplayHeight() / 2;
    opponent_paddle.width = 2;
    opponent_paddle.height = 20;
    opponent_paddle.speed = 5;
    opponent_paddle.direction = -1;
}

void reset_score() {
    SerialUSB.println("Resetting score");

    score.player = 0;
    score.opponent = 0;
    score.target = 7;
}

void reset_events() {
    SerialUSB.println("Resetting score");

    events.ball_collided_with_wall = false;
    events.ball_collided_with_paddle = false;

    events.player_scored_a_point = false;
    events.opponent_scored_a_point = false;
}

/**
 * This function should move the player's paddle based on which button is pressed
 * It should also move the opponent's paddle based on some AI logic.
 */
void update_paddle_positions() {
    SerialUSB.println("Updating paddles");

    update_player_paddle();

    update_opponent_paddle();
}

void update_player_paddle() {
    if (up_button.pressed) {
        // move up
        player_paddle.y += (player_paddle.speed * player_paddle.direction);
        // y = y + (speed*direction)
    }

    if (down_button.pressed) {
        // move down
        player_paddle.y -= (player_paddle.speed * player_paddle.direction);
        // y = y - (speed*direction)
    }

    // If someone was pressing both, the paddle would end up at the same place
}

/**
 * This function should move the opponent paddle using some AI strategy
 * One suggestion:
 *  move the paddle up if the ball is above it
 *  move the paddle down if the ball is below it
 * There are probably more intelligent strategies
 */
void update_opponent_paddle() {}

/**
 * This function moves the ball based on its current angle (direction)
 * and its speed
 */
void update_ball_position() {
    SerialUSB.println("Updating ball");

    ball.x += (cos(ball.direction) * ball.speed);
    ball.y += (sin(ball.direction) * ball.speed);
}

/**
 * This function should detect if the ball hit the top or bottom of the screen or one of the
 * paddles. If it has, return true Else, return false
 *
 */
bool ball_collided() {
    SerialUSB.println("Checking if ball collided");

    bool ball_hit_ceiling = (ball.y - ball.radius) <= 0;

    bool ball_hit_floor = (ball.y + ball.radius) >= screen.getDisplayHeight();

    // ball collided with the wall if it hit the ceiling OR hit the floor
    events.ball_collided_with_wall = ball_hit_ceiling || ball_hit_floor;

    // if the ball is touching the player_p paddle
    bool ball_hit_player_paddle =
        (ball.x <= player_paddle.x) && 
        (ball.y >= player_paddle.y) && 
        (ball.y <= player_paddle.y);

    // if the ball is touching the opponent paddle
    bool ball_hit_opponent_paddle = 
        (ball.x >= opponent_paddle.x) &&
        (ball.y >= opponent_paddle.y) && 
        (ball.y <= opponent_paddle.y);

    // ball collided with a paddle if it hit the player paddle OR opponent paddle
    events.ball_collided_with_paddle = ball_hit_player_paddle || ball_hit_opponent_paddle;

    // ball collided if it collided with a wall OR with a paddle
    bool did_ball_collide = events.ball_collided_with_wall || events.ball_collided_with_paddle;

    SerialUSB.print("Did ball collide? ");
    SerialUSB.print(did_ball_collide);
    SerialUSB.println("");

    return did_ball_collide;
}

/**
 * This function will start playing the buzzer and start a timer
 * so the buzzer will automatically stop in 50 miliseconds.
 * If we didn't use a timer, the game would freeze for 50ms every time the ball hit a wall
 */
void play_sound() {
    SerialUSB.println("Playing sound");
    buzzer.play_sound(494 /*Hz*/, 50 /*ms*/);
}

/**
 * Since the ball has collided, we need to reflect the direction the ball is moving
 * so that it bounces off.
 */
void change_ball_trajectory() {
    SerialUSB.println("Changing ball change_ball_trajectory");

    // undo the last ball movement
    ball.direction -= PI;
    update_ball_position();
    ball.direction += PI;

    // make the ball bounce off
    if (events.ball_collided_with_wall) ball.direction = ball.direction + PI;
    if (events.ball_collided_with_paddle) ball.direction = -1 * ball.direction + PI;
    update_ball_position();

    // clear the events
    events.ball_collided_with_wall = false;
    events.ball_collided_with_paddle = false;
}

/**
 * This function creates a blank canvas for the screen
 * Draws all the elements
 * and sends the new image to the screen so it can display it
 */
void update_screen() {
    SerialUSB.println("Updating screen");

    create_blank_image_for_screen();

    draw_court();

    draw_score();

    draw_paddles();

    draw_ball();

    send_new_image_to_screen();
}

/**
 * Draw dashed line going down the center of the screen
 */
void draw_court() { SerialUSB.println("Drawing court"); }

/**
 * Draw player's score to left of center
 * Draw opponent's score to right of center
 */
void draw_score() {
    SerialUSB.println("Printing scores");

    // use Helvetica font, Bold, 18 pixels tall
    screen.setFont(u8g2_font_helvB18_tr);

    screen.setCursor(/*x=*/20, /*y=*/20);
    screen.print(score.player);

    screen.setCursor(/*x=*/screen.getDisplayWidth() - 20, /*y=*/20);
    screen.print(score.opponent);
}

void draw_paddles() {
    SerialUSB.println("Drawing paddles");

    // draw player paddle
    screen.drawBox(player_paddle.x, player_paddle.y, player_paddle.width, player_paddle.height);

    // draw opponent paddle
    screen.drawBox(opponent_paddle.x, opponent_paddle.y, opponent_paddle.width,
                   opponent_paddle.height);
}

void draw_ball() {
    SerialUSB.println("Drawing ball");

    screen.drawDisc(ball.x, ball.y, ball.radius);
}

/**
 * This function checks if either player has scored a point.
 * It updates the game events if someone has scored;
 * If someone has scored, it returns true.
 * Else, returns false.
 *
 * A point is scored when the ball hits the left or right wall
 */
bool scored_a_point() {
    SerialUSB.println("Checking if someone scored a point");

    // player scored a point if ball has crossed the opponent paddle
    events.player_scored_a_point = ball.x >= (opponent_paddle.x + opponent_paddle.width);

    // player scored a point if ball has crossed the player paddle
    events.opponent_scored_a_point = ball.x <= (player_paddle.x);

    // a point was scored if either the player scored OR the opponent scored
    bool did_score_point = events.player_scored_a_point || events.opponent_scored_a_point;

    SerialUSB.print("Did someone score? ");
    SerialUSB.print(did_score_point);
    SerialUSB.println("");

    return did_score_point;
}

void update_score() {
    SerialUSB.println("Updating score");
    if (events.player_scored_a_point) score.player++;
    if (events.opponent_scored_a_point) score.opponent++;
}

/**
 * This function starts blinking the chosen LED with a timer
 * so that the LEDs will blink for 1.5 seconds
 * without freezing the game.
 */
void blink_LEDs() {
    SerialUSB.println("Blinking LEDs");
    if (events.player_scored_a_point) green_LED.blink(1 /*Hz*/, 1500 /*ms*/);
    if (events.opponent_scored_a_point) red_LED.blink(1 /*Hz*/, 1500 /*ms*/);
}

void create_blank_image_for_screen() {
    SerialUSB.println("Creating blank image in buffer");
    screen.clearBuffer();
}

void send_new_image_to_screen() {
    SerialUSB.println("Sending buffer so screen can display it");
    screen.sendBuffer();
}

/**
 * This function checks if either player has reached the max score.
 * If someone has, then it returns true.
 * Else, returns false.
 */
bool reached_point_goal() {
    SerialUSB.println("Checking if either player has won");

    // return true if player has reached target OR if opponent has reached target
    // double vertical lines means OR
    return (score.player == score.target) || (score.opponent == score.target);
}

void display_win_lose_message() {
    SerialUSB.println("Displaying winner/loser");

    create_blank_image_for_screen();

    // use Helvetica font, Bold, 18 pixels tall
    screen.setFont(u8g2_font_helvB18_tr);

    if (score.player > score.opponent) {
        screen.drawStr(/*x=*/5, /*y=*/20, "You won!");
    } else if (score.opponent > score.player) {
        screen.drawStr(/*x=*/5, /*y=*/20, "You lost!");
        screen.drawStr(/*x=*/5, /*y=*/40, "Try again!");
    }

    send_new_image_to_screen();

    // give player time to read the message
    delay(1000);
}

void display_start_screen() {
    SerialUSB.println("Displaying start screen");

    create_blank_image_for_screen();

    uint line_position = 40;
    // use Helvetica font, Bold, 18 pixels tall
    screen.setFont(u8g2_font_helvB18_tr);
    screen.drawStr(/*x=*/10, /*y=*/line_position, "PONG");

    line_position += 20;
    // use Helvetica font, Bold, 12 pixels tall
    screen.setFont(u8g2_font_helvB12_tr);
    screen.drawStr(/*x=*/10, /*y=*/line_position, "Press");
    line_position += 16;
    screen.drawStr(/*x=*/10, /*y=*/line_position, "Up Button");
    line_position += 16;
    screen.drawStr(/*x=*/10, /*y=*/line_position, "to start a game");

    send_new_image_to_screen();
}

void wait_for_player_to_start_new_game() {
    SerialUSB.println("Waiting for player");

    // wait until player presses the Up button
    // exclamation means NOT
    while (!up_button.pressed) {
        read_buttons();
    }
}

/** List of possible bug ideas:
 * opponent paddle never moves
 * -> student implements some logic by duplicating logic from player
 *
 * ball bounces back in same direction instead of reflecting
 * -> only switch x or y, not both OR flip slope instead of step
 *
 * ball doesn't reflect off the screen at all
 *
 * paddles can go off of the screen
 * -> if y>=127-paddle.height or y<=0, reset
 *
 * Have the students center the text on the screen
 * Font is too big so text doesn't fit in one line
 */
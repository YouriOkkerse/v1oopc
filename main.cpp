#include "hwlib.hpp"
#include "mpu6050.hpp"

const int screen_width = 128;
const int screen_height = 64;
int head_x, head_y, prev_head_x = -3, prev_head_y = -3, dot_x, dot_y, score;
int tail_x[100], tail_y[100];
int tail_length;

int point_location_x = screen_width / 2;
int point_location_y = screen_height / 2;

enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

int dot_rand(int max)
{
    int r = rand() % max;
    for(;;)
    {
        if(r == 0)
        {
            return r + 3;
        }else if(r % 3 == 0)
        {
            return r - 3;
        }else
        {
            r = rand() % max;
        }
    }
}

int dot_rand_v2(int max)
{
    int r = (unsigned)3 * rand() % max;
    if(r > 6)
    {
        return r - 3;
    }else
    {
        return r + 3;
    }
}

void move_location_x(int x_state, int x_location)
{
    if(x_state == 1 && x_location < (128-1))
    {
        x_location += 1;
    }
    else if(x_state == -1 && x_location > 0)
    {
        x_location -= 1;
    }
}

void move_location_y(int y_state, int y_location)
{
    if(y_state == 1 && y_location > 0)
    {
        y_location -= 1;
    }
    else if(y_state == -1 && y_location < (64-1))
    {
        y_location += 1;
    }
}

// If you want to create a circle and update it with it's location
void update_screen(hwlib::location c_location, auto & screen)
{
    hwlib::circle c(c_location, 5);
    screen.clear();
    c.draw(screen);
}

void fix_corner(auto & screen)
{
    screen.write(hwlib::location(0, 0), hwlib::black);
    screen.write(hwlib::location(1, 0), hwlib::black);
    screen.write(hwlib::location(0, 1), hwlib::black);
}

void draw_segment(int x_value, int y_value, auto & screen)
{
    screen.write(hwlib::location(x_value, y_value), hwlib::black);
    screen.write(hwlib::location(x_value + 1, y_value), hwlib::black);
    screen.write(hwlib::location(x_value + 1, y_value + 1), hwlib::black);
    screen.write(hwlib::location(x_value, y_value + 1), hwlib::black);
}

void remove_segment(int x_value, int y_value, auto & screen)
{
    screen.write(hwlib::location(x_value, y_value), hwlib::white);
    screen.write(hwlib::location(x_value + 1, y_value), hwlib::white);
    screen.write(hwlib::location(x_value + 1, y_value + 1), hwlib::white);
    screen.write(hwlib::location(x_value, y_value + 1), hwlib::white);
    fix_corner(screen);
}

void border_draw(int s_width, int s_height, auto & screen)
{
    for(int i = 0; i < s_width; i++)
    {
        screen.write(hwlib::location(i, 0), hwlib::black);
        screen.write(hwlib::location(i, s_height - 1), hwlib::black);
    }
    for(int j = 0; j < s_height; j++)
    {
        screen.write(hwlib::location(0, j), hwlib::black);
        screen.write(hwlib::location(s_width - 1, j), hwlib::black);
    }
}

void snake_draw(int x, int y, auto & screen)
{
    for(int i = 0; i < 64; i++)
    {
        for(int j = 0; j < 128; j++)
        {
            if(i == y && j == x)
            {
                draw_segment(x, y, screen);
            }
            else
            {
                for(int k = 0; k < tail_length; k++)
                {
                    if(tail_x[k] == j && tail_y[k] == i)
                    {
                        draw_segment(j, i, screen);
                    }
                }
            }
        }
    }
}

void snake_remove(int s_x, int s_y, auto & screen)
{
    for(int i = 0; i < 64; i++)
    {
        for(int j = 0; j < 128; j++)
        {
            if(i == s_y && j == s_x)
            {
                remove_segment(s_x, s_y, screen);
            }
        }
    }
}

void dot_draw(auto & screen)
{
    for(int i = 0; i < 64; i++)
    {
        for(int j = 0; j < 128; j++)
        {
            if(i == dot_y && j == dot_x)
            {
                draw_segment(dot_x, dot_y, screen);
            }
        }
    }
}

void snake_move(int x_state, int y_state, int x_value, int y_value)
{
    if(x_state == -1 && dir != RIGHT && x_value > y_value)
    {
        dir = LEFT;
    }else if(x_state == 1 && dir != LEFT && x_value > y_value)
    {
        dir = RIGHT;
    }else if(y_state == -1 && dir != DOWN && y_value > x_value)
    {
        dir = UP;
    }else if(y_state == 1 && dir != UP && y_value > x_value)
    {
        dir = DOWN;
    }
}

void snake_setup(auto & screen)
{
    screen.clear();
    border_draw(screen_width, screen_height, screen);
    tail_length = 1;
    dir = STOP;
    head_x = screen_width / 2;
    head_y = screen_height / 2;
    dot_x = dot_rand_v2(screen_width - 1);
    dot_y = dot_rand_v2(screen_height - 2);
    score = 0;
    snake_draw(head_x, head_y, screen);
    dot_draw(screen);
}

void snake_logic(auto & screen)
{
    int prev_x = tail_x[0];
    int prev_y = tail_y[0];
    int prev_x2, prev_y2;
    for(int i = 0; i <= tail_length; i++)
    {
        tail_x[0] = head_x;
        tail_y[0] = head_y;
        prev_x2 = tail_x[i];
        prev_y2 = tail_y[i];
        tail_x[i] = prev_x;
        tail_y[i] = prev_y;
        prev_x = prev_x2;
        prev_y = prev_y2;
    }
    switch (dir)
    {
        case LEFT:
            head_x-=3;
            break;
        case RIGHT:
            head_x+=3;
            break;
        case UP:
            head_y+=3;
            break;
        case DOWN:
            head_y-=3;
            break;
        default:
            break;
    }
    /* Use this if you want to restart when going out of bounds */
//    if(head_x > (128 - 4) || head_x < 0 || head_y > (64 - 4) || head_y < 0)
//    {
//        snake_setup(screen);
//    }
    /* Use this if you want your snake to continue from the oposite wall when colliding into a wall */
    if(head_x > (128 - 4))
    {
        head_x = 2;
    }else if(head_x < 1)
    {
        head_x = (128 - 4);
    }else if(head_y > (64 - 4))
    {
        head_y = 2;
    }else if(head_y < 2)
    {
        head_y = (64 - 4);
    }
    /*Use this if you want to reset the game when the snake collides with it's tail (Broken values) */
//    for(int i = 1; i < tail_length; i++)
//    {
//        if((head_x == tail_x[i] + 1 && head_y == tail_y[i] + 1) || (head_x == tail_x[i] - 1 && head_y == tail_y[i] + 1) || 
//        (head_x == tail_x[i] + 1 && head_y == tail_y[i] - 1) || (head_x == tail_x[i] - 1 && head_y == tail_y[i] - 1) ||
//        (head_x == tail_x[i] && head_y == tail_y[i] + 1) || (head_x == tail_x[i] - 1 && head_y == tail_y[i]) ||
//        (head_x == tail_x[i] && head_y == tail_y[i] - 1) || (head_x == tail_x[i] + 1 && head_y == tail_y[i]))
//        {
//            snake_setup(screen);
//        }
//    }
    if((head_x == dot_x + 1 && head_y == dot_y + 1) || (head_x == dot_x - 1 && head_y == dot_y + 1) || 
        (head_x == dot_x + 1 && head_y == dot_y - 1) || (head_x == dot_x - 1 && head_y == dot_y - 1) ||
        (head_x == dot_x && head_y == dot_y + 1) || (head_x == dot_x - 1 && head_y == dot_y) ||
        (head_x == dot_x && head_y == dot_y - 1) || (head_x == dot_x + 1 && head_y == dot_y))
    {
        score += 10;
        remove_segment(dot_x, dot_y, screen);
        dot_x = dot_rand_v2(screen_width);
        dot_y = dot_rand_v2(screen_height);
        tail_length++;
        dot_draw(screen);
        hwlib::cout << "score: " << score << "\n";
    }
}

void draw_something(int x_state, int y_state, auto & screen, int speed)
{
    int delay = 100 * speed;
    move_location_x(x_state, point_location_x);
    move_location_y(y_state, point_location_y);
    screen.write(hwlib::location(point_location_x, point_location_y), hwlib::black);
    hwlib::wait_ms(100);
}

int main(void)
{
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    
    namespace target = hwlib::target;
    
    hwlib::wait_ms(500);
    
    auto scl_mpu6050 = target::pin_oc{target::pins::scl};
    auto sda_mpu6050 = target::pin_oc{target::pins::sda};
    auto i2c_bus_mpu6050 = hwlib::i2c_bus_bit_banged_scl_sda{scl_mpu6050, sda_mpu6050};
    auto mpu_6050 = mpu6050(i2c_bus_mpu6050);    
    
    auto scl_oled = target::pin_oc{target::pins::scl1};
    auto sda_oled = target::pin_oc{target::pins::sda1};
    auto i2c_bus_oled = hwlib::i2c_bus_bit_banged_scl_sda{scl_oled, sda_oled};
    auto oled = hwlib::glcd_oled{i2c_bus_oled, 0x3c};
    
    oled.clear();
    
    mpu_6050.start();
    hwlib::cout << "Starting accelerator calibration, do not move the device!\n";
    mpu_6050.calibrate_accel_loading(oled);
    hwlib::cout << "Accelerator calibration complete!\n";
    snake_setup(oled);
    for(;;)
    {
        int x_axis_state = mpu_6050.get_accel_x_state(2);
        int y_axis_state = mpu_6050.get_accel_y_state(2);
        int x_accel_value = mpu_6050.get_accel_x_positive();
        int y_accel_value = mpu_6050.get_accel_y_positive();
        snake_draw(head_x, head_y, oled);
        dot_draw(oled);
        snake_remove(prev_head_x, prev_head_y, oled);
        prev_head_x = tail_x[tail_length];
        prev_head_y = tail_y[tail_length];
        snake_logic(oled);
        snake_move(x_axis_state, y_axis_state, x_accel_value, y_accel_value);
        hwlib::wait_ms(10);
    }
}
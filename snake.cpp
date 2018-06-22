#include "snake.hpp"
#include "hwlib.hpp"

int snake::dot_rand(int max)
{
    int cap = max / 3;
    int r = (unsigned)3 * rand() % cap;
    if(r > 6)
    {
        return r - 3;
    }else
    {
        return r + 3;
    }
}

void snake::fix_corner(hwlib::glcd_oled & screen)
{
    screen.write(hwlib::location(0, 0), hwlib::black);
    screen.write(hwlib::location(1, 0), hwlib::black);
    screen.write(hwlib::location(0, 1), hwlib::black);
}

void snake::draw_segment(int x_value, int y_value, hwlib::glcd_oled & screen)
{
    screen.write(hwlib::location(x_value, y_value), hwlib::black);
    screen.write(hwlib::location(x_value + 1, y_value), hwlib::black);
    screen.write(hwlib::location(x_value + 1, y_value + 1), hwlib::black);
    screen.write(hwlib::location(x_value, y_value + 1), hwlib::black);
}

void snake::remove_segment(int x_value, int y_value, hwlib::glcd_oled & screen)
{
    screen.write(hwlib::location(x_value, y_value), hwlib::white);
    screen.write(hwlib::location(x_value + 1, y_value), hwlib::white);
    screen.write(hwlib::location(x_value + 1, y_value + 1), hwlib::white);
    screen.write(hwlib::location(x_value, y_value + 1), hwlib::white);
    fix_corner(screen);
}

void snake::border_draw(int s_width, int s_height, hwlib::glcd_oled & screen)
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

void snake::snake_draw(int x, int y, hwlib::glcd_oled & screen)
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
//    screen.clear();
}

void snake::snake_remove(int s_x, int s_y, hwlib::glcd_oled & screen)
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
//    screen.clear();
}

void snake::dot_draw(hwlib::glcd_oled & screen)
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
//    screen.clear();
}

void snake::snake_move(int x_state, int y_state, int x_value, int y_value)
{
    if(x_state == -1 && dir != RIGHT && x_value > y_value)
    {
        dir = LEFT;
//        moving = true;
    }else if(x_state == 1 && dir != LEFT && x_value > y_value)
    {
        dir = RIGHT;
//        moving = true;
    }else if(y_state == -1 && dir != DOWN && y_value > x_value)
    {
        dir = UP;
//        moving = true;
    }else if(y_state == 1 && dir != UP && y_value > x_value)
    {
        dir = DOWN;
//        moving = true;
    }
}

void snake::snake_setup(hwlib::glcd_oled & screen)
{
    screen.clear();
    border_draw(screen_width, screen_height, screen);
    tail_length = 1;
    dir = STOP;
    head_x = screen_width / 2;
    head_y = screen_height / 2;
    dot_x = dot_rand(screen_width - 1);
    dot_y = dot_rand(screen_height - 2);
    score = 0;
    snake_draw(head_x, head_y, screen);
    dot_draw(screen);
}

void snake::snake_logic(int & x, int & y, int(&tail_x)[100], int (&tail_y)[100], hwlib::glcd_oled & screen)
{
    int prev_x = tail_x[0];
    int prev_y = tail_y[0];
    int prev_x2, prev_y2;
    for(int i = 0; i <= tail_length; i++)
    {
        tail_x[0] = x;
        tail_y[0] = y;
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
            x-=3;
            break;
        case RIGHT:
            x+=3;
            break;
        case UP:
            y+=3;
            break;
        case DOWN:
            y-=3;
            break;
        default:
            break;
    }
//    if(head_x > (128 - 4) || head_x < 0 || head_y > (64 - 4) || head_y < 0)
//    {
//        snake_setup(screen);
//    }
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
    for(int i = 1; i < tail_length; i++)
    {
        if((head_x == tail_x[i] + 1 && head_y == tail_y[i] + 1) || (head_x == tail_x[i] - 1 && head_y == tail_y[i] + 1) || 
        (head_x == tail_x[i] + 1 && head_y == tail_y[i] - 1) || (head_x == tail_x[i] - 1 && head_y == tail_y[i] - 1) ||
        (head_x == tail_x[i] && head_y == tail_y[i] + 1) || (head_x == tail_x[i] - 1 && head_y == tail_y[i]) ||
        (head_x == tail_x[i] && head_y == tail_y[i] - 1) || (head_x == tail_x[i] + 1 && head_y == tail_y[i]))
        {
            snake_setup(screen);
        }
    }
    if((head_x == dot_x + 1 && head_y == dot_y + 1) || (head_x == dot_x - 1 && head_y == dot_y + 1) || 
        (head_x == dot_x + 1 && head_y == dot_y - 1) || (head_x == dot_x - 1 && head_y == dot_y - 1) ||
        (head_x == dot_x && head_y == dot_y + 1) || (head_x == dot_x - 1 && head_y == dot_y) ||
        (head_x == dot_x && head_y == dot_y - 1) || (head_x == dot_x + 1 && head_y == dot_y))
    {
        score += 10;
        remove_segment(dot_x, dot_y, screen);
        dot_x = dot_rand(screen_width);
        dot_y = dot_rand(screen_height);
        tail_length++;
        dot_draw(screen);
//        hwlib::cout << "score: " << score << "\n";
    }
}
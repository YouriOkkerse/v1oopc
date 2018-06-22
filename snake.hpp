#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "hwlib.hpp"

class snake
{
private:
    int head_x, head_y, prev_head_x = -3, prev_head_y = -3, dot_x, dot_y, score;
    int tail_x[100], tail_y[100];
    int tail_length;
    hwlib::glcd_oled & screen;
    enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN};
    eDirection dir;
    const int screen_width = 128;
    const int screen_height = 64;
public:
//    snake(int head_x, int head_y, hwlib::window & screen):
//    head_x(head_x),
//    head_y(head_y),
//    screen(screen)
//    {}
//    int dot_rand(int max);
//    void fix_corner(hwlib::window & screen);
//    void draw_segment(int x_value, int y_value, hwlib::window & screen);
//    void remove_segment(int x_value, int y_value, hwlib::window & screen);
//    void border_draw(int s_width, int s_height, hwlib::window & screen);
//    void snake_draw(int x, int y, hwlib::window & screen);
//    void snake_remove(int s_x, int s_y, hwlib::window & screen);
//    void dot_draw(hwlib::window & screen);
//    void snake_move(int x_state, int y_state, int x_value, int y_value);
//    void snake_setup(hwlib::window & screen);
//    void snake_logic(hwlib::window & screen);
    
    snake(int head_x, int head_y, hwlib::glcd_oled & screen):
    head_x(head_x),
    head_y(head_y),
    screen(screen)
    {}
    int dot_rand(int max);
    void fix_corner(hwlib::glcd_oled & screen);
    void draw_segment(int x_value, int y_value, hwlib::glcd_oled & screen);
    void remove_segment(int x_value, int y_value, hwlib::glcd_oled & screen);
    void border_draw(int s_width, int s_height, hwlib::glcd_oled & screen);
    void snake_draw(int x, int y, hwlib::glcd_oled & screen);
    void snake_remove(int s_x, int s_y, hwlib::glcd_oled & screen);
    void dot_draw(hwlib::glcd_oled & screen);
    void snake_move(int x_state, int y_state, int x_value, int y_value);
    void snake_setup(hwlib::glcd_oled & screen);
    void snake_logic(int & x, int & y, int (&tail_x)[100], int (&tail_y)[100], hwlib::glcd_oled & screen);
};

#endif // SNAKE_HPP
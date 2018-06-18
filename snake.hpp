#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "hwlib.hpp"

class snake
{
private:
    int x_pos1 = 64, x_pos2, x_pos3, x_pos4, x_pos5, x_pos6, x_pos7, x_pos8;
    int y_pos1 = 32, y_pos2, y_pos3, y_pos4, y_pos5, y_pos6, y_pos7, y_pos8;
    
public:
    snake(int x_pos1, int y_pos1):
    x_pos1(x_pos1),
    y_pos1(y_pos1)
    {}
    directions(int x_direction, int y_direction)
    {
        x_pos1 += x_pos1 + x_direction;
        y_pos1 += y_pos1 + y_direction;
    }
    update()
    {
        x_pos8 = x_pos7;
        x_pos7 = x_pos6;
        x_pos6 = x_pos5;
        x_pos5 = x_pos4;
        x_pos4 = x_pos3;
        x_pos3 = x_pos2;
        x_pos2 = x_pos1;
        
        y_pos8 = y_pos7;
        y_pos7 = y_pos6;
        y_pos6 = y_pos5;
        y_pos5 = y_pos4;
        y_pos4 = y_pos3;
        y_pos3 = y_pos2;
        y_pos2 = y_pos1;
    }
    draw(auto & screen) 
    {
        screen.write(hwlib::location(x_pos1, y_pos1), hwlib::black);
        screen.write(hwlib::location(x_pos2, y_pos2), hwlib::black);
        screen.write(hwlib::location(x_pos3, y_pos3), hwlib::black);
        screen.write(hwlib::location(x_pos4, y_pos4), hwlib::black);
        screen.write(hwlib::location(x_pos5, y_pos5), hwlib::black);
        screen.write(hwlib::location(x_pos6, y_pos6), hwlib::black);
        screen.write(hwlib::location(x_pos7, y_pos7), hwlib::black);
        screen.write(hwlib::location(x_pos8, y_pos8), hwlib::black);
    }
}

#endif // SNAKE_HPP
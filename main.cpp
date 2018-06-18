#include "hwlib.hpp"
#include "mpu6050.hpp"

void move_location_x(int & x_state, int & x_location)
{
    if(x_state == 1 && x_location < 128)
    {
        x_location += 1;
    }
    else if(x_state == -1 && x_location > 0)
    {
        x_location -= 1;
    }
}

void move_location_y(int & y_state, int & y_location)
{
    if(y_state == 1 && y_location > 0)
    {
        y_location -= 1;
    }
    else if(y_state == -1 && y_location < 64)
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
    
//    auto snky = snake::snake(64, 32);
    
    int point_location_x = 64;
    int point_location_y = 32;
    
    oled.clear();
    
    mpu_6050.start();
    hwlib::cout << "Starting calibration, do not move the device!\n";
    mpu_6050.calibrate();
    hwlib::cout << "Calibration complete!\n";
    for(;;)
    {
        int x_axis_state = mpu_6050.get_x_state(1);
        int y_axis_state = mpu_6050.get_y_state(1);
        move_location_x(x_axis_state, point_location_x);
        move_location_y(y_axis_state, point_location_y);
        oled.write(hwlib::location(point_location_x, point_location_y), hwlib::black);
//        int16_t x_value = mpu_6050.get_x();
//        int16_t y_value = mpu_6050.get_y();
//        hwlib::cout << "x: " << x_value << " | " << x_axis_state << " y: " << y_value << " | " << y_axis_state << "\n";
//        snky.directions(x_axis_state, y_axis_state);
//        snky.update();
//        snky.draw(oled);
        hwlib::wait_ms(50);
    }
}
#include "mpu6050.hpp"
#include "hwlib.hpp"

void mpu6050::start()
{
    bus.write(i2c_adress, start_register, 2);
}

int8_t mpu6050::read_8_bit(const reg & r)
{
    uint8_t cmd[1] = {static_cast<uint8_t>(r)};
    bus.write(i2c_adress, cmd, 1);
    bus.read(i2c_adress, cmd, 1);
    int8_t bit_8_value = cmd[0];
    return bit_8_value;
}

int8_t mpu6050::whoami()
{
    uint8_t data[1] = {0x75};
    bus.write(i2c_adress, data, 1);
    bus.read(i2c_adress, data, 1);
    int8_t bit_8_value = data[0];
    return bit_8_value;
}

int16_t mpu6050::read_16_bit(const reg & h, const reg & l)
{
    uint8_t adress_high_value = read_8_bit(h);
    uint8_t adress_low_value = read_8_bit(l);
    int16_t bit_16_value = (adress_high_value << 8) | adress_low_value;
    return bit_16_value;
}

int16_t mpu6050::get_accel_x()
{
    int16_t x_axis = read_16_bit(reg::accel_yout_h, reg::accel_yout_l);
    return x_axis - accel_offset_x;
}

int16_t mpu6050::get_accel_y()
{
    int16_t y_axis = read_16_bit(reg::accel_xout_h, reg::accel_xout_l);
    return y_axis - accel_offset_y;
}

int16_t mpu6050::get_accel_x_positive()
{
    int16_t x_axis = read_16_bit(reg::accel_yout_h, reg::accel_yout_l);
    if(x_axis < 0)
    {
        int rev_x_axis = x_axis*-1 + accel_offset_x;
        return rev_x_axis;
    }
    return x_axis - accel_offset_x;
}

int16_t mpu6050::get_accel_y_positive()
{
    int16_t y_axis = read_16_bit(reg::accel_xout_h, reg::accel_xout_l);
    if(y_axis < 0)
    {
        int rev_y_axis = y_axis*-1 + accel_offset_y;
        return rev_y_axis;
    }
    return y_axis - accel_offset_y;
}

int16_t mpu6050::get_accel_z()
{
    int16_t z_axis = read_16_bit(reg::accel_zout_h, reg::accel_zout_l);
    return z_axis - accel_offset_z;
}

int16_t mpu6050::get_gyro_x()
{
    int16_t x_axis = read_16_bit(reg::gyro_yout_h, reg::gyro_yout_l);
    return x_axis - gyro_offset_x;
}

int16_t mpu6050::get_gyro_y()
{
    int16_t y_axis = read_16_bit(reg::gyro_xout_h, reg::gyro_xout_l);
    return y_axis - gyro_offset_y;
}

int16_t mpu6050::get_gyro_z()
{
    int16_t z_axis = read_16_bit(reg::gyro_zout_h, reg::gyro_zout_l);
    return z_axis - gyro_offset_z;
}

void mpu6050::calibrate_accel()
{
    int count = 0;
    int32_t temp_accel_x = 0;
    int32_t temp_accel_y = 0;
    int32_t temp_accel_z = 0;
    for(int i = 0; i < 50; i++)
    {
        count++;
        temp_accel_x += get_accel_x();
        temp_accel_y += get_accel_y();
        temp_accel_z += get_accel_y();
        hwlib::wait_ms(100);
    }
    accel_offset_x = (temp_accel_x / count);
    accel_offset_y = (temp_accel_y / count);
    accel_offset_z = (temp_accel_z / count);
}

void mpu6050::load_segment(int x, int y, hwlib::glcd_oled & screen)
{
    screen.write(hwlib::location(x, y), hwlib::black);
    screen.write(hwlib::location(x + 1, y), hwlib::black);
    screen.write(hwlib::location(x, y + 1), hwlib::black);
    screen.write(hwlib::location(x + 1, y + 1), hwlib::black);
    screen.write(hwlib::location(x, y + 2), hwlib::black);
    screen.write(hwlib::location(x + 1, y + 2), hwlib::black);
    screen.write(hwlib::location(x, y + 3), hwlib::black);
    screen.write(hwlib::location(x + 1, y + 3), hwlib::black);
    screen.write(hwlib::location(x, y + 4), hwlib::black);
    screen.write(hwlib::location(x + 1, y + 4), hwlib::black);
    screen.write(hwlib::location(x, y + 5), hwlib::black);
    screen.write(hwlib::location(x + 1, y + 5), hwlib::black);
    screen.write(hwlib::location(x, y + 6), hwlib::black);
    screen.write(hwlib::location(x + 1, y + 6), hwlib::black);
    screen.write(hwlib::location(x, y + 7), hwlib::black);
    screen.write(hwlib::location(x + 1, y + 7), hwlib::black);
}

void mpu6050::calibrate_accel_loading(hwlib::glcd_oled & screen)
{
    int count = 0;
    int32_t temp_accel_x = 0;
    int32_t temp_accel_y = 0;
    int32_t temp_accel_z = 0;
    
    for(int i = 12; i < 114; i++)
    {
        screen.write(hwlib::location(i, 26), hwlib::black);
        screen.write(hwlib::location(i, 37), hwlib::black);
    }
    
    for(int i = 26; i < 38; i++)
    {
        screen.write(hwlib::location(11, i), hwlib::black);
        screen.write(hwlib::location(114, i), hwlib::black);
    }
    
    for(int i = 0; i < 50; i++)
    {
        count++;
        temp_accel_x += get_accel_x();
        temp_accel_y += get_accel_y();
        temp_accel_z += get_accel_y();
        load_segment((13 + (i*2)), 28, screen);
        hwlib::wait_ms(100);
    }
    accel_offset_x = (temp_accel_x / count);
    accel_offset_y = (temp_accel_y / count);
    accel_offset_z = (temp_accel_z / count);
}

void mpu6050::calibrate_gyro()
{
    int count = 0;
    int32_t temp_gyro_x = 0;
    int32_t temp_gyro_y = 0;
    int32_t temp_gyro_z = 0;
    for(int i = 0; i < 50; i++)
    {
        count++;
        temp_gyro_x += get_gyro_x();
        temp_gyro_y += get_gyro_y();
        temp_gyro_z += get_gyro_y();
        hwlib::wait_ms(100);
    }
    gyro_offset_x = (temp_gyro_x / count);
    gyro_offset_y = (temp_gyro_y / count);
    gyro_offset_z = (temp_gyro_z / count);
}

int mpu6050::get_accel_x_state(int sensitivity)
{
    int sens = sensitivity * 1000;
    int max_s = 5000;
    int min_s = 1000;
    if(sens < min_s)
    {
        sens = 1000;
    }
    else if(sens > max_s)
    {
        sens = 5000;
    } 
    // x_start  = 0     | 0
    // x_left   < sens  | -1
    // x_right  > sens  | 1
    if(mpu6050::get_accel_x() > (sens))
    {
        return 1;
    }
    else if(mpu6050::get_accel_x() < (-sens))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
int mpu6050::get_accel_y_state(int sensitivity)
{
    int sens = sensitivity * 1000;
    int max_s = 5000;
    int min_s = 1000;
    if(sens < min_s)
    {
        sens = 1000;
    }
    else if(sens > max_s)
    {
        sens = 5000;
    }
    // y_start  = 0     | 0
    // y_top    > sens  | 1
    // y_bottom < sens  | -1
    if(mpu6050::get_accel_y() < (-sens))
    {
        return 1;
    }
    else if(mpu6050::get_accel_y() > (sens))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
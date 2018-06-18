#include "mpu6050.hpp"
#include "hwlib.hpp"

void mpu6050::start()
{
    bus.write(adress_low, start_register, 2);
}
void mpu6050::calibrate()
{
    int count = 0;
    int32_t temp_x = 0;
    int32_t temp_y = 0;
    for(int i = 0; i < 50; i++)
    {
        count++;
        temp_x += mpu6050::get_x();
        temp_y += mpu6050::get_y();
        hwlib::wait_ms(100);
    }
    offset_x = (temp_x / count);
    offset_y = (temp_y / count);
}

int8_t mpu6050::read_8_bit(byte adress[8])
{
    bus.write(MPU_6050_ADDRESS_LOW, adress, 1);
    bus.read(MPU_6050_ADDRESS_LOW, adress, 1);
    int8_t bit_8_value = adress[0];
    return bit_8_value;
}

int16_t mpu6050::read_16_bit(byte adress_high[8], byte adress_low[8])
{
    int8_t adress_high_value = read_8_bit(adress_high);
    int8_t adress_low_value = read_8_bit(adress_low)
    int16_t bit_16_value = (adress_high_value[0] << 8) + adress_low_value[0];
    return bit_16_value;
}

//int16_t mpu6050::get_x()
//{
//    byte data_high[8] = {MPU_6050_ACCEL_YOUT_H};
//    byte data_low[8] = {MPU_6050_ACCEL_YOUT_L};
//    bus.write(MPU_6050_ADDRESS_LOW, data_high, 1);
//    bus.read(MPU_6050_ADDRESS_LOW, data_high, 1);
//    bus.write(MPU_6050_ADDRESS_LOW, data_low, 1);
//    bus.read(MPU_6050_ADDRESS_LOW, data_low, 1);
//    int16_t x_axis = (data_high[0] << 8) + data_low[0];
//    return x_axis - offset_x;
//}

int16_t mpu6050::get_x()
{
    int16_t x_axis = read_16_bit(MPU_6050_ACCEL_YOUTH_H, MPU_6050_ADRESS_LOW);
    return x_axis - offset_x;
}

int16_t mpu6050::get_y()
{
    byte data_high[8] = {MPU_6050_ACCEL_XOUT_H};
    byte data_low[8] = {MPU_6050_ACCEL_XOUT_L};
    bus.write(MPU_6050_ADDRESS_LOW, data_high, 1);
    bus.read(MPU_6050_ADDRESS_LOW, data_high, 1);
    bus.write(MPU_6050_ADDRESS_LOW, data_low, 1);
    bus.read(MPU_6050_ADDRESS_LOW, data_low, 1);
    int16_t y_axis = 0;
    y_axis = (data_high[0] << 8) + data_low[0];
    return y_axis - offset_y;
}
int16_t mpu6050::get_z()
{
    byte data[8] = {0x3F};
    bus.write(adress_low, data, 1);
    bus.read(adress_low, data, 2);
    int16_t z_axis = 0;
    z_axis = (data[0] << 8) + data[1];
    return z_axis;
}
int mpu6050::get_x_state(int sensitivity)
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
    if(mpu6050::get_x() > (sens))
    {
        return 1;
    }
    else if(mpu6050::get_x() < (-sens))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
int mpu6050::get_y_state(int sensitivity)
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
    if(mpu6050::get_y() < (-sens))
    {
        return 1;
    }
    else if(mpu6050::get_y() > (sens))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
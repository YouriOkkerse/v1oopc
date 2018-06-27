#ifndef MPU6050_TEST_HPP
#define MPU6050_TEST_HPP

#include "hwlib.hpp"
#include <cmath>

class mpu6050_test
{
private:
        hwlib::i2c_bus_bit_banged_scl_sda bus;
        const uint8_t i2c_adress  = 0x68;
public:
    enum class reg
    {
        //Gyroscope and accelerometer tests
        self_test_x  = 0x0D,
        self_test_y  = 0x0E,
        self_test_z  = 0x0F,
        self_test_a  = 0x10,
    
        gyro_config  = 0x1B,
        accel_config = 0x1C
    };
    mpu6050_test(hwlib::i2c_bus_bit_banged_scl_sda bus):bus(bus){}
    int8_t read_8_bit(const reg & r)
    {
        uint8_t cmd[1] = {static_cast<uint8_t>(r)};
        bus.write(i2c_adress, cmd, 1);
        bus.read(i2c_adress, cmd, 1);
        int8_t bit_8_value = cmd[0];
        return bit_8_value;
    }
    void print_self_test()
    {
        uint8_t results[4];
        uint8_t test_results[6];
        int convert_factory_values[6];
        int self_test[6];
        
        //Configure the accelerometer and the gyroscope and enable selftest.
        uint8_t cac[1] = {static_cast<uint8_t>(reg::accel_config)};
        bus.write(i2c_adress, cac, 0xF0);
        uint8_t cgc[1] = {static_cast<uint8_t>(reg::gyro_config)};
        bus.write(i2c_adress, cgc, 0xE0);
        
        hwlib::wait_ms(500);
        
        // x, y, z-axis test results (a = mixed-axis results).
        results[0] = read_8_bit(reg::self_test_x);
        results[1] = read_8_bit(reg::self_test_y);
        results[2] = read_8_bit(reg::self_test_z);
        results[3] = read_8_bit(reg::self_test_a);
        
        // Extracting the results (results are 5-bit unsigned integers).
        test_results[0] = (results[0] >> 3) | (results[3] & 0x30) >> 4; // XA
        test_results[1] = (results[1] >> 3) | (results[3] & 0x0C) >> 2; // YA
        test_results[2] = (results[2] >> 3) | (results[3] & 0x03) >> 0; // ZA
        test_results[3] = results[0] & 0x1F; // XG
        test_results[4] = results[1] & 0x1F; // YG
        test_results[5] = results[2] & 0x1F; // ZG
        
        convert_factory_values[0] = (int)((4096 * 0.34 * pow(0.92/0.34, ((test_results[0]-1)/(pow(2, 5)-2)))));
        convert_factory_values[1] = (int)((4096 * 0.34 * pow(0.92/0.34, ((test_results[0]-1)/(pow(2, 5)-2)))));
        convert_factory_values[2] = (int)((4096 * 0.34 * pow(0.92/0.34, ((test_results[0]-1)/(pow(2, 5)-2)))));
        
        convert_factory_values[3] = (int)(25 * 131 * pow(1.046, test_results[3]-1));
        convert_factory_values[4] = (int)(-25 * 131 * pow(1.046, test_results[4]-1));
        convert_factory_values[5] = (int)(25 * 131 * pow(1.046, test_results[5]-1));
        
        for(int i = 0; i < 6; i++)
        {
            self_test[i] = 100 + 100 * (test_results[i] - convert_factory_values[i])/convert_factory_values[i];
        }
        
        if(self_test[0] < 1 && self_test[1] < 1 && self_test[2] < 1 && self_test[3] < 1 && self_test[4] < 1 && self_test[5] < 1)
        {
            hwlib::cout << "mpu6050 selftest passed!\n"; 
        }
        hwlib::wait_ms(50);
        hwlib::cout << (int)test_results[0] << " | " << convert_factory_values[0] <<"\n";
        hwlib::wait_ms(50);
        hwlib::cout << (int)test_results[1] << " | " << convert_factory_values[1] <<"\n";
        hwlib::wait_ms(50);
        hwlib::cout << (int)test_results[2] << " | " << convert_factory_values[2] <<"\n";
        hwlib::wait_ms(50);
        hwlib::cout << (int)test_results[3] << " | " << convert_factory_values[3] <<"\n";
        hwlib::wait_ms(50);
        hwlib::cout << (int)test_results[4] << " | " << convert_factory_values[4] <<"\n";
        hwlib::wait_ms(50);
        hwlib::cout << (int)test_results[5] << " | " << convert_factory_values[5] <<"\n";
    }
    
};

#endif // MPU6050_TEST_HPP
#ifndef TESTMPU6050_HPP
#define TESTMPU6050_HPP

#include "mpu6050.hpp"
#include "hwlib.hpp"

class testmpu6050
{
private:
    mpu6050 & mpu_6050;
public:
    testmpu6050(mpu6050 & mpu_6050):mpu_6050(mpu_6050){}
    
    void print_test_whoami()
    {
        if(mpu_6050.whoami() == 104)
        {
            hwlib::cout << "test_whoami: passed!\n";
        }
        else
        {
            hwlib::cout << "test_whoami: failed!\n";
        }
    }
    
    void print_test_accel_x()
    {
        if(mpu_6050.get_accel_x() > -400 && mpu_6050.get_accel_x() < 100)
        {
            hwlib::cout << "test_accel_x: passed!\n";
        }
        else
        {
            hwlib::cout << "test_accel_x: failed!\nMake sure the mpu6050 is leveled and not moving.\n";
        }
    }
    
    void print_test_accel_y()
    {
        if(mpu_6050.get_accel_y() > 1300 && mpu_6050.get_accel_y() < 1900)
        {
            hwlib::cout << "test_accel_y: passed!\n";
        }
        else
        {
            hwlib::cout << "test_accel_y: failed!\nMake sure the mpu6050 is leveled and not moving.\n";
        }
    }
    
    void print_test_accel_z()
    {
        if(mpu_6050.get_accel_z() > 20000 && mpu_6050.get_accel_z() < 25000)
        {
            hwlib::cout << "test_accel_z: passed!\n";
        }
        else
        {
            hwlib::cout << "test_accel_z: failed!\nMake sure the mpu6050 is leveled and not moving.\n";
        }
    }
    
    void print_test_gyro_x()
    {
        if(mpu_6050.get_gyro_x() > -200 && mpu_6050.get_gyro_x() < 100)
        {
            hwlib::cout << "test_gyro_x: passed!\n";
        }
        else
        {
            hwlib::cout << "test_gyro_x: failed!\nMake sure the mpu6050 is leveled and not moving.\n";
        }
    }
    
    void print_test_gyro_y()
    {
        if(mpu_6050.get_gyro_y() > -1200 && mpu_6050.get_gyro_y() < -400)
        {
            hwlib::cout << "test_gyro_y: passed!\n";
        }
        else
        {
            hwlib::cout << "test_gyro_y: failed!\nMake sure the mpu6050 is leveled and not moving.\n";
        }
    }
    
    void print_test_gyro_z()
    {
        if(mpu_6050.get_gyro_z() > -200 && mpu_6050.get_gyro_z() < 100)
        {
            hwlib::cout << "test_gyro_z: passed!\n";
        }
        else
        {
            hwlib::cout << "test_gyro_z: failed!\nMake sure the mpu6050 is leveled and not moving.\n";
        }
    }
    
    void print_test_accel_state_x()
    {
        if(mpu_6050.get_accel_x_state(2) == 0)
        {
            hwlib::cout << "test_accel_state_x: passed!\n";
        }
        else
        {
            hwlib::cout << "test_accel_state_x: failed!\nMake sure the mpu6050 is leveled and not moving.\n";
        }
    }
    
    void print_test_accel_state_y()
    {
        if(mpu_6050.get_accel_x_state(2) == 0)
        {
            hwlib::cout << "test_accel_state_x: passed!\n";
        }
        else
        {
            hwlib::cout << "test_accel_state_x: failed!\nMake sure the mpu6050 is leveled and not moving.\n";
        }
    }
    
    void print_test_all()
    {
        hwlib::cout << "Running tests for mpu6050..\n";
        hwlib::wait_ms(1000);
        print_test_whoami();
        hwlib::wait_ms(1000);
        print_test_accel_x();
        hwlib::wait_ms(1000);
        print_test_accel_y();
        hwlib::wait_ms(1000);
        print_test_accel_z();
        hwlib::wait_ms(1000);
        print_test_gyro_x();
        hwlib::wait_ms(1000);
        print_test_gyro_y();
        hwlib::wait_ms(1000);
        print_test_gyro_z();
        hwlib::wait_ms(1000);
        print_test_accel_state_x();
        hwlib::wait_ms(1000);
        print_test_accel_state_y();
        hwlib::cout << "\n\n";
    }
};

#endif // TESTMPU6050_HPP
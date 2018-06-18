#ifndef MPU6050_HPP
#define MPU6050_HPP
#define byte uint8_t
#include "hwlib.hpp"
using byte_array = byte[8];

class mpu6050
{
private:
    hwlib::i2c_bus_bit_banged_scl_sda bus;
    byte start_register[2] = {0x6B, 0};
    int16_t offset_x = 0, offset_y = 0;
    
    //MPU6050 Registers
    //MPU addresses
    byte MPU_6050_ADDRESS_LOW  = 0x68;
    byte MPU_6050_ADDRESS_HIGH = 0x69;
    
    //Gyro scale
    byte MPU_6050_GYRO_FS_250  = 0;
    byte MPU_6050_GYRO_FS_500  = 1;
    byte MPU_6050_GYRO_FS_1000 = 2;
    byte MPU_6050_GYRO_FS_2000 = 3;
    
    //Accelerometer scale
    byte MPU_6050_ACCEL_FS_2   = 0;
    byte MPU_6050_ACCEL_FS_4   = 1;
    byte MPU_6050_ACCEL_FS_8   = 2;
    byte MPU_6050_ACCEL_FS_16  = 3;
    
    //Gyroscope and accelerometer tests
    byte MPU_6050_SELF_TEST_X  = 0x0D;
    byte MPU_6050_SELF_TEST_Y  = 0x0E;
    byte MPU_6050_SELF_TEST_Z  = 0x0F;
    byte MPU_6050_SELF_TEST_A  = 0x10;

    //Sample rate
    byte MPU_6050_SMPLRT_DIV   = 0x19;
    
    //Configurations
    byte MPU_6050_CONFIG       = 0x1A;
    byte MPU_6050_GYRO_CONFIG  = 0x1B;
    byte MPU_6050_ACCEL_CONFIG = 0x1C;

    //FIFO buffer
    byte MPU_6050_FIFO_EN          = 0x23;
    
    //Master and slaves
    byte MPU_6050_I2C_MST_CTRL     = 0x24;
    byte MPU_6050_I2C_SLV0_ADDR    = 0x25;
    byte MPU_6050_I2C_SLV0_REG     = 0x26;
    byte MPU_6050_I2C_SLV0_CTRL    = 0x27;
    byte MPU_6050_I2C_SLV1_ADDR    = 0x28;
    byte MPU_6050_I2C_SLV1_REG     = 0x29;
    byte MPU_6050_I2C_SLV1_CTRL    = 0x2A;
    byte MPU_6050_I2C_SLV2_ADDR    = 0x2B;
    byte MPU_6050_I2C_SLV2_REG     = 0x2C;
    byte MPU_6050_I2C_SLV2_CTRL    = 0x2D;
    byte MPU_6050_I2C_SLV3_ADDR    = 0x2E;
    byte MPU_6050_I2C_SLV3_REG     = 0x2F;
    byte MPU_6050_I2C_SLV3_CTRL    = 0x30;
    byte MPU_6050_I2C_SLV4_ADDR    = 0x31;
    byte MPU_6050_I2C_SLV4_REG     = 0x32;
    byte MPU_6050_I2C_SLV4_DO      = 0x33;
    byte MPU_6050_I2C_SLV4_CTRL    = 0x34;
    byte MPU_6050_I2C_SLV4_DI      = 0x35;
    byte MPU_6050_I2C_MST_STATUS   = 0x36;
    
    //Interrupt signals
    byte MPU_6050_INT_PIN_CFG  = 0x37;
    byte MPU_6050_INT_ENABLE   = 0x38;
    byte MPU_6050_INT_STATUS   = 0x3A;
    
    //Accelerometer outputs
    byte MPU_6050_ACCEL_XOUT_H = 0x3B;
    byte MPU_6050_ACCEL_XOUT_L = 0x3C;
    byte MPU_6050_ACCEL_YOUT_H = 0x3D;
    byte MPU_6050_ACCEL_YOUT_L = 0x3E;
    byte MPU_6050_ACCEL_ZOUT_H = 0x3F;
    byte MPU_6050_ACCEL_ZOUT_L = 0x40;
    
    //Temperature outputs
    byte MPU_6050_TEMP_OUT_H   = 0x41;
    byte MPU_6050_TEMP_OUT_L   = 0x42;
    
    //Gyroscope outputs
    byte MPU_6050_GYRO_XOUT_H  = 0x43;
    byte MPU_6050_GYRO_XOUT_L  = 0x44;
    byte MPU_6050_GYRO_YOUT_H  = 0x45;
    byte MPU_6050_GYRO_YOUT_L  = 0x46;
    byte MPU_6050_GYRO_ZOUT_H  = 0x47;
    byte MPU_6050_GYRO_ZOUT_L  = 0x48;
    
    //Clock
    byte MPU_6050_CLOCK_INTERNAL   = 0x00;
    byte MPU_6050_CLOCK_PLL_XGYRO  = 0x01;
    byte MPU_6050_CLOCK_PLL_YGYRO  = 0x02;
    byte MPU_6050_CLOCK_PLL_ZGYRO  = 0x03;
    byte MPU_6050_CLOCK_PLL_EXT32K = 0x04;
    byte MPU_6050_CLOCK_PLL_EXT19M = 0x05;
    byte MPU_6050_CLOCK_KEEP_RESET = 0x07;

    //Data external sensors
    byte MPU_6050_EXT_SENS_DATA_00 = 0x49;
    byte MPU_6050_EXT_SENS_DATA_01 = 0x4A;
    byte MPU_6050_EXT_SENS_DATA_02 = 0x4B;
    byte MPU_6050_EXT_SENS_DATA_03 = 0x4C;
    byte MPU_6050_EXT_SENS_DATA_04 = 0x4D;
    byte MPU_6050_EXT_SENS_DATA_05 = 0x4E;
    byte MPU_6050_EXT_SENS_DATA_06 = 0x4F;
    byte MPU_6050_EXT_SENS_DATA_07 = 0x50;
    byte MPU_6050_EXT_SENS_DATA_08 = 0x51;
    byte MPU_6050_EXT_SENS_DATA_09 = 0x52;
    byte MPU_6050_EXT_SENS_DATA_10 = 0x53;
    byte MPU_6050_EXT_SENS_DATA_11 = 0x54;
    byte MPU_6050_EXT_SENS_DATA_12 = 0x55;
    byte MPU_6050_EXT_SENS_DATA_13 = 0x56;
    byte MPU_6050_EXT_SENS_DATA_14 = 0x57;
    byte MPU_6050_EXT_SENS_DATA_15 = 0x58;
    byte MPU_6050_EXT_SENS_DATA_16 = 0x59;
    byte MPU_6050_EXT_SENS_DATA_17 = 0x5A;
    byte MPU_6050_EXT_SENS_DATA_18 = 0x5B;
    byte MPU_6050_EXT_SENS_DATA_19 = 0x5C;
    byte MPU_6050_EXT_SENS_DATA_20 = 0x5D;
    byte MPU_6050_EXT_SENS_DATA_21 = 0x5E;
    byte MPU_6050_EXT_SENS_DATA_22 = 0x5F;
    byte MPU_6050_EXT_SENS_DATA_23 = 0x60;
    
    //Slave outputs
    byte MPU_6050_I2C_SLV0_DO  = 0x63;
    byte MPU_6050_I2C_SLV1_DO  = 0x64;
    byte MPU_6050_I2C_SLV2_DO  = 0x65;
    byte MPU_6050_I2C_SLV3_DO  = 0x66;
    
    //Master delay control
    byte MPU_6050_I2C_MST_DELAY_CTRL  = 0x67;
    
    //Reset the analog and ditital path
    byte MPU_6050_SIGNAL_PATH_RESET   = 0x68;
    
    //Enable and disable various settings
    byte MPU_6050_USER_CTRL = 0x6A;
    
    //Power management options
    byte MPU_6050_PWR_MGMT_1 = 0x6B;
    byte MPU_6050_PWR_MGMT_2 = 0x6C;
    
    //Sample counter
    byte MPU_6050_FIFO_COUNT_H = 0x72;
    byte MPU_6050_FIFO_COUNT_L = 0x73;
    byte MPU_6050_FIFO_R_W     = 0x74;
    
    //Identity
    byte MPU_6050_WHO_AM_I     = 0x75;
    
public:
    mpu6050(hwlib::i2c_bus_bit_banged_scl_sda bus):bus(bus){}
    void start();
    void calibrate();
    int8_t read_8_bit();
    int16_t read_16_bit();
    int16_t get_x();
    int16_t get_y();
    int16_t get_z();
    int get_x_state(int sensitivity);
    int get_y_state(int sensitivity);
};

#endif // MPU6050_HPP
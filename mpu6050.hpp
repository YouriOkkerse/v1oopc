#ifndef MPU6050_HPP
#define MPU6050_HPP
#define byte uint8_t
#include "hwlib.hpp"
using byte_array = byte[8];

/// @file

/// \brief
/// MPU 6050 Library
/// \details
/// This library gives the user a set of functions to read the output from the mpu6050.
/// There is a set of functions to get raw data from the chip.
/// There are also a set of functions that return values reaching from -1, 0 1.
class mpu6050
{
private:
    hwlib::i2c_bus_bit_banged_scl_sda bus;
    byte start_register[2] = {0x6B, 0};
    int16_t accel_offset_x = 0, accel_offset_y = 0, accel_offset_z = 0,
            gyro_offset_x = 0, gyro_offset_y = 0, gyro_offset_z = 0;
    
    //MPU i2c adress
    const uint8_t i2c_adress  = 0x68;
    
public:
/// \brief
/// MPU 6050 registers
/// \details
/// This enum class contains all the different registers for the mup 6050 sensor.
    enum class reg
    {
        //Gyroscope and accelerometer tests
        self_test_x  = 0x0D,
        self_test_y  = 0x0E,
        self_test_z  = 0x0F,
        self_test_a  = 0x10,

        //Sample rate
        smplrt_div   = 0x19,
        
        //Configurations
        config       = 0x1A,
        gyro_config  = 0x1B,
        accel_config = 0x1C,

        //FIFO buffer
        fifo_en      = 0x23,
        
        //Master and slaves
        i2c_mst_ctrl     = 0x24,
        i2c_slv0_addr    = 0x25,
        i2c_slv0_reg     = 0x26,
        i2c_slv0_ctrl    = 0x27,
        i2c_slv1_addr    = 0x28,
        i2c_slv1_reg     = 0x29,
        i2c_slv1_ctrl    = 0x2A,
        i2c_slv2_addr    = 0x2B,
        i2c_slv2_reg     = 0x2C,
        i2c_slv2_ctrl    = 0x2D,
        i2c_slv3_addr    = 0x2E,
        i2c_slv3_reg     = 0x2F,
        i2c_slv3_ctrl    = 0x30,
        i2c_slv4_addr    = 0x31,
        i2c_slv4_reg     = 0x32,
        i2c_slv4_do      = 0x33,
        i2c_slv4_ctrl    = 0x34,
        i2c_slv4_di      = 0x35,
        i2c_mst_status   = 0x36,
        
        //Interrupt signals
        int_pin_cfg  = 0x37,
        int_enable   = 0x38,
        int_status   = 0x3A,
        
        //Accelerometer outputs
        accel_xout_h = 0x3B,
        accel_xout_l = 0x3C,
        accel_yout_h = 0x3D,
        accel_yout_l = 0x3E,
        accel_zout_h = 0x3F,
        accel_zout_l = 0x40,
        
        //Temperature outputs
        temp_out_h   = 0x41,
        temp_out_l   = 0x42,
        
        //Gyroscope outputs
        gyro_xout_h  = 0x43,
        gyro_xout_l  = 0x44,
        gyro_yout_h  = 0x45,
        gyro_yout_l  = 0x46,
        gyro_zout_h  = 0x47,
        gyro_zout_l  = 0x48,
        
        //Clock
        clock_internal   = 0x00,
        clock_pll_xgyro  = 0x01,
        clock_pll_ygyro  = 0x02,
        clock_pll_zgyro  = 0x03,
        clock_pll_ext32k = 0x04,
        clock_pll_ext19m = 0x05,
        clock_keep_reset = 0x07,

        //Data external sensors
        ext_sens_data_00 = 0x49,
        ext_sens_data_01 = 0x4A,
        ext_sens_data_02 = 0x4B,
        ext_sens_data_03 = 0x4C,
        ext_sens_data_04 = 0x4D,
        ext_sens_data_05 = 0x4E,
        ext_sens_data_06 = 0x4F,
        ext_sens_data_07 = 0x50,
        ext_sens_data_08 = 0x51,
        ext_sens_data_09 = 0x52,
        ext_sens_data_10 = 0x53,
        ext_sens_data_11 = 0x54,
        ext_sens_data_12 = 0x55,
        ext_sens_data_13 = 0x56,
        ext_sens_data_14 = 0x57,
        ext_sens_data_15 = 0x58,
        ext_sens_data_16 = 0x59,
        ext_sens_data_17 = 0x5A,
        ext_sens_data_18 = 0x5B,
        ext_sens_data_19 = 0x5C,
        ext_sens_data_20 = 0x5D,
        ext_sens_data_21 = 0x5E,
        ext_sens_data_22 = 0x5F,
        ext_sens_data_23 = 0x60,
        
        //Slave outputs
        i2c_slv0_do  = 0x63,
        i2c_slv1_do  = 0x64,
        i2c_slv2_do  = 0x65,
        i2c_slv3_do  = 0x66,
        
        //Master delay control
        i2c_mst_delay_ctrl  = 0x67,
        
        //Reset the analog and ditital path
        signal_path_reset   = 0x68,
        
        //Enable and disable various settings
        user_ctrl = 0x6A,
        
        //Power management options
        pwr_mgmt_1 = 0x6B,
        pwr_mgmt_2 = 0x6C,
        
        //Sample counter
        fifo_count_h = 0x72,
        fifo_count_l = 0x73,
        fifo_r_w     = 0x74,
        
        //Identity
        who_am_i   = 0x75
    };
    
    /// \brief
    /// MPU 6050 constructor
    /// \details
    /// Using hwlib::i2c_bus_bit_banged_scl_sda setting up the MPU 6050 for communication.
    mpu6050(hwlib::i2c_bus_bit_banged_scl_sda bus):bus(bus){}
    
    /// \brief
    /// Enable MPU 6050
    /// \details
    /// This function enables the MPU 6050 by writing a high value to the power management register.
    void start();
    
    /// \brief
    /// Veryfy the identity of the device
    /// \details
    /// The contents of whoami() are the upper 6 bits of the MPU-6050’s 7-bit I2C address.
    /// The result should be 0x64 also 104.
    int8_t whoami();
    
    /// \brief
    /// Calibrate MPU 6050 Accelerator
    /// \details
    /// This function will start calibrating the MPU 6050 to get its current accelerometer state and set it to 0 as a starting point on all axis.
    /// While the device is running this calibration, do not move the MPU 6050.
    /// It will do 50 measurements in 5 seconds.
    /// The result of this function will be an offset that is used in the get() functions.
    void calibrate_accel();
    
    /// \brief
    /// Display loading segment on i2c display
    /// \details
    /// This is a loading segment that is used in calibrate_accel_loading(). 
    /// x and y for its position (starting from the left upper corner), and screen for the i2c display you want to display the segment on.
    void load_segment(int x, int y, hwlib::glcd_oled & screen);
    
    /// \brief
    /// Calibrate MPU 6050 with visuals
    /// \details
    /// This function has the exact same functionality as calibrate_accel(), but it comes with a visual representation of a loading bar.
    /// The loading bar consists of 50 segments, every measurement a segment will be added to the loading bar and when the calibration is complete the loading bar will be full.
    void calibrate_accel_loading(hwlib::glcd_oled & screen);
    
    /// \brief
    /// Calibrate MPU 6050 Gyroscope
    /// \details
    /// This function will start calibrating the MPU 6050 gyroscope to get its current state and set it to 0 as a starting point on all axis.
    /// While the device is running this calibration, do not move the MPU 6050.
    /// It will do 50 measurements in 5 seconds.
    /// The result of this function will be an offset that is used in the get() functions.
    void calibrate_gyro();
    
    /// \brief
    /// Read 8 bit from register
    /// \details
    /// This function will return an 8 bit value read from the given register.
    int8_t read_8_bit(const reg & r);
    
    /// \brief
    /// Read two 8 bit from register
    /// \details
    /// This function will return an 16 bit register from two given adresses, using read_8_bit() and converting the two 8 bit values into a 16 bit value.
    int16_t read_16_bit(const reg & h, const reg & l);
    
    /// \brief 
    /// Get accelerator x value
    /// \details
    /// 16-bit 2's complement value.
    /// Returns the most recent stored X axis accelerometer measurement.
    int16_t get_accel_x();
    
    /// \brief 
    /// Get accelerator y value
    /// \details
    /// 16-bit 2's complement value.
    /// Returns the most recent stored Y axis accelerometer measurement.
    int16_t get_accel_y();
    
    /// \brief 
    /// Get accelerator x value (only positive)
    /// \details
    /// 16-bit 2's complement value.
    /// Returns the most recent stored X axis accelerometer measurement.
    /// Convert value to always return the value positive.
    int16_t get_accel_x_positive();
    
    /// \brief 
    /// Get accelerator y value (only positive)
    /// \details
    /// 16-bit 2's complement value.
    /// Returns the most recent stored Y axis accelerometer measurement.
    /// Convert value to always return the value positive.
    int16_t get_accel_y_positive();
    
    /// \brief 
    /// Get accelerator z value
    /// \details
    /// 16-bit 2's complement value.
    /// Returns the most recent stored Z axis accelerometer measurement.
    int16_t get_accel_z();
    
    /// \brief 
    /// Get gyroscope x value
    /// \details
    /// 16-bit 2's complement value.
    /// Returns the most recent stored X axis gyroscope measurement.
    int16_t get_gyro_x();
    
    /// \brief 
    /// Get gyroscope y value
    /// \details
    /// 16-bit 2's complement value.
    /// Returns the most recent stored Y axis gyroscope measurement.
    int16_t get_gyro_y();
    
    /// \brief 
    /// Get gyroscope z value
    /// \details
    /// 16-bit 2's complement value.
    /// Returns the most recent stored Z axis gyroscope measurement.
    int16_t get_gyro_z();
    
    /// \brief
    /// Get accelarator x state
    /// \details
    /// This function returns a value -1, 0 or 1. 
    /// These values represent if the x-axis is not tilted, tilted one way or the other way.
    int get_accel_x_state(int sensitivity);
    
    /// \brief
    /// Get accelerator y state
    /// \details
    /// This function returns a value -1, 0 or 1. 
    /// These values represent if the x-axis is not tilted, tilted one way or the other way.
    int get_accel_y_state(int sensitivity);
};

#endif // MPU6050_HPP
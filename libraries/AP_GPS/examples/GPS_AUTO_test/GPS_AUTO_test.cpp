/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
     Test for AP_GPS_AUTO
*/

#include <AP_HAL/AP_HAL.h>
#include <AP_GPS/AP_GPS.h>
#include <GCS_MAVLink/GCS_Dummy.h>
#include <AP_Notify/AP_Notify.h>
#include <AP_Notify/AP_BoardLED.h>
#include <AP_SerialManager/AP_SerialManager.h>
#include <AP_BoardConfig/AP_BoardConfig.h>

// 此文件最后AP_HAL_MAIN里，会调用setup和loop函数
void setup();// 大多数库中都定义了此函数。此函数在启动时只调用一次。HAL中的main（）函数调用此函数。 
void loop();// 大多数库中都定义了此函数。该函数由HAL中的main函数调用。主要功能实现。 

const AP_HAL::HAL& hal = AP_HAL::get_HAL();// 声明“hal”引用变量。这个变量指向Ap_HAL:：HAL类的对象。这里，AP_HAL是库，HAL是库中的一个类。此参考变量可用于访问特定于硬件的功能 

static AP_BoardConfig board_config;

// create board led object
AP_BoardLED board_led;

// create fake gcs object
GCS_Dummy _gcs;

const AP_Param::GroupInfo GCS_MAVLINK_Parameters::var_info[] = {
        AP_GROUPEND
};

// This example uses GPS system. Create it.
static AP_GPS gps;
// Serial manager is needed for UART communications
static AP_SerialManager serial_manager;

//to be called only once on boot for initializing objects
void setup()
{
    hal.console->printf("GPS AUTO library test\n");

    board_config.init();

    // Initialise the leds
    board_led.init();

    // Initialize the UART for GPS system
    serial_manager.init();
    gps.init(serial_manager);
}


/*
  print a int32_t lat/long in decimal degrees 以十进制度数打印int32_t lat/long 
 */
void print_latlon(AP_HAL::BetterStream *s, int32_t lat_or_lon);
void print_latlon(AP_HAL::BetterStream *s, int32_t lat_or_lon)
{
    int32_t dec_portion, frac_portion;
    int32_t abs_lat_or_lon = labs(lat_or_lon); //返回长整型参数n的绝对值 

    // extract decimal portion (special handling of negative numbers to ensure we round towards zero) 提取整数部分（对负数进行特殊处理，以确保我们向零进位） 
    dec_portion = abs_lat_or_lon / 10000000UL; //取整

    // extract fractional portion
    frac_portion = abs_lat_or_lon - dec_portion*10000000UL; //取余

    // print output including the minus si gn 负数处理
    if( lat_or_lon < 0 ) {
        s->printf("-");
    }
    s->printf("%ld.%07ld",(long)dec_portion,(long)frac_portion);
}

// loop
void loop()
{
    static uint32_t last_msg_ms;

    // Update GPS state based on possible bytes received from the module.
    gps.update();

    // If new GPS data is received, output it's contents to the console
    // Here we rely on the time of the message in GPS class and the time of last message
    // saved in static variable last_msg_ms. When new message is received, the time
    // in GPS class will be updated.
    if (last_msg_ms != gps.last_message_time_ms()) {
        // Reset the time of message
        last_msg_ms = gps.last_message_time_ms();

        // Acquire location
        const Location &loc = gps.location();

        // Print the contents of message
        hal.console->printf("Lat: ");
        print_latlon(hal.console, loc.lat);
        hal.console->printf(" Lon: ");
        print_latlon(hal.console, loc.lng);
        hal.console->printf(" Alt: %.2fm GSP: %.2fm/s CoG: %d SAT: %d TIM: %u/%lu STATUS: %u\n",
                            (double)(loc.alt * 0.01f),
                            (double)gps.ground_speed(),
                            (int)gps.ground_course(),
                            gps.num_sats(),
                            gps.time_week(),
                            (long unsigned int)gps.time_week_ms(),
                            gps.status());
    }

    // Delay for 10 mS will give us 100 Hz invocation rate
    hal.scheduler->delay(10);
}

// Register above functions in HAL board level每一个sketch最底部，都有一个“AP_HAL_MAIN();”指令，它是一个HAL宏，用于定义一个C++ main函数，整个程序的入口
AP_HAL_MAIN();

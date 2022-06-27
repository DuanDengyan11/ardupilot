#include <AP_HAL/AP_HAL.h>

void setup();    //declaration of the setup() function
void loop();     //declaration of the loop() function

const AP_HAL::HAL& hal = AP_HAL::get_HAL();    //create a reference to AP_HAL::HAL object to get access to hardware specific functions. For more info see <https://ardupilot.org/dev/docs/learning-ardupilot-the-example-sketches.html/>  

AP_HAL::PWMSource* chan;    //delare a pointer to AnalogSource object. AnalogSource class can be found in : AP_HAL->AnalogIn.h

void setup(void) {
    hal.console->printf("Starting AP_HAL::gpio pwm test\r\n");    //print a starting message
}

int16_t pin = 50;    //8 bit integer to hold the pin number.Pin number range is [0,15]

void loop(void) {
    hal.console->printf("Starting AP_HAL::gpio pwm test\r\n"); 
    if(chan-> set_pin(pin,"tutu_ceshi"))
    {

        uint16_t pwm = chan->get_pwm_avg_us();
        uint16_t fre_hz = chan->get_fre_avg_hz();
        hal.console->printf("[%u %u %u] \n",
              (unsigned)pin, pwm, fre_hz);
    };    //note:the voltage value is divided into 1024 segments    
    hal.scheduler->delay(100);
    // hal.gpio->valid_pin(50);
}

AP_HAL_MAIN(); //HAL Macro that declares the main function. For more info see <https://ardupilot.org/dev/docs/learning-ardupilot-the-example-sketches.html/>

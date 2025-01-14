#include "GPIO.h"

#include <AP_HAL/AP_HAL.h>

#ifndef HAL_BOOTLOADER_BUILD
#include <GCS_MAVLink/GCS.h>
#endif

#ifndef GCS_SEND_TEXT
#define GCS_SEND_TEXT(severity, format, args...)
#endif

extern const AP_HAL::HAL& hal;

bool AP_HAL::PWMSource::set_pin(int16_t new_pin, const char *subsystem)
{
    if (new_pin == _pin) {
        // we've already tried to attach to this pin  我们已经尝试连接到此pin 
        return interrupt_attached;
    }

    if (interrupt_attached) {  //已连接中断
        if (!hal.gpio->detach_interrupt(_pin)) {
            GCS_SEND_TEXT(MAV_SEVERITY_WARNING,
                          "%s: Failed to detach interrupt from %d",
                          subsystem,
                          _pin);
        }
        interrupt_attached = false;
    }

    // don't want to try to attach more than once:
    _pin = new_pin;

    if (_pin <= 0) {
        // invalid pin
        return false;
    }

    // install interrupt handler on rising and falling edge  在上升沿和下降沿上安装中断处理程序 
    hal.gpio->pinMode(_pin, HAL_GPIO_INPUT);
    if (!hal.gpio->attach_interrupt(
            _pin,
            FUNCTOR_BIND_MEMBER(&AP_HAL::PWMSource::irq_handler,
                                void,
                                uint8_t,
                                bool,
                                uint32_t),
            AP_HAL::GPIO::INTERRUPT_BOTH)) {
        // failed to attach interrupt
        GCS_SEND_TEXT(MAV_SEVERITY_WARNING,
                      "%s: Failed to attach interrupt to %d",
                      subsystem,
                      _pin);
        return false;
    }

    interrupt_attached = true;

    return interrupt_attached;
}

// interrupt handler for reading pwm value 用于读取pwm值的中断处理程序 
void AP_HAL::PWMSource::irq_handler(uint8_t a_pin, bool pin_high, uint32_t timestamp_us)
{
    if (pin_high) {
        fre_hz = 1.0/(timestamp_us - _pulse_start_us) * 1000000;
        fre_hz_sum = fre_hz_sum + fre_hz;
        fre_hz_count++;
        _pulse_start_us = timestamp_us;
        return;
    }

    // if (_pulse_start_us == 0) {
    //     // if we miss interrupts we could get two lows in a row.  If
    //     // we miss interrupts we're definitely handing back bad
    //     // values anyway....
    //     return;
    // }
    _irq_value_us = timestamp_us - _pulse_start_us; //记录从上升沿到下降沿的时间
    // _pulse_start_us = 0;

    // update fields for taking an average reading:
    _irq_value_us_sum += _irq_value_us;
    _irq_value_us_count++;
}


uint16_t AP_HAL::PWMSource::get_pwm_us()
{
    // disable interrupts and grab state 禁止中断并抓取状态
    void *irqstate = hal.scheduler->disable_interrupts_save();
    const uint32_t ret = _irq_value_us;
    _irq_value_us = 0;
    hal.scheduler->restore_interrupts(irqstate);

    return ret;
}

uint16_t AP_HAL::PWMSource::get_fre_hz()
{
    // disable interrupts and grab state 禁止中断并抓取状态
    void *irqstate = hal.scheduler->disable_interrupts_save();
    const uint32_t ret = fre_hz;
    fre_hz = 0;
    hal.scheduler->restore_interrupts(irqstate);

    return ret;
}

uint16_t AP_HAL::PWMSource::get_fre_avg_hz()
{
    // disable interrupts and grab state
    void *irqstate = hal.scheduler->disable_interrupts_save();
    uint32_t ret;
    if (fre_hz_count == 0) {
        ret = 0;
    } else {
        ret = fre_hz_sum / fre_hz_count;
    }
    fre_hz_sum = 0;
    fre_hz_count = 0;
    hal.scheduler->restore_interrupts(irqstate);

    return ret;
}


uint16_t AP_HAL::PWMSource::get_pwm_avg_us()
{
    // disable interrupts and grab state
    void *irqstate = hal.scheduler->disable_interrupts_save();
    uint32_t ret;
    if (_irq_value_us_count == 0) {
        ret = 0;
    } else {
        ret = _irq_value_us_sum / _irq_value_us_count;
    }
    _irq_value_us_sum = 0;
    _irq_value_us_count = 0;
    hal.scheduler->restore_interrupts(irqstate);

    return ret;
}

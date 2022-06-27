#pragma once

#include "string.h"
#include "utility/functor.h"
// namespace 名称空间
namespace AP_HAL {

    /* Toplevel pure virtual class Hal. 顶层纯虚拟类hal*/
    class HAL;

    /* Toplevel class names for drivers:  驱动程序的顶级类名*/
    class UARTDriver;
    class I2CDevice;
    class I2CDeviceManager;
    class Device;

    class SPIDevice;
    class SPIDeviceDriver;
    class SPIDeviceManager;

    class AnalogSource;
    class AnalogIn;
    class Storage;
    class DigitalSource;
    class PWMSource;
    class GPIO;
    class RCInput;
    class RCOutput;
    class Scheduler;
    class EventHandle;
    class EventSource;
    class Semaphore;
    class OpticalFlow;
    class DSP;

    class QSPIDevice;
    class QSPIDeviceDriver;
    class QSPIDeviceManager;

    class CANIface;
    class CANFrame;

    class Util;
    class Flash;

    /* Utility Classes 实用程序类例*/
    class Print;
    class Stream;
    class BetterStream;

    /* Typdefs for function pointers (Procedure, Member Procedure) 对于函数指针（Procedure, member procedure）使用Typdefs

       For member functions we use the FastDelegate delegates class 对于成员函数，我们使用FastDelegate委托类，允许将成员函数封装成一个类型
       which allows us to encapculate a member function as a type
     */
    typedef void(*Proc)(void);
    FUNCTOR_TYPEDEF(MemberProc, void);

    /**
     * Global names for all of the existing SPI devices on all platforms 所有平台上所有现存SPI设备的全局名称.
     */

    enum SPIDeviceType {
        // Devices using AP_HAL::SPIDevice abstraction 设备使用AP_HAL::SPIDevice抽象
        SPIDevice_Type              = -1,
    };

    class SIMState;

    // Must be implemented by the concrete HALs 必须由具体的hals实施.
    const HAL& get_HAL();
}

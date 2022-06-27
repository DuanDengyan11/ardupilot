#pragma once

#include <stdint.h>

#include "AP_HAL_Namespace.h" //AP_HAL名称空间
#include "AP_HAL_Boards.h"
#include "AP_HAL_Macros.h"
#include "AP_HAL_Main.h"

/* HAL Module Classes (all pure virtual) HAL模块类（所有都是纯虚拟） */
#include "UARTDriver.h" //Pure virtual UARTDriver class 纯虚UARTDriver类, 只定义接口, 在具体的四大系统软硬件平台中进行实现 
#include "AnalogIn.h" //代表模拟输入器件，比如声纳等，fanout一定范围内的电压值, 只定义接口，在具体的四大系统软硬件平台中进行实现 
#include "Storage.h" //储存类块设备, 比如sd等, 只定义接口，在具体的四大系统软硬件平台中进行实现
#include "GPIO.h" //GPIO, 及其对应的中断触发方式, 只定义接口，在具体的四大系统软硬件平台中进行实现
#include "RCInput.h" //接收机输入, 只定义接口，在具体的四大系统软硬件平台中进行实现 
#include "RCOutput.h" //接收机输出, 只定义接口，在具体的四大系统软硬件平台中进行实现 
#include "Scheduler.h" //调度器, 提供线程创建等功能 只定义接口，在具体的四大系统软硬件平台中进行实现
#include "Semaphores.h" //信号量相关操作, 只定义接口，在具体的四大系统软硬件平台中进行实现 
#include "EventHandle.h"
#include "Util.h" //一些辅助性，配置性，工具 比如log存储位置，snprintf等 只定义接口，在具体的四大系统软硬件平台中进行实现
#include "OpticalFlow.h" //光流，chibios、linux平台没有实现
#include "Flash.h" //flash 页、块操作相关接口 只定义接口，在具体的四大系统软硬件平台中进行实现
#include "DSP.h"

#include "CANIface.h" //定义can总线协议需要实现的相关接口 只定义接口，在具体的四大系统软硬件平台中进行实现

#include "utility/BetterStream.h" //流操作相关接口printf,write,read..., UARTDriver继承了此接口类

/* HAL Class definition 包含了当前目录下的HAL.h, 主要定义了 AP_HAL::HAL 接口类,聚合了系统硬件的几乎所有接口，比如
 * uart, spi, i2c, scheduler等, 该类在四大系统软硬件平台中进行实现 */
#include "HAL.h"

#include "system.h"

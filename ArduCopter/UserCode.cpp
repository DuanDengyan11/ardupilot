#include "Copter.h"

#ifdef USERHOOK_INIT
void Copter::userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
}
#endif

#ifdef USERHOOK_FASTLOOP
void Copter::userhook_FastLoop()
{
    // put your 100Hz code here
}
#endif

#ifdef USERHOOK_50HZLOOP
void Copter::userhook_50Hz()
{

    Servo_485 servo[6];

    uint8_t rx[10];

    for(int i = 0;i<6;i++)
    {

        switch(i)
        {
        case 0:
            servo[i].SetAddr(0XA1);
            break;
        case 1:
            servo[i].SetAddr(0XA2);
            break;
        case 2:
            servo[i].SetAddr(0XA3);
            break;
        case 3:
            servo[i].SetAddr(0XA4);
            break;
        case 4:
            servo[i].SetAddr(0XA5);
            break;
        case 5:
            servo[i].SetAddr(0XA6);
            break;
        default:
            break;
        }

        //读取通道舵机飞控输出pwm值
        servo[i].GetPwm(SRV_Channels::srv_channel(i)->get_output_pwm()); 
        //将pwm值转换成485帧
        servo[i].pwm2rs();
        //整理帧信号并发送位置命令
        hal.serial(4)->write(servo[i].SetFrame(),10); 
        //发送读取位置命令等待接受
        hal.serial(4)->write(servo[i].ReadOrder(),10);
        //读取该通道舵机回传角度值
        if(hal.serial(4)->available())
        {
            hal.serial(4)->read(rx,10);
        }
        
        // //GCS_SEND_TEXT(MAV_SEVERITY_INFO, "%u,%u",rx[0],rx[9]);

        // //设置舵机回传角度值
        servo[i].SetRxFrame(rx);
        
        // //rx[0] = 1;

    }

    GCS_SEND_TEXT(MAV_SEVERITY_INFO, "%u,%u",rx[0],rx[9]);
    // //if(hal.serial(4)->available())
    // //{
    //     //uint8_t *rx = NULL;
    //     //hal.serial(4)->read(rx,2);
    //     //hal.serial(4)->write(,3); 
    //     //hal.serial(4)->write(data8,2);       
    // //}
    // //log_ServoAngle(servo);

}
#endif

void Servo_485::pwm2rs()
{
    desire_angle = (this->servo_pwm-1500)*2;  

    //低八位
    this->angle_frame[0] = (uint8_t)(desire_angle  & 0xFF);
    //高八位
    this->angle_frame[1] = (uint8_t)(desire_angle >> 8 & 0xFF);
}

uint8_t* Servo_485::SetFrame()
{
    tx_frame[0] = frame_start;
    tx_frame[1] = frame_ver;
    tx_frame[2] = frame_addr;
    tx_frame[3] = 0x09;
    tx_frame[4] = angle_frame[0];
    tx_frame[5] = angle_frame[1];
    tx_frame[6] = 0x00;
    tx_frame[7] = 0x00;
    //校验帧
    tx_frame[8] = (uint8_t)((tx_frame[1]+ tx_frame[2]+ tx_frame[3]+tx_frame[4]+tx_frame[5]+tx_frame[6]+tx_frame[7])&0xFF);
    tx_frame[9] = frame_end;
    return tx_frame;
}

void Servo_485::SetRxFrame(uint8_t* rx)
{
    for(int i = 0 ; i<10 ; i++)
    {
        rx_frame[i] = rx[i];
   }
    // actual_angle = (int16_t)(rx_frame[5]<<8)+(int16_t)rx_frame[4];
  
}

uint8_t* Servo_485::ReadOrder()
{
    rd_frame[0] = frame_start;
    rd_frame[1] = frame_ver;
    rd_frame[2] = frame_addr;
    rd_frame[3] = 0x19;
    rd_frame[4] = 0x00;
    rd_frame[5] = 0x00;
    rd_frame[6] = 0x00;
    rd_frame[7] = 0x00;
    //校验帧
    rd_frame[8] = (uint8_t)((rd_frame[1]+ rd_frame[2]+ rd_frame[3]+rd_frame[4]+rd_frame[5]+rd_frame[6]+rd_frame[7])&0xFF);
    rd_frame[9] = frame_end;
    return rd_frame;
}


uint16_t Copter::U16tU8(uint16_t data16,uint8_t *data8)
{
    *data8 = (uint8_t)(data16 >> 8 & 0x0F);
    *(data8+1)=(uint8_t)(data16 & 0x0F);
    return *data8;
}

#ifdef USERHOOK_MEDIUMLOOP
void Copter::userhook_MediumLoop()
{
    // put your 10Hz code here
}
#endif

#ifdef USERHOOK_SLOWLOOP
void Copter::userhook_SlowLoop()
{
    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void Copter::userhook_SuperSlowLoop()
{
    // put your 1Hz code here
}
#endif

#ifdef USERHOOK_AUXSWITCH
void Copter::userhook_auxSwitch1(const RC_Channel::AuxSwitchPos ch_flag)
{
    // put your aux switch #1 handler here (CHx_OPT = 47)
}

void Copter::userhook_auxSwitch2(const RC_Channel::AuxSwitchPos ch_flag)
{
    // put your aux switch #2 handler here (CHx_OPT = 48)
}

void Copter::userhook_auxSwitch3(const RC_Channel::AuxSwitchPos ch_flag)
{
    // put your aux switch #3 handler here (CHx_OPT = 49)
}
#endif

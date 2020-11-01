#ifndef __BMI_H_
#define	__BMI_H_

#include "stm32f10x.h"
  

#define   BMI_160_SLAVE_ADDRESS        (0X69<<1)

#define   BMI_160_ID                   0x00
#define   BMI_160_ERR_REG              0x02
#define   BMI_160_PMU_STAUS            0x03
#define   BMI_160_MAG_X_L              0x04
#define   BMI_160_MAG_X_H              0x05
#define   BMI_160_MAG_Y_L              0x06
#define   BMI_160_MAG_Y_H              0x07
#define   BMI_160_MAG_Z_L              0x08
#define   BMI_160_MAG_Z_H              0x09
#define   BMI_160_RHALL_L              0x0A
#define   BMI_160_RHALL_H              0x0B
#define   BMI_160_GYR_X_L              0x0C
#define   BMI_160_GYR_X_H              0x0D
#define   BMI_160_GYR_Y_L              0x0E
#define   BMI_160_GYR_Y_H              0x0F
#define   BMI_160_GYR_Z_L              0x10
#define   BMI_160_GYR_Z_H              0x11
#define   BMI_160_ACC_X_L              0x12
#define   BMI_160_ACC_X_H              0x13
#define   BMI_160_ACC_Y_L              0x14
#define   BMI_160_ACC_Y_H              0x15
#define   BMI_160_ACC_Z_L              0x16
#define   BMI_160_ACC_Z_H              0x17
#define   BMI_160_SENSORTIME_0         0x18
#define   BMI_160_SENSORTIME_1         0x19
#define   BMI_160_SENSORTIME_2         0x1A
#define   BMI_160_STATUS               0x1B
#define   BMI_160_STATUS_0             0x1C
#define   BMI_160_STATUS_1             0x1D
#define   BMI_160_STATUS_2             0x1E
#define   BMI_160_STATUS_3             0x1F
#define   BMI_160_TEMPRATURE_0         0x20
#define   BMI_160_TEMPRATURE_1         0x21
#define   BMI_160_FIFO_LENGTH_0        0x22
#define   BMI_160_FIFO_LENGTH_1        0x23
#define   BMI_160_FIFO_DATA            0x24
#define   BMI_160_ACC_CONF             0x40
#define   BMI_160_ACC_RANGE            0x41
#define   BMI_160_GYR_CONF             0x42
#define   BMI_160_GYR_RANGE            0x43
#define   BMI_160_MAG_CONF             0x44
#define   BMI_160_FIFO_DOWNs           0x45
#define   BMI_160_FIFO_CONFIG_0        0x46
#define   BMI_160_FIFO_CONFIG_1        0x47
#define   BMI_160_MAG_IF_0             0x4B
#define   BMI_160_MAG_IF_1             0x4C
#define   BMI_160_MAG_IF_2             0x4D
#define   BMI_160_MAG_IF_3             0x4E
#define   BMI_160_MAG_IF_4             0x4F
#define   BMI_160_INT_EN_0             0x50
#define   BMI_160_INT_EN_1             0x51
#define   BMI_160_INT_EN_2             0x52
#define   BMI_160_INT_OUT_CTRL         0x53
#define   BMI_160_INT_LATCH            0x54
#define   BMI_160_INT_MAP_0            0x55
#define   BMI_160_INT_MAP_1            0x56
#define   BMI_160_INT_MAP_2            0x57
#define   BMI_160_INT_DATA_0           0x58
#define   BMI_160_INT_DATA_1           0x59
#define   BMI_160_INT_LOWHIGH_0        0x5A
#define   BMI_160_INT_LOWHIGH_1        0x5B
#define   BMI_160_INT_LOWHIGH_2        0x5C
#define   BMI_160_INT_LOWHIGH_3        0x5D
#define   BMI_160_INT_LOWHIGH_4        0x5E
#define   BMI_160_INT_MOTION_0         0x5F
#define   BMI_160_INT_MOTION_1         0x60
#define   BMI_160_INT_MOTION_2         0x61
#define   BMI_160_INT_MOTION_3         0x62
#define   BMI_160_INT_TAP_0            0x63
#define   BMI_160_INT_TAP_1            0x64
#define   BMI_160_INT_ORIENT_0         0x65
#define   BMI_160_INT_ORIENT_1         0x66
#define   BMI_160_INT_FLAT_0           0x67
#define   BMI_160_INT_FLAT_1           0x68
#define   BMI_160_FOC_CONF             0x69
#define   BMI_160_CONF                 0x6A
#define   BMI_160_IF_CONF              0x6B
#define   BMI_160_PMU_TRIGGER          0x6C
#define   BMI_160_SELF_TEST            0x6D
#define   BMI_160_NV_CONF              0x70
#define   BMI_160_OFFSET_0             0x71
#define   BMI_160_OFFSET_1             0x72
#define   BMI_160_OFFSET_2             0x73
#define   BMI_160_OFFSET_3             0x74
#define   BMI_160_OFFSET_4             0x75
#define   BMI_160_OFFSET_5             0x76
#define   BMI_160_OFFSET_6             0x77
#define   BMI_160_STEP_CNT_0           0x78
#define   BMI_160_STEP_CNT_1           0x79
#define   BMI_160_STEP_CONF_0          0x7A
#define   BMI_160_STEP_CONF_1          0x7B
#define   BMI_160_CMD                  0x7E

#define   ACC_NORMAL_WORK_MODE         0x11
#define   GYR_NORMAL_WORK_MODE         0x15
#define   WORK_TATUS                   0x3C



/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/*信息输出*/
#define MPU_DEBUG_ON         1

#define MPU_INFO(fmt,arg...)           printf("<<-MPU-INFO->> "fmt"\n",##arg)
#define MPU_ERROR(fmt,arg...)          printf("<<-MPU-ERROR->> "fmt"\n",##arg)
#define MPU_DEBUG(fmt,arg...)          do{\
                                          if(MPU_DEBUG_ON)\
                                          printf("<<-MPU-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)




extern uint8_t BMI_160_Gyro_data[3];
extern uint8_t BMI_160_ACC_data[3];
extern float   BMI_160_ANGLE[3];
extern float   BMI_160_GRAVITY[3];


extern  void BMI_160_IIC_INIT(void);
extern  void BIM_160_Init(void);
extern  uint8_t BMI_160_WriteReg(uint8_t Reg_Adress, uint8_t Red_Data);
extern  void Read_BMI_160_Gyro_data(void);
extern  void Read_BMI_160_ACC_data(void);
extern  void Get_BMI_160_Data(void);
extern  void Sent_BMI_160_USART(void);
extern  uint8_t BMI_160_ReadID(void);
extern  uint8_t BMI_160_ReadRegData(uint8_t* pBuffer, uint8_t Reg_Adress, uint16_t NumByteToRead);
extern  void BMI_160_ReadData(u8 reg_add,unsigned char*Read,u8 num);
extern  void WriteReg(u8 reg_add,u8 reg_dat);
extern  void ReadData(u8 reg_add,unsigned char* Read,u8 num);





#endif


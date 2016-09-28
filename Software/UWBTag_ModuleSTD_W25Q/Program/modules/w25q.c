/*====================================================================================================*/
/*====================================================================================================*/
#include "drivers\stm32f4_system.h"
#include "drivers\stm32f4_spi.h"
#include "modules\w25q.h"
/*====================================================================================================*/
/*====================================================================================================*/
#define W25Q_SPIx                 SPI2
#define W25Q_SPIx_CLK_ENABLE()    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE)

#define W25Q_SCK_PIN              GPIO_Pin_13
#define W25Q_SCK_GPIO_PORT        GPIOB
#define W25Q_SCK_AF               GPIO_AF_SPI2
#define W25Q_SCK_SOURCE           GPIO_PinSource13

#define W25Q_SDO_PIN              GPIO_Pin_14
#define W25Q_SDO_GPIO_PORT        GPIOB
#define W25Q_SDO_AF               GPIO_AF_SPI2
#define W25Q_SDO_SOURCE           GPIO_PinSource14

#define W25Q_SDI_PIN              GPIO_Pin_15
#define W25Q_SDI_GPIO_PORT        GPIOB
#define W25Q_SDI_AF               GPIO_AF_SPI2
#define W25Q_SDI_SOURCE           GPIO_PinSource15

#define W25Q_CSF_PIN              GPIO_Pin_1
#define W25Q_CSF_GPIO_PORT        GPIOB
#define W25Q_CSF_H()              __GPIO_SET(W25Q_CSF_GPIO_PORT, W25Q_CSF_PIN)
#define W25Q_CSF_L()              __GPIO_RST(W25Q_CSF_GPIO_PORT, W25Q_CSF_PIN)

#define W25Q_WP_PIN              GPIO_Pin_12
#define W25Q_WP_GPIO_PORT        GPIOB
#define W25Q_WP_H()              __GPIO_SET(W25Q_WP_GPIO_PORT, W25Q_WP_PIN)
#define W25Q_WP_L()              __GPIO_RST(W25Q_WP_GPIO_PORT, W25Q_WP_PIN)

#define W25Q_HOD_PIN              GPIO_Pin_8
#define W25Q_HOD_GPIO_PORT        GPIOA
#define W25Q_HOD_H()              __GPIO_SET(W25Q_HOD_GPIO_PORT, W25Q_HOD_PIN)
#define W25Q_HOD_L()              __GPIO_RST(W25Q_HOD_GPIO_PORT, W25Q_HOD_PIN)
/*====================================================================================================*/
/*====================================================================================================*
**函數 : W25Q_GetDeviceID
**功能 : Read Device ID
**輸入 : None
**輸出 : deviceID
**使用 : deviceID = W25Q_GetDeviceID();
**====================================================================================================*/
/*====================================================================================================*/
uint16_t W25Q_GetDeviceID( void )
{
  uint16_t deviceID = 0;

  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, 0x90);
  SPI_RW(W25Q_SPIx, 0x00);
  SPI_RW(W25Q_SPIx, 0x00);
  SPI_RW(W25Q_SPIx, 0x00);
  deviceID |= SPI_RW(W25Q_SPIx, 0xFF) << 8;
  deviceID |= SPI_RW(W25Q_SPIx, 0xFF);
  W25Q_CSF_H();

  return deviceID;
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : W25Q_Config
**功能 : SPI Config
**輸入 : None
**輸出 : None
**使用 : W25Q_Config();
**====================================================================================================*/
/*====================================================================================================*/
void W25Q_Config( void )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  SPI_InitTypeDef SPI_InitStruct;

  /* SPI Clk ******************************************************************/
  W25Q_SPIx_CLK_ENABLE();

  /* SPI AF *******************************************************************/
  GPIO_PinAFConfig(W25Q_SCK_GPIO_PORT, W25Q_SCK_SOURCE, W25Q_SCK_AF);
  GPIO_PinAFConfig(W25Q_SDO_GPIO_PORT, W25Q_SDO_SOURCE, W25Q_SDO_AF);
  GPIO_PinAFConfig(W25Q_SDI_GPIO_PORT, W25Q_SDI_SOURCE, W25Q_SDI_AF);

  /* SPI Pin ******************************************************************/
  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = W25Q_CSF_PIN;
  GPIO_Init(W25Q_CSF_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

  GPIO_InitStruct.GPIO_Pin   = W25Q_SCK_PIN;
  GPIO_Init(W25Q_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = W25Q_SDO_PIN;
  GPIO_Init(W25Q_SDO_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin   = W25Q_SDI_PIN;
  GPIO_Init(W25Q_SDI_GPIO_PORT, &GPIO_InitStruct);

  W25Q_CSF_H();  // LOW ENABLE

  /* SPI Init ****************************************************************/
  SPI_InitStruct.SPI_Mode              = SPI_Mode_Master;
  SPI_InitStruct.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_DataSize          = SPI_DataSize_8b;
  SPI_InitStruct.SPI_CPOL              = SPI_CPOL_High;
  SPI_InitStruct.SPI_CPHA              = SPI_CPHA_2Edge;
  SPI_InitStruct.SPI_NSS               = SPI_NSS_Soft;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStruct.SPI_FirstBit          = SPI_FirstBit_MSB;
  SPI_InitStruct.SPI_CRCPolynomial     = 7;
  SPI_Init(W25Q_SPIx, &SPI_InitStruct);

  SPI_Cmd(W25Q_SPIx, ENABLE); 
}
/*====================================================================================================*/
/*====================================================================================================*
**函數 : W25Q_Init
**功能 : IMU Init
**輸入 : None
**輸出 : None
**使用 : status = W25Q_Init();
**====================================================================================================*/
/*====================================================================================================*/
void W25Q_Init( void )
{
  uint16_t deviceID = 0;

  deviceID = W25Q_GetDeviceID();

  switch(deviceID) {
    case W25Q80_ID:
      break;

    case W25Q16_ID:
      break;

    case W25Q32_ID:
      break;

    case W25Q64_ID:
      break;

    case W25Q128_ID:
      break;

    default:
      break;
  }
}
/*==============================================================================================*/
/*==============================================================================================*/




/*====================================================================================================*/
/*====================================================================================================*
**函數 : MPU9250_getData
**功能 : Get IMU Data
**輸入 : *dataIMU
**輸出 : None
**使用 : MPU9250_getData(dataIMU);
**====================================================================================================*/
/*====================================================================================================*/
//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
uint8_t W25Q_GetStatus( void )   
{
  uint8_t status = 0;

  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_READ_STATUS);
  status = SPI_RW(W25Q_SPIx, 0xFF);
  W25Q_CSF_H();

  return status;
}

//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void W25Q_SetStatus( uint8_t status )   
{
  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_WRITE_STATUS);
  SPI_RW(W25Q_SPIx, status);
  W25Q_CSF_H();
}

//W25QXX写使能
//将WEL置位
void W25Q_WriteCMD( uint8_t state )
{
  W25Q_CSF_L();
  if(state == ENABLE) {
    SPI_RW(W25Q_SPIx, W25Q_CMD_WRITE_ENABLE);
  }
  else {
    SPI_RW(W25Q_SPIx, W25Q_CMD_WRITE_DISABLE);
  }
  W25Q_CSF_H();
}

//等待空闲
void W25Q_WaitBusy( void )   
{   
  while((W25Q_GetStatus() & 0x01) == 0x01);
}

//进入掉电模式
void W25Q_PowerDown( void )
{
  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_POWER_DOWN);
  W25Q_CSF_H();
  delay_ms(1);
}

//唤醒
void W25Q_WakeUp( void )
{
  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_RELEASE_POWER_DOWN);
  W25Q_CSF_H();
  delay_ms(1);
}

//擦除整个芯片		  
//等待时间超长...
void W25Q_EraseChip( void )
{
  W25Q_WriteCMD(ENABLE);
  W25Q_WaitBusy();

  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_CHIP_ERASE);
  W25Q_CSF_H();

  W25Q_WaitBusy();
}

//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void W25Q_EraseSector( uint32_t eraseAddr )   
{
  //监视falsh擦除情况,测试用
  // 	printf("fe:%x\r\n",Dst_Addr);
  eraseAddr *= 4096;

  W25Q_WriteCMD(ENABLE);                  //SET WEL
  W25Q_WaitBusy();

  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_SECTOR_ERASE);
  SPI_RW(W25Q_SPIx, (uint8_t)(eraseAddr >> 16));
  SPI_RW(W25Q_SPIx, (uint8_t)(eraseAddr >> 8));
  SPI_RW(W25Q_SPIx, (uint8_t)eraseAddr);
  W25Q_CSF_H();

  W25Q_WaitBusy();
}

//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25Q_ReadData( uint8_t *readBuf, uint32_t readAddr, uint16_t lens )
{
  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_READ_DATA);
  SPI_RW(W25Q_SPIx, (uint8_t)(readAddr >> 16));
  SPI_RW(W25Q_SPIx, (uint8_t)(readAddr >> 8));
  SPI_RW(W25Q_SPIx, (uint8_t)readAddr);
  for(uint16_t i = 0; i < lens; i++) {
    readBuf[i] = SPI_RW(W25Q_SPIx, 0xFF);
  }
  W25Q_CSF_H();
}

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void W25Q_WritePage( uint8_t *writeBuf, uint32_t writeAddr, uint16_t lens )
{
  W25Q_WriteCMD(ENABLE);
  W25Q_CSF_L();
  SPI_RW(W25Q_SPIx, W25Q_CMD_PAGE_PROGRAM);
  SPI_RW(W25Q_SPIx, (uint8_t)(writeAddr >> 16));
  SPI_RW(W25Q_SPIx, (uint8_t)(writeAddr >> 8));
  SPI_RW(W25Q_SPIx, (uint8_t)writeAddr);
  for(uint16_t i = 0; i < lens; i++) {
    SPI_RW(W25Q_SPIx, writeBuf[i]);
  }
  W25Q_CSF_H();
  W25Q_WaitBusy();
}
/*
//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
}

//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
u8 W25QXX_BUFFER[4096];		 
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25Q_ReadData(W25QXX_BUF, secpos*4096, 4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25Q_EraseSector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}
	};
}
*/
/*==============================================================================================*/
/*==============================================================================================*/



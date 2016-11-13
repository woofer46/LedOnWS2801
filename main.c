#include <stm32f4xx_conf.h>
GPIO_InitTypeDef port;
SPI_InitTypeDef spi;

uint16_t del_count=0;
//void SetLed(uint8_t R, uint8_t G, uint8_t B);
int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	port.GPIO_Mode = GPIO_Mode_AF;
	port.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	port.GPIO_OType = GPIO_OType_PP;
	port.GPIO_Speed = GPIO_Speed_50MHz;
	port.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &port);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	SPI_StructInit(&spi);
	spi.SPI_Direction = SPI_Direction_1Line_Tx ;
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_DataSize = SPI_DataSize_8b;
	spi.SPI_CPOL = SPI_CPOL_Low;
	spi.SPI_CPHA = SPI_CPHA_1Edge;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	spi.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI1, &spi);
	SPI_SSOutputCmd(SPI1,ENABLE); //Set SSOE bit in SPI_CR1 register
	SPI_Cmd(SPI1, ENABLE);

	port.GPIO_Mode = GPIO_Mode_IN;
	port.GPIO_Pin = GPIO_Pin_1;
	//port.GPIO_OType = GPIO_OType_PP;
	port.GPIO_Speed = GPIO_Speed_50MHz;
	port.GPIO_PuPd =  GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &port);

	int i=0;
	int r=0;
	int g=0;
	int b=0;
	int f=0;
	int j=3;
	int tmp=0;
	int br[50]={};
	for(i=0;i<50;i++)
	{
		br[i]=0;
	}
	while(1)
    {
		for (i=0; i<50; i++)
		{
			if(br[i]==1)
			{
				SetLed(190,150,250);
			}
			else
			{
				SetLed(0,0,0);
			}

		}
		if( GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0)
		{

			if (f<50)
			{
				br[f]=1;
				f++;
			}
		}
		else
		{
			SetLed(0,0,0);
			f=0;
			j=0;
			for(i=0;i<50;i++)
			{
				br[i]=0;
			}

		}
		if(f==50)
		{
			br[j]=0;
			br[j-1]=0;
			br[j-2]=1;
			br[j-3]=1;
			j++;
		//delay_ms(200);
		}
		if(j==50)
		{
			j=3;
		}
			delay_ms(4);
    }
}
void SetLed(uint8_t R, uint8_t G, uint8_t B)
{
	while (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == SET);
	while(SPI1->SR & SPI_SR_BSY);
	SPI_I2S_SendData(SPI1, R);//red
	while (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == SET);
	while(SPI1->SR & SPI_SR_BSY);
	SPI_I2S_SendData(SPI1, B);//blue
	while (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == SET);
	while(SPI1->SR & SPI_SR_BSY);
	SPI_I2S_SendData(SPI1, G);//green
}
void delay_ms(uint16_t del_temp)
{
	del_count=del_temp;
	while(del_count)
	{

	}
}
void SysTick_Handler(void) // Таймер 1мс
{
	if(del_count>0)
		del_count--;
}

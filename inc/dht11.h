#ifndef DHT11_H_
#define DHT11_H_

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_0
#include "stdint.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx_hal_gpio.h"
#include "inc/tim.h"
#define OUTPUT 1
#define INPUT 0

struct _dht11_t{
	uint8_t temperature; ///Temperature value
	uint8_t humidty; ///humidity value
};
typedef struct _dht11_t dht11_t;

void delay_us(uint32_t us)
{
  __HAL_TIM_SET_COUNTER(&tim7, 0);
  while (__HAL_TIM_GET_COUNTER(&tim7) < us) {}
//  HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);
}
void DHT11_Start(void)
{
	GPIO_InitTypeDef GPIO_InitDef;
	GPIO_InitDef.Pin = DHT11_PIN;
	GPIO_InitDef.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDef.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(DHT11_PORT, &GPIO_InitDef);

}

uint8_t DHT11_Check_Response (void)
{
	uint8_t Response = 0;
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
	delay_us(10);
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
	delay_us(18);
	GPIO_InitTypeDef GPIO_InitDef;
		GPIO_InitDef.Pin = DHT11_PIN;
		GPIO_InitDef.Mode = GPIO_MODE_INPUT;
		GPIO_InitDef.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(DHT11_PORT, &GPIO_InitDef);
	delay_us(40);
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		delay_us(80);
		if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 1;
				else Response = -1; // 255
	}
	while ((HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)));   // wait for the pin to go low

	return Response;
}
uint8_t DHT11_Read (void)
{
	uint8_t i,j;
	for (j=0;j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for the pin to go high
		delay_us(40);   // wait for 40 us
		if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))   // if the pin is low
		{
			i&= ~(1<<(7-j));   // write 0
		}
		else i|= (1<<(7-j));  // if the pin is high, write 1
		while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));  // wait for the pin to go low
	}
	return i;
}

void set_dht11_gpio_mode(uint8_t pMode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(pMode == OUTPUT)
	{
	  GPIO_InitStruct.Pin = DHT11_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
	}else if(pMode == INPUT)
	{
	  GPIO_InitStruct.Pin = DHT11_PIN;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
	}
}
uint8_t readDHT11(dht11_t *dht)
{
	uint16_t mTime1 = 0, mTime2 = 0, mBit = 0;
	uint8_t humVal = 0, tempVal = 0, parityVal = 0, genParity = 0;
	uint8_t mData[40];

	//start comm
	set_dht11_gpio_mode(OUTPUT);			//set pin direction as input
	HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
	HAL_Delay(18);					//wait 18 ms in Low state
	__disable_irq();	//disable all interupts to do only read dht otherwise miss timer
	HAL_TIM_Base_Start(&tim7);
	set_dht11_gpio_mode(INPUT);
	//check dht answer
	__HAL_TIM_SET_COUNTER(&tim7, 0);				//set timer counter to zero
	while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET){
		if((uint16_t)__HAL_TIM_GET_COUNTER(&tim7) > 500){
			__enable_irq();
			return 0;
		}
	}
	__HAL_TIM_SET_COUNTER(&tim7, 0);
	while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET){
		if((uint16_t)__HAL_TIM_GET_COUNTER(&tim7) > 500){
			__enable_irq();
			return 0;
		}
	}
	mTime1 = (uint16_t)__HAL_TIM_GET_COUNTER(&tim7);
	__HAL_TIM_SET_COUNTER(&tim7, 0);
	while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET){
		if((uint16_t)__HAL_TIM_GET_COUNTER(&tim7) > 500){
			__enable_irq();
			return 0;
		}
	}
	mTime2 = (uint16_t)__HAL_TIM_GET_COUNTER(&tim7);

	//if answer is wrong return
	if(mTime1 < 75 && mTime1 > 85 && mTime2 < 75 && mTime2 > 85)
	{
		__enable_irq();
		return 0;
	}

//	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	for(int j = 0; j < 40; j++)
	{
		__HAL_TIM_SET_COUNTER(&tim7, 0);
		while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET){
			if((uint16_t)__HAL_TIM_GET_COUNTER(&tim7) > 500){
				__enable_irq();
				return 0;
			}

		}
		__HAL_TIM_SET_COUNTER(&tim7, 0);
		while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET){
			if((uint16_t)__HAL_TIM_GET_COUNTER(&tim7) > 500){
				__enable_irq();
				return 0;
			}

		}
		mTime1 = (uint16_t)__HAL_TIM_GET_COUNTER(&tim7);

		//check pass time in high state
		//if pass time 25uS set as LOW
		if(mTime1 > 20 && mTime1 < 30)
		{
			mBit = 0;
		}
		else if(mTime1 > 60 && mTime1 < 80) //if pass time 70 uS set as HIGH
		{
			 mBit = 1;
		}

		//set i th data in data buffer
		mData[j] = mBit;

	}

	HAL_TIM_Base_Stop(&tim7); //stop timer
	__enable_irq(); //enable all interrupts

	//get hum value from data buffer
	for(int i = 0; i < 8; i++)
	{
		humVal += mData[i];
		humVal = humVal << 1;
	}

	//get temp value from data buffer
	for(int i = 16; i < 24; i++)
	{
		tempVal += mData[i];
		tempVal = tempVal << 1;
	}

	//get parity value from data buffer
	for(int i = 32; i < 40; i++)
	{
		parityVal += mData[i];
		parityVal = parityVal << 1;
	}

	parityVal = parityVal >> 1;
	humVal = humVal >> 1;
	tempVal = tempVal >> 1;

	genParity = humVal + tempVal;

//	if(genParity == parityVal)

	dht->temperature = tempVal;
	dht->humidty = humVal;


	return 1;
}

#endif /* DHT11_H_ */

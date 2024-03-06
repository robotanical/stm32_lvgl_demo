#ifndef DHT11_H_
#define DHT11_H_

#define DHT11_PORT GPIOA
#define DHT11_PIN GPIO_PIN_0
#include "stdint.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx_hal_gpio.h"
#include "inc/tim.h"
static uint32_t ms_elapsed = 0;

void delay_us(uint32_t us)
{ms_elapsed = HAL_GetTick();
  __HAL_TIM_SET_COUNTER(&tim7, 0);
  while (__HAL_TIM_GET_COUNTER(&tim7) < us) {}
  ms_elapsed = HAL_GetTick() - ms_elapsed;
//  HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_1);
}
void DHT11_Start(void)
{
	GPIO_InitTypeDef GPIO_InitDef;
	GPIO_InitDef.Pin = DHT11_PIN;
	GPIO_InitDef.Mode = GPIO_MODE_INPUT;
	GPIO_InitDef.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(DHT11_PORT, &GPIO_InitDef);

}

uint8_t DHT11_Check_Response (void)
{
	uint8_t Response = 0;
	delay_us(40);
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		delay_us(80);
		if ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN))) Response = 1;
		else Response = -1;
	}
	while ((HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)));   // wait for the pin to go low

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
#endif /* DHT11_H_ */

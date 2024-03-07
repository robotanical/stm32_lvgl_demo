
#include "stm32f7xx.h"
#include "stm32746g_discovery.h"
#include "lvgl/lvgl.h"
#include "stdint.h"
#include "hal_stm_lvgl/tft/tft.h"
#include "hal_stm_lvgl/touchpad/touchpad.h"
#include "HAL_Driver/Inc/stm32f7xx_hal_gpio.h"

#include "lvgl/demos/lv_demos.h"
#include "lvgl/examples/lv_examples.h"
#include "inc/ui/ui.h"
#include "inc/dht11.h"
#include "inc/tim.h"
#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx_hal_gpio.h"

static void SystemClock_Config(void);
void TIM7_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&tim7);
}

int main(void)
{
    HAL_Init();

    /* Configure the system clock to 216 MHz */
    SystemClock_Config();
	HAL_Delay(40);

	tim7_init();
	HAL_Delay(40);

    /* Enable I-Cache */
    SCB_EnableICache();
    /* Enable D-Cache */
    SCB_EnableDCache();

    lv_init();
    tft_init();
    touchpad_init();
    init_main_screen();
delay_us(10);
    /*config of DHT11 sensor*/
//    DHT11_Start();
//	Presence = DHT11_Check_Response();
//	Rh_byte1 = DHT11_Read();
//	Rh_byte2 = DHT11_Read();
//	Temp_byte1 = DHT11_Read();
//	Temp_byte2 = DHT11_Read();
//	SUM = DHT11_Read();
//
//	TEMP = ((Temp_byte1<<8)|Temp_byte2);
//	RH = ((Rh_byte1<<8)|Rh_byte2);
//
//	Temperature = (float) (TEMP/10.0);
//	Humidity = (float) (RH/10.0);
//dht11_t dht;
//readDHT11(&dht);
//while (dht.humidty == 0)
//{readDHT11(&dht);set_humidity_label_text(dht.humidty);}

    while (1)
    {
        HAL_Delay(5);
        lv_task_handler();
//        readDHT11(&dht);
//        set_humidity_label_text(dht.humidty);
    }
}

static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
//    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_TIM7_CLK_ENABLE();
//    __HAL_RCC_TIM6_CLK_ENABLE();


    /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /*##-1- System Clock Configuration #########################################*/
    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 432;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 8;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Activate the Over-Drive mode */
    HAL_PWREx_EnableOverDrive();

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
//     clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);

}


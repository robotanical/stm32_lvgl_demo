#ifndef TIM_H_
#define TIM_H_

TIM_HandleTypeDef tim7;

void tim7_init()
{
	tim7.Instance = TIM7;
	tim7.Init.Period = 0xFFFF;
	tim7.Init.Prescaler = 216-1;
	tim7.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim7.Init.RepetitionCounter = 0;
	tim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	HAL_TIM_Base_Init(&tim7);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	HAL_TIM_Base_Start_IT(&tim7);
}

#endif /* TIM_H_ */

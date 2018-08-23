#include "hal_variables.h"
#include "debug.h"

typedef struct
{
	uint32_t generator;
	uint32_t battery;
} system_voltage_t;

void adc_config_channel(uint32_t channel_number)
{
	ADC_ChannelConfTypeDef sConfig;
	sConfig.Channel = channel_number;
	sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);
}

uint32_t adc_get_voltage(uint32_t channel, size_t n_meas)
{
	uint64_t ret = 0;
	adc_config_channel(channel);
	HAL_ADC_Start(&hadc);
	for (uint32_t i = 0; i < n_meas; ++i)
	{
		HAL_ADC_PollForConversion(&hadc, 1000);
		ret += HAL_ADC_GetValue(&hadc);
	}
	HAL_ADC_Stop(&hadc);
	uint64_t result = (ret * 3300) / n_meas;
	result /= 4095;
	result *= 8143;
	result /= 1000;
	return result;
}

void adc_update_voltage(system_voltage_t *voltage, size_t n_meas)
{
	voltage->battery = adc_get_voltage(ADC_VBAT, n_meas);
	voltage->generator = adc_get_voltage(ADC_VGEN, n_meas);
}

void gpio_gen_enable(uint8_t enabled)
{
	HAL_GPIO_WritePin(CB_GEN_GPIO_Port, CB_GEN_Pin, enabled);
}

uint8_t gpio_gen_get()
{
	return HAL_GPIO_ReadPin(CB_GEN_GPIO_Port, CB_GEN_Pin);
}

uint8_t gpio_bat_get()
{
	return HAL_GPIO_ReadPin(CB_BAT_GPIO_Port, CB_BAT_Pin);
}

void gpio_bat_enable(uint8_t enabled)
{
	HAL_GPIO_WritePin(CB_BAT_GPIO_Port, CB_BAT_GPIO_Port, enabled);
}

void init()
{
	DEBUG("HELLO");
}

void loop()
{
	system_voltage_t voltages;
	adc_update_voltage(&voltages, 1);
	gpio_gen_enable(0);
	DEBUG("%lu %lu %d %d", voltages.battery, voltages.generator, gpio_bat_get(), gpio_gen_get());
	HAL_Delay(5000);
	adc_update_voltage(&voltages, 1);
	gpio_gen_enable(1);
	DEBUG("%lu %lu %d %d", voltages.battery, voltages.generator, gpio_bat_get(), gpio_gen_get());
	HAL_Delay(5000);
}

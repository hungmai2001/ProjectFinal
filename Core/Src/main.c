/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define MAX_STRING 150
#define	COUNT	10
float	MeanPH=0, MeanORP=0;
float PH[COUNT], ORP[COUNT];
float M_ORP, M_PH;
uint8_t ch[MAX_STRING];
uint8_t character;
void delString(uint8_t ch[]){
	for(int i=0;i<MAX_STRING;i++){
		ch[i] = '\0';
	}
}
uint16_t u16_ADCval,u16_ADCval1;
float Voltage_PH, Voltage_ORP;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
	if(hadc->Instance == ADC1)
	{
		u16_ADCval = HAL_ADC_GetValue(&hadc1);
		Voltage_PH = (float)u16_ADCval/4095*3.3;
	}
	else if(hadc->Instance == ADC2)
	{
		u16_ADCval1 = HAL_ADC_GetValue(&hadc2);
		Voltage_ORP = (float)u16_ADCval1/4095*3.3;
	}
  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvCpltCallback must be implemented in the user file.
   */
}
void GetADC(){
	for(int i=0;i<COUNT;i++){
			HAL_ADC_Start_IT(&hadc1);
			HAL_Delay(100);
			PH[i]= Voltage_PH;
		}
		for(int i=0;i<COUNT;i++){
			HAL_ADC_Start_IT(&hadc2);
			HAL_Delay(100);
			ORP[i]= Voltage_ORP;
		}
		for(int i=0;i<COUNT;i++){
			MeanPH += PH[i];
			MeanORP += ORP[i];
		}
		M_ORP = MeanORP/COUNT;
		M_PH = MeanPH/COUNT;
		MeanORP =0;
		MeanPH =0;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	srand(time(NULL));
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		uint8_t randomNumber, character;
		GetADC();
		uint8_t str[] = "AT\n";
	  HAL_UART_Transmit(&huart1,str, strlen((char *)str), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart1,ch,strlen((char *)str) + 5, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,ch,strlen((char *)str) + 5, HAL_MAX_DELAY);
		HAL_Delay(5000);
		delString(ch);
		uint8_t i=0;
		while(i<10){
			HAL_UART_Receive(&huart1,&character,1, 0);
			HAL_Delay(500);
			i++;
		}
		
		uint8_t str99[] = "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\n";
		HAL_UART_Transmit(&huart1,str99, strlen((char *)str99), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart1,ch,strlen((char *)str99) + 5, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,ch,strlen((char *)str99) + 5, HAL_MAX_DELAY);
		HAL_Delay(5000);
		delString(ch);
		i=0;
		while(i<10){
			HAL_UART_Receive(&huart1,&character,1, 0);
			HAL_Delay(500);
			i++;
		}
		
		uint8_t str1[] = "AT+CSTT=\"internet\"\n";
		HAL_UART_Transmit(&huart1,str1, strlen((char *)str1), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart1,ch,strlen((char *)str1) + 5, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,ch,strlen((char *)str1) + 5, HAL_MAX_DELAY);
		HAL_Delay(5000);
		delString(ch);
		i=0;
		while(i<10){
			HAL_UART_Receive(&huart1,&character,1, 0);
			HAL_Delay(500);
			i++;
		}
		
		uint8_t str98[] = "AT+SAPBR=1,1\n";
		HAL_UART_Transmit(&huart1,str98, strlen((char *)str98), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart1,ch,strlen((char *)str98) + 5, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,ch,strlen((char *)str98) + 5, HAL_MAX_DELAY);
		HAL_Delay(5000);
		delString(ch);
		i=0;
		while(i<10){
			HAL_UART_Receive(&huart1,&character,1, 0);
			HAL_Delay(500);
			i++;
		}
		
		uint8_t str97[] = "AT+SAPBR=2,1\n";
		HAL_UART_Transmit(&huart1,str97, strlen((char *)str97), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart1,ch,strlen((char *)str97) + 5, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,ch,strlen((char *)str97) + 5, HAL_MAX_DELAY);
		HAL_Delay(5000);
		delString(ch);
		i=0;
		while(i<10){
			HAL_UART_Receive(&huart1,&character,1, 0);
			HAL_Delay(500);
			i++;
		}
		
//		while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY_RX){
//			HAL_UART_Receive(&huart1,&character,1, HAL_MAX_DELAY);
//			HAL_Delay(500);
//		}
		
		uint8_t str2[] = "AT+CIICR\n";
		HAL_UART_Transmit(&huart1,str2, strlen((char *)str2), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart1,ch,strlen((char *)str2) + 5, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,ch,strlen((char *)str2) + 5, HAL_MAX_DELAY);
		HAL_Delay(5000);
		delString(ch);
		i=0;
		while(i<10){
			HAL_UART_Receive(&huart1,&character,1, 0);
			HAL_Delay(500);
			i++;
		}
		
		uint8_t str3[] = "AT+CIFSR\n";
		HAL_UART_Transmit(&huart1,str3, strlen((char *)str3), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart1,ch,strlen((char *)str3) + 5, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,ch,strlen((char *)str3) + 5, HAL_MAX_DELAY);
		HAL_Delay(5000);
		delString(ch);
		i=0;
		while(i<10){
			HAL_UART_Receive(&huart1,&character,1, 0);
			HAL_Delay(500);
			i++;
		}
		
		uint8_t str4[] = "AT+HTTPINIT\n";
		HAL_UART_Transmit(&huart1,str4, strlen((char *)str4), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart1,ch,strlen((char *)str4) + 5, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,ch,strlen((char *)str4) + 5, HAL_MAX_DELAY);
		HAL_Delay(5000);
		delString(ch);
		i=0;
		while(i<10){
			HAL_UART_Receive(&huart1,&character,1, 0);
			HAL_Delay(500);
			i++;
		}
		
		uint8_t str5[] = "AT+HTTPPARA=\"CID\",1\n";
		HAL_UART_Transmit(&huart1,str5, strlen((char *)str5), HAL_MAX_DELAY);
		HAL_UART_Receive(&huart1,ch,strlen((char *)str5) + 5, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart2,ch,strlen((char *)str5) + 5, HAL_MAX_DELAY);
		HAL_Delay(5000);
		delString(ch);
		i=0;
		while(i<10){
			HAL_UART_Receive(&huart1,&character,1, 0);
			HAL_Delay(500);
			i++;
		}
		
		while(1){
			//randomNumber = rand()%100 + 10;
			uint8_t b = (uint8_t)M_PH;
			float a1 = (M_PH-b)*10;
			uint8_t c = (uint8_t)(a1);
			float a2 = (a1-c)*10;
			uint8_t d = (uint8_t)(a2);
			float a3 = (a2-d)*10;
			uint8_t e = (uint8_t)(a3);
			
			uint8_t b1 = (uint8_t)M_ORP;
			float at1 = (M_ORP-b1)*10;
			uint8_t c1 = (uint8_t)(at1);
			float at2 = (a1-c1)*10;
			uint8_t d1 = (uint8_t)(at2);
			float at3 = (a2-d1)*10;
			uint8_t e1 = (uint8_t)(at3);
			uint8_t string1[] = "AT+HTTPPARA=\"URL\",\"http://api.thingspeak.com/update?api_key=QGHIY6710J8D8QKB&field1=";
			uint8_t string3[] = "\"\n";
			uint8_t string2[] = "&field2=";
			uint8_t str6[strlen((char *)string1)+strlen((char *)string2)+strlen((char *)string3)+12];
			sprintf((char *)str6, "%s%d.%d%d%d%s%d.%d%d%d%s", (char *)string1, b,c,d,e, (char *)string2,b1,c1,d1,e1, (char *)string3);
			HAL_UART_Transmit(&huart1,str6, strlen((char *)str6), HAL_MAX_DELAY);
			HAL_UART_Receive(&huart1,ch,strlen((char *)str6) + 5, HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2,ch,strlen((char *)str6) + 5, HAL_MAX_DELAY);
			HAL_Delay(20000);
			i=0;
			while(i<10){
				HAL_UART_Receive(&huart1,&character,1, 0);
				HAL_Delay(500);
				i++;
			}
			delString(ch);
			
			
			
			uint8_t str7[] = "AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\n";
			HAL_UART_Transmit(&huart1,str7, strlen((char *)str7), HAL_MAX_DELAY);
			HAL_UART_Receive(&huart1,ch,strlen((char *)str7) + 5, HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2,ch,strlen((char *)str7) + 5, HAL_MAX_DELAY);
			HAL_Delay(5000);
			delString(ch);
			i=0;
			while(i<10){
				HAL_UART_Receive(&huart1,&character,1, 0);
				HAL_Delay(500);
				i++;
			}
			
			uint8_t str8[] = "AT+HTTPACTION=0\n"; //24 ky tu
			HAL_UART_Transmit(&huart1,str8, strlen((char *)str8), HAL_MAX_DELAY);
			HAL_UART_Receive(&huart1,ch,strlen((char *)str8) + 5, HAL_MAX_DELAY);
			HAL_UART_Transmit(&huart2,ch,strlen((char *)str8) + 5, HAL_MAX_DELAY);
			HAL_Delay(5000);
			delString(ch);
			i=0;
			while(i<10){
				HAL_UART_Receive(&huart1,&character,1, 0);
				HAL_Delay(500);
				i++;
			}
			GetADC();
			HAL_Delay(100);
		}
		
		
		
 }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

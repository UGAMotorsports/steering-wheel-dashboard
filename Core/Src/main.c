/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "rpi-display/rpiDisplayShapes.h"
#include "rpi-display/FreeMonoBold24pt7b.h"
#include "rpi-display/FreeSans18pt7b.h"
#include "shiftLights.h"
#include <stdio.h>

#include "mcp2515user.h"

#include "stm32f4xx_hal.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern uint8_t CDC_Transmit_FS (uint8_t *data, uint16_t);

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USB_DEVICE_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  for (int i = 0; i < 5; i++) {
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, SET);
	  HAL_Delay(100);
	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, RESET);
	  HAL_Delay(100);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  resetScreen();
  initializeScreen();

  char *uga = "uga motorsports";
  drawString(uga, FREE_SANS_18PT7B, 240, 160, NO_FLIP_OBJECT | CENTER_OBJECT);
  HAL_Delay(400);
  clearScreenfast(0x0000);
  initializeMCP2515();
  uint8_t ledcolors[3 * 16];
  uint16_t ledbytes[(16 * 24) + 150];
  int G1[12] = {834, 1668, 2502, 3336, 4170, 5004, 5838, 6672, 7506, 8340,
  9174, 10008};
  shiftLightsInit(&htim4, TIM_CHANNEL_1, ledcolors, ledbytes);
  HAL_Delay(500);
  startUp(&htim4, TIM_CHANNEL_1, ledcolors, ledbytes);

  struct can_frame frame;
  char rpmresult[20] = "not recieved rpm";
  char tempresult[20] = "not received temp";

  while (1)
  {
	  int canresult = readMessage(&frame);
	  if (canresult == 0) {
		  if (frame.can_id == 1512) {
			  uint16_t rpm = (((uint16_t)frame.data[2]) << 8) + frame.data[3];
			  UpdateShiftLights(&htim4, TIM_CHANNEL_1, ledcolors, ledbytes, rpm, G1);
			  itoa(rpm, rpmresult, 10);
		  } else if (frame.can_id == 1513){
			  uint16_t temp = (((uint16_t)frame.data[4]) << 8) + frame.data[5];
			  itoa(temp, tempresult, 10);
		  }
	  }


	  clearScreenfast(0x0000);
	  drawString(rpmresult, FREE_MONO_BOLD_24PT7B, 240, 160, NO_FLIP_OBJECT | CENTER_OBJECT);
	  drawString(tempresult, FREE_SANS_18PT7B, 240, 80, NO_FLIP_OBJECT | CENTER_OBJECT);
	  //drawString(tempresult, font_FreeSans18pt7b, 240, 180, NO_FLIP_OBJECT | CENTER_OBJECT);

//	  CDC_Transmit_FS (status, sizeof(status));
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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

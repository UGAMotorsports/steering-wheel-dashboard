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
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "rpi-display/rpiDisplayShapes.h"
#include "rpi-display/FreeMonoBold24pt7b.h"
#include "rpi-display/FreeSans18pt7b.h"
#include "rpi-display/rpiSceneBuilderUser.h"
#include "shiftLights.h"
#include "easyusbprintln/easyusbprintln.h"

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

volatile static int buttonpressed = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == button1INT_Pin) {
		buttonpressed = 1;
	} else if (GPIO_Pin == button2INT_Pin) {
		buttonpressed = 2;
	} else if (GPIO_Pin == button3INT_Pin ) {
		buttonpressed = 3;
	} else if (GPIO_Pin == button4INT_Pin) {
		buttonpressed = 4;
	} else {
		USB_Println("unknown button pressed: 0x%x", GPIO_Pin);
	}
}


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
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  HAL_Delay(100);

  FATFS fs;
  FRESULT fresult = f_mount(&fs, "/", 1);

  initializeMCP2515();
  //setFilters();
  uint8_t ledcolors[3 * 16];
  uint16_t ledbytes[(16 * 24) + 150];
  int G1[12] = {500, 3000, 4000, 5000, 6000, 7000, 8000, 8500, 9000, 9500,
  10000, 10200};
  shiftLightsInit(&htim4, TIM_CHANNEL_1, ledcolors, ledbytes);
  setColor(&htim4, TIM_CHANNEL_1, 0, 0, 0, ledcolors, ledbytes, 0);
  setColor(&htim4, TIM_CHANNEL_1, 0, 0, 0, ledcolors, ledbytes, 1);
  setColor(&htim4, TIM_CHANNEL_1, 0, 0, 0, ledcolors, ledbytes, 14);
  setColor(&htim4, TIM_CHANNEL_1, 0, 0, 0, ledcolors, ledbytes, 15);

  resetScreen();
  initializeScreen();
  if (fresult != FR_OK) {
	  USB_Println("There was an error: %d\n", fresult);
  } else {
	  USB_Println("the sdcard is mounted\n");
	  dosplashscene();
  }
  startUp(&htim4, TIM_CHANNEL_1, ledcolors, ledbytes);
  HAL_Delay(200);

  char result[20] = "null"; //rpm
  char result2[20] = "null";//temp
  char result3[20] = "8";//gear
  char result4[20] = "null"; //battery volt
  char result5[20] = "null"; //speed
  settempdata(result2);
  setgeardata(result3);
  setrpmdata(result);
  setbattdata(result4);
  setspeeddata(result5);
  domainscreen();

  struct can_frame frame;
  uint8_t gear = 8;
  uint8_t isNeutral = 0;
  while (1)
  {

	  if (buttonpressed == 3) {
		  frame.can_id = 601;
		  sendMessage(&frame);
		  HAL_Delay(10);
		  USB_Println("button3 job in while loop done\n");
		  buttonpressed = 0;
	  }

	  //for (int i = 0; i < 10; i++) {
		  int canresult = readMessage(&frame);
		  if (canresult == 0) {
			  if (frame.can_id == (1520 + 0)) {
				  uint16_t rpm = (((uint16_t)frame.data[6]) << 8) + frame.data[7];
				  UpdateShiftLights(&htim4, TIM_CHANNEL_1, ledcolors, ledbytes, rpm, G1);

				  itoa(rpm, (char*)(result), 10);
				  setrpmdata(result);
				  domainscreen();
			  }
			  if (frame.can_id == (1520 + 2)) {
				  uint16_t temp = (((uint16_t)frame.data[6]) << 8) + frame.data[7];
				  if (temp > 2150) {
					setColor(&htim4, TIM_CHANNEL_1, 0, 255, 0, ledcolors, ledbytes, 15);
				  } else {
					setColor(&htim4, TIM_CHANNEL_1, 0, 0, 0, ledcolors, ledbytes, 15);
				  }
				  temp /= 10;
				  itoa(temp, result2, 10);
				  settempdata(result2);
			  }
			  if (frame.can_id == 1520 + 33) {
				  gear = ((uint8_t)frame.data[6]);
				  if (gear != 0) {
					  //USB_Println("the gear value is %d\n", gear);
					  itoa(gear, result3, 10);
					  setgeardata(result3);
				  }
			  }
			  if (frame.can_id == (1520 + 3)) {
				  uint8_t battvalue = (((uint16_t)frame.data[2]) << 8) + frame.data[3];
				  uint8_t batIntPart = battvalue / 10;
				  uint8_t batDecimalPart = battvalue % 10;
				  char batint[10];
				  char batdec[10];
				  itoa(batIntPart, batint, 10);
				  itoa(batDecimalPart, batdec, 10);
				  strncpy(result4, "", 20);
				  strncat(result4, batint, 5);
				  strncat(result4, ".", 5);
				  strncat(result4, batdec, 1);
				  setbattdata(result4);
			  } else if (frame.can_id == 504) {
				  uint16_t neutrallight = (uint16_t)((frame.data[6] << 8) | (frame.data[7]));
				  if (neutrallight < 1024) {
					  //isNeutral = 1;
					  setColor(&htim4, TIM_CHANNEL_1, 128, 255, 0, ledcolors, ledbytes, 0);
					  strncpy(result3, "7", 10);
					  setgeardata(result3);
				  } else {
					  //isNeutral = 0;
					  setColor(&htim4, TIM_CHANNEL_1, 0, 0, 0, ledcolors, ledbytes, 0);
					  //strncpy(result3, "8", 10);
					  //setgeardata(result3);
				  }
				  //USB_Println("the neutral light value is %d\n", neutrallight);
			  } else if (frame.can_id == 1520 + 42) {
				  uint16_t speed = (uint16_t)((frame.data[0] << 8) | (frame.data[1]));
				  speed /= 10;
				  itoa(speed, (char*) result5, 10);
				  setspeeddata(result5);
			  }
		  }
	/*
	  if (l) {
		  setColor(&htim4, TIM_CHANNEL_1, 128, 255, 0, ledcolors, ledbytes, 14);
		  l = 0;
	  } else {
		  setColor(&htim4, TIM_CHANNEL_1, 0, 0, 0, ledcolors, ledbytes, 14);
		  l = 1;
	  }
	 */

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

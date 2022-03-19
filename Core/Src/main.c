/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "stm32f429i_discovery_lcd.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <lcd_api.h>
#include <lcd_plotter.h>
#include <sensor_api.h>
#include <stm32f429I_DISC1_board_api.h>
#include <break_light.h>
#include <filter.h>
#include <malloc.h>
#include "fonts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define IMU_ADD     0x6B<<1 //
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

//osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void StartDefaultTask(void const * argument);

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
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();

    MX_I2C3_Init();

    /* USER CODE BEGIN Init */
    initLCD();
    initGyroAcc();
    //writePinState(GPIOE, GPIO_PIN_2, 1);// PE2 starts as 1 logic to turn off the switch for the brake light
    plotInit();

    /* values sensor buffers */
    double acc[3];

    double x;
    double y;
    double z;

    double magnitude;

    double nextValue;

    short plotDelay;

    /* string buffer to show text on screen. For formatting is used sprintf function */
    char string[50];
    char timeString[50];

    int lightDelay = 0;

    /* the number of time the while loops */
    unsigned int refresh = 0;

    /* graph buffer used for filtering */
    double *magnitudeBuffer = NULL;
    int magnitudeBufferLength = 0;

    /* the parameters that change when the filter is changed */
    char currentFilter[30];
    uint32_t graphDelay = 0;

    /* user button parameters */
    short userButtonPressCount = 2;
    short pressed = 0;

    onFilterChanged(currentFilter, &graphDelay, userButtonPressCount, &magnitudeBuffer, &magnitudeBufferLength, &lightDelay);

    uint32_t whileStartTime;
    uint32_t whileEndTime;

    /* USER CODE END Init */

    /* USER CODE BEGIN SysInit */
    /* USER CODE END SysInit */

    /* USER CODE BEGIN 2 */

    readAccValues(acc);
//    alpha = vectorRealAngleInRadians(acc[0], acc[2]);

    /* USER CODE END 2 */

    /* Infinite loop */

    /* USER CODE BEGIN WHILE */
    while (1) {
        whileStartTime = HAL_GetTick();

        /* USER CODE END WHILE */
        refresh++;

        pushValue(lastGraphConfig, LCD_WIDTH, graph[LCD_WIDTH - 1]);

        /* ACC VALUES */
        readAccValues(acc);

        x = acc[0];
        y = acc[1];
        z = acc[2];

        magnitude = (x + z + 1);

        sprintf(string, "Acc: Ox=%-6.2f Oy=%-6.2f Oz=%-6.2f", x, y, z);
        showTextAtLine(0, 7, string);
        sprintf(string, "Threshold: %-3.2f | Force: %-5.2f", THRESHOLD, magnitude);
        showTextAtLine(0, 8, string);
        timeStampToTime(HAL_GetTick(), timeString);
        sprintf(string, "Time: %s", timeString);
        showTextAtLine(0, 9, string);

        /* READ USER BUTTON TO CHANGE FILTER TYPE */
        if (readPinState(GPIOA, USER_BUTTON) == 1 && pressed == 0) {
            userButtonPressCount++;

            userButtonPressCount %= FILTER_NUMBER;

            onFilterChanged(currentFilter, &graphDelay, userButtonPressCount, &magnitudeBuffer, &magnitudeBufferLength, &lightDelay);
            pressed = 1;
        } else if (readPinState(GPIOA, USER_BUTTON) == 0) {
            pressed = 0;
        }

        sprintf(string, "Filter: %s", currentFilter);
        showTextAtLine(0, 5, string);
        sprintf(string, "Plot Delay: %lums RLD: %-4d", graphDelay, lightDelay);
        showTextAtLine(0, 4, string);

        /* DRAWING THE GRAPH */
        drawGraph(graph);

        /* PUSH THE NEXT VALUE */
        pushValue(magnitudeBuffer, magnitudeBufferLength, magnitude);

        /* APPLY THE FILTER ON BUFFER */
        nextValue = nextFilteredValueFromUserChoice(magnitudeBuffer, magnitudeBufferLength, userButtonPressCount);

        if (refresh >= magnitudeBufferLength) {
            pushValue(graph, LCD_WIDTH, nextValue);
        }

        /* STOP FOR 10ms */
        HAL_Delay(10);

        /* CLEAR THE GRAPH */
        clearGraph(graph, lastGraphConfig);

        /* TURN ON THE RED LIGHT WHEN THE CONDITION IS MET */

        if (checkAccForBreakLight(graph, LCD_WIDTH, refresh) == LIGHT_ACTIVATED) {
            pushIntValue(graphColor, LCD_WIDTH, GRAPH_ACTIVATED_COLOR);

        } else {
            uint32_t color = getColorValueInInterval(nextValue);
            pushIntValue(graphColor, LCD_WIDTH, color);
        }

        if (refresh % 200 == 0) {
            clearLCD();
        }

        checkForRedLight(lightDelay);

        whileEndTime = HAL_GetTick();

        plotDelay = (short) (magnitudeBufferLength / 2);
        graphDelay = (whileEndTime - whileStartTime) * (plotDelay == 0 ? 1 : plotDelay);
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
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    /* Activate the Over-Drive mode */
    HAL_PWREx_EnableOverDrive();

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, NCS_MEMS_SPI_Pin|CSX_Pin|OTG_FS_PSO_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(ACP_RST_GPIO_Port, ACP_RST_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOD, RDX_Pin|WRX_DCX_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOG, LD3_Pin|LD4_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : NCS_MEMS_SPI_Pin CSX_Pin OTG_FS_PSO_Pin */
    GPIO_InitStruct.Pin = NCS_MEMS_SPI_Pin|CSX_Pin|OTG_FS_PSO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : B1_Pin MEMS_INT1_Pin MEMS_INT2_Pin TP_INT1_Pin */
    GPIO_InitStruct.Pin = B1_Pin|MEMS_INT1_Pin|MEMS_INT2_Pin|TP_INT1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : ACP_RST_Pin */
    GPIO_InitStruct.Pin = ACP_RST_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ACP_RST_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : OTG_FS_OC_Pin */
    GPIO_InitStruct.Pin = OTG_FS_OC_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(OTG_FS_OC_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : BOOT1_Pin */
    GPIO_InitStruct.Pin = BOOT1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : TE_Pin */
    GPIO_InitStruct.Pin = TE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(TE_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : RDX_Pin WRX_DCX_Pin */
    GPIO_InitStruct.Pin = RDX_Pin|WRX_DCX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pins : LD3_Pin LD4_Pin */
    GPIO_InitStruct.Pin = LD3_Pin|LD4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM6) {
        HAL_IncTick();
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


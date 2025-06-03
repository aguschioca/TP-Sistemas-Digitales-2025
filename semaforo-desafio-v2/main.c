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
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h> // variable booleanas
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TREBOTES 20 // intervalo de rebotes en ms
#define TSONDEO 200 // retardo de sondeo en ms
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
volatile uint8_t cont = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
bool VerificarPedido1(void); // Determinar si el botón 1 fue pulsado
bool VerificarPedido2(void); // Determinar si el botón 2 fue pulsado
bool VerificarPedido3(void); // Determinar si el botón 3 fue pulsado
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
  MX_I2C1_Init();
  MX_USB_HOST_Init();
  /* USER CODE BEGIN 2 */
  //genera condicion inicial con todos los leds en 0
  	HAL_GPIO_WritePin(AMARILLOB_GPIO_Port, AMARILLOB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(VERDEB_GPIO_Port, VERDEB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ROJOB_GPIO_Port, ROJOB_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AMARILLOA_GPIO_Port, AMARILLOA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(VERDEA_GPIO_Port, VERDEA_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ROJOA_GPIO_Port, ROJOA_Pin, GPIO_PIN_RESET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */
   while(!VerificarPedido1() && !VerificarPedido2() && !VerificarPedido3()) // Comprobamos si hay una solicitud haciendo una  lectura filtrada del botón
    HAL_Delay(TSONDEO); // Si no la hay, esperamos por un tiempo y volvemos a   comprobar

    //limito el contador hasta 63 y lo resetea
    if (cont == 64)
    	cont = 0;


    //CHEQUEO BIT 0 DEL CONTADOR
    if(cont & (1 << 0))
    	HAL_GPIO_WritePin(VERDEA_GPIO_Port, VERDEA_Pin, GPIO_PIN_SET); //si es 1 prende el led correspondiente
    else
    	HAL_GPIO_WritePin(VERDEA_GPIO_Port, VERDEA_Pin, GPIO_PIN_RESET); //si es 0 apaga el led correspondiente
    //CHEQUEO BIT 1 DEL CONTADOR
    if(cont & (1 << 1))
    	HAL_GPIO_WritePin(AMARILLOA_GPIO_Port, AMARILLOA_Pin, GPIO_PIN_SET);
    else
    	HAL_GPIO_WritePin(AMARILLOA_GPIO_Port, AMARILLOA_Pin, GPIO_PIN_RESET);
    //CHEQUEO BIT 2 DEL CONTADOR
    if(cont & (1 << 2))
    	HAL_GPIO_WritePin(ROJOA_GPIO_Port, ROJOA_Pin, GPIO_PIN_SET);
    else
    	HAL_GPIO_WritePin(ROJOA_GPIO_Port, ROJOA_Pin, GPIO_PIN_RESET);
    //CHEQUEO BIT 3 DEL CONTADOR
    if(cont & (1 << 3))
    	HAL_GPIO_WritePin(VERDEB_GPIO_Port, VERDEB_Pin, GPIO_PIN_SET);
    else
    	HAL_GPIO_WritePin(VERDEB_GPIO_Port, VERDEB_Pin, GPIO_PIN_RESET);
    //CHEQUEO BIT 4 DEL CONTADOR
    if(cont & (1 << 4))
    	HAL_GPIO_WritePin(AMARILLOB_GPIO_Port, AMARILLOB_Pin, GPIO_PIN_SET);
    else
    	HAL_GPIO_WritePin(AMARILLOB_GPIO_Port, AMARILLOB_Pin, GPIO_PIN_RESET);
    //CHEQUEO BIT 5 DEL CONTADOR
    if(cont & (1 << 5))
    	HAL_GPIO_WritePin(ROJOB_GPIO_Port, ROJOB_Pin, GPIO_PIN_SET);
    else
    	HAL_GPIO_WritePin(ROJOB_GPIO_Port, ROJOB_Pin, GPIO_PIN_RESET);


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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ROJOA_Pin|VERDEA_Pin|AMARILLOA_Pin|VERDEB_Pin
                          |ROJOB_Pin|LD5_Pin|AMARILLOB_Pin|Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PDM_OUT_Pin */
  GPIO_InitStruct.Pin = PDM_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(PDM_OUT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BOTONCAMBIO1_Pin BOTONCAMBIO2_Pin BOTONCAMBIO3_Pin */
  GPIO_InitStruct.Pin = BOTONCAMBIO1_Pin|BOTONCAMBIO2_Pin|BOTONCAMBIO3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_MISO_Pin SPI1_MOSI_Pin */
  GPIO_InitStruct.Pin = SPI1_MISO_Pin|SPI1_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : CLK_IN_Pin */
  GPIO_InitStruct.Pin = CLK_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(CLK_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ROJOA_Pin VERDEA_Pin AMARILLOA_Pin VERDEB_Pin
                           ROJOB_Pin LD5_Pin AMARILLOB_Pin Audio_RST_Pin */
  GPIO_InitStruct.Pin = ROJOA_Pin|VERDEA_Pin|AMARILLOA_Pin|VERDEB_Pin
                          |ROJOB_Pin|LD5_Pin|AMARILLOB_Pin|Audio_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2S3_MCK_Pin I2S3_SCK_Pin I2S3_SD_Pin */
  GPIO_InitStruct.Pin = I2S3_MCK_Pin|I2S3_SCK_Pin|I2S3_SD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// Determinar si el botón fue pulsado
bool VerificarPedido1(void){
// Se crea e inicializa un registro de lectura válida del botón
static GPIO_PinState ultima_lectura_valida = GPIO_PIN_SET; //botón liberado
// Se crean variables para lecturas intermedias
GPIO_PinState lectura1, lectura2;
// Se crea una variable booleana para indicar si hay un pedido
bool pedido1 = false; // No hay pedido hasta que se pulsa el botón
// Se lee el estado del botón
lectura1 = HAL_GPIO_ReadPin(BOTONCAMBIO1_GPIO_Port, BOTONCAMBIO1_Pin);
// Si hubo un cambio
if (lectura1 != ultima_lectura_valida){
// Se espera un tiempo para filtrar los rebotes
HAL_Delay(TREBOTES); // Retardo de TREBOTES milisegundos
// Se lee nuevamente el estado del botón
lectura2 = HAL_GPIO_ReadPin(BOTONCAMBIO1_GPIO_Port, BOTONCAMBIO1_Pin);
// Si ambas lecturas son iguales, se considera una lectura válida
if(lectura2 == lectura1)
ultima_lectura_valida = lectura2;
// Si el botón pasó de liberado a pulsado (1-->0), hubo un pedido de cambio de estado
if (ultima_lectura_valida == GPIO_PIN_RESET){
pedido1 = true;
if (cont == 0)
	cont = 63; //para el underflow
else
	cont--; //resta
}
}

return pedido1;
}

bool VerificarPedido2(void){
// Se crea e inicializa un registro de lectura válida del botón
static GPIO_PinState ultima_lectura_valida = GPIO_PIN_SET; //botón liberado
// Se crean variables para lecturas intermedias
GPIO_PinState lectura1, lectura2;
// Se crea una variable booleana para indicar si hay un pedido
bool pedido2 = false; // No hay pedido hasta que se pulsa el botón
// Se lee el estado del botón
lectura1 = HAL_GPIO_ReadPin(BOTONCAMBIO2_GPIO_Port, BOTONCAMBIO2_Pin);
// Si hubo un cambio
if (lectura1 != ultima_lectura_valida){
// Se espera un tiempo para filtrar los rebotes
HAL_Delay(TREBOTES); // Retardo de TREBOTES milisegundos
// Se lee nuevamente el estado del botón
lectura2 = HAL_GPIO_ReadPin(BOTONCAMBIO2_GPIO_Port, BOTONCAMBIO2_Pin);
// Si ambas lecturas son iguales, se considera una lectura válida
if(lectura2 == lectura1)
ultima_lectura_valida = lectura2;
// Si el botón pasó de liberado a pulsado (1-->0), hubo un pedido de cambio de estado
if (ultima_lectura_valida == GPIO_PIN_RESET){
pedido2 = true;
cont = 0; //resetea
}
}
return pedido2;
}

bool VerificarPedido3(void){
// Se crea e inicializa un registro de lectura válida del botón
static GPIO_PinState ultima_lectura_valida = GPIO_PIN_SET; //botón liberado
// Se crean variables para lecturas intermedias
GPIO_PinState lectura1, lectura2;
// Se crea una variable booleana para indicar si hay un pedido
bool pedido = false; // No hay pedido hasta que se pulsa el botón
// Se lee el estado del botón
lectura1 = HAL_GPIO_ReadPin(BOTONCAMBIO3_GPIO_Port, BOTONCAMBIO3_Pin);
// Si hubo un cambio
if (lectura1 != ultima_lectura_valida){
// Se espera un tiempo para filtrar los rebotes
HAL_Delay(TREBOTES); // Retardo de TREBOTES milisegundos
// Se lee nuevamente el estado del botón
lectura2 = HAL_GPIO_ReadPin(BOTONCAMBIO3_GPIO_Port, BOTONCAMBIO3_Pin);
// Si ambas lecturas son iguales, se considera una lectura válida
if(lectura2 == lectura1)
ultima_lectura_valida = lectura2;
// Si el botón pasó de liberado a pulsado (1-->0), hubo un pedido de cambio de estado
if (ultima_lectura_valida == GPIO_PIN_RESET){
pedido = true;
cont++; //suma
}
}
return pedido;
}
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

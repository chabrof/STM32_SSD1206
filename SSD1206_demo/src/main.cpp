#include <STM32_SSD1306.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"
#include "print.h"

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define NUMFLAKES 10
#define NB_SNOWFLAKES_ANIM 200
#define XPOS 0
#define YPOS 1
#define DELTAY 1

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

const unsigned char logo16_glcd_bmp[] = {
  0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000
};

SPI_HandleTypeDef hspi1;

void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
void testDrawBitmapNInvert(STM32_SSD1306 &display);
void testSnowFlakes(STM32_SSD1306 &display, const uint8_t *bitmap, uint8_t w, uint8_t h);
void testdrawchar(STM32_SSD1306 &display);
void testdrawcircle(STM32_SSD1306 &display);
void testfillrect(STM32_SSD1306 &display);
void testdrawtriangle(STM32_SSD1306 &display);
void testfilltriangle(STM32_SSD1306 &display);
void testdrawroundrect(STM32_SSD1306 &display);
void testfillroundrect(STM32_SSD1306 &display);
void testdrawrect(STM32_SSD1306 &display);
void testdrawline(STM32_SSD1306 &display);
void testscrolltext(STM32_SSD1306 &display);
void testVariousTexts(STM32_SSD1306 &display);


int main(void) {
  HAL_Init();

  // Configure the system clock
  SystemClock_Config();

  // Initialize all configured peripherals
  MX_GPIO_Init();
  MX_SPI1_Init();

  STM32_SSD1306 display(&hspi1, 1, 1, 1, 1, 1);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();

  while (1) {
    display.clearDisplay();

    // Draw multiple rectangles
    testfillrect(display);
    display.display();
    HAL_Delay(2000);
    display.clearDisplay();

    // Draw multiple circles
    testdrawcircle(display);
    display.display();
    HAL_Delay(2000);
    display.clearDisplay();

    // Draw a white circle, 10 pixel radius
    display.fillCircle(display.width()/2, display.height()/2, 10, WHITE);
    display.display();
    HAL_Delay(2000);
    display.clearDisplay();

    testdrawroundrect(display);
    HAL_Delay(2000);
    display.clearDisplay();

    testfillroundrect(display);
    HAL_Delay(2000);
    display.clearDisplay();

    testdrawtriangle(display);
    HAL_Delay(2000);
    display.clearDisplay();

    testfilltriangle(display);
    HAL_Delay(2000);
    display.clearDisplay();

    // Draw the first ~12 characters in the font
    testdrawchar(display);
    HAL_Delay(2000);
    display.clearDisplay();

    // Draw scrolling text
    testscrolltext(display);
    HAL_Delay(2000);
    display.clearDisplay();

    // Text display tests
    testVariousTexts(display);
    HAL_Delay(2000);
    display.clearDisplay();

    testDrawBitmapNInvert(display);
    HAL_Delay(1000);
    display.clearDisplay();

    // Draw a bitmap icon and 'animate' movement
    testSnowFlakes(display, logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
  }
}

void testDrawBitmapNInvert(STM32_SSD1306 &display) {
  // Miniature bitmap display
  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();

  // Invert the display
  display.invertDisplay(true);
  HAL_Delay(1000);
  display.invertDisplay(false);
}


void testSnowFlakes(STM32_SSD1306 &display, const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];

  // Initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = rand() % (display.width() + 1);
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = rand() % 6 + 1;

    printf("x: %d", icons[f][XPOS]);
    printf("y: %d", icons[f][YPOS]);
    printf("dy: %d", icons[f][DELTAY]);
  }

  for (uint8_t i=0; i< NB_SNOWFLAKES_ANIM; i++) {
    // Draw each icon
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
    }
    display.display();
    HAL_Delay(2);

    // Then erase it + move it
    for (uint8_t f = 0; f < NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, BLACK);
      // Move it
      icons[f][YPOS] += icons[f][DELTAY];
      // If its gone, re-init
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = rand() % (display.width() + 1);
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random() % 6 + 1;
      }
    }
  }
}

void testVariousTexts(STM32_SSD1306 &display) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x");
  display.println(0xDEADBEEF, HEX);
  display.display();
}


void testdrawchar(STM32_SSD1306 &display) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n')
      continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }
  display.display();
}

void testdrawcircle(STM32_SSD1306 &display) {
  for (int16_t i = 0; i < display.height(); i+=2) {
    display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
    display.display();
  }
}

void testfillrect(STM32_SSD1306 &display) {
  uint8_t color = 1;
  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
    display.display();
    color++;
  }
}

void testdrawtriangle(STM32_SSD1306 &display) {
  for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5) {
    display.drawTriangle(display.width()/2, display.height() / 2 - i,
                     display.width() / 2 - i, display.height() /2 + i,
                     display.width() / 2 + i, display.height() / 2 + i, WHITE);
    display.display();
  }
}

void testfilltriangle(STM32_SSD1306 &display) {
  uint8_t color = WHITE;
  for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5) {
    display.fillTriangle(display.width() / 2, display.height() / 2 - i,
                     display.width() / 2 - i, display.height() / 2 + i,
                     display.width() / 2 + i, display.height() / 2 + i, WHITE);
    if (color == WHITE)
      color = BLACK;
    else
      color = WHITE;
    display.display();
  }
}

void testdrawroundrect(STM32_SSD1306 &display) {
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height() - 2 * i, display.height() / 4, WHITE);
    display.display();
  }
}

void testfillroundrect(STM32_SSD1306 &display) {
  uint8_t color = WHITE;
  for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
    display.fillRoundRect(i, i, display.width() - 2 * i, display.height() - 2 * i, display.height() / 4, color);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawrect(STM32_SSD1306 &display) {
  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, WHITE);
    display.display();
  }
}

void testdrawline(STM32_SSD1306 &display) {
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(0, 0, i, display.height() - 1, WHITE);
    display.display();
  }
  for (int16_t i = 0; i < display.height(); i += 4) {
    display.drawLine(0, 0, display.width() - 1, i, WHITE);
    display.display();
  }
  HAL_Delay(250);

  display.clearDisplay();
  for (int16_t i=0; i < display.width(); i += 4) {
    display.drawLine(0, display.height() - 1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(0, display.height() - 1, display.width() - 1, i, WHITE);
    display.display();
  }
  HAL_Delay(250);

  display.clearDisplay();
  for (int16_t i = display.width() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, i, 0, WHITE);
    display.display();
  }
  for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
    display.drawLine(display.width() - 1, display.height() - 1, 0, i, WHITE);
    display.display();
  }
  HAL_Delay(250);

  display.clearDisplay();
  for (int16_t i=0; i < display.height(); i += 4) {
    display.drawLine(display.width() - 1, 0, 0, i, WHITE);
    display.display();
  }
  for (int16_t i = 0; i < display.width(); i += 4) {
    display.drawLine(display.width() - 1, 0, i, display.height() - 1, WHITE);
    display.display();
  }
  HAL_Delay(250);
}

void testscrolltext(STM32_SSD1306 &display) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.clearDisplay();
  display.println("scroll");
  display.display();

  display.startscrollright(0x00, 0x0F);
  HAL_Delay(2000);
  display.stopscroll();
  HAL_Delay(1000);
  display.startscrollleft(0x00, 0x0F);
  HAL_Delay(2000);
  display.stopscroll();
  HAL_Delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  HAL_Delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  HAL_Delay(2000);
  display.stopscroll();
}

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  // Initializes the CPU, AHB and APB busses clocks
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  // Initializes the CPU, AHB and APB busses clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    Error_Handler();
  }

  // Configure the Systick interrupt time
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  // Configure the Systick
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  // SysTick_IRQn interrupt configuration
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


static void MX_SPI1_Init(void) {
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS =  SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
}


static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  // GPIO Ports Clock Enable
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // Configure GPIO pin Output Level
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  // Configure GPIO pin Output Level
  HAL_GPIO_WritePin(GPIOA, CS_Pin, GPIO_PIN_RESET);

  // Configure GPIO pin Output Level
  HAL_GPIO_WritePin(GPIOB, DC_Pin|RST_Pin, GPIO_PIN_RESET);

  // Configure GPIO pin : LED_Pin
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  // Configure GPIO pins : CS_Pin CLK_Pin MOSI_Pin
  GPIO_InitStruct.Pin = CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Configure GPIO pins : DC_Pin RST_Pin
  GPIO_InitStruct.Pin = DC_Pin|RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void) {
  // USER CODE BEGIN Error_Handler
  // User can add his own implementation to report the HAL error return state
  while(1) {

  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif

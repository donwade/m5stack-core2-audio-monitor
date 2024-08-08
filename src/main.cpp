#include <Arduino.h>
#include <M5Core2.h>
#include <driver/i2s.h>

#include "Application.h"

// approx 30ms of audio @ 16KHz
#define WINDOW_SIZE 1024 // dwade 512
#define AUDIO_SAMPLE_RATE 64000  // dwade new

// i2s config for reading from both m5stack mic
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
    .sample_rate = AUDIO_SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 1024,
};

// i2s pins
i2s_pin_config_t i2s_pins = {.bck_io_num = GPIO_NUM_12,
                             .ws_io_num = GPIO_NUM_0,
                             .data_out_num = I2S_PIN_NO_CHANGE,
                             .data_in_num = GPIO_NUM_34};

Application *application;

void setup()
{
  Serial.begin(115200);
  delay(500);
  M5.begin(true,   // LCDEnable
           false,  // SDEnable
           false,  // SerialEnable !important
           false   // I2CEnable
  );
  M5.Axp.SetLcdVoltage(2000);  // 3v backlight too obvious/bright
  M5.Lcd.fillScreen(TFT_BLACK);
  // M5Core grabs I2S_NUM_0 so let's get it back
  i2s_driver_uninstall(I2S_NUM_0);
  printf("\nH/W init done \n");

  // create our application
  application = new Application(M5.Lcd, M5.Touch, WINDOW_SIZE);
  application->begin(i2s_config, i2s_pins);

/*
  //https://stackoverflow.com/questions/4364823/how-do-i-obtain-the-frequencies-of-each-value-in-an-fft
  for (int binNum = 0; binNum < WINDOW_SIZE; binNum++)
  {
  	if (!(binNum & 0x3)) printf("\n");
  	printf("%4d -> %5d\t", binNum, binNum * AUDIO_SAMPLE_RATE/ WINDOW_SIZE);
  }
*/
  printf("%s: fft has %d bins ...each bin is %3.1f Hz wide at %d S/s \n",
  		__FUNCTION__,
  		WINDOW_SIZE,
  		(float)AUDIO_SAMPLE_RATE/ (float)WINDOW_SIZE,
  		AUDIO_SAMPLE_RATE);

}

void loop()
{
  // service the application
  application->loop();
}

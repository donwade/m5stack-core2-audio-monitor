#include <Arduino.h>
#include <M5Core2.h>

#include "Waveform.h"

Waveform::Waveform(M5Display &display, int x, int y, int width, int height, int num_samples)
    : Component(x, y, width, height)
{
  printf("Waveform::Waveform : x=%d y=%d w=%d h=%d num_samples=%d\n",
  		  x, y, width, height, num_samples);
  m_num_samples = num_samples;
  m_samples = static_cast<float *>(ps_malloc(sizeof(float) * num_samples));
}

void Waveform::update(const float *samples)
{
  memcpy(m_samples, samples, sizeof(float) * m_num_samples);
}

void Waveform::_draw(M5Display &display)
{
  const float x_step = (float)width / (float)m_num_samples;
  const float y_offset = height/2;

  display.fillRect(lhs, top, width, height, 0);
  // display.drawLine(lhs, y_offset, lhs + width, y_offset, 0xf000);

  float sample_x = 0;

  //normalize all samples across entire physical witdh of display

  for (int i = 4; i < m_num_samples; i += 4) {
    display.drawLine(sample_x,
                     y_offset + m_samples[i - 4] * 3,
                     sample_x + x_step * 4,
                     y_offset + m_samples[i] * 3,
                     0xfff);

    sample_x += x_step * 4; // x_step is num_samples/phyical pixels
  }
  // m_sprite->pushSprite(0, 0);
}

#include <Arduino.h>
#include <M5Core2.h>
#include <algorithm>

#include "GraphicEqualiser.h"
#include "Palette.h"

#undef min

GraphicEqualiser::GraphicEqualiser(
    Palette *palette, int x, int y, int width, int height, int num_bins)
    : Component(x, y, width, height)
{
  printf("GraphicEqualiser::GraphicEqualiser : x=%d y=%d w=%d h=%d num_bins=%d\n",
  		  x, y, width, height, num_bins);

  m_palette = palette;
  m_num_bins = num_bins;
  bar_chart = static_cast<float *>(malloc(sizeof(float) * num_bins));
  for (int i = 0; i < num_bins; i++) {
    bar_chart[i] = 0.0f;
  }
  bar_chart_peaks = static_cast<float *>(malloc(sizeof(float) * num_bins));
  for (int i = 0; i < num_bins; i++) {
    bar_chart_peaks[i] = 0.0f;
  }
}

void GraphicEqualiser::update(float *mag)
{
  for (int i = 0; i < m_num_bins; i++) {
    float m = mag[i];
    if (m > bar_chart[i]) {
      bar_chart[i] = m;
    }
    else {
      bar_chart[i] = 0.7 * bar_chart[i] + 0.3 * m;
    }
    if (m > bar_chart_peaks[i]) {
      bar_chart_peaks[i] = m;
    }
    else {
      bar_chart_peaks[i] = 0.95 * bar_chart_peaks[i] + 0.05 * m;
    }
  }
}

void GraphicEqualiser::_draw(M5Display &display)
{
  int x = 0;
  int x_step = int(320.0f / (m_num_bins / 16));

  for (int i = 2; i < m_num_bins / 4; i += 4)
  {
    // average of 4 samples why?
    float ave = 0;
    for (int j = 0; j < 4; j++)
    {
      ave += bar_chart[i + j];
    }
    ave /= 4;

    // average of this sample of 4.

	// normally confine to vert disp of 'heigth', allow it
	// to be out of its lane for observation.
	float zoom = (float) height * 1.6;

    int bar_value = std::min( zoom , 0.25f * ave);

    //
    ave = 0;
    for (int j = 0; j < 4; j++)
    {
      ave += bar_chart_peaks[i + j];
    }

    ave /= 4;

    int peak_value = std::min(zoom , 0.25f * ave);

    display.fillRect(x, height, x_step, height - bar_value, 0);
    display.drawLine(x,
                     height * 2 - peak_value,
                     x + x_step - 1,
                     height * 2 - peak_value,
                     m_palette->get_color(135 + peak_value));

    display.fillRect(x,
    				height *2 - bar_value,
    				x_step - 1,
    				bar_value,
    				m_palette->get_color(135 + bar_value));

    x += x_step;
  }
  display.fillRect(x, height, width - x, height, 0);
}

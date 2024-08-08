#pragma once

class M5Display;
class Component {
 public:
  int lhs, top, width, height;
  bool visible;
  Component(int lhs, int top, int width, int height)
      : lhs(lhs), top(top), width(width), height(height), visible(true)
  {
  }
  virtual void _draw(M5Display &display) = 0;
  void draw(M5Display &display)
  {
    if (visible) {
      _draw(display);
    }
  }
};

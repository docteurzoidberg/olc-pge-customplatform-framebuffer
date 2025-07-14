#include "FramebufferPlatform.h"

#undef OLC_PLATFORM_CUSTOM_EX
#define OLC_PLATFORM_CUSTOM_EX LinuxFBGPlatform
#define OLC_RENDERER_CUSTOM_EX LinuxFBGRenderer

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine {
public:
  Example() {
    sAppName = "Linux Framebuffer with FBG Example";
  }

  bool OnUserCreate() override {
    // Initialize resources here
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override {
    // Clear screen
    Clear(olc::CYAN);

    // Draw some graphics
    Draw(1, 1, olc::RED);
    Draw(10, 10, olc::WHITE);
    DrawString(20, 20, "Hello Framebuffer!", olc::YELLOW);

    // Input testing
    DrawString(20, 50, "Keys Held:", olc::WHITE);
    if (GetKey(olc::Key::UP).bHeld)    DrawString(40, 60, "UP", olc::WHITE);
    if (GetKey(olc::Key::DOWN).bHeld)  DrawString(40, 70, "DOWN", olc::WHITE);
    if (GetKey(olc::Key::LEFT).bHeld)  DrawString(40, 80, "LEFT", olc::WHITE);
    if (GetKey(olc::Key::RIGHT).bHeld) DrawString(40, 90, "RIGHT", olc::WHITE);

    if (GetKey(olc::Key::SPACE).bPressed)
      DrawString(20, 120, "SPACE Pressed", olc::GREEN);
    if (GetKey(olc::Key::ENTER).bPressed)
      DrawString(20, 130, "ENTER Pressed", olc::GREEN);

    // Quit on ESCAPE
    if (GetKey(olc::Key::ESCAPE).bPressed)
      return false;

    return true;
  }
};

int main() {
  Example demo;
  if (demo.Construct(320, 240, 1, 1)) {
    demo.Start();
  }
  return 0;
}

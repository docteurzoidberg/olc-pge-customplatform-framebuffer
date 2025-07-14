#pragma once

#include <fbgraphics.h>
#include <fbg_fbdev.h>

#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <linux/input.h>
#include <linux/input-event-codes.h>

#define OLC_GFX_CUSTOM_EX
#define OLC_PLATFORM_CUSTOM_EX

#include "olcPixelGameEngine.h"

// Custom Platform for Linux Framebuffer using libfbg + evdev keyboard input
class LinuxFBGPlatform : public olc::Platform {
  private:
  struct _fbg* fbg_instance = nullptr;
  int width = 0, height = 0;
  const char* fbdev = "/dev/fb0";
  
  // evdev keyboard
  int fd_kbd = -1;
  
  // Initialize framebuffer
  olc::rcode CreateFrameBuffer() {
    fbg_instance = fbg_fbdevSetup(const_cast<char*>(fbdev), 0);
    if (!fbg_instance) return olc::rcode::FAIL;
    width = fbg_instance->width;
    height = fbg_instance->height;
    return olc::rcode::OK;
  }
  
  // Map evdev key codes to olc::Key
  static olc::Key EvdevCodeToOlcKey(int code) {
    switch (code) {
      // Letters
      case KEY_A: return olc::Key::A; case KEY_B: return olc::Key::B;
      case KEY_C: return olc::Key::C; case KEY_D: return olc::Key::D;
      case KEY_E: return olc::Key::E; case KEY_F: return olc::Key::F;
      case KEY_G: return olc::Key::G; case KEY_H: return olc::Key::H;
      case KEY_I: return olc::Key::I; case KEY_J: return olc::Key::J;
      case KEY_K: return olc::Key::K; case KEY_L: return olc::Key::L;
      case KEY_M: return olc::Key::M; case KEY_N: return olc::Key::N;
      case KEY_O: return olc::Key::O; case KEY_P: return olc::Key::P;
      case KEY_Q: return olc::Key::Q; case KEY_R: return olc::Key::R;
      case KEY_S: return olc::Key::S; case KEY_T: return olc::Key::T;
      case KEY_U: return olc::Key::U; case KEY_V: return olc::Key::V;
      case KEY_W: return olc::Key::W; case KEY_X: return olc::Key::X;
      case KEY_Y: return olc::Key::Y; case KEY_Z: return olc::Key::Z;
      
      // Numbers
      case KEY_1: return olc::Key::K1; case KEY_2: return olc::Key::K2;
      case KEY_3: return olc::Key::K3; case KEY_4: return olc::Key::K4;
      case KEY_5: return olc::Key::K5; case KEY_6: return olc::Key::K6;
      case KEY_7: return olc::Key::K7; case KEY_8: return olc::Key::K8;
      case KEY_9: return olc::Key::K9; case KEY_0: return olc::Key::K0;
      
      // Function keys
      case KEY_F1: return olc::Key::F1; case KEY_F2: return olc::Key::F2;
      case KEY_F3: return olc::Key::F3; case KEY_F4: return olc::Key::F4;
      case KEY_F5: return olc::Key::F5; case KEY_F6: return olc::Key::F6;
      case KEY_F7: return olc::Key::F7; case KEY_F8: return olc::Key::F8;
      case KEY_F9: return olc::Key::F9; case KEY_F10: return olc::Key::F10;
      case KEY_F11: return olc::Key::F11; case KEY_F12: return olc::Key::F12;
      
      // Arrows
      case KEY_UP: return olc::Key::UP; case KEY_DOWN: return olc::Key::DOWN;
      case KEY_LEFT: return olc::Key::LEFT; case KEY_RIGHT: return olc::Key::RIGHT;
      
      // Control keys
      case KEY_SPACE: return olc::Key::SPACE; case KEY_TAB: return olc::Key::TAB;
      case KEY_LEFTSHIFT: case KEY_RIGHTSHIFT: return olc::Key::SHIFT;
      case KEY_LEFTCTRL: case KEY_RIGHTCTRL: return olc::Key::CTRL;
      case KEY_CAPSLOCK: return olc::Key::CAPS_LOCK;
      case KEY_ESC: return olc::Key::ESCAPE; case KEY_PAUSE: return olc::Key::PAUSE;
      case KEY_SCROLLLOCK: return olc::Key::SCROLL;
      case KEY_INSERT: return olc::Key::INS; case KEY_DELETE: return olc::Key::DEL;
      case KEY_HOME: return olc::Key::HOME; case KEY_END: return olc::Key::END;
      case KEY_PAGEUP: return olc::Key::PGUP; case KEY_PAGEDOWN: return olc::Key::PGDN;
      case KEY_BACKSPACE: return olc::Key::BACK;
      
      // Enter / Return
      case KEY_ENTER: return olc::Key::RETURN; case KEY_KPENTER: return olc::Key::ENTER;
      
      // Numpad (using correct evdev codes)
      case KEY_KP0: return olc::Key::NP0; case KEY_KP1: return olc::Key::NP1;
      case KEY_KP2: return olc::Key::NP2; case KEY_KP3: return olc::Key::NP3;
      case KEY_KP4: return olc::Key::NP4; case KEY_KP5: return olc::Key::NP5;
      case KEY_KP6: return olc::Key::NP6; case KEY_KP7: return olc::Key::NP7;
      case KEY_KP8: return olc::Key::NP8; case KEY_KP9: return olc::Key::NP9;
      case KEY_KPASTERISK: return olc::Key::NP_MUL;
      case KEY_KPSLASH: return olc::Key::NP_DIV;
      case KEY_KPPLUS: return olc::Key::NP_ADD;
      case KEY_KPMINUS: return olc::Key::NP_SUB;
      case KEY_KPDOT: return olc::Key::NP_DECIMAL;
      
      // Punctuation
      case KEY_COMMA: return olc::Key::COMMA;
      case KEY_DOT: return olc::Key::PERIOD;
      case KEY_MINUS: return olc::Key::MINUS;
      case KEY_EQUAL: return olc::Key::EQUALS;
      
      // OEM / Scan codes (approximate)
      case KEY_SEMICOLON: return olc::Key::OEM_1;
      case KEY_SLASH: return olc::Key::OEM_2;
      case KEY_GRAVE: return olc::Key::OEM_3;
      case KEY_LEFTBRACE: return olc::Key::OEM_4;
      case KEY_BACKSLASH: return olc::Key::OEM_5;
      case KEY_RIGHTBRACE: return olc::Key::OEM_6;
      case KEY_APOSTROPHE: return olc::Key::OEM_7;
      
      default: return olc::Key::NONE;
    }
  }
  
  public:
  // Called once before the engine thread starts
  olc::rcode ApplicationStartUp() override {
    olc::rcode rc = CreateFrameBuffer();
    if (rc != olc::rcode::OK) return rc;
    // Open keyboard event device
    fd_kbd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
    if (fd_kbd < 0) {
      std::cerr << "WARNING: Cannot open /dev/input/event0: "
      << strerror(errno) << std::endl;
    }
    return olc::rcode::OK;
  }
  
  // Called once after the engine thread exits
  olc::rcode ApplicationCleanUp() override {
    if (fd_kbd >= 0) close(fd_kbd);
    if (fbg_instance) {
      fbg_close(fbg_instance);
      fbg_instance = nullptr;
    }
    return olc::rcode::OK;
  }
  
  // No setup needed here
  olc::rcode ThreadStartUp() override { return olc::rcode::OK; }
  // No teardown here
  olc::rcode ThreadCleanUp() override {
    if (olc::renderer) olc::renderer->DestroyDevice();
    return olc::rcode::OK;
  }
  
  // Create graphics context
  olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC,
  const olc::vi2d& vViewPos,
  const olc::vi2d& vViewSize) override {
    if (olc::renderer) {
      olc::renderer->CreateDevice({ this }, bFullScreen, bEnableVSYNC);
      olc::renderer->UpdateViewport(vViewPos, vViewSize);
    }
    return olc::rcode::OK;
  }
  
  // Tell PGE the "window" size
  olc::rcode CreateWindowPane(const olc::vi2d& /*pos*/,
  olc::vi2d& vSize,
  bool /*fsm*/) override {
    vSize = { width, height };
    ptrPGE->olc_UpdateWindowSize(width, height);
    return olc::rcode::OK;
  }
  
  olc::rcode SetWindowTitle(const std::string& s) override {
    std::cout << "PGE Framebuffer: " << s << std::endl;
    return olc::rcode::OK;
  }
  
  // Main loop
  olc::rcode StartSystemEventLoop() override {
    while (ptrPGE->olc_IsRunning()) {
      HandleSystemEvent();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return olc::rcode::OK;
  }
  
  // Poll keyboard and update PGE key state
  olc::rcode HandleSystemEvent() override {
    if (fd_kbd < 0) return olc::rcode::OK;
    struct input_event ev;
    ssize_t n;
    while ((n = read(fd_kbd, &ev, sizeof(ev))) > 0) {
      if (ev.type == EV_KEY) {
        olc::Key k = EvdevCodeToOlcKey(ev.code);
        if (k != olc::Key::NONE) {                                        
          bool pressed = (ev.value != 0);
          ptrPGE->olc_UpdateKeyState(static_cast<int>(k), pressed);
        }
      }
    }
    return olc::rcode::OK;
  }
  
  // Accessors
  struct _fbg* GetFrameBufferInstance() const { return fbg_instance; }
  int GetScreenWidth() const { return width; }
  int GetScreenHeight() const { return height; }
};

// Software Renderer using libfbg
class LinuxFBGRenderer : public olc::Renderer {
  private:
  struct _fbg* fbg_instance = nullptr;
  
  public:
  // Called from CreateGraphics
  olc::rcode CreateDevice(std::vector<void*> params, bool, bool) override {
    auto platform = static_cast<LinuxFBGPlatform*>(params[0]);
    fbg_instance = platform->GetFrameBufferInstance();
    ptrPGE = platform->ptrPGE;
    return olc::rcode::OK;
  }
  
  olc::rcode DestroyDevice() override { return olc::rcode::OK; }
  void PrepareDevice() override { if (fbg_instance) fbg_clear(fbg_instance, 0); }
  void PrepareDrawing() override { if (fbg_instance) fbg_clear(fbg_instance, 0); }
  
  void DisplayFrame() override {
    auto spr = ptrPGE->GetDrawTarget();
    for (int y = 0; y < spr->height; ++y) {
      for (int x = 0; x < spr->width; ++x) {
        auto p = spr->GetPixel(x, y);
        fbg_pixela(fbg_instance, x, y, p.b, p.g, p.r, p.a);
      }
    }
    fbg_draw(fbg_instance);
    fbg_flip(fbg_instance);
  }
  
  void SetDecalMode(const olc::DecalMode&) override {}
  void UpdateViewport(const olc::vi2d&, const olc::vi2d&) override {}
  void ClearBuffer(olc::Pixel, bool) override { if (fbg_instance) fbg_clear(fbg_instance, 0); }
  
  uint32_t CreateTexture(uint32_t, uint32_t, bool, bool) override { return 0; }
  void UpdateTexture(uint32_t, olc::Sprite*) override {}
  void ReadTexture(uint32_t, olc::Sprite*) override {}
  uint32_t DeleteTexture(uint32_t) override { return 0; }
  void ApplyTexture(uint32_t) override {}
  
  void DrawLayerQuad(const olc::vf2d& off, const olc::vf2d& sc, const olc::Pixel tint) override {
    if (!fbg_instance) return;
    int w = int(sc.x * fbg_instance->width);
    int h = int(sc.y * fbg_instance->height);
    fbg_fill(fbg_instance, tint.b, tint.g, tint.r);
    fbg_frect(fbg_instance, int(off.x), int(off.y), w, h);
  }
  
  void DrawDecal(const olc::DecalInstance& decal) override {
    if (!decal.decal || !fbg_instance) return;
    auto spr = decal.decal->sprite;
    for (uint32_t i = 0; i < decal.points; ++i) {
      int sx = int(decal.pos[i].x);
      int sy = int(decal.pos[i].y);
      int u  = int(decal.uv[i].x * spr->width);
      int v  = int(decal.uv[i].y * spr->height);
      auto p = spr->GetPixel(u, v);
      olc::Pixel tp(p.b, p.g, p.r, p.a);
      fbg_pixela(fbg_instance, sx, sy, tp.r, tp.g, tp.b, tp.a);
    }
  }
};
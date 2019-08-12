#include "window.h"

#include <functional>
#include <utility>
#include <thread>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

void Window::init()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  window = SDL_CreateWindow("RayTracer",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            width,
                            height,
                            SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

  if (window == nullptr) {
    throw std::runtime_error("Failed to create SDL window");
  }

  // core = Core(window);
  core.init(window);
}

void Window::cleanup()
{
  core.cleanup();
  SDL_DestroyWindow(window);
  SDL_Quit();
}
/**
* @brief
*
* @return true
* @return false
*/
bool Window::rendering_loop()
{
  SDL_Event event;
  bool      loop   = true;
  bool      resize = false;
  bool      result = true;

  while (loop) {
    if (SDL_PollEvent(&event)) {
      // Process event
      switch (event.type) {  //xevent->response_type & 0x7f) {
          // resize
        case SDL_WINDOWEVENT: {
          std::cout << "SDL_WINDOWEVENT:\n\t";
          switch (event.window.event) {
            case SDL_WINDOWEVENT_RESIZED: {
              std::cout << "SDL_WINDOWEVENT_RESIZED:\n\t width: " << width << " height: " << height << "\n";
              width  = event.window.data1;
              height = event.window.data2;
              std::cout << "width: " << width << " height: " << height << "\n";
              if (((event.window.data1 > 0) && (width != event.window.data1)) ||
                  ((event.window.data2 > 0) && height != event.window.data2)) {
                resize = true;
                width  = event.window.data1;
                height = event.window.data2;
                std::cout << "width: " << width << " height: " << height << "\n";
              }
              std::cout << "width: " << width << " height: " << height << "\n";
            } break;
          }
        } break;
          /*          //close // Eric: Unneeded with SDL?
        case XCB_CLIENT_MESSAGE:
          if ((*(xcb_client_message_event_t*)xevent).data.data32[0] == (*delete_reply).atom) {
            loop = false;
            free(delete_reply);
          }
          break;
*/
        case SDL_KEYUP:
          loop = false;
          break;
        case SDL_QUIT:
          loop = false;
          break;
      }
      // free(xevent);
    } else {
      // Draw
      if (resize) {
        resize = false;
        if (!core.on_window_size_changed()) {
          result = false;
          break;
        }
      }
      if (core.ready_to_draw()) {
        if (!core.draw()) {
          result = false;
          break;
        }
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    }
  }

  cleanup();
  // \todo
  // vkDeviceWaitIdle(device);
  return result;
}

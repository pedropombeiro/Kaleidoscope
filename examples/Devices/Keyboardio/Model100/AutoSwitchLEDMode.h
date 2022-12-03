
#include "Kaleidoscope-LED-Stalker.h"
#include "Kaleidoscope-Colormap.h"

namespace kaleidoscope {
namespace plugin {

class AutoSwitchLEDMode : public Plugin {
 public:
  const uint16_t timeout = 2000;

  EventHandlerResult onKeyEvent(KeyEvent &event) {
    start_time_ = Runtime.millisAtCycleStart();
    if (timer_running_ && keyToggledOn(event.state) && event.key.isLayerKey()) {
      ::ColormapEffect.activate();
    }
    if (!timer_running_ && keyToggledOn(event.state) && !event.key.isLayerKey()) {
      timer_running_ = true;
      ::StalkerEffect.activate();
    }
    return EventHandlerResult::OK;
  }

  EventHandlerResult afterEachCycle() {
    if (Runtime.hasTimeExpired(start_time_, timeout)) {
      timer_running_ = false;
      ::ColormapEffect.activate();
    }
    return EventHandlerResult::OK;
  }

 private:
  bool timer_running_ = false;
  uint16_t start_time_;
};

}  // namespace plugin
}  // namespace kaleidoscope

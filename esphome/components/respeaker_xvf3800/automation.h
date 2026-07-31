#pragma once
#include "respeaker_xvf3800.h"

#include "esphome/core/automation.h"

namespace esphome {
namespace respeaker_xvf3800 {

template<typename... Ts> class RespeakerXVF3800FlashAction : public Action<Ts...> {
 public:
  RespeakerXVF3800FlashAction(RespeakerXVF3800 *parent) : parent_(parent) {}
  void play(Ts... x) override { this->parent_->start_dfu_update(); }

 protected:
  RespeakerXVF3800 *parent_;
};

// Generic control-parameter write (float) — see set_control_param_float() in
// respeaker_xvf3800.h for why this exists (USB control interface unavailable on
// the "With Case" hardware variant).
template<typename... Ts> class RespeakerXVF3800SetControlParamFloatAction : public Action<Ts...> {
 public:
  RespeakerXVF3800SetControlParamFloatAction(RespeakerXVF3800 *parent) : parent_(parent) {}
  TEMPLATABLE_VALUE(uint8_t, resid)
  TEMPLATABLE_VALUE(uint8_t, cmd)
  TEMPLATABLE_VALUE(float, value)
  void play(Ts... x) override {
    this->parent_->set_control_param_float(this->resid_.value(x...), this->cmd_.value(x...),
                                            this->value_.value(x...));
  }

 protected:
  RespeakerXVF3800 *parent_;
};

template<typename... Ts> class RespeakerXVF3800SaveConfigurationAction : public Action<Ts...> {
 public:
  RespeakerXVF3800SaveConfigurationAction(RespeakerXVF3800 *parent) : parent_(parent) {}
  void play(Ts... x) override { this->parent_->save_configuration(); }

 protected:
  RespeakerXVF3800 *parent_;
};

template<typename... Ts> class RespeakerXVF3800ClearConfigurationAction : public Action<Ts...> {
 public:
  RespeakerXVF3800ClearConfigurationAction(RespeakerXVF3800 *parent) : parent_(parent) {}
  void play(Ts... x) override { this->parent_->clear_configuration(); }

 protected:
  RespeakerXVF3800 *parent_;
};
#ifdef USE_RESPEAKER_XVF3800_STATE_CALLBACK
class DFUStartTrigger : public Trigger<> {
 public:
  explicit DFUStartTrigger(RespeakerXVF3800 *parent) {
    parent->add_on_state_callback(
        [this, parent](DFUAutomationState state, float progress, RespeakerXVF3800UpdaterStatus error) {
          if (state == DFU_START && !parent->is_failed()) {
            trigger();
          }
        });
  }
};

class DFUProgressTrigger : public Trigger<float> {
 public:
  explicit DFUProgressTrigger(RespeakerXVF3800 *parent) {
    parent->add_on_state_callback(
        [this, parent](DFUAutomationState state, float progress, RespeakerXVF3800UpdaterStatus error) {
          if (state == DFU_IN_PROGRESS && !parent->is_failed()) {
            trigger(progress);
          }
        });
  }
};

class DFUEndTrigger : public Trigger<> {
 public:
  explicit DFUEndTrigger(RespeakerXVF3800 *parent) {
    parent->add_on_state_callback(
        [this, parent](DFUAutomationState state, float progress, RespeakerXVF3800UpdaterStatus error) {
          if (state == DFU_COMPLETE && !parent->is_failed()) {
            trigger();
          }
        });
  }
};

class DFUErrorTrigger : public Trigger<uint8_t> {
 public:
  explicit DFUErrorTrigger(RespeakerXVF3800 *parent) {
    parent->add_on_state_callback(
        [this, parent](DFUAutomationState state, float progress, RespeakerXVF3800UpdaterStatus error) {
          if (state == DFU_ERROR && !parent->is_failed()) {
            trigger(error);
          }
        });
  }
};
#endif
}  // namespace respeaker_xvf3800
}  // namespace esphome

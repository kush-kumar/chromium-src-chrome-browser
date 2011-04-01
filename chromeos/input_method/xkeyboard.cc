// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/chromeos/input_method/xkeyboard.h"

#include <utility>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>

#include "base/memory/singleton.h"
#include "base/logging.h"
#include "base/string_util.h"
#include "base/process_util.h"

namespace chromeos {
namespace input_method {
namespace {

// The default keyboard layout name in the xorg config file.
const char kDefaultLayoutName[] = "us";
// The command we use to set the current XKB layout and modifier key mapping.
// TODO(yusukes): Use libxkbfile.so instead of the command (crosbug.com/13105)
const char kSetxkbmapCommand[] = "/usr/bin/setxkbmap";
// See the comment at ModifierKey in the .h file.
ModifierKey kCustomizableKeys[] = {
  kSearchKey,
  kLeftControlKey,
  kLeftAltKey
};

// This is a wrapper class around Display, that opens and closes X display in
// the constructor and destructor.
class ScopedDisplay {
 public:
  explicit ScopedDisplay(Display* display) : display_(display) {
    if (!display_) {
      LOG(ERROR) << "NULL display_ is passed";
    }
  }

  ~ScopedDisplay() {
    if (display_) {
      XCloseDisplay(display_);
    }
  }

  Display* get() const {
    return display_;
  }

 private:
  Display* display_;

  DISALLOW_COPY_AND_ASSIGN(ScopedDisplay);
};

// A singleton class which wraps the setxkbmap command.
class XKeyboard {
 public:
  // Returns the singleton instance of the class. Use LeakySingletonTraits.
  // We don't delete the instance at exit.
  static XKeyboard* GetInstance() {
    return Singleton<XKeyboard, LeakySingletonTraits<XKeyboard> >::get();
  }

  // Sets the current keyboard layout to |layout_name|. This function does not
  // change the current mapping of the modifier keys. Returns true on success.
  bool SetLayout(const std::string& layout_name) {
    if (SetLayoutInternal(layout_name, current_modifier_map_)) {
      current_layout_name_ = layout_name;
      return true;
    }
    return false;
  }

  // Remaps modifier keys. This function does not change the current keyboard
  // layout. Returns true on success.
  bool RemapModifierKeys(const ModifierMap& modifier_map) {
    if (SetLayoutInternal(current_layout_name_, modifier_map)) {
      current_modifier_map_ = modifier_map;
      return true;
    }
    return false;
  }

  // Turns on and off the auto-repeat of the keyboard. Returns true on success.
  // TODO(yusukes): Remove this function.
  bool SetAutoRepeatEnabled(bool enabled) {
    ScopedDisplay display(XOpenDisplay(NULL));
    if (!display.get()) {
      return false;
    }
    if (enabled) {
      XAutoRepeatOn(display.get());
    } else {
      XAutoRepeatOff(display.get());
    }
    DLOG(INFO) << "Set auto-repeat mode to: " << (enabled ? "on" : "off");
    return true;
  }

  // Sets the auto-repeat rate of the keyboard, initial delay in ms, and repeat
  // interval in ms.  Returns true on success.
  // TODO(yusukes): Call this function in non-UI thread or in an idle callback.
  bool SetAutoRepeatRate(const AutoRepeatRate& rate) {
    // TODO(yusukes): write auto tests for the function.
    ScopedDisplay display(XOpenDisplay(NULL));
    if (!display.get()) {
      return false;
    }

    DLOG(INFO) << "Set auto-repeat rate to: "
               << rate.initial_delay_in_ms << " ms delay, "
               << rate.repeat_interval_in_ms << " ms interval";
    if (XkbSetAutoRepeatRate(display.get(), XkbUseCoreKbd,
                             rate.initial_delay_in_ms,
                             rate.repeat_interval_in_ms) != True) {
      LOG(ERROR) << "Failed to set auto-repeat rate";
      return false;
    }
    return true;
  }

 private:
  friend struct DefaultSingletonTraits<XKeyboard>;

  XKeyboard() : current_layout_name_(kDefaultLayoutName) {
    for (size_t i = 0; i < arraysize(kCustomizableKeys); ++i) {
      ModifierKey key = kCustomizableKeys[i];
      current_modifier_map_.push_back(ModifierKeyPair(key, key));
    }
  }
  ~XKeyboard() {
  }

  // This function is used by SetLayout() and RemapModifierKeys(). Calls
  // setxkbmap command if needed, and updates the last_full_layout_name_ cache.
  bool SetLayoutInternal(const std::string& layout_name,
                         const ModifierMap& modifier_map) {
    const std::string layouts_to_set = CreateFullXkbLayoutName(
        layout_name, modifier_map);
    if (layouts_to_set.empty()) {
      return false;
    }

    const std::string current_layout = CreateFullXkbLayoutName(
        current_layout_name_, current_modifier_map_);
    if (current_layout == layouts_to_set) {
      DLOG(INFO) << "The requested layout is already set: " << layouts_to_set;
      return true;
    }

    // Turn off caps lock if there is no kCapsLockKey in the remapped keys.
    if (!ContainsModifierKeyAsReplacement(
            modifier_map, kCapsLockKey)) {
      SetCapsLockEnabled(false);
    }

    ExecuteSetLayoutCommand(layouts_to_set);
    return true;
  }

  // Executes 'setxkbmap -layout ...' command asynchronously.
  // TODO(yusukes): Use libxkbfile.so instead of the command (crosbug.com/13105)
  void ExecuteSetLayoutCommand(const std::string& layouts_to_set) {
    std::vector<std::string> argv;
    base::file_handle_mapping_vector fds_to_remap;
    base::ProcessHandle handle = base::kNullProcessHandle;

    argv.push_back(kSetxkbmapCommand);
    argv.push_back("-layout");
    argv.push_back(layouts_to_set);
    const bool result = base::LaunchApp(argv,
                                        fds_to_remap,  // No remapping.
                                        false,  // Don't wait.
                                        &handle);
    if (!result) {
      LOG(ERROR) << "Failed to execute setxkbmap: " << layouts_to_set;
      return;
    }

    // g_child_watch_add is necessary to prevent the process from becoming a
    // zombie.
    const base::ProcessId pid = base::GetProcId(handle);
    g_child_watch_add(pid,
                      reinterpret_cast<GChildWatchFunc>(OnSetLayoutFinish),
                      this);
  }

  static void OnSetLayoutFinish(GPid pid, gint status, XKeyboard* self) {
    DLOG(INFO) << "OnSetLayoutFinish: pid=" << pid;
  }

  // The XKB layout name which we set last time like "us" and "us(dvorak)".
  std::string current_layout_name_;
  // The mapping of modifier keys we set last time.
  ModifierMap current_modifier_map_;

  DISALLOW_COPY_AND_ASSIGN(XKeyboard);
};

}  // namespace

std::string CreateFullXkbLayoutName(const std::string& layout_name,
                                    const ModifierMap& modifier_map) {
  static const char kValidLayoutNameCharacters[] =
      "abcdefghijklmnopqrstuvwxyz0123456789()-_";

  if (layout_name.empty()) {
    LOG(ERROR) << "Invalid layout_name: " << layout_name;
    return "";
  }

  if (layout_name.find_first_not_of(kValidLayoutNameCharacters) !=
      std::string::npos) {
    LOG(ERROR) << "Invalid layout_name: " << layout_name;
    return "";
  }

  std::string use_search_key_as_str;
  std::string use_left_control_key_as_str;
  std::string use_left_alt_key_as_str;

  for (size_t i = 0; i < modifier_map.size(); ++i) {
    std::string* target = NULL;
    switch (modifier_map[i].original) {
      case kSearchKey:
        target = &use_search_key_as_str;
        break;
      case kLeftControlKey:
        target = &use_left_control_key_as_str;
        break;
      case kLeftAltKey:
        target = &use_left_alt_key_as_str;
        break;
      default:
        break;
    }
    if (!target) {
      LOG(ERROR) << "We don't support remaping "
                 << ModifierKeyToString(modifier_map[i].original);
      return "";
    }
    if (!(target->empty())) {
      LOG(ERROR) << ModifierKeyToString(modifier_map[i].original)
                 << " appeared twice";
      return "";
    }
    *target = ModifierKeyToString(modifier_map[i].replacement);
  }

  if (use_search_key_as_str.empty() ||
      use_left_control_key_as_str.empty() ||
      use_left_alt_key_as_str.empty()) {
    LOG(ERROR) << "Incomplete ModifierMap: size=" << modifier_map.size();
    return "";
  }

  std::string full_xkb_layout_name =
      StringPrintf("%s+chromeos(%s_%s_%s)", layout_name.c_str(),
                   use_search_key_as_str.c_str(),
                   use_left_control_key_as_str.c_str(),
                   use_left_alt_key_as_str.c_str());

  if ((full_xkb_layout_name.substr(0, 3) != "us+") &&
      (full_xkb_layout_name.substr(0, 3) != "us(")) {
    full_xkb_layout_name += ",us";
  }

  return full_xkb_layout_name;
}

// This function is only for unittest.
bool CapsLockIsEnabled() {
  ScopedDisplay display(XOpenDisplay(NULL));
  if (!display.get()) {
    return false;
  }
  XkbStateRec status;
  XkbGetState(display.get(), XkbUseCoreKbd, &status);
  return status.locked_mods & LockMask;
}

// TODO(yusukes): Call this function in non-UI thread or in an idle callback.
void SetCapsLockEnabled(bool enable_caps_lock) {
  ScopedDisplay display(XOpenDisplay(NULL));
  if (!display.get()) {
    return;
  }
  XkbLockModifiers(
      display.get(), XkbUseCoreKbd, LockMask, enable_caps_lock ? LockMask : 0);
}

bool ContainsModifierKeyAsReplacement(
    const ModifierMap& modifier_map, ModifierKey key) {
  for (size_t i = 0; i < modifier_map.size(); ++i) {
    if (modifier_map[i].replacement == key) {
      return true;
    }
  }
  return false;
}

bool SetCurrentKeyboardLayoutByName(const std::string& layout_name) {
  return XKeyboard::GetInstance()->SetLayout(layout_name);
}

bool RemapModifierKeys(const ModifierMap& modifier_map) {
  return XKeyboard::GetInstance()->RemapModifierKeys(modifier_map);
}

bool SetAutoRepeatEnabled(bool enabled) {
  return XKeyboard::GetInstance()->SetAutoRepeatEnabled(enabled);
}

bool SetAutoRepeatRate(const AutoRepeatRate& rate) {
  return XKeyboard::GetInstance()->SetAutoRepeatRate(rate);
}

}  // namespace input_method
}  // namespace chromeos

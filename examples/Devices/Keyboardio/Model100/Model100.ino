// -*- mode: c++ -*-
// Copyright 2016-2022 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

/**
 * These #include directives pull in the Kaleidoscope firmware core,
 * as well as the Kaleidoscope plugins we use in the Model 100's firmware
 */

// The Kaleidoscope core
#include "Kaleidoscope.h"

// Support for storing the keymap in EEPROM
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"

// Support for communicating with the host via a simple Serial protocol
#include "Kaleidoscope-FocusSerial.h"

// Support for querying the firmware version via Focus
#include "Kaleidoscope-FirmwareVersion.h"

// Support for keys that move the mouse
#include "Kaleidoscope-MouseKeys.h"

// Support for macros
#include "Kaleidoscope-Macros.h"

// Support for controlling the keyboard's LEDs
#include "Kaleidoscope-LEDControl.h"

// Support for an LED mode that makes all the LEDs 'breathe'
// #include "Kaleidoscope-LEDEffect-Breathe.h"

// Support for an LED mode that lights up the keys as you press them
#include "Kaleidoscope-LED-Stalker.h"

// Support for shared palettes for other plugins, like Colormap below
#include "Kaleidoscope-LED-Palette-Theme.h"

// Support for an LED mode that lets one configure per-layer color maps
#include "Kaleidoscope-Colormap.h"

// Support for turning the LEDs off after a certain amount of time
#include "Kaleidoscope-IdleLEDs.h"

// Support for setting and saving the default LED mode
// #include "Kaleidoscope-DefaultLEDModeConfig.h"

// Support for changing the brightness of the LEDs
#include "Kaleidoscope-LEDBrightnessConfig.h"

// Support for Keyboardio's internal keyboard testing mode
#include "Kaleidoscope-HardwareTestMode.h"

// Support for host power management (suspend & wakeup)
#include "Kaleidoscope-HostPowerManagement.h"

// Support for magic combos (key chords that trigger an action)
#include "Kaleidoscope-MagicCombo.h"

// Support for USB quirks, like changing the key state report protocol
#include "Kaleidoscope-USB-Quirks.h"

// Support for secondary actions on keys
#include "Kaleidoscope-Qukeys.h"

// Support for one-shot modifiers and layer keys
// #include "Kaleidoscope-OneShot.h"
// #include "Kaleidoscope-Escape-OneShot.h"

// Support for dynamic, Chrysalis-editable macros
// #include "Kaleidoscope-DynamicMacros.h"

// Support for editable layer names
#include "Kaleidoscope-LayerNames.h"

// Any active modifier on the keyboard will have the LED under it highlighted
#include "Kaleidoscope-LED-ActiveModColor.h"

#include <Kaleidoscope-TapDance.h>

// #include "AutoSwitchLEDMode.h"

// kaleidoscope::plugin::AutoSwitchLEDMode AutoSwitchLEDMode;

/** This 'enum' is a list of all the macros used by the Model 100's firmware
  * The names aren't particularly important. What is important is that each
  * is unique.
  *
  * These are the names of your macros. They'll be used in two places.
  * The first is in your keymap definitions. There, you'll use the syntax
  * `M(MACRO_NAME)` to mark a specific keymap position as triggering `MACRO_NAME`
  *
  * The second usage is in the 'switch' statement in the `macroAction` function.
  * That switch statement actually runs the code associated with a macro when
  * a macro key is pressed.
  */

enum {
  MACRO_VERSION_INFO,
  MACRO_ANY,
};


/** The Model 100's key layouts are defined as 'keymaps'. By default, there are three
  * keymaps: The standard QWERTY keymap, the "Function layer" keymap and the "Numpad"
  * keymap.
  *
  * Each keymap is defined as a list using the 'KEYMAP_STACKED' macro, built
  * of first the left hand's layout, followed by the right hand's layout.
  *
  * Keymaps typically consist mostly of `Key_` definitions. There are many, many keys
  * defined as part of the USB HID Keyboard specification. You can find the names
  * (if not yet the explanations) for all the standard `Key_` defintions offered by
  * Kaleidoscope in these files:
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs/keyboard.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs/consumerctl.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs/sysctl.h
  *    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs/keymaps.h
  *
  * Additional things that should be documented here include
  *   using ___ to let keypresses fall through to the previously active layer
  *   using XXX to mark a keyswitch as 'blocked' on this layer
  *   using ShiftToLayer() and LockLayer() keys to change the active keymap.
  *   keeping NUM and FN consistent and accessible on all layers
  *
  * The PROG key is special, since it is how you indicate to the board that you
  * want to flash the firmware. However, it can be remapped to a regular key.
  * When the keyboard boots, it first looks to see whether the PROG key is held
  * down; if it is, it simply awaits further flashing instructions. If it is
  * not, it continues loading the rest of the firmware and the keyboard
  * functions normally, with whatever binding you have set to PROG. More detail
  * here: https://community.keyboard.io/t/how-the-prog-key-gets-you-into-the-bootloader/506/8
  *
  * The "keymaps" data structure is a list of the keymaps compiled into the firmware.
  * The order of keymaps in the list is important, as the ShiftToLayer(#) and LockLayer(#)
  * macros switch to key layers based on this list.
  *
  *

  * A key defined as 'ShiftToLayer(FUNCTION)' will switch to FUNCTION while held.
  * Similarly, a key defined as 'LockLayer(NUMPAD)' will switch to NUMPAD when tapped.
  */

/**
  * Layers are "0-indexed" -- That is the first one is layer 0. The second one is layer 1.
  * The third one is layer 2.
  * This 'enum' lets us use names like QWERTY, FUNCTION, and NUMPAD in place of
  * the numbers 0, 1 and 2.
  *
  */

enum {
  PRIMARY,
  NUMPAD,
  FUNCTION,
  META,
};  // layers

enum {
  TD_LSHIFT,
  TD_RSHIFT,
  TD_LCMD,
  TD_RALT,
  TD_LCTRL,
  TD_RCTRL
}; // tap dance

/* This comment temporarily turns off astyle's indent enforcement
 *   so we can make the keymaps actually resemble the physical key layout better
 */
// clang-format off

KEYMAPS(

  [PRIMARY] = KEYMAP_STACKED
  (Key_Escape,   Key_1, Key_2, Key_3, Key_4, Key_5, ___,
   Key_Backtick, Key_Q, Key_W, Key_E, Key_R, Key_T, Key_Tab,
   Key_Home,     Key_A, Key_S, Key_D, Key_F, Key_G,
   Key_End,      Key_Z, Key_X, Key_C, Key_V, Key_B, Key_Escape,
   TD(TD_LCTRL), Key_Backspace, TD(TD_LCMD), TD(TD_LSHIFT),
   ShiftToLayer(META),

   LCTRL(LGUI(Key_LeftAlt)), Key_6,       Key_7,        Key_8,     Key_9,      Key_0,         LockLayer(NUMPAD),
   Key_Enter,                Key_Y,       Key_U,        Key_I,     Key_O,      Key_P,         Key_Equals,
                             Key_H,       Key_J,        Key_K,     Key_L,      Key_Semicolon, Key_Quote,
   LGUI(Key_Spacebar),       Key_N,       Key_M,        Key_Comma, Key_Period, Key_Slash,     Key_Minus,
   TD(TD_RSHIFT),            TD(TD_RALT), Key_Spacebar, TD(TD_RCTRL),
   ShiftToLayer(FUNCTION)),

  [NUMPAD] =  KEYMAP_STACKED
  (___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___, ___, ___, ___,
   ___, ___, ___, ___,
   ___,

   M(MACRO_VERSION_INFO),  ___, Key_7, Key_8,      Key_9,              Key_KeypadSubtract, ___,
   ___,                    ___, Key_4, Key_5,      Key_6,              Key_KeypadAdd,      ___,
                           ___, Key_1, Key_2,      Key_3,              Key_Equals,         ___,
   ___,                    ___, Key_0, Key_Period, Key_KeypadMultiply, Key_KeypadDivide,   Key_Enter,
   ___, ___, ___, ___,
   ___),

  [FUNCTION] =  KEYMAP_STACKED
  (___,          Key_F1,            Key_F2,      Key_F3,            Key_F4,        Key_F5,           Key_LEDEffectPrevious,
   Key_Tab,      Key_mouseScrollDn, Key_mouseUp, Key_mouseScrollUp, Key_mouseBtnR, Key_mouseWarpEnd, Key_mouseWarpNE,
                 Key_PageUp,        Key_mouseL,  Key_mouseDn,       Key_mouseR,    Key_mouseBtnL,    Key_mouseWarpNW,
   Key_PageDown, Key_PrintScreen,   Key_Insert,  ___,               Key_mouseBtnM, Key_mouseWarpSW,  Key_mouseWarpSE,
   ___,          Key_Delete, ___, ___,
   ___,

   ___,                Key_F6,        Key_F7,                   Key_F8,                   Key_F9,          Key_F10,          Key_F11,
   ___,                ___,           Key_LeftCurlyBracket,     Key_RightCurlyBracket,    Key_LeftBracket, Key_RightBracket, Key_F12,
                       Key_LeftArrow, Key_DownArrow,            Key_UpArrow,              Key_RightArrow,  Consumer_ScanPreviousTrack, Consumer_ScanNextTrack,
   LALT(Key_Spacebar), Consumer_Mute, Consumer_VolumeDecrement, Consumer_VolumeIncrement, Consumer_PlaySlashPause,             Key_Backslash,    Key_Pipe,
   ___, ___, Key_Enter, ___,
   ___),

  [META] =  KEYMAP_STACKED
  (___, LSHIFT(LGUI(Key_Spacebar)), Key_F2, Key_F3, Key_F4,                    Key_F5, ___,
   ___, ___,                        ___,    ___,    LCTRL(LALT(LGUI(Key_R))),  ___,    ___,
        HYPER(Key_UpArrow),         ___,    ___,    ___,                       ___,    ___,
   HYPER(Key_DownArrow),            ___,    ___,    LSHIFT(LALT(LGUI(Key_C))), ___,    ___,    ___,
   ___,                             ___,    ___,    ___,
   ___,

   ___,               Key_F6,                     Key_F7,                              Key_F8,                              Key_KeypadLeftParen,         Key_KeypadRightParen, ___,
   ___,               ___,                        Key_LeftCurlyBracket,                Key_RightCurlyBracket,               Key_LeftBracket,             Key_RightBracket,     ___,
                      LCTRL(LALT(Key_LeftArrow)), LCTRL(Key_DownArrow),                LCTRL(Key_UpArrow),                  LCTRL(LALT(Key_RightArrow)), ___,                  ___,
   ___,               ___,                        Consumer_DisplayBrightnessDecrement, Consumer_DisplayBrightnessIncrement, Consumer_PlaySlashPause,     Key_Backslash,        Key_Pipe,
   ___, ___, ___, ___,
   ___)
) // KEYMAPS(


// Colors names of the EGA palette, for convenient use in colormaps. Should
// match the palette definition below. Optional, one can just use the indexes
// directly, too.
enum {
  BLACK,
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  ORANGE,
  LIGHT_GRAY,
  DARK_GRAY,
  BRIGHT_BLUE,
  BRIGHT_GREEN,
  BRIGHT_CYAN,
  BRIGHT_RED,
  BRIGHT_MAGENTA,
  YELLOW,
  WHITE
};

// Define an EGA palette. Conveniently, that's exactly 16 colors, just like the
// limit of LEDPaletteTheme.
PALETTE(
    CRGB(0x00, 0x00, 0x00),  // [0x0] black
    CRGB(0x00, 0x00, 0xaa),  // [0x1] blue
    CRGB(0x00, 0xaa, 0x00),  // [0x2] green
    CRGB(0x00, 0xaa, 0xaa),  // [0x3] cyan
    CRGB(0xaa, 0x00, 0x00),  // [0x4] red
    CRGB(0xaa, 0x00, 0x00),  // [0x5] magenta
    CRGB(0xFF, 0xa5, 0x00),  // [0x6] orange
    CRGB(0xaa, 0xaa, 0xaa),  // [0x7] light gray
    CRGB(0x55, 0x55, 0x55),  // [0x8] dark gray
    CRGB(0x55, 0x55, 0xff),  // [0x9] bright blue
    CRGB(0x55, 0xff, 0x55),  // [0xa] bright green
    CRGB(0x55, 0xff, 0xff),  // [0xb] bright cyan
    CRGB(0xff, 0x00, 0x00),  // [0xc] bright red
    CRGB(0xff, 0x55, 0xff),  // [0xd] bright magenta
    CRGB(0xff, 0xff, 0x55),  // [0xe] yellow
    CRGB(0xff, 0xff, 0xff)   // [0xf] white
)

COLORMAPS(
  // List of palette indexes for each key, using the same layout
  // as the `COLORMAP` macro does for keys.
  [PRIMARY] = COLORMAP_STACKED
  (BRIGHT_RED,   BLACK,  BLACK,        BLACK,        BLACK,        BLACK,        BLACK,
   BLACK,        BLACK,  BLACK,        BLACK,        BLACK,        BLACK,        BLACK,
                 BLACK,  BRIGHT_BLUE,  BRIGHT_BLUE,  BRIGHT_BLUE,  BRIGHT_BLUE,  BLACK,
   BLACK,        BLACK,  BLACK,        BLACK,        BLACK,        BLACK,        BRIGHT_RED,
   BLACK,        ORANGE, BLACK, BLACK,
   BLACK,

   BLACK, BLACK, BLACK,         BLACK,         BLACK,         BLACK,         BLACK,
   GREEN, BLACK, BLACK,         BLACK,         BLACK,         BLACK,         BLACK,
          BLACK, BRIGHT_BLUE,   BRIGHT_BLUE,   BRIGHT_BLUE,   BRIGHT_BLUE,   BLACK,
   BLACK, BLACK, BLACK,         BLACK,         BLACK,         BLACK,         BLACK,
   BLACK, BLACK, ORANGE, BLACK,
   BLACK,
  ),

  [NUMPAD] =  COLORMAP_STACKED
  (BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
   BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
   BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
   BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
   BLACK, BLACK, BLACK, BLACK,
   BLACK,

   BLACK,  BLACK, BRIGHT_BLUE, BRIGHT_BLUE, BRIGHT_BLUE,   YELLOW, BLACK,
   BLACK,  BLACK, BRIGHT_BLUE, BRIGHT_BLUE, BRIGHT_BLUE,   YELLOW, BLACK,
           BLACK, BRIGHT_BLUE, BRIGHT_BLUE, BRIGHT_BLUE,   YELLOW, BLACK,
   BLACK,  BLACK, BRIGHT_BLUE, BRIGHT_BLUE, YELLOW,        YELLOW, GREEN,
   BLACK, BLACK, BLACK, BLACK,
   BLACK),

  [FUNCTION] =  COLORMAP_STACKED
  (BLACK,  BLACK,      BLACK, BLACK, BLACK,  BLACK,  BLACK,
   BLACK,  GREEN,      CYAN,  GREEN, YELLOW, BLACK,  BLACK,
           ORANGE,     CYAN,  CYAN,  CYAN,   YELLOW, BLACK,
   ORANGE, BLACK,      BLACK, BLACK, YELLOW, BLACK,  BLACK,
   BLACK,  BRIGHT_RED, BLACK, BLACK,
   BLACK,

   BLACK,        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,
   BLACK,        BLACK,  BLACK,  BLACK,  BLACK,  BLACK,  BLACK,
                 GREEN,  GREEN,  GREEN,  GREEN,  ORANGE, ORANGE,
   BRIGHT_RED,   ORANGE, ORANGE, ORANGE, ORANGE, BLACK,  BLACK,
   BLACK, BLACK, GREEN,  BLACK,
   BLACK),

  [META] =  COLORMAP_STACKED
  (BLACK, GREEN, BLACK, BLACK, BLACK,  BLACK, BLACK,
   BLACK, BLACK, BLACK, BLACK, ORANGE, BLACK, BLACK,
          BLACK, BLACK, BLACK, BLACK,  BLACK, BLACK,
   BLACK, BLACK, BLACK, GREEN, BLACK,  BLACK, BLACK,
   BLACK, BLACK, BLACK, BLACK,
   BLACK,

   BLACK, BLACK,        BLACK,  BLACK,  YELLOW,        YELLOW, BLACK,
   BLACK, BLACK,        YELLOW, YELLOW, YELLOW,        YELLOW, BLACK,
          BRIGHT_RED,   BLACK,  BLACK,  BRIGHT_RED,    BLACK,  BLACK,
   BLACK, BLACK,        BLACK,  BLACK,  BLACK,         YELLOW, YELLOW,
   BLACK, BLACK, BLACK, BLACK,
   BLACK)
)

/* Re-enable astyle's indent enforcement */
// clang-format on

/** versionInfoMacro handles the 'firmware version info' macro
 *  When a key bound to the macro is pressed, this macro
 *  prints out the firmware build information as virtual keystrokes
 */

static void versionInfoMacro(uint8_t key_state) {
  if (keyToggledOn(key_state)) {
    Macros.type(PSTR("Keyboardio Model 100 - Firmware version "));
    Macros.type(PSTR(KALEIDOSCOPE_FIRMWARE_VERSION));
  }
}

/** anyKeyMacro is used to provide the functionality of the 'Any' key.
 *
 * When the 'any key' macro is toggled on, a random alphanumeric key is
 * selected. While the key is held, the function generates a synthetic
 * keypress event repeating that randomly selected key.
 *
 */

static void anyKeyMacro(KeyEvent &event) {
  if (keyToggledOn(event.state)) {
    event.key.setKeyCode(Key_A.getKeyCode() + (uint8_t)(millis() % 36));
    event.key.setFlags(0);
  }
}


/** macroAction dispatches keymap events that are tied to a macro
    to that macro. It takes two uint8_t parameters.

    The first is the macro being called (the entry in the 'enum' earlier in this file).
    The second is the state of the keyswitch. You can use the keyswitch state to figure out
    if the key has just been toggled on, is currently pressed or if it's just been released.

    The 'switch' statement should have a 'case' for each entry of the macro enum.
    Each 'case' statement should call out to a function to handle the macro in question.

 */

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
  switch (macro_id) {

    case MACRO_VERSION_INFO:
      versionInfoMacro(event.state);
      break;

    case MACRO_ANY:
      anyKeyMacro(event);
      break;
  }
  return MACRO_NONE;
}

/** toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
 * and turns them back on when it wakes up.
 */
void toggleLedsOnSuspendResume(kaleidoscope::plugin::HostPowerManagement::Event event) {
  switch (event) {
    case kaleidoscope::plugin::HostPowerManagement::Suspend:
    case kaleidoscope::plugin::HostPowerManagement::Sleep:
      LEDControl.disable();
      break;
    case kaleidoscope::plugin::HostPowerManagement::Resume:
      LEDControl.enable();
      break;
  }
}

/** hostPowerManagementEventHandler dispatches power management events (suspend,
 * resume, and sleep) to other functions that perform action based on these
 * events.
 */
void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event) {
  toggleLedsOnSuspendResume(event);
}

/** This 'enum' is a list of all the magic combos used by the Model 100's
 * firmware The names aren't particularly important. What is important is that
 * each is unique.
 *
 * These are the names of your magic combos. They will be used by the
 * `USE_MAGIC_COMBOS` call below.
 */
enum {
  // Toggle between Boot (6-key rollover; for BIOSes and early boot) and NKRO
  // mode.
  COMBO_TOGGLE_NKRO_MODE,
  // Enter test mode
  COMBO_ENTER_TEST_MODE
};

/** Wrappers, to be used by MagicCombo. **/

/**
 * This simply toggles the keyboard protocol via USBQuirks, and wraps it within
 * a function with an unused argument, to match what MagicCombo expects.
 */
static void toggleKeyboardProtocol(uint8_t combo_index) {
  USBQuirks.toggleKeyboardProtocol();
}

/**
 * Toggles between using the built-in keymap, and the EEPROM-stored one.
 */
static void toggleKeymapSource(uint8_t combo_index) {
  if (Layer.getKey == Layer.getKeyFromPROGMEM) {
    Layer.getKey = EEPROMKeymap.getKey;
  } else {
    Layer.getKey = Layer.getKeyFromPROGMEM;
  }
}

/**
 *  This enters the hardware test mode
 */
static void enterHardwareTestMode(uint8_t combo_index) {
  HardwareTestMode.runTests();
}


/** Magic combo list, a list of key combo and action pairs the firmware should
 * recognise.
 */
USE_MAGIC_COMBOS({ .action = toggleKeyboardProtocol,
                   // Left Fn + Esc + Shift
                   .keys = { R3C6, R2C6, R3C7 } },
                 { .action = enterHardwareTestMode,
                   // Left Fn + Prog + LED
                   .keys = { R3C6, R0C0, R0C6 } },
                 { .action = toggleKeymapSource,
                   // Left Fn + Prog + Shift
                   .keys = { R3C6, R0C0, R3C7 } });

void tapDanceAction(uint8_t tap_dance_index, KeyAddr key_addr, uint8_t tap_count,
                    kaleidoscope::plugin::TapDance::ActionType tap_dance_action) {
  switch (tap_dance_index) {
  case TD_LSHIFT:
    return tapDanceActionKeys(tap_count, tap_dance_action, Key_LeftShift, LSHIFT(Key_9));
  case TD_RSHIFT:
    return tapDanceActionKeys(tap_count, tap_dance_action, Key_RightShift, LSHIFT(Key_0));
  case TD_LCMD:
    return tapDanceActionKeys(tap_count, tap_dance_action, Key_LeftGui, Key_LeftBracket);
  case TD_RALT:
    return tapDanceActionKeys(tap_count, tap_dance_action, Key_LeftAlt, Key_RightBracket);
  case TD_LCTRL:
    return tapDanceActionKeys(tap_count, tap_dance_action, Key_LeftControl, Key_LeftCurlyBracket);
  case TD_RCTRL:
    return tapDanceActionKeys(tap_count, tap_dance_action, Key_RightControl, Key_RightCurlyBracket);
  }
}

// First, tell Kaleidoscope which plugins you want to use.
// The order can be important. For example, LED effects are
// added in the order they're listed here.
KALEIDOSCOPE_INIT_PLUGINS(
  // ----------------------------------------------------------------------
  // Chrysalis plugins

  // The EEPROMSettings & EEPROMKeymap plugins make it possible to have an
  // editable keymap in EEPROM.
  EEPROMSettings,
  EEPROMKeymap,

  // Focus allows bi-directional communication with the host, and is the
  // interface through which the keymap in EEPROM can be edited.
  Focus,

  // FocusSettingsCommand adds a few Focus commands, intended to aid in
  // changing some settings of the keyboard, such as the default layer (via the
  // `settings.defaultLayer` command)
  FocusSettingsCommand,

  // FocusEEPROMCommand adds a set of Focus commands, which are very helpful in
  // both debugging, and in backing up one's EEPROM contents.
  FocusEEPROMCommand,

  // The FirmwareVersion plugin lets Chrysalis query the version of the firmware
  // programmatically.
  FirmwareVersion,

  // The LayerNames plugin allows Chrysalis to display - and edit - custom layer
  // names, to be shown instead of the default indexes.
  LayerNames,

  // Enables controlling (and saving) the brightness of the LEDs via Focus.
  LEDBrightnessConfig,

  // ----------------------------------------------------------------------
  // Keystroke-handling plugins

  // The Qukeys plugin enables the "Secondary action" functionality in
  // Chrysalis. Keys with secondary actions will have their primary action
  // performed when tapped, but the secondary action when held.
  Qukeys,

  // Enables the "Sticky" behavior for modifiers, and the "Layer shift when
  // held" functionality for layer keys.
  // OneShot,
  // OneShotConfig,
  // EscapeOneShot,
  // EscapeOneShotConfig,

  // The macros plugin adds support for macros
  Macros,

  // Tap-dance keys are general purpose, multi-use keys, which trigger
  // a different action based on the number of times they were tapped in sequence.
  TapDance,

  // Enables dynamic, Chrysalis-editable macros.
  // DynamicMacros,

  // The HostPowerManagement plugin allows us to turn LEDs off when then host
  // goes to sleep, and resume them when it wakes up.
  HostPowerManagement,

  // The MouseKeys plugin lets you add keys to your keymap which move the mouse.
  MouseKeys,
  MouseKeysConfig,

  // The MagicCombo plugin lets you use key combinations to trigger custom
  // actions - a bit like Macros, but triggered by pressing multiple keys at the
  // same time.
  MagicCombo,

  // ----------------------------------------------------------------------
  // LED mode plugins

  // LEDControl provides support for other LED modes
  LEDControl,

  // We start with the LED effect that turns off all the LEDs.
  LEDOff,

  // The stalker effect lights up the keys you've pressed recently
  // StalkerEffect,

  // The LED Palette Theme plugin provides a shared palette for other plugins,
  // like Colormap below
  LEDPaletteTheme,

  // The Colormap effect makes it possible to set up per-layer colormaps
  ColormapEffect,

  DefaultColormap,

  // Turns LEDs off after a configurable amount of idle time.
  IdleLEDs,
  PersistentIdleLEDs,

  ActiveModColorEffect,

  // ----------------------------------------------------------------------
  // Miscellaneous plugins

  // The USBQuirks plugin lets you do some things with USB that we aren't
  // comfortable - or able - to do automatically, but can be useful
  // nevertheless. Such as toggling the key report protocol between Boot (used
  // by BIOSes) and Report (NKRO).
  USBQuirks,

  // The hardware test mode, which can be invoked by tapping Prog, LED and the
  // left Fn button at the same time.
  HardwareTestMode  //,
);

/** The 'setup' function is one of the two standard Arduino sketch functions.
 * It's called when your keyboard first powers up. This is where you set up
 * Kaleidoscope and any plugins.
 */
void setup() {
  // QUKEYS(
  //   kaleidoscope::plugin::Qukey(PRIMARY, KeyAddr(0, 13), LSHIFT(Key_9)),             // 9
  //   kaleidoscope::plugin::Qukey(PRIMARY, KeyAddr(0, 14), LSHIFT(Key_0)),             // 0
  //   kaleidoscope::plugin::Qukey(PRIMARY, KeyAddr(1, 11), LSHIFT(Key_LeftBracket)),   // U
  //   kaleidoscope::plugin::Qukey(PRIMARY, KeyAddr(1, 12), LSHIFT(Key_RightBracket)),  // I
  //   kaleidoscope::plugin::Qukey(PRIMARY, KeyAddr(1, 13), Key_LeftBracket),           // O
  //   kaleidoscope::plugin::Qukey(PRIMARY, KeyAddr(1, 14), Key_RightBracket)           // P
  // )
  // Qukeys.setMinimumHoldTime(200);  // Try to avoid parasite keystrokes: https://community.keyboard.io/t/unexpected-characters-using-qukeys/2726

  // First, call Kaleidoscope's internal setup function
  Kaleidoscope.setup();

  // Set the action key the test mode should listen for to Left Fn
  HardwareTestMode.setActionKey(R3C6);

  // The LED Stalker mode has a few effects. The one we like is called
  // 'BlazingTrail'. For details on other options, see
  // https://github.com/keyboardio/Kaleidoscope/blob/master/docs/plugins/LED-Stalker.md
  // StalkerEffect.variant = STALKER(BlazingTrail);
  StalkerEffect.variant = STALKER(Haunt, (CRGB(255, 255, 255)));

  // To make the keymap editable without flashing new firmware, we store
  // additional layers in EEPROM. For now, we reserve space for eight layers. If
  // one wants to use these layers, just set the default layer to one in EEPROM,
  // by using the `settings.defaultLayer` Focus command, or by using the
  // `keymap.onlyCustom` command to use EEPROM layers only.
  EEPROMKeymap.setup(8);

  // We need to tell the Colormap plugin how many layers we want to have custom
  // maps for. To make things simple, we set it to eight layers, which is how
  // many editable layers we have (see above).
  ColormapEffect.max_layers(8);
  ColormapEffect.activate();

  DefaultColormap.setup();

  // For Dynamic Macros, we need to reserve storage space for the editable
  // macros. A kilobyte is a reasonable default.
  // DynamicMacros.reserve_storage(1024);

  // If there's a default layer set in EEPROM, we should set that as the default
  // here.
  Layer.move(EEPROMSettings.default_layer());

  // Editable layer names are stored in EEPROM too, and we reserve 16 bytes per
  // layer for them. We need one extra byte per layer for bookkeeping, so we
  // reserve 17 / layer in total.
  LayerNames.reserve_storage(17 * 8);
}

/** loop is the second of the standard Arduino sketch functions.
  * As you might expect, it runs in a loop, never exiting.
  *
  * For Kaleidoscope-based keyboard firmware, you usually just want to
  * call Kaleidoscope.loop(); and not do anything custom here.
  */

void loop() {
  Kaleidoscope.loop();
}

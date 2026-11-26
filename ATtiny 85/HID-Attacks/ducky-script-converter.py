
 #!/usr/bin/env python3
  """
  Ducky Script → Digispark ATtiny85 (.ino) Converter
  Author: Akzhyyy
  License: MIT

  Usage:
    python ducky_converter.py payload.ducky > payload.ino

  Supports: STRING, DELAY, ENTER, TAB, SPACE, GUI, SHIFT, ALT, CTRL,
            UP, DOWN, LEFT, RIGHT, WINDOWS, MENU, F1-F12
  """

  import sys
  import re

  KEY_MAP = {
      'ENTER': 'KEY_ENTER',
      'TAB': 'KEY_TAB',
      'SPACE': 'KEY_SPACE',
      'ESC': 'KEY_ESC',
      'UP': 'KEY_UP_ARROW',
      'DOWN': 'KEY_DOWN_ARROW',
      'LEFT': 'KEY_LEFT_ARROW',
      'RIGHT': 'KEY_RIGHT_ARROW',
      'WINDOWS': 'KEY_R, MOD_GUI_LEFT',
      'GUI': 'KEY_R, MOD_GUI_LEFT',
      'MENU': 'KEY_MENU',
      'SHIFT': 'MOD_SHIFT_LEFT',
      'ALT': 'MOD_ALT_LEFT',
      'CONTROL': 'MOD_CONTROL_LEFT',
      'CTRL': 'MOD_CONTROL_LEFT',
      'CAPSLOCK': 'KEY_CAPS_LOCK',
      'NUMLOCK': 'KEY_NUM_LOCK',
      'SCROLLLOCK': 'KEY_SCROLL_LOCK',
      'PRINTSCREEN': 'KEY_PRINT_SCREEN',
      'PAUSE': 'KEY_PAUSE',
      'INSERT': 'KEY_INSERT',
      'DELETE': 'KEY_DELETE',
      'HOME': 'KEY_HOME',
      'END': 'KEY_END',
      'PAGEUP': 'KEY_PAGE_UP',
      'PAGEDOWN': 'KEY_PAGE_DOWN',
  }

  for i in range(1, 13):
      KEY_MAP[f'F{i}'] = f'KEY_F{i}'

  MODIFIERS = {
      'SHIFT': 'MOD_SHIFT_LEFT',
      'ALT': 'MOD_ALT_LEFT',
      'CTRL': 'MOD_CONTROL_LEFT',
      'CONTROL': 'MOD_CONTROL_LEFT',
      'GUI': 'MOD_GUI_LEFT',
      'WINDOWS': 'MOD_GUI_LEFT',
  }

  HEADER = """/*
   * Auto-generated from Ducky Script
   * Converter: ducky_converter.py
   */

  #include "DigiKeyboard.h"

  void setup() {
    DigiKeyboard.delay(2000);
  """

  FOOTER = """
  }

  void loop() {
    digitalWrite(1, HIGH);
    DigiKeyboard.delay(500);
    digitalWrite(1, LOW);
    DigiKeyboard.delay(500);
  }
  """

  def parse_line(line):
      line = line.strip()
      if not line or line.startswith('#') or line.startswith('//'):
          return None

      # DELAY
      m = re.match(r'DELAY\s+(\d+)', line, re.I)
      if m:
          return f'  DigiKeyboard.delay({m.group(1)});'

      # STRING
      m = re.match(r'STRING\s+(.+)', line, re.I)
      if m:
          text = m.group(1).replace('"', '\\"').replace('\\', '\\\\')
          return f'  DigiKeyboard.print("{text}");'

      # KEY / COMBO
      parts = line.split()
      if not parts:
          return None

      # Check for modifier + key (e.g., "GUI r", "CTRL SHIFT ESC")
      mods = []
      key = None

      for part in parts:
          upper = part.upper()
          if upper in MODIFIERS:
              mods.append(MODIFIERS[upper])
          elif upper in KEY_MAP:
              key = KEY_MAP[upper]
          else:
              # Try single char
              if len(part) == 1:
                  key = f"'{part}'"

      if key and mods:
          mod_str = ' | '.join(mods)
          return f'  DigiKeyboard.sendKeyStroke({key}, {mod_str});'
      elif key:
          return f'  DigiKeyboard.sendKeyStroke({key});'

      return None

  def main():
      if len(sys.argv) < 2:
          print("Usage: python ducky_converter.py input.ducky [output.ino]")
          sys.exit(1)

      input_file = sys.argv[1]
      output_file = sys.argv[2] if len(sys.argv) > 2 else None

      with open(input_file, 'r') as f:
          lines = f.readlines()

      output = [HEADER]

      for line in lines:
          parsed = parse_line(line)
          if parsed:
              output.append(parsed)

      output.append(FOOTER)

      result = '\n'.join(output)

      if output_file:
          with open(output_file, 'w') as f:
              f.write(result)
          print(f"✅ Written to {output_file}")
      else:
          print(result)

  if __name__ == '__main__':
      main()

  ---

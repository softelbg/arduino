/*
  Softel Labs 2019
  s.georgiev@softel.bg
*/

#ifndef __SOFTEL_IREMOTE_CONTROL_BLACK_H__
#define __SOFTEL_IREMOTE_CONTROL_BLACK_H__

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <StandardCplusplus.h>
#include <IRremote.h>
#include <map>

class IRemoteControlBlack {
public:
  explicit IRemoteControlBlack(int pin) : _irrecv(pin)
  {
    _cmd2code["0"] = 16730805;
    _cmd2code["1"] = 16738455;
    _cmd2code["2"] = 16750695;
    _cmd2code["3"] = 16756815;
    _cmd2code["4"] = 16724175;
    _cmd2code["5"] = 16718055;
    _cmd2code["6"] = 16743045;
    _cmd2code["7"] = 16716015;
    _cmd2code["8"] = 16726215;
    _cmd2code["9"] = 16734885;
    _cmd2code["left"] = 16720605;
    _cmd2code["right"] = 16761405;
    _cmd2code["up"] = 16736925;
    _cmd2code["down"] = 16754775;
    _cmd2code["ok"] = 16712445;
    _cmd2code["*"] = 16728765;
    _cmd2code["#"] = 16732845;
    for (auto it = _cmd2code.begin(); it != _cmd2code.end(); ++it) {
      _code2cmd[it->second] = it->first;
    }
  }

  void setup() {
    _irrecv.enableIRIn();
    Serial.println("Enabled IRin");
  }

  String run() {
    String result = "";
    if (_irrecv.decode(&_iresult)) {
      if (_iresult.value != 0xFFFFFFFF) {
        auto it = _code2cmd.find(_iresult.value);
        if (it != _code2cmd.end()) {
          result = it->second;
        }
      }
      _irrecv.resume(); // Receive the next value
    }

    return result;
  }

  const String& cmd(unsigned long code) const {
    return _code2cmd[code];
  }

  void print() {
    for (auto it = _code2cmd.begin(); it != _code2cmd.end(); ++it) {
      Serial.println(String(it->first) + " => " + String(it->second));
    }
  }
private:
  std::map<unsigned long, String> _code2cmd;
  std::map<String, unsigned long> _cmd2code;

  IRrecv _irrecv;
  decode_results _iresult;
};


#endif

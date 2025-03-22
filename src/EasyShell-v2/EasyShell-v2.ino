#include "EasyShell.h"


char eshell_rcv_buffer1[ESHELL_CMD_PARAM_MAX_NUM + 1] = {0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  eshell_get(eshell_rcv_buffer1, 128);
}

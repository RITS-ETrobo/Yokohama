#include "app.h"

#include "Walker.h"

using namespace ev3api;

Walker walker;

void walker_cyc(intptr_t exinf) {
    act_tsk(WALKER_TASK);
}

void walker_task(intptr_t exinf) {
  if (ev3_button_is_pressed(LEFT_BUTTON)) {
    wup_tsk(MAIN_TASK);  // 左ボタン押下でメインを起こす
  } else {
    walker.run();  // ;走行
  }
  ext_tsk();
}

void main_task(intptr_t unused) {
  walker.init();
  ev3_sta_cyc(WALKER_CYC);
  slp_tsk();  // 起きたら、走行をやめる
  ev3_stp_cyc(WALKER_CYC);
  walker.terminate();
  ext_tsk();
}

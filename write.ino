void writenex() {

  speed_i = speed;
  rate_act_i = rate_act;
  rate_min_i = rate_min;
  speed_std = pitch_mm / rate_min;

  myNex.writeStr("t2.txt", String(speed_i));
  myNex.writeStr("t3.txt", String(rate_act_i));
  myNex.writeStr("t1.txt", String(rate_min_i));

  myNex.writeStr("t0.txt", String(speed_std));
  myNex.writeStr("t4.txt", String(pulses));

  if ((rate_act > (rate_max_i)) || (rate_act < rate_min_i)) {
    myNex.writeNum("t2.pic", 5);
    myNex.writeNum("t3.pic", 6);//warna merah
  } else {
    myNex.writeNum("t2.pic", 3);//hijau
    myNex.writeNum("t3.pic", 4);
  }

}

void writenex() {

  speedInt = speed;
  rateActInt = ceil(rateAct); // ceil bulat ke atas
  rateMinInt = rateMin;             
  rateMaxInt = rateMax;                                                                           ;
  speedStd = pitch / rateMin;

  myNex.writeStr("t2.txt", String(speedInt));
  myNex.writeStr("t3.txt", String(rateActInt));
  myNex.writeStr("t1.txt", String(rateMinInt));

  myNex.writeStr("t0.txt", String(speedStd));
  myNex.writeStr("t4.txt", String(pulseCount));

  if (rateActInt >= rateMinInt && rateActInt <= rateMaxInt) {
    myNex.writeNum("t2.pic", 3);//hijau
    myNex.writeNum("t3.pic", 4);
  } else {
    myNex.writeNum("t2.pic", 5);
    myNex.writeNum("t3.pic", 6);//warna merah
  }

}

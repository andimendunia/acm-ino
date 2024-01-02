void writenex() {
  fix_velocity = velocity;
  fix_shoepairs = shoepairs;
  fix_standarddown = standarddown;
  standardspeed = shoedistance / standarddown;
  fix_standardspeed = standardspeed;
  myNex.writeStr("t2.txt", String(fix_velocity));
  myNex.writeStr("t3.txt", String(fix_shoepairs));
  myNex.writeStr("t1.txt", String(fix_standarddown));
  myNex.writeStr("t0.txt", String(fix_standardspeed));
  myNex.writeStr("t4.txt", String(pulses));
  if ((shoepairs > (standardup)) || (shoepairs < standarddown)) {
    myNex.writeNum("t2.pic", 5);
    myNex.writeNum("t3.pic", 6);//warna merah
  } else {
    myNex.writeNum("t2.pic", 3);//hijau
    myNex.writeNum("t3.pic", 4);
  }
}

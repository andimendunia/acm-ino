void readnex() {

  rate_min_s = myNex.readStr("t6.txt");
  rate_max_s = myNex.readStr("t7.txt");
  if (((rate_min_s == "ERROR") || (rate_min_s == "")) && ((rate_max_s == "ERROR") || (rate_max_s == ""))) {
    rate_min_i = rate_min_i;
    rate_max_i = rate_max_i;
  } else {
    rate_min_i = rate_min_s.toFloat();
    rate_max_i = rate_max_s.toFloat();
    myNex.writeStr("t8.txt", msg_simpan);
  }

}

void readnex() {

  rateMinStr = myNex.readStr("t6.txt");
  rateMaxStr = myNex.readStr("t7.txt");
  if (((rateMinStr == "ERROR") || (rateMinStr == "")) && ((rateMaxStr == "ERROR") || (rateMaxStr == ""))) {
    rateMinInt = rateMinInt;
    rateMaxInt = rateMaxInt;
  } else {
    rateMinInt = rateMinStr.toFloat();
    rateMaxInt = rateMaxStr.toFloat();
    myNex.writeStr("t8.txt", msg_simpan);
  }

}

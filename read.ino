void readnex() {
  str_down = myNex.readStr("t6.txt");
  str_up = myNex.readStr("t7.txt");
  // Serial.println(standarddown);
  if(((str_down == "ERROR") || (str_down == "")) && ((str_up == "ERROR") || (str_up == ""))){
    standarddown = standarddown;
    standardup = standardup;
  }else{
    standarddown = str_down.toFloat();
    standardup = str_up.toFloat();
    myNex.writeStr("t8.txt",save);
  }
}

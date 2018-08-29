float readDS18B20() {
  sensors.setWaitForConversion(false);     // this option avoid sensor delay. without this the reading delay is 750 ms for each sensor
  sensors.requestTemperatures();
  camera = sensors.getTempCByIndex(0);
  ext = sensors.getTempCByIndex(1);
}

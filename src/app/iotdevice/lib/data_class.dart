class DataClass {
  final double x;
  final double y;
  final double z;
  final double temperature;
  final double photoresistor;
  final int timeStamp;

  DataClass(this.x, this.y, this.z,this.temperature, this.photoresistor, this.timeStamp);
  Map toJson() => {
        'x': x,
        'y': y,
        'z': z,
        'temp': temperature,
        'light': photoresistor,
        'time': timeStamp
      };
}
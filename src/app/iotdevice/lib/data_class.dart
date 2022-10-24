class DataClass {
  late double x;
  late double y;
  late double z;
  late double temperature;
  late double photoresistor;
  late int timeStamp;
  // location data is still missing

  DataClass(this.x, this.y, this.z,this.temperature, this.photoresistor, this.timeStamp);
  Map toJson() => {
        'x': x,
        'y': y,
        'z': z,
        'temp': temperature,
        'light': photoresistor,
        'time': timeStamp
  };
  
  DataClass.fromJson(Map<String, dynamic> json)
      : x = json['x'],
        y = json['y'],
        z = json['z'],
        temperature = json['temp'],
        photoresistor = json['light'],
        timeStamp = json['time'];

}
class DataClass {
  late List<dynamic> acc;
  late List<dynamic> gyro;
  late double temperature;
  late double photoresistor;
  late int timeStamp;
  // location data is still missing

  DataClass(this.acc, this.gyro ,this.temperature, this.photoresistor, this.timeStamp);
  Map toJson() => {
        'acc': acc,
        'gyro': gyro,
        'temp': temperature,
        'light': photoresistor,
        'time': timeStamp
  };
  
  DataClass.fromJson(Map<String, dynamic> json)
      : acc = json['acc'],
        gyro = json['gyro'],
        temperature = json['temp'],
        photoresistor = json['light'],
        timeStamp = json['time'];

}
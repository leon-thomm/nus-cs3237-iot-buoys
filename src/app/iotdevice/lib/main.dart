import 'dart:async';
import 'dart:io';
import 'dart:typed_data';

import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'package:mqtt_client/mqtt_server_client.dart';
import 'package:sensors_plus/sensors_plus.dart';
import 'package:wakelock/wakelock.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
 
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // Try running your application with "flutter run". You'll see the
        // application has a blue toolbar. Then, without quitting the app, try
        // changing the primarySwatch below to Colors.green and then invoke
        // "hot reload" (press "r" in the console where you ran "flutter run",
        // or simply save your changes to "hot reload" in a Flutter IDE).
        // Notice that the counter didn't reset back to zero; the application
        // is not restarted.
        primarySwatch: Colors.blue,
      ),
      home: const MyHomePage(title: 'IoT Checkin 2 Demo'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key, required this.title}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {

  // MQTT Setup
  static String mqttAddress   = '104.248.98.70';        // IP of the Mqtt Server
  static int mqttPort         = 1883;                   // Port of the Mqtt Server
  static String usr           = "cs3237";               // 
  static String pwd           = "thisisag00dp4ssw0rd";  // 
  static int sendThreshold    = 10;                     // always sending #sendThreshold data collection to the mqtt server (might safe power)
  static String topic         = 'topic/test';           // Topic to send the data to
  String statusMessageMqtt    = "Not Started";          // Possible Status: Not Started, Running, Error

  // Socket Setup
  static String socketAddress = '192.168.43.1';         // IP of the local socket server (this is the default IP of mobile hotspots)
  static int socketPort       = 4567;                   // Port of the local socket server     
  bool running                = false;                  // Used to enable or disable button           
  late ServerSocket server;                             // Server Object; late tells dart compiler that it will be initialised later 
  String statusMessageSocket  = "Not Started";          // Possible Status: Not Started, Running, Error

  // send buffer
  var toSend = [];
  
  void connected() {
    if (kDebugMode) {
      print("Connected");
    }

    setState(() {
      statusMessageMqtt = "Running";
    });
  }

  void disconnected() {
    if (kDebugMode) {
      print("Disconnected");
    }

    setState(() {
      statusMessageMqtt = "Not Started";
    });
  }

  void error() {
    if (kDebugMode) {
      print("Error Occured while connecting to Mqtt Server");
    }

    setState(() {
      statusMessageMqtt = "Error";
    });
  }

  MqttServerClient mqttclient =
      MqttServerClient.withPort(mqttAddress, 'iot_device', mqttPort);

  void handleConnection(Socket client) {
    if (kDebugMode) {
      print('Connection from'
      ' ${client.remoteAddress.address}:${client.remotePort}');
    }

    // listen for events from the client
    client.listen(

    // handle data from the client
    (Uint8List data) async {
      
      if (kDebugMode) {
        print(data);
      }
     
      final message = String.fromCharCodes(data);
      if (kDebugMode) {
        print(data);
        print(message);

      }

      toSend.add(message);
      // Simple Response 
      if (message == "Ping"){
        client.write("Pong");
      }

      // Close Connection Call
      if (message.compareTo("Bye")==0){
        client.close();
      }

      // publish data to mqtt if toSend is larger than certain threshold (in this case 10)
      if (toSend.length > sendThreshold) {
        final builder = MqttClientPayloadBuilder();
        var data2Send = toSend.getRange(0, 10);
        String data2SendStr = data2Send.toString();
        builder.addString(data2SendStr);
        toSend.removeRange(0, 10);
        mqttclient.publishMessage(topic, MqttQos.atLeastOnce, builder.payload!);
      }
    },

    // handle errors
    onError: (error) {
      if (kDebugMode) {
        print(error);
      }
      client.close();
    },

    // handle the client closing the connection
    onDone: () {
      if (kDebugMode) {
        print('Client left');
      }
      client.close();
    },
  );
  }  

  List<double>? _accelerometerValues;
  final _streamSubscriptions = <StreamSubscription<dynamic>>[]; // used for the accelometer to get a constant stream of data

  @override
  void initState() {
    super.initState();
    
    // setting up reading data from accelometer
    if (kDebugMode) {
      print("setting up...");
    }
    _streamSubscriptions.add(
      userAccelerometerEvents.listen(
        (UserAccelerometerEvent event) {
          setState(() {
            _accelerometerValues = <double>[event.x, event.y, event.z];
          });
        },
      ),
    );
    
    
  }
  
  @override
  void dispose() {
    super.dispose();
    //destroy streams
    for (final subscription in _streamSubscriptions) {
      subscription.cancel();
    }
  }
  

  @override
  Widget build(BuildContext context) {

    // Access Accelometer without gravity effects
    final accelerometer =
        _accelerometerValues?.map((double v) => v.toStringAsFixed(1)).toList();
  
    return Scaffold(
      appBar: AppBar(
        // Here we take the value from the MyHomePage object that was created by
        // the App.build method, and use it to set our appbar title.
        title: Text(widget.title),
      ),
      body: SingleChildScrollView(
        child: Column(
          children:  [
            const SizedBox(height: 20,),
            Row(
              children: [
                Expanded(child: Container()),
                const Text("Hosting Socket at 192.168.43.1:4567"),
                Expanded(child: Container())
              ],
            ),
            const SizedBox(height: 20,),
            Row(
              children: [
                Expanded(child: Container()),
                Text("Data to be send: ${toSend.length}"),
                Expanded(child: Container())
              ],
            ),
            const SizedBox(height: 20,),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: <Widget>[
                Expanded(child: Container()),
                Text('Accelerometer: $accelerometer'),
                Expanded(child: Container())
              ],
            ),
            const SizedBox(height: 20,),
            Row(
              children: [
                Expanded(child: Container()),
                Text("Status MQTT Client: $statusMessageMqtt"),
                Expanded(child: Container())
              ]
            ,),
            const SizedBox(height: 20,),
            Row(
              children: [
                Expanded(child: Container()),
                Text("Status Socket Client: $statusMessageSocket"),
                Expanded(child: Container())
              ]
            ,),
            const SizedBox(height: 20,),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: <Widget>[
                Expanded(child: Container()),
                (!running) ? TextButton(
                  onPressed: () async {
                    // Create Server
                    try{
                      server = await ServerSocket.bind(socketAddress, socketPort);
                      server.listen((client) {
                        handleConnection(client);
                      });
                      statusMessageSocket = "Running";
                      if (kDebugMode) {
                        print("binding done");
                      }
                    }catch (e) {
                      statusMessageSocket = e.toString();
                    }
                    
                    
                    // Setup Mqtt Client
                    mqttclient.onConnected = connected;
                    mqttclient.onDisconnected = disconnected;
                    mqttclient.autoReconnect = true;  // enabling auto reconnect 

                    final connMessage = MqttConnectMessage()
                      .authenticateAs(usr, pwd)
                      .withWillTopic('willtopic')
                      .withWillMessage('Will message')
                      .startClean()
                      .withWillQos(MqttQos.atLeastOnce);
                    
                    mqttclient.keepAlivePeriod = 60;
                    
                    mqttclient.connectionMessage = connMessage;

                    try {
                      await mqttclient.connect();
                      if (kDebugMode) {
                        print("Connected to Mqtt Server");
                      }
                    } catch (e) {
                      if (kDebugMode) {
                        print('Error Connecting');
                      }
                      error();
                    }

                    // Preventing the phone from falling asleep
                    Wakelock.enable();
                    setState(() {
                      running = true;
                    });
                  },
                  child: const Text("Press to start")
                ) : TextButton(onPressed: () async {
                  await server.close();
                  statusMessageSocket = "Not Started";

                  mqttclient.disconnect();
                  running = false;
                  statusMessageMqtt = "Not Started";
                }, child: const Text("Press to Stop")),
                Expanded(child: Container())
              ],
            )
          ],
        ),
      ),
    );
  }
}

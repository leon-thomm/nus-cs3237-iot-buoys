import 'dart:async';
import 'dart:convert';
import 'dart:io';

import 'package:camera/camera.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:iotdevice/components/mqtt_form.dart';
import 'package:iotdevice/models/data_class.dart';
import 'package:iotdevice/services/mqtt_client_service.dart';
import 'package:light/light.dart';
import 'package:mqtt_client/mqtt_server_client.dart';
import 'package:provider/provider.dart';
import 'package:sensors_plus/sensors_plus.dart';
import 'package:wakelock/wakelock.dart';

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key, required this.title, required this.camera})
      : super(key: key);

  final String title;
  final CameraDescription camera;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  // Lux Sensor Setup
  static final Light _light = Light();

  // Camera Setup
  late CameraController _controller;
  late Future<void> _initializeControllerFuture;

  static String topic = 'test/abc'; // Topic to send the data to
  String statusMessageMqtt =
      "Not Started"; // Possible Status: Not Started, Running, Error

  // Socket Setup
  static String socketAddress =
      '192.168.43.1'; // IP of the local socket server (this is the default IP of mobile hotspots)
  static int socketPort = 4567; // Port of the local socket server
  bool running = false; // Used to enable or disable button
  late ServerSocket
      server; // Server Object; late tells dart compiler that it will be initialised later
  String statusMessageSocket =
      "Not Started"; // Possible Status: Not Started, Running, Error

  // send buffer
  var toSend = [];

  void publishData(MqttClientService mqttclientservice) {
    if (toSend.length > MqttClientService.sendThreshold && running) {
      String data2SendStr = jsonEncode(toSend);
      if (kDebugMode) print(data2SendStr);
      try {
        int res = mqttclientservice.publish(data2SendStr);
        if (kDebugMode) print("Publish Result $res");
        toSend = [];
      } catch (e) {
        if (kDebugMode) print(e.toString());
      }
    }
  }

  void handleConnection(Socket client, MqttClientService mqttClientService) {
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
          print(message);
        }

        DataClass tmp = DataClass.fromJson(json.decode(message));
        toSend.add(message);

        // Close Connection Call
        if (message.compareTo("Bye") == 0) {
          client.close();
        }

        // publish data to mqtt if toSend is larger than certain threshold (in this case 10)
        publishData(mqttClientService);
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
  final _streamSubscriptions = <
      StreamSubscription<
          dynamic>>[]; // used for the accelometer to get a constant stream of data

  @override
  void initState() {
    super.initState();

    // setting up reading data from accelometer
    if (kDebugMode) {
      print("setting up...");
    }

    // Adding Accelometer
    _streamSubscriptions.add(
      userAccelerometerEvents.listen(
        (UserAccelerometerEvent event) {
          setState(() {
            _accelerometerValues = <double>[event.x, event.y, event.z];
          });

          // Alert this is for debug purposes only
          // Add accelometer data to toSend
          int timestamp = DateTime.now().microsecondsSinceEpoch; //DateTime.
          DataClass data =
              DataClass(_accelerometerValues!, [0, 0, 0], 28.0, 512, timestamp);
          /* if (running) toSend.add(data);
          publishData();*/
        },
      ),
    );

    // Adding Lux Sensor
    /* _streamSubscriptions.add(
      _light.lightSensorStream.listen((event) { })
    );*/

    // Create Camera Controller
    /*_controller = CameraController(
      // Get a specific camera from the list of available cameras.
      widget.camera,
      // Define the resolution to use.
      ResolutionPreset.low,
    );
    // Next, initialize the controller. This returns a Future.
    _initializeControllerFuture = _controller.initialize();*/
  }

  @override
  void dispose() {
    super.dispose();
    //destroy streams
    for (final subscription in _streamSubscriptions) {
      subscription.cancel();
    }
  }

  Future<void> turnOnTorch() async {
    await _controller.setFlashMode(FlashMode.torch);
  }

  Future<void> turnoffTorch() async {
    await _controller.setFlashMode(FlashMode.off);
  }

  Future<double> getExposureOffSet() async {
    return await _controller.getExposureOffsetStepSize();
  }

  Future<XFile?> takePicture() async {
    // turn flash on
    _controller.setFlashMode(FlashMode.always);

    if (_controller.value.isTakingPicture) {
      return null;
    }

    try {
      XFile file = await _controller.takePicture();
      return file;
    } catch (e) {
      return null;
    }
  }

  bool _showCamera = false;

  void setUpSocketServer(MqttClientService mqttClientService) async {
    // Create Server
    try {
      server = await ServerSocket.bind(socketAddress, socketPort);
      server.listen((client) {
        handleConnection(client, mqttClientService);
      });
      statusMessageSocket = "Running";
      if (kDebugMode) {
        print("binding done");
      }
      // Preventing the phone from falling asleep
      Wakelock.enable();
      setState(() {
        running = true;
      });
    } catch (e) {
      statusMessageSocket = e.toString();
    }
  }

  void closeSocketServer(MqttServerClient mqttClient) async {
    await server.close();
    statusMessageSocket = "Not Started";

    mqttClient.disconnect();
    running = false;

    Wakelock.disable();
    statusMessageMqtt = "Not Started";
  }

  @override
  Widget build(BuildContext context) {
    MqttClientService mqttclientservice = Provider.of<MqttClientService>(context, listen: true);
    MqttServerClient mqttclient = mqttclientservice.getClient;
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
          children: [
            const SizedBox(
              height: 20,
            ),
            Row(
              children: [
                Expanded(child: Container()),
                Text("Hosting Socket at $socketAddress:$socketPort"),
                Expanded(child: Container())
              ],
            ),
            const SizedBox(
              height: 20,
            ),
            Row(
              children: [
                Expanded(child: Container()),
                Text("Data to be send: ${toSend.length}"),
                Expanded(child: Container())
              ],
            ),
            const SizedBox(
              height: 20,
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: <Widget>[
                Expanded(child: Container()),
                Text('Accelerometer: $accelerometer'),
                Expanded(child: Container())
              ],
            ),
            const SizedBox(
              height: 20,
            ),
            Row(
              children: [
                Expanded(child: Container()),
                Text("Status MQTT Client: ${MqttClientService.statusMessageMqtt}"),
                Expanded(child: Container())
              ],
            ),
            const SizedBox(
              height: 20,
            ),
            Row(
              children: [
                Expanded(child: Container()),
                Text("Status Socket Client: $statusMessageSocket"),
                Expanded(child: Container())
              ],
            ),
            const SizedBox(
              height: 20,
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: <Widget>[
                Expanded(child: Container()),
                (!running)
                    ? TextButton(
                        onPressed: () {
                          setUpSocketServer(mqttclientservice);
                          mqttclientservice.connectMqttClient();
                        },
                        child: const Text("Press to start"))
                    : TextButton(
                        onPressed: () => closeSocketServer(mqttclient),
                        child: const Text("Press to Stop")),
                Expanded(child: Container())
              ],
            ),
            /*Switch(value: _showCamera, onChanged: (val) {
              if (kDebugMode) print(val);
              setState(() {
                _showCamera = val;
              });
              
            }),
            const SizedBox(height: 20,),
            (_showCamera) ? Row(
              children: [
                Expanded(child: Container()),
                // View of the camera
                SizedBox(
                  height: 100,
                  width: 100,
                  child: FutureBuilder<void>(
                    future: _initializeControllerFuture,
                    builder: (context, snapshot) {
                      if (snapshot.connectionState == ConnectionState.done) {
                        // If the Future is complete, display the preview.
                        return CameraPreview(_controller);
                      } else {
                        // Otherwise, display a loading indicator.
                        return const Center(child: CircularProgressIndicator());
                      }
                    },
                  ),
                ),
                Expanded(child: Container())
              ],
            ) : Container()*/
            const SizedBox(
              height: 20,
            ),
            Center(
              child: MqttForm(
                onAdd: (value) {
                  toSend.add(value);
                },
                onSend: (value) {
                  String s = json.encode(value.toJson());
                  mqttclientservice.publish(s);
                },
              ),
            )
          ],
        ),
      ),
    );
  }
}

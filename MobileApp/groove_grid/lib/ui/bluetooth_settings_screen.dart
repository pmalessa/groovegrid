import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

BluetoothDevice lastConnectedDevice;

class BluetoothSettingsView extends StatefulWidget {

  @override
  _BluetoothSettingsViewState createState() => new _BluetoothSettingsViewState();
}


class _BluetoothSettingsViewState extends State<BluetoothSettingsView> {
  static final TextEditingController _message = new TextEditingController();
  static final TextEditingController _text = new TextEditingController();

  FlutterBluetoothSerial bluetooth = FlutterBluetoothSerial.instance;

  List<BluetoothDevice> _devices = [];
  BluetoothDevice _deviceInternal;

  get _device {
    return _deviceInternal;
  }
  set _device(BluetoothDevice newDevice) {
    _deviceInternal = newDevice;
    lastConnectedDevice = newDevice;
  }
  bool _connected = false;
  bool _pressed = false;


  ///
  ///
  ///
  @override
  void initState() {
    super.initState();
    _device = lastConnectedDevice;
    bluetooth.isConnected.then((connected) {
      setState(() {
        _connected = connected;
      });
    });
    initPlatformState();
  }

  ///
  ///
  ///
  Future<void> initPlatformState() async {
    List<BluetoothDevice> devices = [];

    try {
      devices = await bluetooth.getBondedDevices();
    } on PlatformException {
      // TODO - Error
    }

    bluetooth.onStateChanged().listen((state) {
      switch (state) {
        case FlutterBluetoothSerial.CONNECTED:
          setState(() {
            _connected = true;
            _pressed = false;
          });
          break;
        case FlutterBluetoothSerial.DISCONNECTED:
          setState(() {
            _connected = false;
            _pressed = false;
          });
          break;
        default:
        // TODO
          print(state);
          break;
      }
    });

    bluetooth.onRead().listen((msg) {
      setState(() {
        print('Read: $msg');
        _text.text += msg;
      });
    });

    //if (!mounted) return;
    setState(() {
      _devices = devices;
      if (_device != null) {
        _device = _devices.where((BluetoothDevice device) => device.name == _device.name).elementAt(0);
        print("Last used Device name: ");
        print(_device.name);
      }
    });
  }

  ///
  ///
  ///
  @override
  Widget build(BuildContext context) {
    print("Device during Widget Build:");
    print(_device);
      return Scaffold(
        appBar: AppBar(
          title: Text('Flutter Bluetooth Serial'),
        ),
        body: Container(
          child: ListView(
            children: <Widget>[
              Padding(
                padding: const EdgeInsets.fromLTRB(10.0, 10.0, 10.0, 0.0),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: <Widget>[
                    Text(
                      'Device:',
                      style: TextStyle(
                        fontWeight: FontWeight.bold,
                      ),
                    ),
                    DropdownButton(
                      items: _getDeviceItems(),
                      onChanged: (value) => setState(() => _device = value),
                      value: _device,
                    ),
                    RaisedButton(
                      onPressed:
                      _pressed ? null : _connected ? _disconnect : _connect,
                      child: Text(_connected ? 'Disconnect' : 'Connect'),
                    ),
                  ],
                ),
              ),
              Padding(
                padding: const EdgeInsets.fromLTRB(10.0, 6.0, 10.0, 0.0),
                child: Row(
                  children: <Widget>[
                    Expanded(
                      child: new TextField(
                        controller: _message,
                        autocorrect: false,
                        keyboardType: TextInputType.text,
                        decoration: new InputDecoration(
                          border: InputBorder.none,
                          labelText: 'Message:',
                        ),
                      ),
                    ),
                    RaisedButton(
                      onPressed: _connected ? _writeTest : null,
                      child: Text('Send'),
                    ),
                  ],
                ),
              ),
              Padding(
                padding: const EdgeInsets.only(left: 10.0, right: 10.0),
                child: TextField(
                  controller: _text,
                  maxLines: null,
                  enabled: false,
                  decoration: new InputDecoration(
                    border: InputBorder.none,
                    labelText: 'Message:',
                  ),
                ),
              ),
            ],
          ),
        ),
      );
  }

  ///
  ///
  ///
  List<DropdownMenuItem<BluetoothDevice>> _getDeviceItems() {
    List<DropdownMenuItem<BluetoothDevice>> items = [];
    if (_devices.isEmpty) {
      items.add(DropdownMenuItem(
        child: Text('NONE'),
      ));
    } else {
      _devices.forEach((device) {
        items.add(DropdownMenuItem(
          child: Text(device.name),
          value: device,
        ));
      });
    }
    return items;
  }

  BluetoothDevice testeroo() {
    var _deviceMenuItem = DropdownMenuItem<BluetoothDevice>(
      child: Text(_device.name),
      value: _device,
    );
    print(_device.toString());
    var menuItems = _getDeviceItems();
    var numberOfMatchingItems = menuItems.where((DropdownMenuItem<BluetoothDevice> item) => item.value == _deviceMenuItem.value).length;
    print("Number of matching items: $numberOfMatchingItems");
    menuItems.forEach((DropdownMenuItem<BluetoothDevice> item) => print(item.value.toString()));
    if (numberOfMatchingItems == 1) {
      return _device;
    } else {
      return null;
    }
  }

  ///
  ///
  ///
  void _connect() {
    if (_device == null) {
      show('No device selected.');
    } else {
      bluetooth.isConnected.then((isConnected) {
        if (!isConnected) {
          bluetooth.connect(_device).catchError((error) {
            lastConnectedDevice = _device;
            setState(() => _pressed = false);
          });
          setState(() => _pressed = true);
        }
      });
    }
  }




  ///
  ///
  ///
  void _disconnect() {
    bluetooth.disconnect();
    setState(() => _pressed = true);
  }

  ///
  ///
  ///
  void _writeTest() {
    bluetooth.isConnected.then((isConnected) {
      if (isConnected) {
        bluetooth.write(_message.text);
      }
    });
  }

  ///
  ///
  ///
  Future show(
      String message, {
        Duration duration: const Duration(seconds: 3),
      }) async {
    await new Future.delayed(new Duration(milliseconds: 100));
    Scaffold.of(context).showSnackBar(
      new SnackBar(
        content: new Text(
          message,
          style: new TextStyle(
            color: Colors.white,
          ),
        ),
        duration: duration,
      ),
    );
  }
}
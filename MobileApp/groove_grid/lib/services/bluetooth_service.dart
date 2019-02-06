

import 'dart:async';

import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'package:groove_grid/services/connection_service.dart';

class BluetoothStatus {
  static const int STATE_OFF = FlutterBluetoothSerial.STATE_OFF;
  static const int STATE_TURNING_ON = FlutterBluetoothSerial.STATE_TURNING_ON;
  static const int STATE_ON = FlutterBluetoothSerial.STATE_ON;
  static const int STATE_TURNING_OFF = FlutterBluetoothSerial.STATE_TURNING_OFF;
  static const int STATE_BLE_TURNING_ON = FlutterBluetoothSerial.STATE_BLE_TURNING_ON;
  static const int STATE_BLE_ON = FlutterBluetoothSerial.STATE_BLE_ON;
  static const int STATE_BLE_TURNING_OFF = FlutterBluetoothSerial.STATE_BLE_TURNING_OFF;
  static const int ERROR = FlutterBluetoothSerial.ERROR;
  static const int CONNECTED = FlutterBluetoothSerial.CONNECTED;
  static const int DISCONNECTED = FlutterBluetoothSerial.DISCONNECTED;
}

enum BluetoothState {
  STATE_OFF,
STATE_TURNING_ON,
STATE_ON,
STATE_TURNING_OFF,
STATE_BLE_TURNING_ON,
STATE_BLE_ON,
STATE_BLE_TURNING_OFF,
ERROR,
CONNECTED,
DISCONNECTED,
}

class BluetoothService implements ConnectionService {

  static final BluetoothService _singleton = BluetoothService._internal();

  FlutterBluetoothSerial _bluetoothSerial = FlutterBluetoothSerial.instance;

  factory BluetoothService() {
    return _singleton;
  }

  BluetoothService._internal() {
    _bluetoothSerial.onStateChanged().listen((int serialState) {
      BluetoothState state;
      switch(serialState) {
        case FlutterBluetoothSerial.STATE_OFF: {
          state = BluetoothState.STATE_OFF;
        }
        break;

        case FlutterBluetoothSerial.STATE_BLE_TURNING_ON: {
          state = BluetoothState.STATE_TURNING_ON;
        }
        break;

        case FlutterBluetoothSerial.STATE_ON: {
          state = BluetoothState.STATE_ON;
        }
        break;

        case FlutterBluetoothSerial.STATE_TURNING_OFF: {
          state = BluetoothState.STATE_TURNING_OFF;
        }
        break;

        case FlutterBluetoothSerial.STATE_BLE_TURNING_ON: {
          state = BluetoothState.STATE_BLE_TURNING_ON;
        }
        break;

        case FlutterBluetoothSerial.STATE_BLE_TURNING_OFF: {
          state = BluetoothState.STATE_BLE_TURNING_OFF;
        }
        break;

        case FlutterBluetoothSerial.STATE_BLE_ON: {
          state = BluetoothState.STATE_BLE_ON;
        }
        break;

        case FlutterBluetoothSerial.ERROR: {
          state = BluetoothState.ERROR;
        }
        break;

        case FlutterBluetoothSerial.DISCONNECTED: {
          state = BluetoothState.DISCONNECTED;
        }
        break;

        case FlutterBluetoothSerial.CONNECTED: {
          state = BluetoothState.CONNECTED;
        }
        break;
      }

      _stateChangeController.sink.add(state);

    });
  }


  BluetoothDevice _connectedDevice;
  BluetoothDevice _lastConnectedDevice;

  BluetoothDevice get lastConnectedDevice => _lastConnectedDevice;

  BluetoothDevice get connectedDevice => _connectedDevice;

  set connectedDevice(BluetoothDevice device) {
    _connectedDevice = device;
    if (device != null) _lastConnectedDevice = device;
    print("Connected Device: ${device?.name}");
    print("Last connected Device: ${_lastConnectedDevice?.name}");
  }

  StreamController<BluetoothState> _stateChangeController = StreamController<BluetoothState>.broadcast();

  // Forwards to BluetoothSerial

  //Stream<int> get stateChange => _bluetoothSerial.onStateChanged();

  Stream<BluetoothState> get stateChange => _stateChangeController.stream;

  Stream<String> get read => _bluetoothSerial.onRead();

  Future<bool> get isAvailable => _bluetoothSerial.isAvailable;

  Future<bool> get isOn => _bluetoothSerial.isOn;

  Future<bool> get isConnected => _bluetoothSerial.isConnected;

  Future<bool> get openSettings => _bluetoothSerial.openSettings;

  Future<List> getBondedDevices() => _bluetoothSerial.getBondedDevices();

  /// [device] must be of Type [BluetoothDevice]
  Future<dynamic> connect(dynamic device) {
    assert(device is BluetoothDevice);
    return _bluetoothSerial.connect(device).then((_) => connectedDevice = device);
  }

  Future<dynamic> disconnect() => _bluetoothSerial.disconnect()
      .then((_) => connectedDevice = null);

  Future<dynamic> write(String message) => _bluetoothSerial.write(message);

}
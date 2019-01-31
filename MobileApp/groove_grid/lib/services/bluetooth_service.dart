

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

class BluetoothService implements ConnectionService {

  static final BluetoothService _singleton = BluetoothService._internal();

  FlutterBluetoothSerial _bluetoothSerial = FlutterBluetoothSerial.instance;

  factory BluetoothService() {
    return _singleton;
  }

  BluetoothService._internal();

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

  // Forwards to BluetoothSerial

  Stream<int> get stateChange => _bluetoothSerial.onStateChanged();

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
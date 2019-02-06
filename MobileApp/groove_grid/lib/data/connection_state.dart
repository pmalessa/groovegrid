import 'package:groove_grid/services/bluetooth_service.dart';

class GridConnectionState {
  bool isConnected = false;

  BluetoothState connectionState;

  String lastReceivedMessage = "";
}
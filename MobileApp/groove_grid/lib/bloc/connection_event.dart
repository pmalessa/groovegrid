import 'package:groove_grid/services/bluetooth_service.dart';
import 'package:meta/meta.dart';

class ConnectionEvent {

}

class ConnectionStateChanged extends ConnectionEvent {
  BluetoothState state;

  ConnectionStateChanged({@required this.state}): super();
}

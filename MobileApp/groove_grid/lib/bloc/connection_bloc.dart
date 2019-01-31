import 'package:groove_grid/bloc/bloc.dart';
import 'package:groove_grid/bloc/connection_event.dart';
import 'package:groove_grid/data/connection_state.dart';
import 'package:groove_grid/services/bluetooth_service.dart';

class ConnectionBloc extends Bloc<ConnectionEvent, ConnectionState> {

  BluetoothService bluetooth = BluetoothService();

  @override
  // TODO: implement initialState
  ConnectionState get initialState => ConnectionState();

  @override
  Future<List<Sink>> mapEventToState(ConnectionEvent event) async {
    // TODO: implement mapEventToState
    List<Sink> sinks = [];

    if (event is ConnectionStateChanged) {
      if (event.state == BluetoothStatus.CONNECTED) {
        state.isConnected = true;
        sinks.add(outputSink);
      } else if (event.state == BluetoothStatus.DISCONNECTED) {
        state.isConnected = false;
        sinks.add(outputSink);
      }
    }

    return sinks;
  }

  ConnectionBloc() {
    setupListeners();
  }

  void setupListeners() {
    bluetooth.stateChange.listen((int state) => dispatch(ConnectionStateChanged(state: state)));
  }

}
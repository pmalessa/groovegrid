import 'package:groove_grid/bloc/bloc.dart';
import 'package:groove_grid/bloc/connection_event.dart';
import 'package:groove_grid/data/connection_state.dart';
import 'package:groove_grid/services/bluetooth_service.dart';


// TODO: Split up into ConnectionBloc and MessageBloc
class ConnectionBloc extends Bloc<ConnectionEvent, GridConnectionState> {

  BluetoothService bluetooth = BluetoothService();

  @override
  GridConnectionState get initialState => GridConnectionState();

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
    else if (event is MessageReceived) {
      state.lastReceivedMessage = event.message;
      sinks.add(outputSink);
    }

    return sinks;
  }

  ConnectionBloc() {
    setupListeners();
  }

  void setupListeners() {
    bluetooth.stateChange.listen((BluetoothState state) => dispatch(ConnectionStateChanged(state: state)));
    bluetooth.read.listen((String message) => dispatch(MessageReceived(message)));
  }

}
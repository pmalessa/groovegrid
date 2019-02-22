import 'package:groove_grid/bloc/bloc.dart';
import 'package:groove_grid/bloc/connection_event.dart';
import 'package:groove_grid/data/connection_state.dart';
import 'package:groove_grid/services/bluetooth_service.dart';
import 'package:tuple/tuple.dart';

// TODO: Split up into ConnectionBloc and MessageBloc
class ConnectionBloc extends Bloc<ConnectionEvent, GridConnectionState> {
  BluetoothService bluetooth = BluetoothService();

  @override
  GridConnectionState get initialState => GridConnectionState();

  @override
  Future<Tuple2<GridConnectionState, Set<Sink>>> mapEventToState(
      ConnectionEvent event) async {
    Set<Sink> sinks = Set<Sink>();

    if (event is ConnectionStateChanged) {
      if (event.state == BluetoothState.CONNECTED) {
        state.isConnected = true;
        sinks.add(outputSink);
      } else if (event.state == BluetoothState.DISCONNECTED) {
        state.isConnected = false;
        sinks.add(outputSink);
      }
    } else if (event is MessageReceived) {
      state.lastReceivedMessage = event.message;
      sinks.add(outputSink);
    }

    return Tuple2<GridConnectionState, Set<Sink>>(this.state, sinks);
  }

  ConnectionBloc() {
    setupListeners();
  }

  void setupListeners() {
    bluetooth.stateChange.listen((BluetoothState state) =>
        dispatch(ConnectionStateChanged(state: state)));
    bluetooth.read
        .listen((String message) => dispatch(MessageReceived(message)));
  }
}

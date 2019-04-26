import 'package:groove_grid/bloc/bloc.dart';
import 'package:groove_grid/bloc/connection_event.dart';
import 'package:groove_grid/data/connection_state.dart';
import 'package:groove_grid/services/bluetooth_service.dart';

// TODO: Split up into ConnectionBloc and MessageBloc
class ConnectionBloc extends Bloc<ConnectionEvent, GridConnectionState> {
  GrooveBluetoothService bluetooth = GrooveBluetoothService();

  @override
  GridConnectionState get initialState => GridConnectionState();

  @override
  Future<GridConnectionState> mapEventToState(
      ConnectionEvent event) async {

    if (event is ConnectionStateChanged) {
      if (event.state == BluetoothState.CONNECTED) {
        state.isConnected = true;
      } else if (event.state == BluetoothState.DISCONNECTED) {
        state.isConnected = false;
      }
    }

    return this.state;
  }

  ConnectionBloc() {
    setupListeners();
  }

  void setupListeners() {
    bluetooth.stateChange.listen((BluetoothState state) =>
        dispatch(ConnectionStateChanged(state: state)));
  }
}

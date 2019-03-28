import 'package:groove_grid/bloc/bloc.dart';
import 'package:groove_grid/services/bluetooth_service.dart';

class MessageBloc extends Bloc<GridMessageEvent, GridMessageState> {

  BluetoothService bluetooth = BluetoothService();

  MessageBloc():super() {
    setupListeners();
  }

  void setupListeners() {
    bluetooth.read
        .listen((String message) => dispatch(MessageReceived(message)));
  }

  @override
  // TODO: implement initialState
  GridMessageState get initialState => GridMessageState(lastReceivedMessage: "");

  @override
  Future<GridMessageState> mapEventToState(GridMessageEvent event) async {
    // TODO: implement mapEventToState

    if (event is MessageReceived) {
      GridMessageState newState = GridMessageState(lastReceivedMessage: event.message);
      return newState;
    }

    return state;
  }

  Future<dynamic> sendSwipe(SwipeDirection swipeDirection) {
    String directionMessage;
    switch(swipeDirection) {
      case SwipeDirection.up:
        directionMessage = "u";
          break;
      case SwipeDirection.down:
        directionMessage = "d";
        break;
      case SwipeDirection.left:
        directionMessage = "l";
        break;
      case SwipeDirection.right:
        directionMessage = "r";
        break;
    }
    return BluetoothService().write(directionMessage);
  }

  Future<dynamic> sendRestartCommand() {
    return BluetoothService().write("x");
  }

}

enum SwipeDirection {
  up,
  down,
  left,
  right,
}

class GridMessageEvent {
}

class MessageReceived extends GridMessageEvent {
  String message;

  MessageReceived(this.message): super();
}

class GridMessageState {
  String lastReceivedMessage;

  GridMessageState({this.lastReceivedMessage});
}
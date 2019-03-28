import 'package:groove_grid/bloc/bloc.dart';
import 'package:groove_grid/services/bluetooth_service.dart';

class MessageBloc extends Bloc<GridMessageEvent, GridMessageState> {
  @override
  // TODO: implement initialState
  GridMessageState get initialState => GridMessageState();

  @override
  Future<GridMessageState> mapEventToState(GridMessageEvent event) async {
    // TODO: implement mapEventToState
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

class GridMessageState {
}
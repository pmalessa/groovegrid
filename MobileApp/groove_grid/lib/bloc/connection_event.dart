import 'package:meta/meta.dart';

class ConnectionEvent {

}

class ConnectionStateChanged extends ConnectionEvent {
  int state;

  ConnectionStateChanged({@required this.state}): super();
}
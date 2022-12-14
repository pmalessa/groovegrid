abstract class ConnectionService {

  Stream<dynamic> get stateChange;

  Future<bool> get isConnected;

  Future connect(dynamic device);

  Future disconnect();

  Stream<String> get read;

  Future write(String message);
}
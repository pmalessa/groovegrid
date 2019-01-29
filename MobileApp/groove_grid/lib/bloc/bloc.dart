import 'dart:async';

import 'package:meta/meta.dart';

// E is an Event Type, S is a State
abstract class Bloc<E, S> {

  @protected
  final outputController = StreamController<int>();
  @protected
  StreamSink<int> get _outputSink => outputController.sink;
  // For state, exposing only a stream which outputs data
  Stream<int> get output => outputController.stream;

  @protected
  final inputController = StreamController<E>();
  // For events, exposing only a sink which is an input
  Sink<E> get input => inputController.sink;

  Bloc() {
    // Whenever there is a new event, we want to map it to a new state
    inputController.stream.listen(mapEventToState);
  }

  void mapEventToState(E event);

  void dispose() {
    outputController.close();
    inputController.close();
  }
}
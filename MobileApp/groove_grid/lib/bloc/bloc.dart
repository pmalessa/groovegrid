import 'dart:async';

import 'package:meta/meta.dart';

// E is an Event Type, S is a State
abstract class Bloc<Event, State> {
  
  State state;

  State get initialState;

  @protected
  final outputController = StreamController<int>();
  @protected
  StreamSink<int> get _outputSink => outputController.sink;
  // For state, exposing only a stream which outputs data
  Stream<int> get output => outputController.stream;

  @protected
  final inputController = StreamController<Event>();
  // For events, exposing only a sink which is an input
  Sink<Event> get input => inputController.sink;

  Bloc() {
    // Whenever there is a new event, we want to map it to a new state
    state = initialState;
    inputController.stream.listen(mapEventToState);
  }

  void mapEventToState(Event event);

  void dispose() {
    outputController.close();
    inputController.close();
  }
}
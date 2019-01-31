import 'dart:async';

import 'package:meta/meta.dart';

/// Takes a [Stream] of [Event]s as input
/// and transforms them into a [Stream] of [State]s as output.
abstract class Bloc<Event, State> {
  State state;

  State get initialState;

  @protected
  final outputController = StreamController<State>();
  @protected
  StreamSink<State> get outputSink => outputController.sink;
  // For state, exposing only a stream which outputs data
  /// Subscribe to this [Stream] to receive notifications
  /// whenever any change in the [State] object took place
  Stream<State> get output => outputController.stream;

  @protected
  final inputController = StreamController<Event>();
  // For events, exposing only a sink which is an input
  Sink<Event> get input => inputController.sink;

  Bloc() {
    // Whenever there is a new event, we want to map it to a new state
    state = initialState;
    inputController.stream.listen(_processEvent);
  }

  /// Takes an [Event] and triggers `mapEventToState`.
  /// `Dispatch` may be called from the presentation layer or from within the [Bloc].
  /// `Dispatch` notifies the [Bloc] of a new [Event].
  void dispatch(Event event) {
    input.add(event);
  }

  void _processEvent(Event event) async {
    List<Sink> sinks = await mapEventToState(event);
    if (sinks == null || sinks.isEmpty) return;

    sinks.forEach((sink) => sink.add(state));
  }

  /// Receives an [Event] and modifies the [State] object
  /// according to the data contained in the [Event].
  /// Returns a [List] of [Sink]s that should be triggered with the new [State].
  /// If the [State] did not change, null or an empty [List] may be returned
  /// which causes no sinks to be triggered.
  Future<List<Sink>> mapEventToState(Event event);

  /// Closes the [Event] and [State] [Stream]s.
  void dispose() {
    outputController.close();
    inputController.close();
  }
}





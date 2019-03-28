import 'dart:async';

import 'package:meta/meta.dart';

/// Takes a [Stream] of [Event]s as input
/// and transforms them into a [Stream] of [State]s as output.
abstract class Bloc<Event, State> {
  State state;

  State get initialState;

  @protected
  final outputController = StreamController<State>.broadcast();
  @protected
  StreamSink<State> get outputSink => outputController.sink;
  // For state, exposing only a stream which outputs data
  /// Subscribe to this [Stream] to receive notifications
  /// whenever any change in the [State] object took place
  Stream<State> get output => outputController.stream;

  @protected
  final inputController = StreamController<Event>.broadcast();
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
    State newState = await mapEventToState(event);
    if (newState != null) {
      this.state = newState;
      outputSink.add(newState);
    }
  }

  /// Receives an [Event] and modifies the [State] object or creates a new one
  /// according to the data contained in the [Event].
  /// Returns the new [State] that should be assigned to [this.state]
  /// and added to the standard output sink.
  /// If the [State] did not change, null may be returned
  /// which causes no sinks to be triggered and [this.state] stays the same.
  Future<State> mapEventToState(Event event);

  /// Closes the [Event] and [State] [Stream]s.
  void dispose() {
    outputController.close();
    inputController.close();
  }
}





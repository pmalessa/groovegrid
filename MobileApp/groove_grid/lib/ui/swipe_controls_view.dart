import 'package:flutter/material.dart';
import 'package:groove_grid/bloc/bloc_provider.dart';
import 'package:groove_grid/bloc/global_bloc.dart';
import 'package:groove_grid/bloc/message_bloc.dart';

class SwipeControlsView extends StatefulWidget {
  SwipeControlsView({Key key, @required this.title}) : super(key: key);

  final String title;

  @override
  _SwipeControlsViewState createState() => _SwipeControlsViewState();
}

class _SwipeControlsViewState extends State<SwipeControlsView> {
  String _labelText = "Swipe to move 2048 tiles in the corresponding direction";

  void _setLabel(String text) {
    setState(() {
      _labelText = text;
    });
  }

  @override
  Widget build(BuildContext context) {

    MessageBloc messageBloc = BlocProvider.of<GlobalBloc>(context).messageBloc;

    return Scaffold(
      appBar: AppBar(
        title: Text(widget.title),
        actions: <Widget>[
          IconButton(icon: Icon(Icons.refresh), onPressed: (){
            showDialog(
              context: context,
              builder: (BuildContext context) {
                // return object of type Dialog
                return AlertDialog(
                  backgroundColor: Theme.of(context).primaryColor,
                  title: new Text("Restart Game"),
                  content: new Text("Restarting will reset the game to its initial state. "
                      "All progress will be lost."),
                  actions: <Widget>[
                    // usually buttons at the bottom of the dialog
                    new FlatButton(
                      child: new Text("Cancel".toUpperCase()),
                      onPressed: () {
                        Navigator.of(context).pop();
                      },
                    ),
                    new FlatButton(
                      child: new Text("Restart".toUpperCase()),
                      onPressed: () {
                        print("Restarting Groovegrid App");
                        messageBloc.sendRestartCommand();
                        Navigator.of(context).pop();
                      },
                    ),
                  ],
                );
              },
            );
          }),
        ],
      ),
      body: GestureDetector(
        behavior: HitTestBehavior.translucent,
        onTap: () => print("Tap Gesture recognized!"),
        onHorizontalDragStart: (DragStartDetails details) async {
          print("Horizontal Drag Started!");
          print(details);
        },
        onHorizontalDragEnd: (DragEndDetails details) async {
          var velocity = details.primaryVelocity;
          print("Horitiontal Gesture recognized with speed $velocity");
          if (velocity < -2) {
            _setLabel("Left swipe detected!");
            messageBloc.sendSwipe(SwipeDirection.left);
          } else if (velocity > 2) {
            _setLabel("Right swipe detected!");
            messageBloc.sendSwipe(SwipeDirection.right);
          } else {
            _setLabel("Horizontal swipe detected!");
          }
        },
        onHorizontalDragCancel: () => (print("Vertical Drag cancelled!")),
        onVerticalDragEnd: (DragEndDetails details) async {
          var velocity = details.primaryVelocity;
          print("Vertical Gesture recognized with speed $velocity");
          if (velocity < -2) {
            _setLabel("Up swipe detected!");
            messageBloc.sendSwipe(SwipeDirection.up);
          } else if (velocity > 2) {
            _setLabel("Down swipe detected!");
            messageBloc.sendSwipe(SwipeDirection.down);
          } else {
            _setLabel("Vertical swipe detected!");
          }
        },
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              Text(_labelText),
            ],
          ),
        ),
      ),
    );
  }
}

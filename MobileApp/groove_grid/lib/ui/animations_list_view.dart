import 'package:flutter/material.dart';
import 'package:groove_grid/bloc/global_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';
import 'package:groove_grid/data/groove_grid_apps.dart';
import 'package:groove_grid/bloc/bloc_provider.dart';
import 'package:groove_grid/data/groove_grid_apps_state.dart';
import 'package:groove_grid/ui/grid_card.dart';

class AnimationsListView extends StatefulWidget {
  AnimationsListView({@required this.animations});

  final List<GrooveGridAnimation> animations;

  @override
  _AnimationsListViewState createState() => _AnimationsListViewState();
}

class _AnimationsListViewState extends State<AnimationsListView> {
  @override
  Widget build(BuildContext context) {

    final GrooveGridAppsBloc _appsBloc = BlocProvider.of<GlobalBloc>(context).grooveGridAppsBloc;

    ListTile makeListTile({@required String title, bool highlight}) => ListTile(
      contentPadding: EdgeInsets.symmetric(horizontal: 20.0, vertical: 0.0),
      leading: Container(
        padding: EdgeInsets.only(right: 12.0),
        decoration: new BoxDecoration(
            border: new Border(
                right: new BorderSide(
                    width: 1.0, color: Theme.of(context).hintColor))),
        child: Icon(Icons.bubble_chart, color: Theme.of(context).hintColor),
      ),
      title: Text(
        title,
        style: highlight
            ? Theme.of(context)
            .textTheme
            .subhead
            .apply(color: Theme.of(context).accentColor)
            : Theme.of(context)
            .textTheme
            .subhead, //TextStyle(color: Theme.of(context).text, fontWeight: FontWeight.bold),
      ),
      // subtitle: Text("Intermediate", style: TextStyle(color: Colors.white)),
      trailing: Icon(Icons.more_vert,
          color: Theme.of(context).hintColor, size: 25.0),
      onTap: null,
    );

    Widget makeCard(
        {@required String title, VoidCallback onPressed, bool highlight}) =>
        GridCard(
          elevation: 8.0,
          margin: new EdgeInsets.symmetric(horizontal: 10.0, vertical: 6.0),
          child: FlatButton(
            onPressed: onPressed,
            child: makeListTile(
                title: title, highlight: highlight != null ? highlight : false),
          ),
        );

    return StreamBuilder(
      initialData: _appsBloc.state,
      stream: _appsBloc.output,
      builder: (BuildContext context, AsyncSnapshot<GrooveGridAppsState> snapshot) {
        GrooveGridAppsState state = snapshot.data;
        return ListView.builder(
          itemCount: state.animations.length,
          itemBuilder: (context, index) {
            GrooveGridAnimation animation = state.animations[index];
            return makeCard(
              title: animation.title,
              onPressed: () {
                animation.start();
//            bool isCurrentlyRunning = animation == GrooveGridApp.runningApplication;
//            print("This is the currently running application: $isCurrentlyRunning");
//            print("Current running Application: ${GrooveGridApp.runningApplication}");
              },
              highlight: animation == state.runningApplication,
            );
          },
        );
      },
    );
  }
}
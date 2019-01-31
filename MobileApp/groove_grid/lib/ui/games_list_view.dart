import 'package:flutter/material.dart';
import 'package:groove_grid/bloc/bloc_provider.dart';
import 'package:groove_grid/bloc/global_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';
import 'package:groove_grid/data/controls.dart';
import 'package:groove_grid/data/groove_grid_apps.dart';
import 'package:groove_grid/data/groove_grid_apps_state.dart';
import 'package:groove_grid/ui/controls_view_builder.dart';

class GamesListView extends StatefulWidget {
  GamesListView({@required this.games});

  final List<GrooveGridGame> games;

  @override
  _GamesListViewState createState() => _GamesListViewState();
}

class _GamesListViewState extends State<GamesListView> {
  @override
  Widget build(BuildContext context) {
    final GrooveGridAppsBloc _appsBloc =
        BlocProvider.of<GlobalBloc>(context).grooveGridAppsBloc;

    ListTile makeListTile(
        {@required String title,
          String subtitle,
          IconData icon,
          double progress,
          @required bool highlight}) =>
        ListTile(
          contentPadding: EdgeInsets.symmetric(horizontal: 20.0, vertical: 0.0),
          leading: Container(
            padding: EdgeInsets.only(right: 12.0),
            decoration: new BoxDecoration(
              border: new Border(
                right: new BorderSide(
                  width: 1.0,
                  color: highlight
                      ? Theme.of(context).accentColor
                      : Theme.of(context).hintColor,
                ),
              ),
            ),
            child: Icon(
              Icons.videogame_asset,
              color: highlight
                  ? Theme.of(context).accentColor
                  : Theme.of(context).hintColor,
            ),
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

          subtitle: Row(
            children: <Widget>[
              Expanded(
                  flex: 1,
                  child: Container(
                    // tag: 'hero',
                    child: LinearProgressIndicator(
                        backgroundColor: Color.fromRGBO(209, 224, 224, 0.4),
                        value: progress != null ? progress : 0.0,
                        valueColor: AlwaysStoppedAnimation(Colors.green)),
                  )),
              Expanded(
                flex: 4,
                child: Padding(
                    padding: EdgeInsets.only(left: 10.0),
                    child: Text(subtitle != null ? subtitle : "Undefined",
                        style: Theme.of(context).textTheme.body1)),
              )
            ],
          ),
          trailing: Icon(Icons.keyboard_arrow_right,
              color: Theme.of(context).hintColor, size: 30.0),
          onTap: null,
        );

    Card makeCard(
        {@required String title,
          @required VoidCallback onPressed,
          String subtitle,
          IconData icon,
          double progress,
          bool highlight}) =>
        Card(
          elevation: 8.0,
          margin: new EdgeInsets.symmetric(horizontal: 10.0, vertical: 6.0),
          child: FlatButton(
            onPressed: onPressed,
            child: makeListTile(
              title: title,
              subtitle: subtitle,
              icon: icon,
              progress: progress,
              highlight: highlight,
            ),
          ),
        );

    return StreamBuilder(
      initialData: _appsBloc.state,
      stream: _appsBloc.output,
      builder: (
          BuildContext context,
          AsyncSnapshot<GrooveGridAppsState> snapshot,
          ) {
        GrooveGridAppsState state = snapshot.data;
        return ListView.builder(
          itemCount: state.games.length,
          itemBuilder: (context, index) {
            var game = state.games[index];
            return makeCard(
              title: game.title,
              onPressed: () {
                game.start().then((_) {
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) {
                      if (game.controls is SwipeControls) {
                        return ControlsViewBuilder(game);
                      }
                    }),
                  );
                });
              },
              subtitle: game.subtitle,
              icon: game.iconData,
              progress: game.progress,
              highlight:
              game == GrooveGridApp.runningApplication ? true : false,
            );
          },
        );
      },
    );
  }
}
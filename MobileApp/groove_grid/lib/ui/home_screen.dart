import 'package:flutter/material.dart';
import 'package:groove_grid/bloc/global_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';

import 'package:groove_grid/bloc/bloc_provider.dart';
import 'package:groove_grid/ui/bluetooth_settings_screen.dart';
import 'package:groove_grid/data/groove_grid_apps.dart';

import 'package:groove_grid/data/groove_grid_apps_state.dart';
import 'package:groove_grid/ui/animations_list_view.dart';
import 'package:groove_grid/ui/games_list_view.dart';

class HomeScreen extends StatefulWidget {
  HomeScreen({Key key, this.title,}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {

  GamesListView gamesView;
  AnimationsListView animationsView;

  _HomeScreenState() {
    //setupStreams();
  }

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
  }

  void setupStreams() {
    GrooveGridApp.onRunningApplicationChanged()
        .listen((GrooveGridApp runningApp) {
      print("Running App changed to $runningApp");
      // Only redraw to remove the controls floating action button
      // When an app that has controls starts, the controls view is opened,
      // so this view doesn't need to refresh
      if (!runningApp.hasControls) {
        setState(() {});
      }
    });
  }


  @override
  Widget build(BuildContext context) {

    final GrooveGridAppsBloc _appsBloc = BlocProvider.of<GlobalBloc>(context).grooveGridAppsBloc;

    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.
    return DefaultTabController(
      length: 2,
      child: Scaffold(
        appBar: AppBar(
          leading: IconButton(
            icon: Icon(Icons.settings_bluetooth),
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(
                    builder: (context) => BluetoothSettingsView()),
              );
            },
          ),
          // Here we take the value from the MyHomePage object that was created by
          // the App.build method, and use it to set our appbar title.
          title: Text(widget.title),
          bottom: TabBar(tabs: [
            Tab(icon: Icon(Icons.bubble_chart)),
            Tab(icon: Icon(Icons.videogame_asset)),
          ]),
        ),
        body: TabBarView(
          children: [
            AnimationsListView(
              animations: _appsBloc.state.animations,
            ),
            //Center(child: Text(_tab1LabelText, style: Theme.of(context).textTheme.subhead,)),
            GamesListView(
              games: _appsBloc.state.games,
            ),
          ],
        ),
        floatingActionButton: StreamBuilder(
          initialData: _appsBloc.state,
          stream: _appsBloc.output,
          builder: (BuildContext context, AsyncSnapshot<GrooveGridAppsState> snapshot) {
            GrooveGridAppsState state = snapshot.data;
            return state.runningApplication == null
                ? Container(width: 0, height: 0,)
                : !state.runningApplication.hasControls
                ? Container(width: 0, height: 0,)
                : FloatingActionButton(
              onPressed: () {
                state.runningApplication.start().then((_) {
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                        builder: (context) => state
                            .runningApplication.controlsView),
                  );
                });
              },
              tooltip: 'Play',
              child: Icon(Icons.play_arrow),
            );
          },
        ),
      ),
    );
  }
}
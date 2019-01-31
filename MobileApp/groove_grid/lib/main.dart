import 'package:flutter/material.dart';
import 'package:groove_grid/bloc/global_bloc.dart';
import 'package:groove_grid/bloc/groove_grid_apps_bloc.dart';

import 'package:groove_grid/bloc/bloc_provider.dart';

import 'package:groove_grid/model.dart';
import 'bluetooth.dart';
import 'groove_grid_apps.dart';

void main() {
  print("App starting...");
  runApp(GrooveGridRemoteApp());
}

class GrooveGridRemoteApp extends StatelessWidget {
  // This widget is the root of your application.

  final GrooveGridAppsState appState = GrooveGridAppsState();
  final GrooveGridAppsBloc appsBloc = GrooveGridAppsBloc();
  final GlobalBloc globalBloc = GlobalBloc();

  @override
  Widget build(BuildContext context) {
    return BlocProvider(
      bloc: globalBloc,
      child: MaterialApp(
        title: 'GrooveGrid',
        theme: ThemeData(
          // This is the theme of your application.
          //
          // Try running your application with "flutter run". You'll see the
          // application has a blue toolbar. Then, without quitting the app, try
          // changing the primarySwatch below to Colors.green and then invoke
          // "hot reload" (press "r" in the console where you ran "flutter run",
          // or simply save your changes to "hot reload" in a Flutter IDE).
          // Notice that the counter didn't reset back to zero; the application
          // is not restarted.
          primarySwatch: Colors.blue,
        ),
        home: HomePage(
          title: 'GrooveGrid',
          appState: appState,
        ),
      ),
    );
  }
}

class HomePage extends StatefulWidget {
  HomePage({Key key, this.title, @required this.appState}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;
  final GrooveGridAppsState appState;

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {

  GamesListView gamesView;
  AnimationsListView animationsView;

  _HomePageState() {
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

  GamesListView createGamesView() {
    gamesView = GamesListView(
      games: widget.appState.games,
    );
    return gamesView;
  }

  AnimationsListView createAnimationsView() {
    animationsView = AnimationsListView(
      animations: widget.appState.animations,
    );
    return animationsView;
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
            createAnimationsView(),
            //Center(child: Text(_tab1LabelText, style: Theme.of(context).textTheme.subhead,)),
            createGamesView(),
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
         // This trailing comma makes auto-formatting nicer for build methods.
      ),
    );
  }
}

import 'package:flutter/material.dart';
import 'bluetooth.dart';
import 'controls.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

void main() {
  print("App starting...");
  runApp(GrooveGridApp());
}

class GrooveGridApp extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
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
      home: HomePage(title: 'GrooveGrid'),
    );
  }
}

class HomePage extends StatefulWidget {
  HomePage({Key key, this.title}) : super(key: key);

  // This widget is the home page of your application. It is stateful, meaning
  // that it has a State object (defined below) that contains fields that affect
  // how it looks.

  // This class is the configuration for the state. It holds the values (in this
  // case the title) provided by the parent (in this case the App widget) and
  // used by the build method of the State. Fields in a Widget subclass are
  // always marked "final".

  final String title;

  @override
  _HomePageState createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  String _tab1LabelText = "First Tab";
  String _tab2LabelText = "Second Tab";

  void _setTab1Label(String text) {
    setState(() {
      _tab1LabelText = text;
    });
  }

  @override
  Widget build(BuildContext context) {
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
            icon: Icon(Icons.bluetooth),
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => BluetoothSettingsView()),
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
            AnimationsListView(),
            //Center(child: Text(_tab1LabelText, style: Theme.of(context).textTheme.subhead,)),
            GamesListView(),
          ],
        ),
        floatingActionButton: FloatingActionButton(
          onPressed: () {
            Navigator.push(
              context,
              MaterialPageRoute(
                  builder: (context) => SwipeControlsView(
                        title: 'Animations/2048',
                      )),
            );
          },
          tooltip: 'Play',
          child: Icon(Icons.play_arrow),
        ), // This trailing comma makes auto-formatting nicer for build methods.
      ),
    );
  }
}

class AnimationsListView extends StatefulWidget {
  @override
  _AnimationsListViewState createState() => _AnimationsListViewState();
}

class _AnimationsListViewState extends State<AnimationsListView> {
  @override
  Widget build(BuildContext context) {
    ListTile makeListTile(String title) => ListTile(
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
            style: Theme.of(context).textTheme.subhead,//TextStyle(color: Theme.of(context).text, fontWeight: FontWeight.bold),
          ),
          // subtitle: Text("Intermediate", style: TextStyle(color: Colors.white)),
          trailing:
              Icon(Icons.more_vert, color: Theme.of(context).hintColor, size: 25.0),
          onTap: null,
        );

    Card makeCard(String title) => Card(
          elevation: 8.0,
          margin: new EdgeInsets.symmetric(horizontal: 10.0, vertical: 6.0),
          child: Container(
            decoration: BoxDecoration(color: Theme.of(context).cardColor),
            child: makeListTile(title),
          ),
        );

    return ListView(
      children: <Widget>[
        makeCard("Standard Animation"),
        makeCard("New Animation"),
        makeCard("Another New Animation"),
      ],
    );
  }
}


class GamesListView extends StatefulWidget {
  @override
  _GamesListViewState createState() => _GamesListViewState();
}

class _GamesListViewState extends State<GamesListView> {
  @override
  Widget build(BuildContext context) {
    ListTile makeListTile({@required String title, String subtitle, Icon icon, double progress}) => ListTile(
      contentPadding: EdgeInsets.symmetric(horizontal: 20.0, vertical: 0.0),
      leading: Container(
        padding: EdgeInsets.only(right: 12.0),
        decoration: new BoxDecoration(
            border: new Border(
                right: new BorderSide(
                    width: 1.0, color: Theme.of(context).hintColor))),
        child: Icon(Icons.videogame_asset, color: Theme.of(context).hintColor),
      ),
      title: Text(
        title,
        style: Theme.of(context).textTheme.subhead,//TextStyle(color: Theme.of(context).text, fontWeight: FontWeight.bold),
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
                    value: progress != null? progress: 0.0,
                    valueColor: AlwaysStoppedAnimation(Colors.green)),
              )),
          Expanded(
            flex: 4,
            child: Padding(
                padding: EdgeInsets.only(left: 10.0),
                child: Text(subtitle != null? subtitle:"Undefined",
                    style: Theme.of(context).textTheme.body1)),
          )
        ],
      ),
      trailing:
      Icon(Icons.keyboard_arrow_right, color: Theme.of(context).hintColor, size: 30.0),
      onTap: null,
    );

    Card makeCard({@required String title, String subtitle, Icon icon, double progress}) => Card(
      elevation: 8.0,
      margin: new EdgeInsets.symmetric(horizontal: 10.0, vertical: 6.0),
      child: Container(
        decoration: BoxDecoration(color: Theme.of(context).cardColor),
        child: makeListTile(title: title, subtitle: subtitle, icon: icon, progress: progress),
      ),
    );

    return ListView(
      children: <Widget>[
        makeCard(title: "2048", subtitle: "Can math really be fun?", progress: 0.3),
        makeCard(title: "Invisible", subtitle: "Sneak yourself to victory", progress: 0.8),
        makeCard(title: "Whack-A-Mole", subtitle: "Can you whack 'em all?", progress: 0.5),
      ],
    );
  }
}

import 'package:flutter/material.dart';

class GridThemeData {
  final Color shadowColor;

  GridThemeData({this.shadowColor});

  @override
  bool operator ==(other) {
    // TODO: implement ==
    return other is GridThemeData && other.shadowColor == shadowColor;
  }

  @override
  // TODO: implement hashCode
  int get hashCode => shadowColor.hashCode;

}

class GridTheme extends InheritedWidget {

  final GridThemeData data;

  GridTheme({this.data, Widget child}): super(child: child);

  @override
  bool updateShouldNotify(InheritedWidget oldWidget) {
    // TODO: implement updateShouldNotify
    return oldWidget != this;
  }
}
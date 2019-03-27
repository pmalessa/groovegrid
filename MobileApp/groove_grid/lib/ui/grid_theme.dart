import 'package:flutter/material.dart';

enum Highlight {
  foreground,
  background,
}

class GridThemeData {
  final Color shadowColor;

  final Color highlightedShadowColor;

  final Highlight highlightBehaviour;
  final Gradient highlightGradient;

  GridThemeData(
      {this.shadowColor,
      this.highlightedShadowColor,
      this.highlightBehaviour,
      this.highlightGradient});

  @override
  bool operator ==(other) {
    // TODO: implement ==
    return other is GridThemeData &&
        other.shadowColor == shadowColor &&
        other.highlightBehaviour == highlightBehaviour &&
        other.highlightedShadowColor == highlightedShadowColor;
  }

  @override
  // TODO: implement hashCode
  int get hashCode => shadowColor.hashCode;
}

class GridTheme extends InheritedWidget {
  final GridThemeData data;

  GridTheme({this.data, Widget child}) : super(child: child);

  static GridThemeData of(BuildContext context) {
    final GridTheme provider =
        context.ancestorInheritedElementForWidgetOfExactType(GridTheme)?.widget;

    if (provider == null) {
      throw FlutterError(
          'GridTheme.of() called with a context that does not contain ThemeData.\n'
          'No ancestor could be found starting from the context that was passed '
          'to GridTheme.of(). This can happen '
          'if the context you use comes from a widget above the GridTheme.\n'
          'The context used was:\n'
          '  $context');
    }
    return provider?.data;
  }

  @override
  bool updateShouldNotify(InheritedWidget oldWidget) {
    // TODO: implement updateShouldNotify
    return oldWidget != this;
  }
}

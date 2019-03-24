import 'package:flutter/material.dart';
import 'package:groove_grid/ui/grid_theme.dart';




/// A material design card. A card has slightly rounded corners and a shadow.
///
/// A card is a sheet of [Material] used to represent some related information,
/// for example an album, a geographical location, a meal, contact details, etc.
///
/// This is what it looks like when run:
///
/// ![A card with a slight shadow, consisting of two rows, one with an icon and
/// some text describing a musical, and the other with buttons for buying
/// tickets or listening to the show.](https://flutter.github.io/assets-for-api-docs/assets/material/card.png)
///
/// {@tool snippet --template=stateless_widget}
///
/// This sample shows creation of a [Card] widget that shows album information
/// and two actions.
///
/// ```dart
/// Center(
///   child: Card(
///     child: Column(
///       mainAxisSize: MainAxisSize.min,
///       children: <Widget>[
///         const ListTile(
///           leading: Icon(Icons.album),
///           title: Text('The Enchanted Nightingale'),
///           subtitle: Text('Music by Julie Gable. Lyrics by Sidney Stein.'),
///         ),
///         ButtonTheme.bar( // make buttons use the appropriate styles for cards
///           child: ButtonBar(
///             children: <Widget>[
///               FlatButton(
///                 child: const Text('BUY TICKETS'),
///                 onPressed: () { /* ... */ },
///               ),
///               FlatButton(
///                 child: const Text('LISTEN'),
///                 onPressed: () { /* ... */ },
///               ),
///             ],
///           ),
///         ),
///       ],
///     ),
///   ),
/// )
/// ```
/// {@end-tool}
///
/// See also:
///
///  * [ListTile], to display icons and text in a card.
///  * [ButtonBar], to display buttons at the bottom of a card. Typically these
///    would be styled using a [ButtonTheme] created with [new ButtonTheme.bar].
///  * [showDialog], to display a modal card.
///  * <https://material.google.com/components/cards.html>
class GridCardOld extends StatelessWidget {

  /// Creates a material design card.
  ///
  /// The [clipBehavior] argument must not be null.
  const GridCardOld({
    Key key,
    this.color,
    this.elevation,
    this.shape,
    this.margin = const EdgeInsets.all(4.0),
    this.clipBehavior = Clip.none,
    this.child,
    this.semanticContainer = true,
    this.shadowColor = Colors.black,
  }) : super(key: key);

  /// The card's background color.
  ///
  /// Defines the card's [Material.color].
  ///
  /// The default color is defined by the ambient [Theme]: [ThemeData.cardColor].
  final Color color;

  /// The z-coordinate at which to place this card. This controls the size of
  /// the shadow below the card.
  ///
  /// Defines the card's [Material.elevation].
  ///
  /// The default elevation is 1.0.
  final double elevation;

  /// The shape of the card's [Material].
  ///
  /// Defines the card's [Material.shape].
  ///
  /// The default shape is a [RoundedRectangleBorder] with a circular corner
  /// radius of 4.0.
  final ShapeBorder shape;

  /// {@macro flutter.widgets.Clip}
  final Clip clipBehavior;

  /// The empty space that surrounds the card.
  ///
  /// Defines the card's outer [Container.margin].
  ///
  /// The default margin is 4.0 logical pixels on all sides:
  /// `EdgeInsets.all(4.0)`.
  final EdgeInsetsGeometry margin;

  /// Whether this widget represents a single semantic container, or if false
  /// a collection of individual semantic nodes.
  ///
  /// Defaults to true.
  ///
  /// Setting this flag to true will attempt to merge all child semantics into
  /// this node. Setting this flag to false will force all child semantic nodes
  /// to be explicit.
  ///
  /// This flag should be false if the card contains multiple different types
  /// of content.
  final bool semanticContainer;

  /// The card's shadow color.
  ///
  /// Defines the card's [Material.shadowColor].
  ///
  /// The default color is fully opaque black.
  final Color shadowColor;

  /// The widget below this widget in the tree.
  ///
  /// {@macro flutter.widgets.child}
  final Widget child;

  @override
  Widget build(BuildContext context) {
    return Semantics(
      container: semanticContainer,
      explicitChildNodes: !semanticContainer,
      child: Container(
        margin: margin ?? const EdgeInsets.all(4.0),
        child: Material(
          type: MaterialType.card,
          color: color ?? Theme.of(context).cardColor,
          shadowColor: this.shadowColor,
          elevation: elevation ?? 1.0,
          shape: shape ?? const RoundedRectangleBorder(
            borderRadius: BorderRadius.all(Radius.circular(4.0)),
          ),
          clipBehavior: clipBehavior,
          child: child,
        ),
      ),
    );
  }
}


/// A material design card. A card has slightly rounded corners and a shadow.
///
/// A card is a sheet of [Material] used to represent some related information,
/// for example an album, a geographical location, a meal, contact details, etc.
///
/// This is what it looks like when run:
///
/// ![A card with a slight shadow, consisting of two rows, one with an icon and
/// some text describing a musical, and the other with buttons for buying
/// tickets or listening to the show.](https://flutter.github.io/assets-for-api-docs/assets/material/card.png)
///
/// {@tool snippet --template=stateless_widget}
///
/// This sample shows creation of a [Card] widget that shows album information
/// and two actions.
///
/// ```dart
/// Center(
///   child: Card(
///     child: Column(
///       mainAxisSize: MainAxisSize.min,
///       children: <Widget>[
///         const ListTile(
///           leading: Icon(Icons.album),
///           title: Text('The Enchanted Nightingale'),
///           subtitle: Text('Music by Julie Gable. Lyrics by Sidney Stein.'),
///         ),
///         ButtonTheme.bar( // make buttons use the appropriate styles for cards
///           child: ButtonBar(
///             children: <Widget>[
///               FlatButton(
///                 child: const Text('BUY TICKETS'),
///                 onPressed: () { /* ... */ },
///               ),
///               FlatButton(
///                 child: const Text('LISTEN'),
///                 onPressed: () { /* ... */ },
///               ),
///             ],
///           ),
///         ),
///       ],
///     ),
///   ),
/// )
/// ```
/// {@end-tool}
///
/// Sometimes the primary action area of a card is the card itself. Cards can be
/// one large touch target that shows a detail screen when tapped.
///
/// {@tool snippet --template=stateless_widget}
///
/// This sample shows creation of a [Card] widget that can be tapped. When
/// tapped this [Card]'s [InkWell] displays an "ink splash" that fills the
/// entire card.
///
/// ```dart
/// Card(
///   child: InkWell(
///     splashColor: Colors.blue.withAlpha(30),
///     onTap: () { /* ... */ },
///     child: Text('A card that can be tapped'),
///   ),
/// )
/// ```
///
/// {@end-tool}
///
/// See also:
///
///  * [ListTile], to display icons and text in a card.
///  * [ButtonBar], to display buttons at the bottom of a card. Typically these
///    would be styled using a [ButtonTheme] created with [new ButtonTheme.bar].
///  * [showDialog], to display a modal card.
///  * <https://material.io/design/components/cards.html>
class GridCard extends StatelessWidget {
  /// Creates a material design card.
  ///
  /// The [elevation] must be null or non-negative. The [borderOnForeground]
  /// must not be null.
  const GridCard({
    Key key,
    this.color,
    this.gradient,
    this.elevation,
    this.shape,
    this.borderOnForeground = true,
    this.margin,
    this.clipBehavior,
    this.child,
    this.semanticContainer = true,
    this.shadowColor,
    this.decoration,
    this.border,
  }) : assert(elevation == null || elevation >= 0.0),
        assert(borderOnForeground != null),
        super(key: key);

  /// The card's background color.
  ///
  /// Defines the card's [Material.color].
  ///
  /// If this property is null then [ThemeData.cardTheme.color] is used,
  /// if that's null then [ThemeData.cardColor] is used.
  final Color color;

  /// If [gradient] is specified, the card no longer uses the background color
  /// as defined by [color]. Instead [gradient] is used for the card's background.
  ///
  /// If this property is null then the card has
  /// a single background color defined by [color].
  final Gradient gradient;

  /// The z-coordinate at which to place this card. This controls the size of
  /// the shadow below the card.
  ///
  /// Defines the card's [Material.elevation].
  ///
  /// If this property is null then [ThemeData.cardTheme.elevation] is used,
  /// if that's null, the default value is 1.0.
  final double elevation;

  /// The shape of the card's [Material].
  ///
  /// Defines the card's [Material.shape].
  ///
  /// If this property is null then [ThemeData.cardTheme.shape] is used.
  /// If that's null then the shape will be a [RoundedRectangleBorder] with a
  /// circular corner radius of 4.0.
  final ShapeBorder shape;

  /// Whether to paint the [shape] border in front of the [child].
  ///
  /// The default value is true.
  /// If false, the border will be painted behind the [child].
  final bool borderOnForeground;

  /// {@macro flutter.widgets.Clip}
  /// If this property is null then [ThemeData.cardTheme.clipBehavior] is used.
  /// If that's null then the behavior will be [Clip.none].
  final Clip clipBehavior;

  /// The empty space that surrounds the card.
  ///
  /// Defines the card's outer [Container.margin].
  ///
  /// If this property is null then [ThemeData.cardTheme.margin] is used,
  /// if that's null, the default margin is 4.0 logical pixels on all sides:
  /// `EdgeInsets.all(4.0)`.
  final EdgeInsetsGeometry margin;

  /// Whether this widget represents a single semantic container, or if false
  /// a collection of individual semantic nodes.
  ///
  /// Defaults to true.
  ///
  /// Setting this flag to true will attempt to merge all child semantics into
  /// this node. Setting this flag to false will force all child semantic nodes
  /// to be explicit.
  ///
  /// This flag should be false if the card contains multiple different types
  /// of content.
  final bool semanticContainer;

  /// The card's shadow color.
  ///
  /// Defines the card's [Material.shadowColor].
  ///
  /// The default color is fully opaque black.
  final Color shadowColor;

  final Decoration decoration;

  final Border border;

  /// The widget below this widget in the tree.
  ///
  /// {@macro flutter.widgets.child}
  final Widget child;

  static const double _defaultElevation = 1.0;
  static const Clip _defaultClipBehavior = Clip.none;
  static const Color _defaultShadowColor = Colors.black;



  @override
  Widget build(BuildContext context) {
    final CardTheme cardTheme = CardTheme.of(context);

    final Decoration defaultDecoration = BoxDecoration(
      color: gradient != null ? null : color ?? cardTheme.color ?? Theme.of(context).cardColor,
      gradient: gradient,
      boxShadow: <BoxShadow>[
        BoxShadow(
          color: shadowColor ?? GridTheme.of(context).shadowColor,
          blurRadius: 20.0,
          offset: Offset(0, 2),
        ),
      ],
      border: border ?? Border.all(
        color: Color(0xFFA9156D),
        width: 1,
      ),
      borderRadius: BorderRadius.only(bottomLeft: Radius.circular(100), topRight: Radius.circular(100)),
    );

    return Semantics(
      container: semanticContainer,
      child: Container(
        margin: margin ?? cardTheme.margin ?? const EdgeInsets.all(4.0),
        decoration: decoration ?? defaultDecoration,
        child: Material(
          type: MaterialType.transparency,
          shadowColor: this.shadowColor ?? GridTheme.of(context).shadowColor ?? _defaultShadowColor,
          elevation: elevation ?? cardTheme.elevation ?? _defaultElevation,
          shape: shape ?? cardTheme.shape ?? const RoundedRectangleBorder(
            borderRadius: BorderRadius.all(Radius.circular(4.0)),
          ),
          borderOnForeground: borderOnForeground,
          clipBehavior: clipBehavior ?? cardTheme.clipBehavior ?? _defaultClipBehavior,
          child: Semantics(
            explicitChildNodes: !semanticContainer,
            child: child,
          ),
        ),
      ),
    );
  }
}


//class GridCard extends Card {
//
//  /// Creates a material design Groovegrid card.
//  ///
//  /// The [clipBehavior] argument must not be null.
//  const GridCard({
//    Key key,
//    Color color,
//    double elevation,
//    ShapeBorder shape,
//    EdgeInsetsGeometry margin,
//    Clip clipBehavior,
//    Widget child,
//    bool semanticContainer,
//    this.shadowColor = Colors.black,
//  }) : super(
//            key: key,
//            color: color,
//            elevation: elevation,
//            shape: shape,
//            margin: margin,
//            clipBehavior: clipBehavior,
//            child: child,
//            semanticContainer: semanticContainer);
//
//
//  /// The card's background color.
//  ///
//  /// Defines the card's [Material.shadowColor].
//  ///
//  /// The default color is fully opaque black.
//  final Color shadowColor;
//
//
//  @override
//  Widget build(BuildContext context) {
//    return Semantics(
//      container: semanticContainer,
//      explicitChildNodes: !semanticContainer,
//      child: Container(
//        margin: margin ?? const EdgeInsets.all(4.0),
//        child: Material(
//          type: MaterialType.card,
//          color: color ?? Theme.of(context).cardColor,
//          shadowColor: this.shadowColor,
//          elevation: elevation ?? 1.0,
//          shape: shape ??
//              const RoundedRectangleBorder(
//                borderRadius: BorderRadius.all(Radius.circular(4.0)),
//              ),
//          clipBehavior: clipBehavior,
//          child: child,
//        ),
//      ),
//    );
//  }
//}

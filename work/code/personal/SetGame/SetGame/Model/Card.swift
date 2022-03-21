/// Card represents a single SET card from a standard SET Game® deck.
///
public class Card {

    private(set) var color   : CardColor;
    private(set) var shape   : CardShape;
    private(set) var filling : CardFilling;
    private(set) var number  : CardNumber;

    /// Creates a card from a String representation of the card or nil if not unparsable.
    /// Required so we (CardList) can create a generic Card or subclass thereof.
    ///
    required convenience init?(_ value: String) {
        if let card = Card.from(value) {
            self.init(card);
        }
        else {
            return nil;
        }
    }

    /// Creates a card from a Substring representation of the card or nil if not unparsable.
    /// Required so we (CardList) can create a generic Card or subclass thereof.
    ///
    required convenience init?(_ value: Substring) {
        self.init(String(value));
    }

    /// Creates random card.
    ///
    required convenience init() {
         self.init(color: .random, shape: .random, filling: .random, number: .random);
    }

    /// Creates Card from given specific attributes (designated init).
    ///
    required init(color: CardColor, shape: CardShape, filling: CardFilling, number: CardNumber) {
        self.color   = color;
        self.shape   = shape;
        self.filling = filling;
        self.number  = number;
    }

    /// Creates card from given Card; i.e. copy constructor (designated init).
    ///
    required init(_ card : Card) {
        self.color   = card.color;
        self.shape   = card.shape;
        self.filling = card.filling;
        self.number  = card.number;
    }

    /// Returns true iff the given two cards form a SET with this card, otherwise false.
    ///
    func formsSetWith(_ b: Card, _ c: Card) -> Bool {
        return Card.isSet(self, b, c);
    }

    /// Returns the unique card which completes the SET for the given two cards.
    ///
    static func matchingSetValue(_ b: Card, _ c: Card) -> Card {
        let color   : CardColor   = .matchingSetValue(b.color,   c.color);
        let shape   : CardShape   = .matchingSetValue(b.shape,   c.shape);
        let filling : CardFilling = .matchingSetValue(b.filling, c.filling);
        let number  : CardNumber  = .matchingSetValue(b.number,  c.number);
        return Card(color: color, shape: shape, filling: filling, number: number);
    }

    /// Returns true iff the given cards form a SET; note this is static.
    /// Maybe could reside in CardList; but will keep the rules of SET
    /// logic in here; and in the attribute classes, i.e. formsSetWith,
    /// matchingSetValue in CardColor, CardShape, CardFilling, CardNumber.
    ///
    static func isSet(_ cards : Card...) -> Bool {
        if (cards.count != 3) { return false; }
        return cards[0].color   .formsSetWith(cards[1].color,   cards[2].color)
            && cards[0].shape   .formsSetWith(cards[1].shape,   cards[2].shape)
            && cards[0].filling .formsSetWith(cards[1].filling, cards[2].filling)
            && cards[0].number  .formsSetWith(cards[1].number,  cards[2].number);
    }

    static func isSet(_ cards : [Card]) -> Bool {
        return cards.count == 3 && Card.isSet(cards[0], cards[1], cards[2]);
    }

    /// Returns the unique 'codename' for this card.
    /// This can be used as a short name for this card and could conveniently
    /// be used to identify an (image) asset for the card.
    ///
    var codename : String {
        return color.codename + shape.codename + filling.codename + number.codename;
    }

    /// Parses a string representation of a SET card and returns its Card instance; returns
    /// nil if unparsable. This representation can be a dash-separated list of attribute
    /// names, e.g. 'Red-Oval-Stripped-Two' where the order of attributes doesn't matter.
    /// A set of single characters, unique across all attributes, are also defined as
    /// follows, so we can represent a card like 'ROS2' (meaning: Red-Oval-Solid-Two):
    ///
    /// Color:   G = Green
    ///          P = Purple
    ///          R = Red
    ///
    /// Shape:   D = Diamond
    ///          O = Oval
    ///          Q = Squiggle
    ///
    /// Filling: H = Hollow
    ///          S = Solid
    ///          T = Stripped
    ///
    /// Number:  1 = One
    ///          2 = Two
    ///          3 = Three
    ///
    /// Cards represented with these codes will also be used for the Card 'codename'
    /// which will be used to identify the name of the image asset to use in the UI;
    /// attributes being in the above order, i.e. color, shape, filling, number.
    ///
    class func from(_ value: String) -> Card? {
        var value = value.filter{!$0.isWhitespace}.lowercased();
        if (value.count == 4) {
            value = String(value[0]) + "-" +
                    String(value[1]) + "-" +
                    String(value[2]) + "-" +
                    String(value[3]) + "-";
        }
        let components = value.split(){($0 == "-") || ($0 == ":")};
        if (components.count == 4) {
            var components = components.map {String($0)};
            if let (color, index) = CardColor.from(components)  {
                components.remove(at: index);
                if let (shape, index) = CardShape.from(components)  {
                    components.remove(at: index);
                    if let (filling, index) = CardFilling.from(components)  {
                        components.remove(at: index);
                        if let (number, _) = CardNumber.from(components)  {
                            return Card(color: color, shape: shape, filling: filling, number: number);
                        }
                    }
                }
            }
        }
        return nil;
    }

    /// Returns a string representation of this card.
    ///
    func toString(_ verbose : Bool = false) -> String {
        return verbose ? "\(color)-\(shape)-\(filling)-\(number)"
                       : "\(codename)"
    }
}

/// Card extensions to conform to sundry protocols.
///
extension Card : Identifiable, Equatable, CustomStringConvertible {

    public var id : String { self.codename; }

    public static func == (lhs: Card, rhs: Card) -> Bool {
        return (lhs.color   == rhs.color)   &&
               (lhs.shape   == rhs.shape)   &&
               (lhs.filling == rhs.filling) &&
               (lhs.number  == rhs.number);
    }

    public var description : String {
        return self.toString();
    }
}

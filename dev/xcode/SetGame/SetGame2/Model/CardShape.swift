/// CardShape represents the shape attributes of a SET card.
///
enum CardShape : Int, CaseIterable {

    case Diamond;
    case Oval;
    case Squiggle;

    /// Returns the single  'codename' character/string for this attribute.
    ///
    var codename : String {
        switch self {
            case .Diamond:  return "D";
            case .Oval:     return "O";
            case .Squiggle: return "Q";
        }
    }

    /// Returns true iff the given two CardShape attributes
    /// form a SET with this CardShape, otherwise false.
    ///
    func formsSetWith(_ b: CardShape, _ c: CardShape) -> Bool {
        return !(((self == b) && (self != c)) ||
                 ((self == c) && (self != b)) ||
                 ((b    == c) && (b   != self)));
    }

    /// Returns the unique CardShape attribute which would
    /// complete the SET for the given two CardShape attributes.
    ///
    static func matchingSetValue(_ a: CardShape, _ b: CardShape) -> CardShape {
        if (a == b) {
            return a;
        }
        switch a {
            case .Diamond:  return (b == Oval)    ? Squiggle : Oval;
            case .Oval:     return (b == Diamond) ? Squiggle : Diamond;
            case .Squiggle: return (b == Diamond) ? Oval     : Diamond;
        }
    }

    /// Parses a string representation of a SET card shape and
    /// returns its CardShape instance; returns nil if unparsable.
    ///
    static func from(_ value: String) -> CardShape? {
        let value = value.filter{!$0.isWhitespace}.lowercased();
        switch value {
            case "d",
                 "diamond",
                 "diamonds":  return Diamond;
            case "o",
                 "oval":      return Oval;
            case "q",
                 "squiggle",
                 "squiggled": return Squiggle;
            default:          return nil;
        }
    }

    /// Parses each of the strings in the given array, per the from method above,
    /// and returns the CardShape for the first one that maches, and its index.
    ///
    static func from(_ values: [String]) -> (shape: CardShape, index: Int)? {
        for (index,value) in values.enumerated() {
            if let result = CardShape.from(value) {
                return (result, index);
            }
        }
        return nil;
    }

    /// Returns a random enumerated value.
    ///
    static var random : CardShape {
        return CardShape.allCases.randomElement()!;
    }
}

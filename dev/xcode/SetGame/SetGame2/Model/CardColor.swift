/// CardColor represents the color attributes of a SET card.
///
enum CardColor : Int, CaseIterable {

    case Green;
    case Purple;
    case Red;

    /// Returns the single  'codename' character/string for this attribute.
    ///
    var codename : String {
        switch self {
            case .Green:  return "G";
            case .Purple: return "P";
            case .Red:    return "R";
        }
    }

    /// Returns true iff the given two CardColor attributes
    /// form a SET with this CardColor, otherwise false.
    ///
    func formsSetWith(_ b: CardColor, _ c: CardColor) -> Bool {
        return !(((self == b) && (self != c)) ||
                 ((self == c) && (self != b)) ||
                 ((b    == c) && (b    != self)));
    }

    /// Returns the unique CardColor attribute which would
    /// complete the SET for the given two CardColor attributes.
    ///
    static func matchingSetValue(_ a: CardColor, _ b: CardColor) -> CardColor {
        if (a == b) {
            return a;
        }
        switch a {
            case .Green:  return (b == Purple) ? Red    : Purple;
            case .Purple: return (b == Green)  ? Red    : Green;
            case .Red:    return (b == Green)  ? Purple : Green;
        }
    }

    /// Parses a string representation of a SET card color and
    /// returns its CardColor instance; returns nil if unparsable.
    ///
    static func from(_ value: String) -> CardColor? {
        let value = value.filter{!$0.isWhitespace}.lowercased();
        switch value {
            case "g",
                 "green":  return Green;
            case "r",
                 "red":    return Red;
            case "p",
                 "purple": return Purple;
            default:       return nil;
        }
    }

    /// Parses each of the strings in the given array, per the from method above,
    /// and returns the CardColor for the first one that maches, and its index.
    ///
    static func from(_ values: [String]) -> (color: CardColor, index: Int)? {
        for (index,value) in values.enumerated() {
            if let result = CardColor.from(value) {
                return (result, index);
            }
        }
        return nil;
    }

    /// Returns a random enumerated value.
    ///
    static var random : CardColor {
        return CardColor.allCases.randomElement()!;
    }
}

/// CardFilling represents the filling attributes of a SET card.
///
enum CardFilling : Int, CaseIterable {

    case Hollow;
    case Solid;
    case Stripped;

    /// Returns the single  'codename' character/string for this attribute.
    ///
    var codename : String {
        switch self {
            case .Hollow:   return "H";
            case .Solid:    return "S";
            case .Stripped: return "T";
        }
    }

    /// Returns true iff the given two CardFilling attributes
    /// form a SET with this CardFilling, otherwise false.
    ///
    func formsSetWith(_ b: CardFilling, _ c: CardFilling) -> Bool {
        return !(((self == b) && (self != c)) ||
                 ((self == c) && (self != b)) ||
                 ((b    == c) && (b    != self)));
    }

    /// Returns the unique CardFilling attribute which would
    /// complete the SET for the given two CardFilling attributes.
    ///
    static func matchingSetValue(_ a: CardFilling, _ b: CardFilling) -> CardFilling {
        if (a == b) {
            return a;
        }
        switch a {
            case .Hollow:   return (b == Solid)  ? Stripped : Solid;
            case .Solid:    return (b == Hollow) ? Stripped : Hollow;
            case .Stripped: return (b == Hollow) ? Solid    : Hollow;
        }
    }

    /// Parses a string representation of a SET card filling and
    /// returns its CardFilling instance; returns nil if unparsable.
    ///
    static func from(_ value: String) -> CardFilling? {
        let value = value.filter{!$0.isWhitespace}.lowercased();
        switch value {
            case "h",
                 "hollow":   return Hollow;
            case "s",
                 "solid":    return Solid;
            case "t",
                 "stripe",
                 "stripes",
                 "stripped": return Stripped;
            default:         return nil;
        }
    }

    /// Parses each of the strings in the given array, per the from method above,
    /// and returns the CardFilling for the first one that maches, and its index.
    ///
    static func from(_ values: [String]) -> (filling: CardFilling, index: Int)? {
        for (index,value) in values.enumerated() {
            if let result = CardFilling.from(value) {
                return (result, index);
            }
        }
        return nil;
    }

    /// Returns a random enumerated value.
    ///
    static var random : CardFilling {
        return CardFilling.allCases.randomElement()!;
    }
}

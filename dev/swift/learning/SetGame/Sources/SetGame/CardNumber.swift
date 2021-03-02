/// CardNumber represents the number attributes of a SET card.
///
enum CardNumber : Int, CaseIterable  {

    case One   = 1;
    case Two   = 2;
    case Three = 3;

    /// Returns the single  'codename' character/string for this attribute.
    ///
    var codename : String {
        switch self {
            case .One:   return "1";
            case .Two:   return "2";
            case .Three: return "3";
        }
    }

    /// Returns true iff the given two CardNumber attributes
    /// form a SET with this CardNumber, otherwise false.
    ///
    func formsSetWith(_ b: CardNumber, _ c: CardNumber) -> Bool {
        return !(((self == b) && (self != c)) ||
                 ((self == c) && (self != b)) ||
                 ((b    == c) && (b    != self)));
    }

    /// Returns the unique CardNumber attribute which would
    /// complete the SET for the given two CardNumber attributes.
    ///
    static func matchingSetValue(_ a: CardNumber, _ b: CardNumber) -> CardNumber {
        if (a == b) {
            return a;
        }
        switch a {
            case .One:   return (b == Two) ? Three : Two;
            case .Two:   return (b == One) ? Three : One;
            case .Three: return (b == One) ? Two   : One;
        }
    }

    /// Parses a string representation of a SET card number and
    /// returns its CardNumber instance; returns nil if unparsable.
    ///
    static func from(_ value: String) -> CardNumber? {
        let value = value.filter{!$0.isWhitespace}.lowercased();
        switch value {
            case "1",
                 "one":   return .One;
            case "2",
                 "two":   return .Two;
            case "3",
                 "three": return .Three;
            default:      return nil;
        }
    }

    /// Parses each of the strings in the given array, per the from method above,
    /// and returns the CardNumber for the first one that maches, and its index.
    ///
    static func from(_ values: [String]) -> (number: CardNumber, index: Int)? {
        for (index,value) in values.enumerated() {
            if let result = CardNumber.from(value) {
                return (result, index);
            }
        }
        return nil;
    }

    /// Returns a random enumerated value.
    ///
    static var random : CardNumber {
        return CardNumber.allCases.randomElement()!;
    }
}

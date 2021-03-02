enum CardSuit : String, CaseIterable {

    case Clubs;
    case Diamonds;
    case Hearts;
    case Spades;

    var isRed : Bool {
        switch self {
        case .Diamonds, .Hearts:
            return true;
        default:
            return false;
        }
    }

    var isBlack : Bool {
        return !self.isRed;
    }

    var description : String {
        return self.toString();
    }

    func toString(_ verbose : Bool = false) -> String {
        switch self {
            case .Clubs:    return verbose ? self.rawValue : "C";
            case .Diamonds: return verbose ? self.rawValue : "D";
            case .Hearts:   return verbose ? self.rawValue : "H";
            case .Spades:   return verbose ? self.rawValue : "S";
        }
    }

    static func from(_ value: String) -> CardSuit? {
        switch value.lowercased() {
            case "c", "clubs":    return .Clubs;
            case "d", "diamonds": return .Diamonds;
            case "h", "hearts":   return .Hearts;
            case "s", "spades":   return .Spades;
            default:              return  nil;
        }
    }
}

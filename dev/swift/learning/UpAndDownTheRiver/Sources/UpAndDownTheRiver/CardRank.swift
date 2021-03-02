enum CardRank : Int, CaseIterable, CustomStringConvertible {

    case Two   =  2;
    case Three =  3;
    case Four  =  4;
    case Five  =  5;
    case Six   =  6;
    case Seven =  7;
    case Eight =  8;
    case Nine  =  9;
    case Ten   = 10;
    case Jack  = 11;
    case Queen = 12;
    case King  = 13;
    case Ace   = 14;
    
    var isFace : Bool {
        switch self {
        case .Jack, .Queen, .King:
            return true;
        default:
            return false;
        }
    }

    var description : String {
        return self.toString();
    }

    func toString(_ verbose : Bool = false) -> String {
        switch self {
            case .Two:   return verbose ? "Two"    :  "2";
            case .Three: return verbose ? "Three"  :  "3";
            case .Four:  return verbose ? "Four"   :  "4";
            case .Five:  return verbose ? "Five"   :  "5";
            case .Six:   return verbose ? "Six"    :  "6";
            case .Seven: return verbose ? "Seven"  :  "7";
            case .Eight: return verbose ? "Eight"  :  "8";
            case .Nine:  return verbose ? "Nine"   :  "9";
            case .Ten:   return verbose ? "Ten"    : "10";
            case .Jack:  return verbose ? "Jack"   :  "J";
            case .Queen: return verbose ? "Queen"  :  "Q";
            case .King:  return verbose ? "King"   :  "K";
            case .Ace:   return verbose ? "Ace"    :  "A";
        }
    }

    static func from(_ value: String) -> CardRank? {
        switch value.lowercased() {
            case "2",  "two":        return .Two;
            case "3",  "three":      return .Three;
            case "4",  "four":       return .Four;
            case "5",  "five":       return .Five;
            case "6",  "six":        return .Six;
            case "7",  "seven":      return .Seven;
            case "8",  "eight":      return .Eight;
            case "9",  "nine":       return .Nine;
            case "10", "ten":        return .Ten;
            case "11", "jack",  "j": return .Jack;
            case "12", "queen", "q": return .Queen;
            case "13", "king",  "k": return .King;
            case "14", "ace",   "a": return .Ace;
            default:                 return  nil;
        }
    }
}

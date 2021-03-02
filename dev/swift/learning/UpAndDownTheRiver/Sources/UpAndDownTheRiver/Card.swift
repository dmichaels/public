class Card {

    public private(set) var suit: CardSuit;
    public private(set) var rank: CardRank;

    init(_ suit: CardSuit, _ rank: CardRank) {
        self.suit = suit;
        self.rank = rank;
    }

    init(_ rank: CardRank, _ suit: CardSuit) {
        self.rank = rank;
        self.suit = suit;
    }

    var isFace : Bool {
        return self.rank.isFace;
    }

    var isRed: Bool {
        return self.suit.isRed;
    }

    var isBlack: Bool {
        return self.suit.isBlack;
    }

    static func from(_ value: String) -> Card? {

        // First try parsing values like: H-A, A-H, hearts-ace, ace-hearts

        let components = value.split() { $0 == "-" }

        if (components.count == 2) {

            let first  : Substring = components[0];
            let second : Substring = components[1];

            if let card = Card.from(String(first), String(second)) {
                return card;
            }

            if let card = Card.from(String(second), String(first)) {
                return card;
            }
        }

        // Then try parsing values like: HA, AH, 10H, H10

        if (value.count >= 2) {

            var suitPart : Substring = value.prefix(1);
            var rankPart : Substring = value.suffix(value.count - 1);

            if let card = Card.from(String(suitPart), String(rankPart)) {
                return card;
            }

            suitPart = value.suffix(1);
            rankPart = value.prefix(value.count - 1);

            if let card = Card.from(String(suitPart), String(rankPart)) {
                return card;
            }
        }

        return nil;
    }

    private static func from(_ suitPart: String, _ rankPart: String) -> Card? {
        var rank : CardRank? = nil;
        var suit : CardSuit? = nil;
        suit = CardSuit.from(String(suitPart));
        if (suit != nil) {
            rank = CardRank.from(String(rankPart));
            if (rank != nil) {
                return Card(suit!, rank!);
            }
        }
        return nil;
    }
}

extension Card : Equatable, CustomStringConvertible {

    static func == (lhs: Card, rhs: Card) -> Bool {
        return lhs.rank == rhs.rank && lhs.suit == rhs.suit;
    }

    var description : String {
        return self.toString();
    }

    func toString(_ verbose : Bool = false) -> String {
        return "\(suit.toString(verbose))-\(rank.toString(verbose))";
    }
}

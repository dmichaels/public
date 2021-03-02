/// Deck represents a standard deck of SET Game® cards.
/// Actually supports multiple decks in a single Deck.
///
class Deck : CardList<Card> {

    let readonly: Bool;

    static private let pristine : Deck = Deck(readonly: true);

    /// Creates a new shuffled SET Game® card deck.
    /// Can be a "simple" deck in which case the only filling is solid; this,
    /// surprisingly, is actually the default for the official SET Game® app deck.
    /// Can contain just a random subset of cards if the given ncards is greater than 1.
    /// Can be readonly (for the pristine deck here) in which case no cards will be removed.
    ///
    init(ncards: Int = 0, simple: Bool = false, readonly: Bool = false) {
        self.readonly = readonly;
        let fillings = simple ? [CardFilling.Solid] : CardFilling.allCases;
        super.init([]);
        for color in CardColor.allCases {
            for shape in CardShape.allCases {
                for filling in fillings {
                    for number in CardNumber.allCases {
                        super.add(Card(color: color, shape: shape, filling: filling, number: number));
                    }
                }
            }
        }
        if ((ncards > 0) && (ncards < super.count)) {
            let nremove = super.count - ncards;
            for _ in 0..<nremove {
                _ = super.takeRandomCard();
            }
        }
        self.shuffle();
    }

    static func randomCard() -> Card {
        let i = Int.random(in: 0..<Deck.pristine.count);
        return Deck.pristine[i];
    }

    static func randomSet() -> CardList<Card> {
        let deck = Deck();
        let a = deck.takeRandomCard()!;
        let b = deck.takeRandomCard()!;
        let c = Card.matchingSetValue(a, b);
        return CardList(a, b, c);
    }

    /// Returns the number of cards in a standard SET Game® deck.
    ///
    static var size : Int = {
        return pristine.count;
    }();

    /// Returns the average number of SETs in a deal of the given number of cards.
    ///
    static func averageNumberOfSets(_ ncards: Int, iterations: Int = 500) -> Float {
        if (ncards < 3) {
            return 0;
        }
        var totalSets = 0;
        for _ in 1...iterations {
            let deck = Deck();
            let cards = deck.takeRandomCards(ncards);
            totalSets += cards.numberOfSets();
        }
        return Float(totalSets) / Float(iterations);
    }

    /// Base class overrides to enforce readonly for the Deck.

    override func add(_ card : Card, _ cards : Card...) {
        if (!self.readonly) {
            super.add(card);
            super.add(cards);
        }
    }

    override func add(_ cards : [Card]) {
        if (!self.readonly) {
            super.add(cards);
        }
    }

    override func add(_ cards : CardList<Card>) {
        if (!self.readonly) {
            super.add(cards);
        }
    }

    override func remove(_ card : Card, _ cards : Card...) {
        if (!self.readonly) {
            super.remove(card);
            super.remove(cards);
        }
    }

    override func remove(_ cards : [Card]) {
        if (!self.readonly) {
            super.remove(cards);
        }
    }

    override func remove(_ cards : CardList<Card>) {
        if (!self.readonly) {
            super.remove(cards);
        }
    }

    override func clear() {
        if (!self.readonly) {
            super.clear();
        }
    }

    override func takeCard() -> Card? {
        if (!self.readonly) {
            return super.takeCard();
        }
        return super[0];
    }

    override func takeRandomCard() -> Card? {
        if (!self.readonly) {
            return super.takeRandomCard();
        }
        return super.randomCard();
    }

    override func takeRandomCards(_ n : Int, plantSet: Bool = false) -> CardList<Card> {
        if (!self.readonly) {
            return super.takeRandomCards(n, plantSet: plantSet);
        }
        return super.randomCards(n);
    }
}

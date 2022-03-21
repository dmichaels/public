/// Deck represents a deck of SET Game® card deck.
/// Can be created with a subclass of Card if you like.
///
class Deck<T : Card> {

    public private(set) var cards    : [T];
    public              let readonly : Bool;

    /// Creates a new shuffled SET Game® card deck.
    /// - Can be a "simple" deck in which case the only filling is solid; this,
    ///   surprisingly, is actually the default for the official SET Game® app deck.
    /// - Can contain just a random subset of cards if the given ncards is greater than 1;
    ///   the default card count is, of course, 3 * 3 * 3 * 3 = 81.
    /// - Can be readonly (for the standard deck here) in which case no cards will be removed.
    ///
    init(simple: Bool = false, ncards: Int = 0, readonly: Bool = false) {
        self.readonly = readonly;
        self.cards = [T]();
        let fillings : [CardFilling] = simple ? [CardFilling.Solid] : CardFilling.allCases;
        for color in CardColor.allCases {
            for shape in CardShape.allCases {
                for filling in fillings {
                    for number in CardNumber.allCases {
                        cards.add(T(color: color, shape: shape, filling: filling, number: number));
                    }
                }
            }
        }
        if ((ncards > 0) && (ncards < cards.count)) {
            let nremove = cards.count - ncards;
            for _ in 0..<nremove {
                _ = cards.takeRandomCard();
            }
        }
        self.cards.shuffle();
    }

    var count : Int {
        return self.cards.count;
    }

    /// Returns true iff the given card is in this deck.
    ///
    func contains(_ card : T) -> Bool {
        return self.cards.contains(card);
    }

    /// Returns true iff the given card is in this deck.
    /// Don't fully understand why this it's necessary to have this specialized
    /// function for Card vs. T; need because we get a compiler type mismatch
    /// error when calling the T based function above with a card of type Card.
    ///
    func contains(_ card : Card) -> Bool {
        return self.cards.contains(T(card));
    }

    func takeCard(_ card : T) -> T? {
        if (self.contains(card)) {
            self.cards.remove(card);
            return card;
        }
        return nil;
    }

    func takeCard(_ card : Card) -> Card? {
        if (self.contains(card)){
            self.cards.remove(T(card));
            return card;
        }
        return nil;
    }
    /// Removes and returns a random card from this deck;
    /// returns nil if no more cards in the deck.
    //
    func takeRandomCard() -> T? {
        return self.readonly ? nil : self.cards.takeRandomCard();
    }

    func takeRandomCards(_ n : Int, plantSet: Bool = false) -> [T] {
        return self.readonly ? [T]() : self.cards.takeRandomCards(n, plantSet: plantSet);
    }

    /// Returns true iff there exists at least one SET in this deck.
    ///
    func containsSet() -> Bool {
        return self.cards.containsSet();
    }

    /// Returns the number of unique SETs in this deck.
    ///
    func numberOfSets() -> Int {
        return self.cards.numberOfSets();
    }

    /// Identifies/enumerates any/all SETs in this deck and returns them in an array
    /// of array of cards, each representing a unique (possibily overlaping) SET
    /// within this deck. If no SETs exist then returns an empty array.
    ///
    func enumerateSets(limit: Int = 0) -> [[T]] {
        return self.cards.enumerateSets(limit: limit);
    }

    /// Returns the number of cards in a standard deck.
    ///
    static var count : Int {
        return StandardDeck.instance.count;
    }

    /// Returns (without removal) a random card from a standard deck.
    ///
    static func randomCard() -> T {
        return T(StandardDeck.instance.randomCard());
    }

    /// Returns (without removal) a random SET from a standard deck.
    ///
    static func randomSet() -> [T] {
        return StandardDeck.instance.randomSet().map { T($0) };
    }

    /// Returns the average number of SETs in a deal of the given number of cards.
    ///
    static func averageNumberOfSets(_ ncards: Int, iterations: Int = 100) -> Float {
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
}

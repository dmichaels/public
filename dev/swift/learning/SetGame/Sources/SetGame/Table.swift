/// Table represents a SET Game® table for (dis)play.
/// TODO: VERY MUCH A WIP ...
///
class Table {

    var deck                : Deck;
    var cards               : CardList<TableCard>;
    var lastSetFound        : CardList<TableCard>?;
    var initialCardCount    : Int;
    var foundSetCount       : Int;
    var incorrectGuessCount : Int;

    init(initialCardCount : Int = 9) {
        self.deck                = Deck();
        self.cards               = CardList<TableCard>();
        self.initialCardCount    = initialCardCount;
        self.foundSetCount       = 0;
        self.incorrectGuessCount = 0;
        self.lastSetFound        = nil;
    }

    /// Fresh deal.
    ///
    func deal() {
        self.deck = Deck();
        self.cards = CardList();
        self.fill();
    }
    
    /// Selects the given card.
    ///
    func selectCard(card : TableCard) {

        if (card.selected) {
            //
            // Selecting a selected cards unselects it.
            //
            card.selected = false;
            self.updateState();
        }

        let selectedCards = self.selectedCards();

        if (selectedCards.count >= 3) {
            //
            // Cannot select more than three cards; noop. 
            //
            return;
        }

        card.selected = true;

        self.updateState();
    }

    /// Unselect all cards.
    ///
    func unselectCards() {
        self.cards.forEach { $0.selected = false };
    }

    /// Returns the set (CardList) of currently selected cards.
    ///
    func selectedCards() -> CardList<TableCard> {
        return self.cards.filter { $0.selected };
    }

    /// Returns the number of currently selected cards.
    ///
    func selectedCardCount() -> Int {
        return self.selectedCards().count;
    }

    /// Returns the number SETs present on the table.
    ///
    func setsPresentCount() -> Int {
        return self.cards.numberOfSets();
    }

    func setsPresent() -> [CardList<TableCard>] {
        return self.cards.enumerateSets();
    }

    /// Returns the number of cards remaining in the deck.
    ///
    func remainingCardCount() -> Int {
        return self.deck.count;
    }

    /// Returns true iff the currently selected cards for a partial SET.
    ///
    func partialSetSelected() -> Bool {
        let selectedCards = self.selectedCards();
        if (selectedCards.count == 1) {
            let sets = self.setsPresent();
            for set in sets {
                if (set.contains(selectedCards[0])) {
                    return true;
                }
            }
        }
        else if (selectedCards.count == 2) {
            let sets = self.setsPresent();
            for set in sets {
                if (set.contains(selectedCards[0]) || set.contains(selectedCards[1])) {
                    return true;
                }
            }
        }
        return false;
    }

    private func updateState() {

        let selectedCards = self.selectedCards();

        if (selectedCards.count == 3) {
            if (selectedCards.isSet()) {
                self.foundSetCount += 1;
                self.lastSetFound = selectedCards;
                self.cards.remove(selectedCards);
                self.fill();
            }
            else {
                self.incorrectGuessCount += 1;
                self.unselectCards();
            }
        }
        else if (selectedCards.count == 2) {
        }
    }

    /// Repopulate the table cards up to the initialCardCount.
    ///
    private func fill() {
        for _ in 1...(initialCardCount - self.cards.count)  {
            if let card = self.deck.takeRandomCard() {
                self.cards.add(TableCard(card));
            }
            else {
                break;
            }
        }
    }
}

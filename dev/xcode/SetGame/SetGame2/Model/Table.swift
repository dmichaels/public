import SwiftUI

/// Table represents a SET Game® table for (dis)play.
/// We mainly have a deck from which we deal, and the set/array of table cards
/// which are on display; and sundry other data points.
///
class Table<TC : TableCard> : ObservableObject {

    struct Settings {
        var table                           : Table?;
        var preferredCardCount              : Int  = 12;
        var cardsPerRow                     : Int  = 4;
        var useSimpleDeck                   : Bool = false {
            didSet {
                self.table?.startNewGame(simple: self.useSimpleDeck);
            }
        }
        var plantSet                        : Bool = false;
        var showPartialSetSelectedIndicator : Bool = true;
        var showNumberOfSetsPresent         : Bool = true;
        var moveAnyExistingSetToFront       : Bool = false {
            didSet {
                if (self.table != nil && self.moveAnyExistingSetToFront) {
                    self.table?.moveAnyExistingSetToFront();
                }
            }
        }
    }

    struct State {
        var partialSetSelected            : Bool = false;
        var incorrectGuessCount           : Int  = 0;
        var setsFoundCount                : Int  = 0;
        var setJustFound                  : Bool = false;
        var setJustNotFound               : Bool = false;
        var setLastFound                  : [TC] = [TC]();
        var showingCardsWhichArePartOfSet : Bool = false;
    }

                      private      var deck     : Deck<TC>;
    @Published public private(set) var cards    : [TC];
    @Published                     var state    : State;
    @Published                     var settings : Settings;

    init(preferredCardCount : Int = 9, plantSet : Bool = false) {
        self.deck                        = Deck();
        self.cards                       = [TC]();
        self.state                       = State();
        self.settings                    = Settings();
        self.settings.preferredCardCount = preferredCardCount;
        self.settings.plantSet           = plantSet;
        self.settings.table              = self;
        self.fillTable();
    }

    func startNewGame(simple : Bool = false) {
        self.deck  = Deck(simple: simple);
        self.cards = [TC]();
        self.state = State();
        self.fillTable();
    }

    /// Touch the given card; selects or unselects as appropriate.
    ///
    func touchCard(_ card : TC) {

        if (!self.cards.contains(card)) {
            //
            // Given card isn't even on the table; no-op.
            // This should not happen!
            //
            return;
        }

        self.state.setJustFound = false;
        self.state.setJustNotFound = false;

        if (self.state.showingCardsWhichArePartOfSet) {
            //
            // Three or more cards selected; presumably
            // due to selectAllCardsWhichArePartOfSet;
            // unselect all.
            //
            //
            self.unselectCards();
            self.state.showingCardsWhichArePartOfSet = false;
        }

        if (card.selected) {
            //
            // Selecting a selected cards unselects it.
            //
            card.selected = false;
            self.state.partialSetSelected = self.partialSetSelected();
            return;
        }

        let selectedCards = self.selectedCards();

        // Select the given card.

        card.selected = true;
        self.state.partialSetSelected = self.partialSetSelected();
    }

    /// Checks whether or not a SET is currently selected on the table.
    /// If a SET is selected, then removes these selected SET cards and
    /// replaces them with new ones from the deck (in an unselected state).
    /// If a non-SET is selected (i.e. three cards selected but which do
    /// not form SET), then these cards will simply be unselected.
    ///
    func checkForSet() {

        let selectedCards = self.selectedCards();

        // See if we have a SET selected now.

        if (selectedCards.count == 3) {
            //
            // Here we either have a SET selected or a wrong guess.
            //
            if (selectedCards.isSet()) {
                //
                // SET!
                // Unselect the SET cards, calling the given callback if any,
                // and then remove these SET cards from the table,
                // and replace them with cards from the deck.
                //
                self.state.setsFoundCount += 1;
                self.state.setJustFound = true;
                self.state.setLastFound = selectedCards;
                self.unselectCards(selectedCards, set: true);
                self.cards.remove(selectedCards);
                self.fillTable();
            }
            else {
                //
                // Dummy :-(
                //
                self.state.setJustNotFound = true;
                self.state.incorrectGuessCount += 1;
                self.unselectCards();
            }
        }
    }

    func selectAllCardsWhichArePartOfSet() {
        self.unselectCards();
        let sets : [[TC]] = self.enumerateSets();
        for set in sets {
            for card in set {
                card.selected = true;
            }
        }
    }

    /// Unselects all cards.
    ///
    func unselectCards(set: Bool = false) {
        self.unselectCards(self.cards, set: set);
    }

    /// Unselects the given cards (could be static).
    /// If the set parameter is true then mark the card as part of a SET.
    ///
    private func unselectCards(_ cards    : [TC],
                                 set      : Bool = false) {
        cards.forEach {
            let card : TC = $0;
            card.selected = false;
            if (set) {
                card.set = true;
            }
        }
    }

    /// Returns the set (array) of currently selected cards.
    ///
    func selectedCards() -> [TC] {
        return self.cards.filter { $0.selected };
    }

    /// Returns the number of currently selected cards.
    ///
    func selectedCardCount() -> Int {
        return self.selectedCards().count;
    }

    /// Returns true iff there is at least one SET present on the table.
    ///
    func containsSet() -> Bool {
        return self.cards.containsSet();
    }

    /// Returns the number SETs present on the table.
    ///
    func numberOfSets() -> Int {
        return self.cards.numberOfSets();
    }

    /// Identifies/enumerates any/all SETs present on this table,
    /// and returns them in an array of array of cards, each being
    /// a unique (possibily overlaping) SET within the table cards.
    /// If no SETs exist then returns an empty array.
    ///
    func enumerateSets() -> [[TC]] {
        return self.cards.enumerateSets();
    }

    /// Returns the number of cards remaining in the deck.
    ///
    func remainingCardCount() -> Int {
        return self.deck.count;
    }

    /// Returns true iff the currently selected cards form a partial SET.
    ///
    func partialSetSelected() -> Bool {
        let selectedCards = self.selectedCards();
        if (selectedCards.count == 1) {
            let cardA = selectedCards[0];
            let sets = self.enumerateSets();
            for set in sets {
                if (set.contains(cardA)) {
                    return true;
                }
            }
        }
        else if (selectedCards.count == 2) {
            let cardA = selectedCards[0];
            let cardB = selectedCards[1];
            let sets = self.enumerateSets();
            for set in sets {
                if (set.contains(cardA) && set.contains(cardB)) {
                    return true;
                }
            }
        }
        return false;
    }

    /// Move and SET which might exist in the table cards
    /// to the front of the cards list/array.
    ///
    func moveAnyExistingSetToFront() {
        _ = self.cards.moveAnyExistingSetToFront();
    }

    /// Adds (at most) the given number of cards to the table from the deck.
    ///
    func addMoreCards(_ ncards : Int, plantSet : Bool? = nil) {
        let plantSet : Bool = (plantSet == nil) ? self.settings.plantSet : plantSet!;
        if (ncards > 0) {
            if (plantSet && !self.containsSet() && (self.cards.count + min(self.deck.count, ncards)) >= 3) {
                //
                // If we want a SET planted, and only if there are not already
                // any SETs on the table, if there are even enough cards between
                // what's on the table and we're adding and what's left in the
                // deck a then try to plant a SET with these newly added cards.
                //
                if (ncards >= 3) {
                    self.cards.add(self.deck.takeRandomCards(ncards, plantSet: true));
                }
                else {
                    for i in 0..<(self.cards.count - 1) {
                        for j in (i + 1)..<(self.cards.count) {
                            let a : TC = self.cards[i];
                            let b : TC = self.cards[j];
                            let c : TC = TC(Card.matchingSetValue(a, b));
                            if let c = self.deck.takeCard(c) {
                                self.cards.add(c);
                                self.addMoreCards(ncards - 1, plantSet: false);
                            }
                        }
                    }
                }
            }
            else {
                self.cards.add(self.deck.takeRandomCards(ncards));
            }
            if (self.settings.moveAnyExistingSetToFront) {
                self.moveAnyExistingSetToFront();
            }
        }
    }

    /// Populate the table cards from the deck up to the preferredCardCount.
    ///
    private func fillTable() {
        self.addMoreCards(self.settings.preferredCardCount - self.cards.count);
    }
}

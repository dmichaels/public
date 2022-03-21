/// Array extension for convenience in handling simple Card arrays.
///
extension Array where Element : Card {

    /// Adds the given card(s) to this array.
    ///
    init(_ cards : Element...) {
        self = cards;
    }

    /// Creates an array of card(s) containgin cards specified by given string
    /// representations of cards to this array. Empty array of unparseable.
    ///
    init(_ values : String) {
        self = Self.from(values);
    }

    init(_ values : [String]) {
        self = Self.from(values);
    }

    /// Adds given card(s) to this array.
    ///
    mutating func add(_ card : Element, _ cards : Element...) {
        self.append(card);
        self.append(contentsOf: cards);
    }

    mutating func add(_ cards : [Element]) {
        self.append(contentsOf: cards);
    }

    /// Remove (all instances of the) given card(s) from this array.
    ///
    mutating func remove(_ card : Element, _ cards  : Element...) {
        self.removeAll(where: {$0 == card});
        cards.forEach { let card = $0; self.removeAll(where: {$0 == card}); }
    }

    mutating func remove(_ cards : [Element]) {
        cards.forEach { self.remove($0); }
    }

    mutating func clear() {
        self.removeAll();
    }

    /// Removes and returns the first card from this array;
    /// returns nil if no more cards in the array.
    ///
    mutating func takeCard() -> Element? {
        return self.count > 0 ? self.remove(at: 0) : nil;
    }

    /// Removes and returns a random card from this array;
    /// returns nil if no more cards in the array.
    ///
    mutating func takeRandomCard() -> Element? {
        return (self.count > 0) ? self.remove(at: Int.random(in: 0..<self.count)) : nil;
    }

    /// Removes and returns, at most, the specified number of random cards
    /// from this array, in a new array; if fewer cards are in this array
    /// than the number requested, then so be it, just that many will
    /// be returned, and then this array will end up being empty.
    ///
    /// If the plantSet argument is true then the returned array will, if possible,
    /// contain (at least) one SET, randomly distributed in the returned cards array.
    ///
    mutating func takeRandomCards(_ n : Int, plantSet: Bool = false) -> [Element] {
        var n = n < 0 ? 0 : n;
        var cards : [Element] = [Element]();
        var plantedSet : [Element]? = nil;
        if (plantSet && (n >= 3)) {
            let set : [[Element]] = self.enumerateSets(limit: 1);
            if (set.count == 1) {
                plantedSet = set[0];
                self.remove(plantedSet!);
                if (n == 3) {
                    return plantedSet!
                }
            }
        }
        if let plantedSet = plantedSet {
            cards.add(plantedSet);
            n -= plantedSet.count;
        }
        for _ in 0..<n {
            if let card = self.takeRandomCard() {
                cards.add(card);
            }
            else {
                break;
            }
        }
        if (plantedSet != nil) {
            cards.shuffle();
        }
        return cards;
    }

    /// Returns (without removal), a random card from this array,
    /// or nil if this array is empty.
    ///
    func randomCard() -> Element? {
        return (self.count > 0) ? self[Int.random(in: 0..<self.count)] : nil;
    }

    func isSet() -> Bool {
        return (self.count == 3) && Element.isSet(self[0], self[1], self[2]);
    }

    func formsSetWith(_ a : Element, _ b : Element) -> Bool {
        return self.count == 1 && self[0].formsSetWith(a, b);
    }

    func formsSetWith(_ a : Element) -> Bool {
        return self.count == 2 && self[0].formsSetWith(self[1], a);
    }

    /// Returns true iff there exists at least one SET in this array.
    ///
    func containsSet() -> Bool {
        var nsets : Int = 0;
        self.enumerateSets(limit: 1) { _ in nsets += 1; }
        return nsets > 0;
    }

    /// Returns the number of unique SETs in this array.
    ///
    func numberOfSets() -> Int {
        var nsets : Int = 0;
        self.enumerateSets() { _ in nsets += 1; }
        return nsets;
    }

    /// Identifies/enumerates any/all SETs in this array and returns them in an array
    /// of array of cards, each representing a unique (possibily overlaping) SET
    /// within this array. If no SETs exist then returns an empty array.
    ///
    func enumerateSets(limit: Int = 0) -> [[Element]] {
        var sets : [[Element]] = [[Element]]();
        self.enumerateSets(limit: limit) { sets.append($0); }
        return sets;
    }

    func enumerateSets(limit: Int = 0, _ handler : ([Element]) -> Void) {
        var nsets : Int = 0;
        if (self.count > 2) {
            for i in 0..<(self.count - 2) {
                for j in (i + 1)..<(self.count - 1) {
                    for k in (j + 1)..<(self.count) {
                        let a : Element = self[i], b : Element = self[j], c : Element = self[k];
                        if (a.formsSetWith(b, c)) {
                            handler([a, b, c]);
                            nsets += 1;
                            if ((limit > 0) && (limit == nsets)) {
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    /// If there is at least one SET in this array then move any single
    /// one of them (the SET of three cards) to the front of this array.
    ///
    mutating func moveAnyExistingSetToFront() -> Bool {
        let sets : [[Element]] = self.enumerateSets(limit: 1);
        if (sets.count == 1) {
            for card in sets[0] {
                if let index = self.firstIndex(where: {$0 == card}) {
                    self.remove(at: index);
                    self.insert(card, at: 0);
                }
            }
            return true;
        }
        return false;
    }

    /// Parses and returns a card array representing given comma-separated list of
    /// string representations of SET cards. See Card.from for details of format.
    /// If no parsable card formats, then returns an empty array.
    ///
    static func from(_ values : String) -> [Element] {
        return Self.from(values.filter{!$0.isWhitespace}.split(){$0 == ","}.map{String($0)});
    }

    /// Parses and returns a card array representing given array of of string
    /// representations of SET cards. See Card.from for details of format.
    /// If no parsable card formats, then returns an empty array.
    ///
    static func from(_ values : [String]) -> [Element] {
        var cards = [Element]();
        for value in values {
            if let card = Element(value) {
                cards.add(card);
            }
        }
        return cards;
    }
}

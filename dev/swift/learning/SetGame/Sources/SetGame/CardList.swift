/// CardList represents any list of zero of SET Card instances; duplicates are allowed.
///
public class CardList<T : Card> {

    private var array : Array<T> = Array<T>();

    //xyzzy
    convenience init(_ cards : (Card, Card)) {
        self.init();
    }

    convenience init(_ cards : T...) {
        self.init(cards);
    }

    init(_ cards : [T]) {
        self.array.append(contentsOf: cards);
    }

    convenience init(_ cards : CardList) {
        self.init(cards.array);
    }

    /// Returns the number of cards in this list.
    ///
    var count : Int {
        return self.array.count;
    }

    /// Adds the given card(s) to this list.
    ///
    func add(_ card : T, _ cards : T...) {
        self.array.append(card);
        self.array.append(contentsOf: cards);
    }

    func add(_ cards : [T]) {
        self.array.append(contentsOf: cards);
    }

    func add(_ cards : CardList) {
        self.array.append(contentsOf: cards);
    }

    /// Removes (all instances of) the given card(s) from this list.
    ///
    func remove(_ card : T, _ cards  : T...) {
        self.array.removeAll(where: {$0 == card});
        for card in cards {
            self.array.removeAll(where: {$0 == card});
        }
    }

    func remove(_ cards : [T]) {
        for card in cards {
            self.remove(card);
        }
    }

    func remove(_ cards : CardList) {
        for card in cards {
            self.remove(card);
        }
    }

    /// Clears this list of all its cards.
    ///
    func clear() {
        self.array.removeAll();
    }

    /// Returns true iff the given card is in this list, otherwise false.
    ///
    func contains(_ card : T) -> Bool {
        return self.array.contains(card);
    }

    /// Randomly shuffles this CardList in place.
    ///
    func shuffle() {
        self.array.shuffle();
    }

    /// Removes and returns the first card from this list;
    /// returns nil if no more cards in the list.
    ///
    func takeCard() -> T? {
        return self.array.count > 0 ? self.array.remove(at: 0) : nil;
    }

    /// Removes and returns a random card from this list;
    /// returns nil if no more cards in the list.
    ///
    func takeRandomCard() -> T? {
        if (self.array.count > 0) {
            let n = Int.random(in: 0..<self.count);
            let card = self.array.remove(at: n);
            return card;
        }
        return nil;
    }

    /// Removes and returns, at most, the specified number of random cards
    /// from this list, in a new CardList; if fewer cards are in this list
    /// than the number requested, then so be it, just that many will
    /// be returned, and then this list will end up being empty.
    ///
    /// If the plantSet argument is true then the returned list
    /// will contain, if possible, (at least) one SET, as the
    /// first cards, at beginning of the list.
    ///
    func takeRandomCards(_ n : Int, plantSet: Bool = false) -> CardList {
        var n = n < 0 ? 0 : n;
        let cards : CardList = CardList();
        var plantedSet : CardList? = nil;
        if (plantSet && (n >= 3)) {
            let set = self.enumerateSets(limit: 1);
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
        return cards;
    }

    /// Returns (without removal), a random card from this list,
    /// or nil if this list is empty..
    ///
    func randomCard() -> T? {
        if (self.array.count > 0) {
            let n = Int.random(in: 0..<self.count);
            return self.array[n];
        }
        return nil;
    }

    /// Returns (without removal), at most, the specified number of random cards
    /// from this list, in a new CardList; if fewer cards are in this list than
    /// the number requested, then so be it, just that many will be returned.
    ///
    func randomCards(_ n : Int) -> CardList {
        let n = n < 0 ? 0 : n;
        let cards : CardList = CardList();
        for _ in 0..<n {
            if let card = self.randomCard() {
                cards.add(card);
            }
            else {
                break;
            }
        }
        return cards;
    }

    /// Returns true if this list comprises a SET.
    ///
    func isSet() -> Bool {
        return (self.count == 3) && T.isSet(self[0], self[1], self[2]);
    }

    /// Returns true iff there exists at least one SET in this list.
    ///
    func containsSet() -> Bool {
        if (self.count > 2) {
            for i in 0..<(self.count - 2) {
                for j in (i + 1)..<(self.count - 1) {
                    for k in (j + 1)..<(self.count) {
                        let a = self[i], b = self[j], c = self[k];
                        if (a.formsSetWith(b, c)) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    /// Returns the number of unique SETs in this list.
    ///
    func numberOfSets() -> Int {
        var nsets = 0;
        if (self.count > 2) {
            for i in 0..<(self.count - 2) {
                for j in (i + 1)..<(self.count - 1) {
                    for k in (j + 1)..<(self.count) {
                        let a = self[i], b = self[j], c = self[k];
                        if (a.formsSetWith(b, c)) {
                            nsets += 1;
                        }
                    }
                }
            }
        }
        return nsets;
    }

    /// Identifies/enumerates any/all SETs in this list and returns them in an array
    /// of CardList instances, each representing a unique (possibily overlaping) SET
    /// within this list. If no SETs exist then returns an empty array.
    ///
    /// TODO: This method and numberOfSets and containsSet I'm sure can be simplified
    /// with a single generic function with a lambda, function, callback or something.
    ///
    func enumerateSets(limit: Int = 0) -> [CardList] {
        var sets : [CardList] = [CardList]();
        if (self.count > 2) {
            for i in 0..<(self.count - 2) {
                for j in (i + 1)..<(self.count - 1) {
                    for k in (j + 1)..<(self.count) {
                        let a = self[i], b = self[j], c = self[k];
                        if (a.formsSetWith(b, c)) {
                            sets.append(CardList(a, b, c));
                            if ((limit > 0) && (limit == sets.count)) {
                                return sets;
                            }
                        }
                    }
                }
            }
        }
        return sets;
    }

    /// If there is at least one SET in this list then move and single one
    /// of them (the SET of three cards) to the front of this list.
    ///
    func moveAnyExistingSetToFront() -> Bool {
        let sets : [CardList] = self.enumerateSets(limit: 1);
        if (sets.count == 1) {
            sets[0].forEach {
                let setcard = $0;
                if let index = self.array.firstIndex(where: {$0 == setcard}) {
                    self.array.remove(at: index);
                    self.array.insert(setcard, at: 0);
                }
            }
        }
        return false;
    }

    func filter(_ f: (T) -> Bool) -> CardList<T> {
        return CardList(self.array.filter { f($0) });
    }

    /// Parses and returns a CardList representing given comma-separated list of
    /// string representations of SET cards. See T.from for detials of format.
    /// If no parsable card formats, then returns an empty list.
    ///
    static func from(_ value : String) -> CardList<T> {
        let cards = CardList<T>();
        let value = value.filter{!$0.isWhitespace};
        let components = value.split(){$0 == ","}
        for component in components {
            if let card = T(component) {
                cards.add(card);
            }
        }
        return cards;
    }

    func codenames() -> String {
        var value : String = "";
        for card in self.array {
            if (value.count > 0) {
                value += ",";
            }
            value += card.codename;
        }
        return value;
    }

    func toString(_ verbose : Bool = false) -> String {
        var value : String = "";
        for card in self {
            if (value.count > 0) {
                value += ",";
            }
            value += card.toString(verbose);
        }
        return value;
    }
}

/// CardList extensions to conform to sundry protocols.
///
extension CardList : Sequence, CustomStringConvertible {

    subscript (index: Int) -> T {
        return self.array[index];
    }

    public typealias Iterator = IndexingIterator<Array<T>>;

    public func makeIterator() -> Iterator {
      return self.array.makeIterator()
    }

    public var description : String {
        return self.toString();
    }
}

/// Array extension for convenience in handling simple Card arrays.
/// Maybe since we have something like this we don't even need CardList at all?
/// Or need some other kind of extension or protocol or something.
///
extension Array where Element : Card {

    mutating func add(_ card : Element) {
        self.append(card);
    }

    mutating func add(_ card : Element, _ cards : Element...) {
        self.add(card);
        self.add(cards);
    }

    mutating func add(_ cards : [Element]) {
        self.append(contentsOf: cards);
    }


    mutating func add(_ cards : CardList<Element>) {
        for card in cards {
            cards.add(card);
        }
    }

    func isSet() -> Bool {
        return self.count == 3 && self[0].formsSetWith(self[1], self[2]);
    }

    func formsSetWith(_ a : Element, _ b : Element) -> Bool {
        return self.count == 1 && self[0].formsSetWith(a, b);
    }

    func formsSetWith(_ a : Element) -> Bool {
        return self.count == 2 && self[0].formsSetWith(self[1], a);
    }

    func containsSet() -> Bool {
        return CardList(self).containsSet();
    }

    func numberOfSets() -> Int {
        return CardList(self).numberOfSets();
    }

    func enumerateSets(limit : Int = 0) -> [[Card]] {
        return
            CardList(self).enumerateSets(limit: limit)
                .map { let set = $0; return set.map{Card($0)} };
    }
}

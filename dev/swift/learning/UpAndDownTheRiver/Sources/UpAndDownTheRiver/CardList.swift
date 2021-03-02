class CardList : CustomStringConvertible {

    private var cards : Array<Card> = Array<Card>();

    var list : Array<Card> {
        get {
            return cards;
        }
    }
    
    func add(_ card : Card) {
        cards.append(card);
    }   

    func contains(_ card : Card) -> Bool {
        return self.cards.contains(card);
    }

    func shuffle() {
        self.cards.shuffle();
    }
    
    func takeCard() -> Card? {
        return self.cards.count > 1 ? self.cards.remove(at: 0) : nil;
    }
    
    func takeRandomCard() -> Card? {
        if (self.cards.count > 1) {
            let n = Int.random(in: 0..<self.cards.count);
            let card = self.cards.remove(at: n);
            return card;
        }
        else {
            return nil;
        }
    }

    var description : String {
        return self.toString();
    }

    func toString(_ verbose : Bool = false) -> String {
        var value : String = "";
        for card in cards {
            if (value.count > 0) {
                value += ",";
            }
            value += card.toString(verbose);
        }
        return value;
    }

    static func from(_ value : String) -> CardList {
        let cards = CardList();
        let value = String(value.filter { !" \n\t\r".contains($0) });
        let components = value.split() { $0 == "," }
        for component in components {
            if let card = Card.from(String(component)) {
                cards.add(card);
            }
        }
        return cards;
    }
}

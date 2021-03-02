print("Hello, world!")

let prompt = "set: ";
print(prompt, terminator: "");
while let line = readLine() {
    var cards = CardList.from(line);
    if (line == "random") {
        let deck = Deck();
        cards = deck.takeRandomCards(3);
    }
    else if (line.starts(with: "deal")) {
        let deck = Deck();
        var ncards : Int = 9;
        let components = line.split(){$0 == " "}
        if (components.count == 2) {
            if let n = Int(String(components[1])) {
                ncards = (n > deck.count) ? deck.count : n;
            }
        }
        cards = deck.takeRandomCards(ncards);
    }
    else if (line == "set") {
        cards = CardList.generateRandomSet();
    }
    else if (line == "deck") {
        cards = Deck();
    }
    else if (line.starts(with: "stats")) {
        var ncards : Int = 9;
        let components = line.split(){$0 == " "}
        if (components.count == 2) {
            if let n = Int(String(components[1])) {
                ncards = n > Deck.size ? Deck.size : n;
            }
        }
        let average = Deck.averageNumberOfSets(ncards);
        print("AVERAGE NUMBER OF SETS FOR DEAL SIZE OF \(ncards) CARDS: \(average)");
    }
    for card in cards {
        print("\(card) (\(card.codename))");
    }
    if (cards.count == 2) {
        let matchingCard : Card = Card.matchingSetValue(cards[0], cards[1]);
        print("MATCHING CARD: \(matchingCard) (\(matchingCard.codename))");
    }
    else if (cards.count == 3) {
        let isSet = cards.isSet();
        if (isSet) {
            print("SET! \(cards.codenames())");
        }
    }
    else if (cards.count > 3) {
        let sets = cards.enumerateSets();
        if (sets.count > 0) {
            print("ENUMERATION OF SETS (\(sets.count)):");
            for set in sets {
                print("\(set.codenames())");
            }
        }
    }
    print(prompt, terminator: "");
}

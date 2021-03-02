class CardSet : CardList {

    func add(_ card: Card) -> Bool {
        if (!super.list.contains(card)) {
            super.add(card);
            return true;
        }
        return false;
    }
}

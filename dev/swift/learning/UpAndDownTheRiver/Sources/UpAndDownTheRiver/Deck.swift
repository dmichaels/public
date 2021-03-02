class Deck : CardList {

    let ndecks: Int;

    init(ndecks: Int = 1) {
        self.ndecks = (ndecks > 1) ? ndecks : 1;
        super.init();
        for _ in 1...ndecks {
            for suit in CardSuit.allCases {
                for rank in CardRank.allCases {
                    super.add(Card(suit, rank));
                }
            }
        }
    }
}

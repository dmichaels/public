import SwiftUI

struct TableView: View {

    @EnvironmentObject var table : Table;

    var body: some View {
        ScrollView(.vertical, showsIndicators: false) {
            VStack {
                let nrows = Int(ceil(Float(table.cards.count) / Float(table.settings.cardsPerRow)));
                ForEach (0..<nrows, id: \.self) { row in
                    HStack {
                        ForEach(0..<table.settings.cardsPerRow, id: \.self) { column in
                            let index = row * table.settings.cardsPerRow + column;
                            if (index < table.cards.count) {
                                CardView(card: table.cards[index]) {
                                    cardTouched($0)
                                }
                            }
                            else {
                                Image("dummy").resizable()
                            }
                        }
                    }
                }
                Divider()
                StatusBarView()
                Divider()
            }.padding()
        }
    }

    private func cardTouched(_ card : TableCard) {
        //
        // First we notify the table model that the card has been touched,
        // i.e. selected/unselected toggle, then we ask the table check to
        // check if a 3 card SET has been selected, in which case it would
        // remove the SET cards and replace them with new ones from the deck,
        // or if no SET, but 3 cards selected, then it would unselect the cards.
        //
        // Done in two steps because the CardView needs a breather to do its
        // visual flipping. This breather is manifested as a delay on the SET
        // check action. Without this delay, when the third card was selected,
        // we wouldn't see its flipping before it either got replaced by a new
        // card, if a SET; or got immediatly unselected, if no SET.
        //
        // No idea right now if this is the right/Swift-y way
        // to handle such a situation; but it does work for now.
        //
        table.touchCard(card);
        delayQuick() {
            table.checkForSet();
        }
    }

    private func delayQuick(_ seconds : Float = 0.0, _ callback: @escaping () -> Void) {
        if (seconds < 0) {
            callback();
        }
        else {
            DispatchQueue.main.asyncAfter(deadline: .now()) {
                callback();
            }
        }
    }
}

struct TableView_Previews: PreviewProvider {
    static var previews: some View {
        TableView()
            .environmentObject(Table(preferredDisplayCardCount: 12, plantSet: true))
    }
}

import SwiftUI

struct StatusBarView: View {
    
    @EnvironmentObject var table : Table;

    let OK_SYMBOL           : String = "\u{1F44C}";
    let THUMBSUP_SYMBOL     : String = "\u{1F44D}";
    let NEUTRAL_FACE_SYMBOL : String = "\u{1F610}";
    let HAPPY_FACE_SYMBOL   : String = "\u{1F642}";
    let SAD_FACE_SYMBOL     : String = "\u{1F641}";

    func partialSetSelectedOne() -> Bool { self.table.state.partialSetSelected && table.cards.count == 1; }
    func partialSetSelectedTwo() -> Bool { self.table.state.partialSetSelected && table.cards.count == 2; }
    
    var body: some View {
        HStack(alignment: .firstTextBaseline) {
            Text("SETs: \(table.state.setsFoundCount)")
                .font(.subheadline)
                .frame(alignment: .leading)
                .foregroundColor(Color.blue)
            if (self.table.settings.showNumberOfSetsPresent) {
                Text("\u{2756} \(table.numberOfSets())")
                    .font(.subheadline)
            }
            Text("\u{2756} Deck: \(table.remainingCardCount())")
                .font(.subheadline)
            if (self.table.settings.showPartialSetSelectedIndicator) {
                if (self.table.state.partialSetSelected) {
                    if (self.table.selectedCardCount() == 1) {
                        Text(OK_SYMBOL)
                            .font(.subheadline)
                    }
                    else if (self.table.selectedCardCount() == 2) {
                        Text(THUMBSUP_SYMBOL)
                            .font(.subheadline)
                    }
                    else {
                        Text(NEUTRAL_FACE_SYMBOL)
                            .font(.subheadline)
                    }
                }
                else if (self.table.state.setJustFound) {
                    Text(HAPPY_FACE_SYMBOL)
                        .font(.subheadline)
                }
                else if (self.table.state.setJustNotFound) {
                    Text(SAD_FACE_SYMBOL)
                        .font(.subheadline)
                }
                else {
                    Text(NEUTRAL_FACE_SYMBOL)
                        .font(.subheadline)
                }
            }
            Spacer()
            Button(action: {
                self.table.state.showingCardsWhichArePartOfSet.toggle();
                if (self.table.state.showingCardsWhichArePartOfSet) {
                    self.table.selectAllCardsWhichArePartOfSet();
                }
                else {
                    self.table.unselectCards();
                }
            }) {
                Image(systemName: "eye")
            }
            Button(action: { self.table.addMoreCards(1) }) {
                Image(systemName: "plus.app")
            }
            Button(action: { self.table.startNewGame() }) {
                Image(systemName: "arrow.clockwise.circle")
            }
        }
    }
}

struct StatusBarView_Previews: PreviewProvider {
    static var previews: some View {
        StatusBarView()
            .environmentObject(Table(preferredCardCount: 12, plantSet: true))
    }
}

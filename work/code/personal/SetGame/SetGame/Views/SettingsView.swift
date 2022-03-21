import SwiftUI

struct SettingsView: View {
    
    @EnvironmentObject var table : Table;
    let cardsPerRowChoices = [ 1, 2, 3, 4, 5, 6 ];
    let preferredDisplayCountCardChoices = [ 3, 4, 6, 9, 12, 15, 16, 20 ];
    let limitDeckSizeChoices = [ 18, 27, 36, 45, 54, 63, 72, 81 ];
    
    var body: some View {
        ScrollView(.vertical, showsIndicators: false) {
            VStack {
                VStack {
                    Toggle(isOn: $table.settings.useSimpleDeck) {
                        Text("Use simplified deck (restarts!): ")
                    }
                    HStack() {
                        Text("Limit deck size (n cards):")
                            .frame(alignment: .leading)
                        Spacer()
                        Picker("\(table.settings.limitDeckSize) \u{25BC}", selection: $table.settings.limitDeckSize) {
                            ForEach(limitDeckSizeChoices, id: \.self) {
                                Text(String($0))
                            }
                        }.pickerStyle(MenuPickerStyle())
                    }
                    Divider()
                    Toggle(isOn: $table.settings.showNumberOfSetsPresent) {
                        Text("Show SETs displayed count: ")
                    }
                    Toggle(isOn: $table.settings.plantSet) {
                        Text("Try planting at least one SET: ")
                    }
                    Toggle(isOn: $table.settings.moreCardsIfNoSet) {
                        Text("Deal more cards if no SETs: ")
                    }
                    Divider()
                    Toggle(isOn: $table.settings.showPartialSetSelectedIndicator) {
                        Text("Show partial SET selection hint: ")
                    }
                    Toggle(isOn: $table.settings.moveAnyExistingSetToFront) {
                        Text("Move any existing SET to front: ")
                    }
                }
                Divider()
                HStack() {
                    Text("Preferred displayed card count:")
                        .frame(alignment: .leading)
                    Spacer()
                    Picker("\(table.settings.preferredDisplayCardCount) \u{25BC}", selection: $table.settings.preferredDisplayCardCount) {
                        ForEach(preferredDisplayCountCardChoices, id: \.self) {
                            Text(String($0))
                        }
                    }.pickerStyle(MenuPickerStyle())
                }
                HStack() {
                    Text("Cards per row:")
                        .frame(alignment: .leading)
                    Spacer()
                    Picker("\(table.settings.cardsPerRow) \u{25BC}", selection: $table.settings.cardsPerRow) {
                        ForEach(cardsPerRowChoices, id: \.self) {
                            Text(String($0))
                        }
                    }.pickerStyle(MenuPickerStyle())
                }
                Divider()
                HStack () {
                    NavigationLink(destination: StatsView()) {
                        Text("Stats")
                    }
                    Spacer()
                }.frame(alignment: .leading)
            }.padding()
        }
        .navigationTitle("SET Game® Settings")
    }
}

struct SettingsView_Previews: PreviewProvider {
    static var previews: some View {
        SettingsView()
            .environmentObject(Table(preferredDisplayCardCount: 12, plantSet: true))
    }
}

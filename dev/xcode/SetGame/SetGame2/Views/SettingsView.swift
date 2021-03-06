import SwiftUI

struct SettingsView: View {
    
    @EnvironmentObject var table : Table;
    let cardsPerRowChoices = [ 3, 4, 5, 6 ];
    let preferredCountCardChoices = [ 3, 4, 6, 9, 12, 15, 16, 20 ];
    
    var body: some View {
        ScrollView(.vertical, showsIndicators: false) {
            VStack {
                Text("SET Game® Settings").font(.title)
                Divider()
                Toggle(isOn: $table.settings.useSimpleDeck) {
                    Text("Use simplified deck (restarts!): ")
                }
                Toggle(isOn: $table.settings.showNumberOfSetsPresent) {
                    Text("Show SETs present count: ")
                }
                Toggle(isOn: $table.settings.plantSet) {
                    Text("Plant at least one SET: ")
                }
                Toggle(isOn: $table.settings.showPartialSetSelectedIndicator) {
                    Text("Show partial SET selected hint: ")
                }
                Toggle(isOn: $table.settings.moveAnyExistingSetToFront) {
                    Text("Move any existing SET to front: ")
                }
                HStack() {
                    Text("Preferred card count:")
                        .frame(alignment: .leading)
                    Spacer()
                    Picker("\(table.settings.preferredCardCount) \u{25BC}", selection: $table.settings.preferredCardCount) {
                        ForEach(preferredCountCardChoices, id: \.self) {
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
            }.padding()
        }
    }
}

struct SettingsView_Previews: PreviewProvider {
    static var previews: some View {
        SettingsView()
            .environmentObject(Table(preferredCardCount: 12, plantSet: true))
    }
}

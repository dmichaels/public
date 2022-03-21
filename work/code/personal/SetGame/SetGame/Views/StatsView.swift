import SwiftUI

struct StatsView: View  {
    
    @EnvironmentObject var table : Table;
    @State var isViewDisplayed = false;

    var body: some View {
        ScrollView(.vertical, showsIndicators: false) {
            VStack(alignment: .leading) {
                ForEach(3...21, id: \.self) { index in
                    if (index == 3) {
                        HStack {
                            Text("# of Cards")
                                .font(.system(size: 14, design: .monospaced))
                                .frame(alignment: .leading)
                            Spacer()
                            Text("Average # of SETs")
                                .font(.system(size: 14, design: .monospaced))
                                .frame(alignment: .trailing)
                        }
                        Divider()
                    }
                    HStack {
                        if (self.isViewDisplayed) {
                        let number = Deck.averageNumberOfSets(index);
                        let string = String(format: "%-0.1f", number);
                        Text("\(index)")
                            .font(.system(size: 14, design: .monospaced))
                            .frame(alignment: .leading)
                        Spacer()
                        Text("\(string)")
                            .font(.system(size: 14, design: .monospaced))
                            .frame(alignment: .trailing)
                        }
                    }
                }
            }.padding(30)
            .onAppear {
                self.isViewDisplayed = true
            }
            .onDisappear {
                self.isViewDisplayed = false
            }
            
        }.navigationTitle("SET Game® Stats")
    }
}

struct StatsView_Previews: PreviewProvider {
    static var previews: some View {
        StatsView()
            .environmentObject(Table(preferredDisplayCardCount: 12, plantSet: true))
    }
}

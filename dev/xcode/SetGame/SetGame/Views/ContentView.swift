//
//  ContentView.swift
//  SetGame2
//
//  Created by David Michaels on 3/1/21.
//

import SwiftUI

struct ContentView: View {
    
    @EnvironmentObject var table : Table;

    var body: some View {
        NavigationView {
            TableView()
                .navigationBarTitleDisplayMode(.inline)
                .toolbar {
                    ToolbarItem(placement: .navigationBarLeading) {
                        Text("SET Game®")
                            .foregroundColor(Color(UIColor.black))
                            .font(.title)
                            .fontWeight(.bold)
                            .padding(.top, 6)
                            .padding(.bottom, 2)
                    }
                    ToolbarItem(placement: .navigationBarTrailing) {
                        NavigationLink(destination: SettingsView()) {
                            Image(systemName: "gearshape.fill")
                                .foregroundColor(Color(UIColor.darkGray))
                        }
                    }
                }
        }.padding(.top, 6)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
            .environmentObject(Table(preferredDisplayCardCount: 12, plantSet: true))
    }
}

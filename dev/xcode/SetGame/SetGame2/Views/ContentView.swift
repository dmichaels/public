//
//  ContentView.swift
//  SetGame2
//
//  Created by David Michaels on 3/1/21.
//

import SwiftUI

struct ContentView: View {
    
    @EnvironmentObject var table : Table;
    init() {
        // From: https://prafullkumar77.medium.com/swiftui-managing-navigation-bar-2210a2a72a89
        UINavigationBar.appearance().tintColor = UIColor.systemIndigo
        UINavigationBar.appearance().backgroundColor = UIColor.gray
        UINavigationBar.appearance().shadowImage = UIImage()
        // UINavigationBar.appearance().isTranslucent = false
    }
    var body: some View {
        NavigationView {
            TableView()
                .navigationBarTitleDisplayMode(.inline)
                .toolbar {
                    ToolbarItem(placement: .navigationBarLeading) {
                        Text("SET Game®")
                            .foregroundColor(Color(UIColor.systemIndigo))
                            .font(.title)
                            .fontWeight(.bold)
                            .padding(.top, 6)
                            .padding(.bottom, 2)
                    }
                    ToolbarItem {
                        NavigationLink(destination: SettingsView()) {
                            Image(systemName: "gearshape.fill")
                        }
                    }
                }
        }.padding(.top, 6)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
            .environmentObject(Table(preferredCardCount: 12, plantSet: true))
    }
}

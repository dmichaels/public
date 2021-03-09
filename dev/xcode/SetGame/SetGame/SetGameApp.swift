//
//  SetGame2App.swift
//  SetGame2
//
//  Created by David Michaels on 3/1/21.
//

import SwiftUI

@main
struct SetGameApp: App {
    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(Table(preferredDisplayCardCount: 12, plantSet: false))
        }
    }
}

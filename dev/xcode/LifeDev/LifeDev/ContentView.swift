//
//  ContentView.swift
//  LifeDev
//
//  Created by David Michaels on 4/5/25.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        NavigationView {
            VStack {
                Rectangle()
                    .fill(.teal)
            }
            .navigationTitle("Game of Life")
            .padding()
        }
        .background(.red)
    }
}

#Preview {
    ContentView()
}

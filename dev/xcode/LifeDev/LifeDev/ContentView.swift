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
                    .fill(.orange)
                    .frame(height: 200)
                GeometryReader { geometry in
                    Rectangle()
                        .fill(.green)
                    VStack(alignment: .leading) {
                        // Text("L: \(geometry.frame(in: .local))")
                        // Text("G: \(geometry.frame(in: .global))")
                        // Text("N: \(geometry.frame(in: .named("VStack")).debugDescription)")
                    }
                    Text("Hello, world!")
                        .position(x: geometry.frame(in: .local).midX,
                                  y: geometry.frame(in: .named("VStack")).midY)
                     // .position(x: geometry.frame(in: .local).midX,
                     //           y: geometry.frame(in: .local).midY)
                }
            }
            .coordinateSpace(name: "VStack")
            .navigationTitle("GeometryReader")
        }
    }
    
}

#Preview {
    ContentView()
}

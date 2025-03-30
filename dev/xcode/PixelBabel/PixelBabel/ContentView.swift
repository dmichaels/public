//
//  ContentView.swift
//  PixelBabel
//
//  Created by David Michaels on 3/29/25.
//

import SwiftUI

struct ContentView: View {
    @EnvironmentObject var settings: AppSettings
    @State private var randomImage: CGImage = RandomPixelGenerator.generate(useGrayscale: false, pixelSize: 1)
    @State private var showSettings = false  // Controls settings screen visibility

    var body: some View {
        ZStack {
            Image(decorative: randomImage, scale: 1.0)
                .resizable()
                .scaledToFill()
                .ignoresSafeArea()
                .onTapGesture {
                    randomImage = RandomPixelGenerator.generate(useGrayscale: settings.useGrayscale, pixelSize: 1) // settings.pixelSize)
                }
                .onChange(of: settings.useGrayscale) { _ in
                    randomImage = RandomPixelGenerator.generate(useGrayscale: settings.useGrayscale, pixelSize: 1) // settings.pixelSize)
                }
                .gesture(
                    DragGesture()
                        .onEnded { value in
                            if value.translation.width < -100 { // Swipe left
                                showSettings = true
                            }
                        }
                )
            
            if showSettings {
                SettingsView(showSettings: $showSettings)
                    .transition(.move(edge: .trailing))
            }
        }
    }
}

struct SettingsView: View {
    @EnvironmentObject var settings: AppSettings
    @Binding var showSettings: Bool

    var body: some View {
        VStack(spacing: 20) {
            Toggle("Use GrayScale", isOn: $settings.useGrayscale)
                .padding()
                .onChange(of: settings.useGrayscale) { _ in
                    print("Toggle changed: \(settings.useGrayscale)")
                }

            // Stepper("Pixel Size: \(settings.pixelSize)", value: $settings.pixelSize, in: 1...16)
                // .padding()

            Button("Close") {
                showSettings = false
            }
            .padding()
            .background(Color.blue)
            .foregroundColor(.white)
            .clipShape(Capsule())

        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .background(Color.white)
        .ignoresSafeArea()
        .onTapGesture {
            showSettings = false
        }
    }
}

struct RandomPixelGenerator {
    static func generate(useGrayscale: Bool, pixelSize: Int) -> CGImage {
        let screenWidth = Int(UIScreen.main.bounds.width)
        let screenHeight = Int(UIScreen.main.bounds.height)
        let width = screenWidth / pixelSize
        let height = screenHeight / pixelSize
        let size = width * height * 4
        var pixelData = [UInt8](repeating: 0, count: size)

        for y in 0..<height {
            for x in 0..<width {
                let i = (y * width + x) * 4
                let value = UInt8.random(in: 0...255)
                if (false) {
                    let bwValue: UInt8 = (value > 128) ? 255 : 0
                    pixelData[i] = bwValue
                    pixelData[i+1] = bwValue
                    pixelData[i+2] = bwValue
                }
                else if useGrayscale {
                    pixelData[i] = value
                    pixelData[i+1] = value
                    pixelData[i+2] = value
                } else {
                    pixelData[i] = UInt8.random(in: 0...255)
                    pixelData[i+1] = UInt8.random(in: 0...255)
                    pixelData[i+2] = UInt8.random(in: 0...255)
                }
                pixelData[i+3] = 255
            }
        }

        let colorSpace = CGColorSpaceCreateDeviceRGB()
        let bitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue)

        guard let context = CGContext(
            data: &pixelData,
            width: width * pixelSize,
            height: height * pixelSize,
            bitsPerComponent: 8,
            bytesPerRow: width * pixelSize * 4,
            space: colorSpace,
            bitmapInfo: bitmapInfo.rawValue
        ) else { fatalError("Failed to create CGContext") }

        guard let cgImage = context.makeImage() else { fatalError("Failed to create CGImage") }
        return cgImage
    }
}

#Preview {
    ContentView()
        .environmentObject(AppSettings())  // Inject settings for preview
}

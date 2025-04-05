import SwiftUI

struct ContentView: View
{
    @EnvironmentObject var settings: AppSettings
    @State private var _randomImage: CGImage?
    @State private var showSettings = false

    private var _feedback: Feedback {
        return Feedback(settings) // TODO: Figure out how to make lazy evaluation so not creating every time.
    }
    
    func refreshRandomImage() {
        self._randomImage = RandomPixelGenerator.generate(settings: settings)
    }
    
    var body: some View {
        ZStack {
            if let image = self._randomImage {
                Image(decorative: image, scale: 1.0)
                    .resizable()
                    .scaledToFill()
                    .ignoresSafeArea()
                    .onChange(of: settings.colorMode) { _ in
                        refreshRandomImage()
                    }
                    .onChange(of: settings.pixelSize) { _ in
                        refreshRandomImage()
                    }
                    .gesture(
                        DragGesture()
                            .onEnded { value in
                                if value.translation.width < -100 { // Swipe left
                                    withAnimation {
                                        showSettings = true
                                    }
                                }
                            }
                    )
                
                if showSettings {
                    SettingsView(showSettings: $showSettings)
                        .transition(.move(edge: .trailing))
                }
            }
        }
        .onAppear {
            refreshRandomImage()
            // _randomImage = RandomPixelGenerator.generate(settings: settings) // Ensure the image is set on appear
        }
        .simultaneousGesture(
            TapGesture().onEnded {
                if !showSettings {
                    refreshRandomImage()
                    self._feedback.triggerHaptic()
                }
            }
        )
    }
}

struct RandomPixelGenerator {
    static func generate(settings: AppSettings) -> CGImage {
        let screenWidth = Int(UIScreen.main.bounds.width)
        let screenHeight = Int(UIScreen.main.bounds.height)
        var pixels: PixelMap = PixelMap(screenWidth, screenHeight, scale: settings.pixelSize)

        for y in 0..<pixels.height {
            for x in 0..<pixels.width {
                if (settings.colorMode == ColorMode.monochrome) {
                    let value: UInt8 = UInt8.random(in: 0...1) * 255
                    pixels.write(x, y, red: value, green: value, blue: value)
                }
                else if settings.colorMode == ColorMode.grayscale {
                    let value = UInt8.random(in: 0...255)
                    pixels.write(x, y, red: value, green: value, blue: value)
                }
                else {
                    let rgb = UInt32.random(in: 0...0xFFFFFF)
                    let red = UInt8((rgb >> 16) & 0xFF)
                    let green = UInt8((rgb >> 8) & 0xFF)
                    let blue = UInt8(rgb & 0xFF)
                    pixels.write(x, y, red: red, green: green, blue: blue)
                }
            }
        }

        let colorSpace = CGColorSpaceCreateDeviceRGB()
        let bitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue)

        guard let context = CGContext(
            data: pixels.data,
            width: screenWidth,
            height: screenHeight,
            bitsPerComponent: 8,
            bytesPerRow: screenWidth * pixels.depth,
            space: colorSpace,
            bitmapInfo: bitmapInfo.rawValue
        ) else { fatalError("Failed to create CGContext") }

        guard let cgImage = context.makeImage() else { fatalError("Failed to create CGImage") }
        return cgImage
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
            .environmentObject(AppSettings())
    }
}

import SwiftUI

struct ContentView: View
{
    @EnvironmentObject var settings: AppSettings
    @State private var _randomImage: CGImage?
    @State private var tapCount = 0
    @State private var showSettings = false

    private var _feedback: Feedback {
        return Feedback(settings) // TODO: Figure out how to make lazy evaluation so not creating every time.
    }
    
    func refreshRandomImage() {
        if let randomImage = RandomPixelGenerator.generate(settings: settings, taps: tapCount) {
            self._randomImage = randomImage
        }
    }

    var body: some View {
        ZStack {
            if let image = self._randomImage {
                Image(decorative: image, scale: 1.0)
                    .resizable()
                    .scaledToFill()
                    .ignoresSafeArea()
                    .onChange(of: settings.colorMode) { _ in
                        settings.pixels.mode = settings.colorMode
                        if (!showSettings) {
                            refreshRandomImage()
                        }
                    }
                    .onChange(of: settings.pixelSize) { _ in
                        settings.pixels.scale = settings.pixelSize
                        if (!showSettings) {
                            refreshRandomImage()
                        }
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
            // This is only called on first appearance;
            // not called after coming back from settings view.
            refreshRandomImage()
        }
        // .onAppear(perform: prepareHaptics)
        .simultaneousGesture(
            TapGesture().onEnded {
                if (!showSettings) {
                    tapCount += 1
                    self._feedback.triggerHaptic()
                    refreshRandomImage()
                }
            }
        )
        /*
        .gesture(TapGesture(count: 2).onEnded {
            tapCount += 1
            self._feedback.triggerHaptic()
        })
        .gesture(TapGesture(count: 1).onEnded {
            tapCount += 1
            self._feedback.triggerHaptic()
            refreshRandomImage()
        })
        */
        .edgesIgnoringSafeArea(.all)    
    }
}

struct RandomPixelGenerator {

    static func generate(settings: AppSettings, taps: Int = 0) -> CGImage?
    {
        let colorSpace = CGColorSpaceCreateDeviceRGB()
        let bitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue)
        var image: CGImage? = nil
        var randomFixedImage: Bool = false

        if (settings.randomFixedImage) {
            if (settings.randomFixedImagePeriod == RandomFixedImagePeriod.frequent) {
                if (taps % 4 == 1) {
                    randomFixedImage = true
                }
            }
            else if (settings.randomFixedImagePeriod == RandomFixedImagePeriod.sometimes) {
                if (taps % 8 == 1) {
                    randomFixedImage = true
                }
            }
            else if (settings.randomFixedImagePeriod == RandomFixedImagePeriod.seldom) {
                if (taps % 16 == 1) {
                    randomFixedImage = true
                }
            }
        }

        if (randomFixedImage) {
            settings.pixels.load("flowers")
        }
        else {
            print("ContentView: settings.pixels.randomize()")
            settings.pixels.randomize()
        }

        settings.pixels.data.withUnsafeMutableBytes { rawBuffer in
            guard let baseAddress = rawBuffer.baseAddress else {
                fatalError("Buffer has no base address")
            }

            let context = CGContext(
                data: baseAddress,
                width: ScreenWidth,
                height: ScreenHeight,
                bitsPerComponent: 8,
                bytesPerRow: ScreenWidth * ScreenDepth,
                space: colorSpace,
                bitmapInfo: bitmapInfo.rawValue
            )
        
            if let cgImage = context?.makeImage() {
                image = cgImage
            } else {
                fatalError("Failed to make CGImage")
            }
        }
        if (image != nil) {
            return image!
        }
        return nil
    }
}

struct ContentView_Previews: PreviewProvider {
    static let settings = AppSettings()
    static var previews: some View {
        ContentView()
            .environmentObject(settings)
    }
}

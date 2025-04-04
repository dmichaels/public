import SwiftUI
import AudioToolbox
import CoreHaptics
import AVFoundation

@main
struct PixelBabelApp: App {
    @State private var useGrayscale = false
    @StateObject private var settings = AppSettings()
    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(settings)
        }
    }
}

enum ColorMode: String, CaseIterable, Identifiable {
    case monochrome = "Monochrome"
    case grayscale = "Grayscale"
    case color = "Color"
    var id: String { self.rawValue }
}

// AppSettings model example
class AppSettings: ObservableObject {
    @Published var useMonochrome: Bool = false
    @Published var useGrayscale: Bool = false
    @Published var useColor: Bool = false
    @Published var colorMode: ColorMode = ColorMode.color
}

struct ContentView: View {
    @EnvironmentObject var settings: AppSettings
    @State private var randomImage: CGImage?
    @State private var showSettings = false  // Controls settings screen visibility
    @State private var hapticEngine: CHHapticEngine?
    //@State private var audioPlayer: AVAudioPlayer?
    // var audioPlayer: AVAudioPlayer?

    func playClickSound() {
        print("xyzzy: playClickSound")
        // let currentVolume = AVAudioSession.sharedInstance().outputVolume
        do {
            let session = AVAudioSession.sharedInstance()
            try session.setCategory(.playback, mode: .default, options: .mixWithOthers)
            try session.setActive(true)
            AudioServicesPlaySystemSound(1104)
        } catch {
            print("Error setting audio session: \(error)")
        }
    }
    
    init() {
        _randomImage = State(initialValue: RandomPixelGenerator.generate(settings: AppSettings()))  // Generate the initial random image
    }
    
    func prepareHaptics() {
        print("xyzzy: prepareHaptics")
        do {
            hapticEngine = try CHHapticEngine()
            try hapticEngine?.start()
        } catch {
            print("Haptics not supported.")
        }
    }
    
    func triggerHaptic() {
        print("xyzzy: triggerHaptic")
        playClickSound()
        let sharpTap = CHHapticEvent(eventType: .hapticTransient, parameters: [], relativeTime: 0)
        do {
            let pattern = try CHHapticPattern(events: [sharpTap], parameters: [])
            let player = try hapticEngine?.makePlayer(with: pattern)
            try player?.start(atTime: 0)
        } catch {
            print("Failed to play haptic.")
        }
    }
    
    func refreshRandomImage() {
        print("xyzzy: refreshRandomImage")
        randomImage = RandomPixelGenerator.generate(settings: settings)
    }
    
    var body: some View {
        ZStack {
            if let image = randomImage {
                Image(decorative: image, scale: 1.0)
                    .resizable()
                    .scaledToFill()
                    .ignoresSafeArea()
                    /*
                    .onTapGesture {
                        refreshRandomImage()
                        triggerHaptic()
                    }
                    */
                    .onChange(of: settings.colorMode) { _ in
                        refreshRandomImage()
                        //randomImage = RandomPixelGenerator.generate(settings: settings) // Regenerate on grayscale change
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
            randomImage = RandomPixelGenerator.generate(settings: settings) // Ensure the image is set on appear
        }
        .onAppear(perform: prepareHaptics)
        /*
        .onTapGesture {
            refreshRandomImage()
            triggerHaptic()
        }
        */
        // .disabled(showSettings)
        .simultaneousGesture(
            // Only trigger the image refresh and haptic feedback if settings page is not shown
            TapGesture().onEnded {
                if !showSettings {
                    refreshRandomImage()
                    triggerHaptic()
                }
            }
        )
    }
}

struct SettingsView: View {
    @EnvironmentObject var settings: AppSettings
    @Binding var showSettings: Bool
    @State private var selectedMode: ColorMode = .color
    
    var body: some View {
        VStack(spacing: 20) {
            Spacer()
            HStack {
                // Left-justified label
                Text("Color Mode")
                    .bold()
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .padding(.leading)
                    .lineLimit(1)
                
                // Right-justified dropdown
                Picker("Color Mode", selection: $selectedMode) {
                    ForEach(ColorMode.allCases) { mode in
                        Text(mode.rawValue).tag(mode)
                    }
                }
                .pickerStyle(MenuPickerStyle())
                .onChange(of: selectedMode) { newMode in
                    settings.colorMode = newMode
                }
                .fixedSize()
                .frame(width: 100) // Fixed width for the dropdown to prevent shifting
                .padding(.trailing)
            }
            .padding(.horizontal)
            
            Spacer()
            
            .onChange(of: selectedMode) { newMode in
                settings.colorMode = newMode
                // Handle mode change, update settings accordingly
                // You can bind it to settings here if needed
            }
            Spacer()
            Spacer()
            Spacer()
            //Spacer()
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .background(Color.white)
        .ignoresSafeArea()
        .gesture(
            DragGesture()
                .onEnded { value in
                    if value.translation.width > 100 { // Swipe right threshold
                        withAnimation {
                            showSettings = false // Go back to random pixel view
                        }
                    }
                }
        )
        .onAppear {
            selectedMode = settings.colorMode
        }
    }
}

struct RandomPixelGenerator {
    static func generate(settings: AppSettings) -> CGImage {
        let useGrayscale = settings.useGrayscale
        let pixelSize = 1
        let screenWidth = Int(UIScreen.main.bounds.width)
        let screenHeight = Int(UIScreen.main.bounds.height)
        let width = screenWidth / pixelSize
        let height = screenHeight / pixelSize
        let size = width * height * 4
        var pixelData = [UInt8](repeating: 0, count: size)

        for y in 0..<height {
            for x in 0..<width {
                let i = (y * width + x) * 4
                if settings.colorMode == ColorMode.monochrome {
                    let value: UInt8 = UInt8.random(in: 0...1) * 255
                    pixelData[i] = value
                    pixelData[i+1] = value
                    pixelData[i+2] = value
                }
                else if settings.colorMode == ColorMode.grayscale {
                    let value = UInt8.random(in: 0...255)
                    pixelData[i] = value
                    pixelData[i+1] = value
                    pixelData[i+2] = value
                } else {
                    pixelData[i] = UInt8.random(in: 0...255)
                    pixelData[i+1] = UInt8.random(in: 0...255)
                    pixelData[i+2] = UInt8.random(in: 0...255)
                }
                pixelData[i+3] = 255 // Alpha channel (fully opaque)
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

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
            .environmentObject(AppSettings())  // Inject the AppSettings here for the preview
    }
}

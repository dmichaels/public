import SwiftUI
import AudioToolbox
import CoreHaptics
import AVFoundation

@main
struct PixelBabelApp: App {
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

class AppSettings: ObservableObject {
    @Published var colorMode: ColorMode = ColorMode.monochrome
    @Published var pixelSize: Int = 4
    @Published var feedbackEnabled: Bool = true
}

struct ContentView: View {
    @EnvironmentObject var settings: AppSettings
    @State private var randomImage: CGImage?
    @State private var showSettings = false  // Controls settings screen visibility
    @State private var hapticEngine: CHHapticEngine?

    func playClickSound() {
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
        do {
            hapticEngine = try CHHapticEngine()
            try hapticEngine?.start()
        } catch {
            print("Haptics not supported.")
        }
    }
    
    func triggerHaptic() {
        if (settings.feedbackEnabled) {
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
    }
    
    func refreshRandomImage() {
        randomImage = RandomPixelGenerator.generate(settings: settings)
    }
    
    var body: some View {
        ZStack {
            if let image = randomImage {
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
            randomImage = RandomPixelGenerator.generate(settings: settings) // Ensure the image is set on appear
        }
        .onAppear(perform: prepareHaptics)
        .simultaneousGesture(
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
        VStack(spacing: 2) {

            Spacer()
            HStack {
                Text("Color Mode")
                    .bold()
                    .lineLimit(1)
                    //.frame(width: 100, alignment: .leading)
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .padding(.leading)
                Spacer()
                Picker("Color Mode", selection: $selectedMode) {
                    ForEach(ColorMode.allCases) { mode in
                        Text(mode.rawValue)
                            .lineLimit(1)
                            .truncationMode(.tail)
                            .tag(mode)
                    }
                }
                .pickerStyle(MenuPickerStyle())
                // .fixedSize()
                .frame(width: 200, alignment: .trailing)
                .padding(.trailing)
                .lineLimit(1)
                .onChange(of: selectedMode) { newMode in
                    settings.colorMode = newMode
                }
            }
            
            HStack {
                Text("Clicky Sound")
                    .bold()
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .padding(.leading)
                Toggle("", isOn: $settings.feedbackEnabled)
                    .labelsHidden()
                    .padding(.trailing, 30) // Align with other right-aligned controls
            }

            Divider()
                .frame(height: 3)
                .background(Color.gray.opacity(0.3))
                .padding(.horizontal)
                // .padding(.vertical, 20)
                .padding(.top, 20)
                .padding(.bottom, 10)
            HStack {
                Text("Pixel Size")
                    .bold()
                    .frame(width: 100, alignment: .leading)
                    .padding(.leading)
                Spacer()
                Text("\(settings.pixelSize)")
                    .monospacedDigit()
                    .frame(width: 50, alignment: .trailing)
                    .padding(.trailing)
            }
            Slider(
                value: Binding(
                    get: { Double(settings.pixelSize) },
                    set: { settings.pixelSize = Int($0) }
                ),
                in: 1...50,
                step: 1
            )
            .padding(.horizontal)
            .padding(.top, 0)
            .onChange(of: settings.pixelSize) { newValue in
                settings.pixelSize = newValue
            }

            Spacer()
            Spacer()
            Spacer()
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .background(Color.white)
        .ignoresSafeArea()
        .gesture(
            DragGesture()
                .onEnded { value in
                    if value.translation.width > 100 {
                        withAnimation(.easeInOut(duration: 0.3)) {
                            showSettings = false
                        }
                    }
                }
        )
        .onAppear {
            selectedMode = settings.colorMode
        }
    }
}

struct PixelMap {

    private var _pixels: [UInt8]
    private var _pixelsWidth: Int
    private var _pixelsHeight: Int
    private var _scale: Int = 1
    private let _depth: Int = 4

    init(_ width: Int, _ height: Int, scale: Int = 1) {
        self._pixelsWidth = width
        self._pixelsHeight = height
        self._scale = scale
        self._pixels = [UInt8](repeating: 0, count: self._pixelsWidth * self._pixelsHeight * self._depth)
    }

    var width: Int {
        return (self._pixelsWidth + self._scale - 1) / self._scale
    }

    var height: Int {
        return (self._pixelsHeight + self._scale - 1) / self._scale
    }

    var depth: Int {
        return self._depth
    }

    var data: UnsafeMutablePointer<UInt8> {
        return UnsafeMutablePointer(mutating: self._pixels)
    }

    mutating func xxx_write(_ x: Int, _ y: Int, red: UInt8, green: UInt8, blue: UInt8, transparency: UInt8 = 255) {
        for dy in 0..<self._scale {
            for dx in 0..<self._scale {
                let ix = x * self._scale + dx
                let iy = y * self._scale + dy
                let i = (iy * self._pixelsWidth + ix) * self._depth
                if (i + 3 < self._pixels.count) {
                    self._pixels[i] = red
                    self._pixels[i + 1] = green
                    self._pixels[i + 2] = blue
                    self._pixels[i + 3] = transparency
                }
            }
        }
    }

    mutating func write(_ x: Int, _ y: Int, red: UInt8, green: UInt8, blue: UInt8, transparency: UInt8 = 255) {
        for dy in 0..<self._scale {
            for dx in 0..<self._scale {
                let ix = x * self._scale + dx
                let iy = y * self._scale + dy
                let i = (iy * self._pixelsWidth + ix) * self._depth
                if ((ix < self._pixelsWidth) && (i < self._pixels.count)) {
                    self._pixels[i] = red
                    self._pixels[i + 1] = green
                    self._pixels[i + 2] = blue
                    self._pixels[i + 3] = transparency
                }
            }
        }
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
            .environmentObject(AppSettings())  // Inject the AppSettings here for the preview
    }
}

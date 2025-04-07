import SwiftUI

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

/*
class AppSettings: ObservableObject {
    @Published var useGrayscale: Bool = false {
        didSet { print("Grayscale changed: \(useGrayscale)") }
    }
    @Published var pixelSize: Int = 1 {
        didSet { print("Pixel Size changed: \(pixelSize)") }
    }
}
*/

import SwiftUI
import Foundation

class AppSettings: ObservableObject {
    @Published var pixels: PixelMap
    @Published var colorMode: ColorMode = ColorMode.monochrome
    @Published var pixelSize: Int = 4
    @Published var soundEnabled: Bool = true
    @Published var hapticEnabled: Bool = true
    @Published var randomFixedImage: Bool = true
    @Published var randomFixedImagePeriod: RandomFixedImagePeriod = RandomFixedImagePeriod.sometimes
    init() {
        pixels = PixelMap(ScreenWidth, ScreenHeight, scale: 4)
    }
}

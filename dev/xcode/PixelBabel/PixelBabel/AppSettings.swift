import SwiftUI
import Foundation

class DefaultAppSettings
{
    public static let pixelSize: Int = 10
    public static let colorMode: ColorMode = ColorMode.color
}

class AppSettings: ObservableObject
{
    @Published var pixels: PixelMap
    @Published var colorMode: ColorMode = DefaultAppSettings.colorMode
    @Published var pixelSize: Int = DefaultAppSettings.pixelSize
    @Published var soundEnabled: Bool = false // xyzzy
    @Published var hapticEnabled: Bool = false // xyzzy
    @Published var randomFixedImage: Bool = true
    @Published var randomFixedImagePeriod: RandomFixedImagePeriod = RandomFixedImagePeriod.sometimes

    init() {
        self.pixels = PixelMap(ScreenWidth, ScreenHeight,
                               scale: DefaultAppSettings.pixelSize, mode: DefaultAppSettings.colorMode)
    }
}

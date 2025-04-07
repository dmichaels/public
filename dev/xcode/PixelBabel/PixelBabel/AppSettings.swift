import SwiftUI
import Foundation

class DefaultAppSettings
{
    public static let pixelSize: Int = 10
}

class AppSettings: ObservableObject
{
    @Published var pixels: PixelMap
    @Published var colorMode: ColorMode = ColorMode.color
    @Published var pixelSize: Int = DefaultAppSettings.pixelSize
    @Published var soundEnabled: Bool = true
    @Published var hapticEnabled: Bool = true
    @Published var randomFixedImage: Bool = true
    @Published var randomFixedImagePeriod: RandomFixedImagePeriod = RandomFixedImagePeriod.sometimes

    init() {
        self.pixels = PixelMap(ScreenWidth, ScreenHeight, scale: DefaultAppSettings.pixelSize)
    }
}

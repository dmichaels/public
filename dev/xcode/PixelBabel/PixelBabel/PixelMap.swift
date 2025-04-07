import Foundation
import SwiftUI

let ScreenWidth = Int(UIScreen.main.bounds.width)
let ScreenHeight = Int(UIScreen.main.bounds.height)
let ScreenDepth = 4

class PixelMap {

    private var _pixels: [UInt8]
    private var _pixelsWidth: Int
    private var _pixelsHeight: Int
    private var _pixelsCount: Int
    private var _scale: Int = 1

    init(_ width: Int, _ height: Int, scale: Int = 1) {
        self._pixelsWidth = width
        self._pixelsHeight = height
        self._pixelsCount = self._pixelsWidth * self._pixelsHeight * ScreenDepth
        self._scale = scale
        self._pixels = [UInt8](repeating: 0, count: self._pixelsCount)
    }

    public var width: Int {
        return (self._pixelsWidth + self._scale - 1) / self._scale
    }

    public var height: Int {
        return (self._pixelsHeight + self._scale - 1) / self._scale
    }

    public var scale: Int {
        get { return self._scale }
        set { self._scale = newValue }
    }

    public var data: [UInt8] {
        get { return self._pixels }
        set { self._pixels = newValue }
    }

    public func randomize(_ settings: AppSettings)
    {
        PixelMap._randomize(self, settings: settings)
    }

    public func load(_ name: String)
    {
        guard let image = UIImage(named: name),
            let cgImage = image.cgImage else {
            return
        }

        let colorSpace = CGColorSpaceCreateDeviceRGB()
        let bitmapInfo = CGBitmapInfo(rawValue: CGImageAlphaInfo.premultipliedLast.rawValue)
    
        guard let context = CGContext(
            data: &self._pixels,
            width: self._pixelsWidth,
            height: self._pixelsHeight,
            bitsPerComponent: 8,
            bytesPerRow: self._pixelsWidth * ScreenDepth,
            space: colorSpace,
            bitmapInfo: bitmapInfo.rawValue
        ) else {
            return
        }

        context.draw(cgImage, in: CGRect(x: 0, y: 0, width: self._pixelsWidth, height: self._pixelsHeight))
    }

    private static func _randomize(_ pixelMap: PixelMap, settings: AppSettings)
    {
        let colorMode = settings.colorMode
        for y in 0..<pixelMap.height {
            for x in 0..<pixelMap.width {
                if (colorMode == ColorMode.monochrome) {
                    let value: UInt8 = UInt8.random(in: 0...1) * 255
                    PixelMap._write(pixelMap, x, y, red: value, green: value, blue: value)
                }
                else if colorMode == ColorMode.grayscale {
                    let value = UInt8.random(in: 0...255)
                    PixelMap._write(pixelMap, x, y, red: value, green: value, blue: value)
                }
                else {
                    let rgb = UInt32.random(in: 0...0xFFFFFF)
                    let red = UInt8((rgb >> 16) & 0xFF)
                    let green = UInt8((rgb >> 8) & 0xFF)
                    let blue = UInt8(rgb & 0xFF)
                    PixelMap._write(pixelMap, x, y, red: red, green: green, blue: blue)
                }
            }
        }
    }

    private static func _write(_ pixelMap: PixelMap, _ x: Int, _ y: Int,
                               red: UInt8, green: UInt8, blue: UInt8, transparency: UInt8 = 255)
    {
        for dy in 0..<pixelMap._scale {
            for dx in 0..<pixelMap._scale {
                let ix = x * pixelMap._scale + dx
                let iy = y * pixelMap._scale + dy
                let i = (iy * pixelMap._pixelsWidth + ix) * ScreenDepth
                if ((ix < pixelMap._pixelsWidth) && (i < pixelMap._pixels.count)) {
                    pixelMap._pixels[i] = red
                    pixelMap._pixels[i + 1] = green
                    pixelMap._pixels[i + 2] = blue
                    pixelMap._pixels[i + 3] = transparency
                }
            }
        }
    }
}

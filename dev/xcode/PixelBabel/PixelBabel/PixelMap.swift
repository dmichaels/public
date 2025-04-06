import Foundation
import SwiftUI

let ScreenWidth = Int(UIScreen.main.bounds.width)
let ScreenHeight = Int(UIScreen.main.bounds.height)
let ScreenDepth = 4

struct PixelMap {

    public var _pixels: [UInt8]
    private var _pixelsWidth: Int
    private var _pixelsHeight: Int
    private var _scale: Int = 1

    init(_ width: Int, _ height: Int, scale: Int = 1) {
        self._pixelsWidth = width
        self._pixelsHeight = height
        self._scale = scale
        self._pixels = [UInt8](repeating: 0, count: self._pixelsWidth * self._pixelsHeight * ScreenDepth)
        print("ALLOCATE")
    }

    var width: Int {
        return (self._pixelsWidth + self._scale - 1) / self._scale
    }

    var height: Int {
        return (self._pixelsHeight + self._scale - 1) / self._scale
    }

    var data: [UInt8] {
        get { return self._pixels }
        set { self._pixels = newValue }
    }

    mutating func write(_ x: Int, _ y: Int, red: UInt8, green: UInt8, blue: UInt8, transparency: UInt8 = 255)
    {
        for dy in 0..<self._scale {
            for dx in 0..<self._scale {
                let ix = x * self._scale + dx
                let iy = y * self._scale + dy
                let i = (iy * self._pixelsWidth + ix) * ScreenDepth
                if ((ix < self._pixelsWidth) && (i < self._pixels.count)) {
                    self._pixels[i] = red
                    self._pixels[i + 1] = green
                    self._pixels[i + 2] = blue
                    self._pixels[i + 3] = transparency
                }
            }
        }
    }

    mutating func randomize(_ settings: AppSettings)
    {
        print("RANDOMIZE")
        let colorMode = settings.colorMode
        for y in 0..<self.height {
            for x in 0..<self.width {
                if (colorMode == ColorMode.monochrome) {
                    let value: UInt8 = UInt8.random(in: 0...1) * 255
                    self.write(x, y, red: value, green: value, blue: value)
                }
                else if colorMode == ColorMode.grayscale {
                    let value = UInt8.random(in: 0...255)
                    self.write(x, y, red: value, green: value, blue: value)
                }
                else {
                    let rgb = UInt32.random(in: 0...0xFFFFFF)
                    let red = UInt8((rgb >> 16) & 0xFF)
                    let green = UInt8((rgb >> 8) & 0xFF)
                    let blue = UInt8(rgb & 0xFF)
                    self.write(x, y, red: red, green: green, blue: blue)
                }
            }
        }
    }

    mutating func load(_ name: String)
    {
        print("LOAD")
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
}

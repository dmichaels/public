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
    private var _pixelsCache: [[UInt8]] = []
    private let _pixelsCacheQueueSize: Int = 5
    private let _pixelsCacheQueue = DispatchQueue(label: "_pixelsCacheQueue" /* , attributes: .concurrent */)
    private var _pixelsCacheQueueGenerating: Bool = false

    init(_ width: Int, _ height: Int, scale: Int = 1, _internal: Bool = false) {
        self._pixelsWidth = width
        self._pixelsHeight = height
        self._pixelsCount = self._pixelsWidth * self._pixelsHeight * ScreenDepth
        self._scale = scale
        self._pixels = [UInt8](repeating: 0, count: self._pixelsCount)
        self._pixelsCache = Array(repeating: [UInt8](repeating: 0, count: self._pixelsCount), count: _pixelsCacheQueueSize)
        print("ALLOCATE")
        if !_internal {
            self.replenishPixelsCache()
        }
    }

    var width: Int {
        return (self._pixelsWidth + self._scale - 1) / self._scale
    }

    var height: Int {
        return (self._pixelsHeight + self._scale - 1) / self._scale
    }

    var scale: Int {
        get { return self._scale }
        set { self._scale = newValue }
    }

    var data: [UInt8] {
        get { return self._pixels }
        set { self._pixels = newValue }
    }

    private func replenishPixelsCache() {
        // _pixelsCacheQueue.async { [weak self] in
        print("REPLENISH")
        _pixelsCacheQueue.async {
            print("CACHING: \(self._pixelsCache.count)")
            // var xxx = PixelMap(self._pixelsWidth, self._pixelsHeight, scale: self._scale, _internal: true)
            self._pixelsCacheQueueGenerating = true
            for index in 0..<self._pixelsCache.count {
                print("FILL: \(index) \(self._pixelsCount)")
                for i in 0..<self._pixelsCount {
                    self._pixelsCache[index][i] = UInt8(255)
                }
                self._pixelsCache.append(self._pixelsCache[index])
                // self.fillPixelSet(at: i)
            }
            self._pixelsCacheQueueGenerating = false
        }
    }

    /*
    private func replenishPixelsCache() {
        _pixelsCacheQueue.async { [weak self] in
            guard let self = self else { return }
            self._pixelsCacheQueueGenerating = true
            while self._pixelsCacheQueue.count < self._pixelsCacheQueue {
                let pixels = self.generateRandomPixels(count: self.pixelsPerSet)
                DispatchQueue.main.async {
                    self._pixelsCacheQueue.append(pixels)
                    // if self.currentPixels.isEmpty {
                    //     self.currentPixels = pixels
                    // }
                }
            }
            self._pixelsCacheQueueGenerating = false
        }
    }
    */

    /*mutating*/ func write(_ x: Int, _ y: Int, red: UInt8, green: UInt8, blue: UInt8, transparency: UInt8 = 255)
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

    /*mutating*/ func randomize(_ settings: AppSettings)
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

    /*mutating*/ func load(_ name: String)
    {
        var xxx = self._pixelsCache.remove(at: 0)
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

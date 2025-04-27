import Foundation
import SwiftUI

let ScreenWidth = Int(UIScreen.main.bounds.width)
let ScreenHeight = Int(UIScreen.main.bounds.height)
let ScreenDepth = 4

class PixelMap {

    private var _pixels: [UInt8]
    private var _pixelsWidth: Int
    private var _pixelsHeight: Int
    private var _scale: Int = 1
    private var _mode: ColorMode = ColorMode.color

    private let _producer: Bool
    private let _pixelsListMax: Int = 5
    private var _pixelsList: [[UInt8]]? = nil
    private var _pixelsListAccessQueue: DispatchQueue? = nil
    private var _pixelsListReplenishQueue: DispatchQueue? = nil

    init(_ width: Int, _ height: Int, scale: Int = 1, mode: ColorMode = ColorMode.color, producer: Bool = true) {
        print("PixelMap.init: \(producer)")
        self._pixelsWidth = width
        self._pixelsHeight = height
        self._scale = scale
        self._pixels = [UInt8](repeating: 0, count: self._pixelsWidth * self._pixelsHeight * ScreenDepth)
        self._producer = producer
        if (producer) {
            self._pixelsList = []
            self._pixelsListAccessQueue = DispatchQueue(label: "PixelBabel.PixelMap.ACCESS" /*, qos: .background */)
            self._pixelsListReplenishQueue = DispatchQueue(label: "PixelBabel.PixelMap.REPLENISH", qos: .background)
            self._replenishPixelsListAsync()
        }
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

    public var mode: ColorMode {
        get { return self._mode }
        set { self._mode = newValue }
    }

    public var data: [UInt8] {
        get { return self._pixels }
        set { self._pixels = newValue }
    }

    public func randomize()
    {
        var done: Bool = false
        print("xyzzy.a")
        if (self._producer) {
            print("xyzzy.b")
            self._pixelsListAccessQueue!.async {
                print("FOO: [\(self._pixelsList!.count)]")
                print("xyzzy.c")
                // This block of code is effectively to synchronize
                // access to pixelsList between (this) producer and consumer.
                if !self._pixelsList!.isEmpty {
                    print("xyzzy.d")
                    print("PixelMap.randomize.grab-pre-computed-pixels-todo: [\(self._pixelsList!.count)]")
                    let pixels: [UInt8] = self._pixelsList!.removeFirst()
                    print("PixelMap.randomize.grab-pre-computed-pixels-done: [\(self._pixelsList!.count)]")
                    self._pixels = pixels
                    done = true
                    // pixels = nil
                    // print("PixelMap.randomize.c")
                    // print("PixelMap.randomize.d")
                }
                else {
                    print("xyzzy.e")
                    print("PixelMap.randomize.initial")
                }
                print("xyzzy.f")
            }
            print("xyzzy.g")
            self._replenishPixelsListAsync()
            print("xyzzy.h")
        }
        print("xyzzy.i")
        if (!done) {
            print("randomize.xyzzy")
            let pixelMap: PixelMap = PixelMap(self._pixelsWidth, self._pixelsHeight,
                                              scale: self._scale, mode: self._mode, producer: false)
            PixelMap._randomize(pixelMap)
            self._pixels = pixelMap._pixels
            // PixelMap._randomize(self)
        }
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

    public func invalidate()
    {
        if (self._producer) {
            self._pixelsListAccessQueue!.async {
                self._pixelsList!.removeAll()
            }
        }
    }

    private static func _randomize(_ pixelMap: PixelMap)
    {
        for y in 0..<pixelMap.height {
            for x in 0..<pixelMap.width {
                if (pixelMap._mode == ColorMode.monochrome) {
                    let value: UInt8 = UInt8.random(in: 0...1) * 255
                    PixelMap._write(pixelMap, x, y, red: value, green: value, blue: value)
                }
                else if pixelMap._mode == ColorMode.grayscale {
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

    private func _replenishPixelsListAsync() {
        // DispatchQueue.global(qos: .background).async {
        self._pixelsListReplenishQueue!.async {
            print("_replenishPixelsListAsync.start")
            // This block of code runs OFF of the main thread (i.e. in the background);
            // and no more than one of these will ever be running at a time. 
            var additionalPixelsProbableCount: Int = 0
            self._pixelsListAccessQueue!.async {
                // This block of code is effectively to synchronize
                // access to pixelsList between (this) producer and consumer.
                additionalPixelsProbableCount = self._pixelsListMax - self._pixelsList!.count
                print("_replenishPixelsListAsync.probable-count: [\(self._pixelsListMax)] - [\(self._pixelsList!.count)] = [\(additionalPixelsProbableCount)]")
            }
            print("_replenishPixelsListAsync.probable-count-after-async")
            if (additionalPixelsProbableCount > 0) {
                print("_replenishPixelsListAsync.probable-count-yes")
                var additionalPixelsProbableList: [[UInt8]] = []
                for _ in 0..<additionalPixelsProbableCount {
                    let pixelMap: PixelMap = PixelMap(self._pixelsWidth, self._pixelsHeight,
                                                      scale: self._scale, mode: self._mode, producer: false)
                    pixelMap.randomize()
                    additionalPixelsProbableList.append(pixelMap._pixels)
                }
                self._pixelsListAccessQueue!.async {
                    let additionalPixelsActualCount: Int = self._pixelsListMax - self._pixelsList!.count
                    print("_replenishPixelsListAsync.actual-count: [\(self._pixelsListMax)] - [\(self._pixelsList!.count)] = [\(additionalPixelsActualCount)]")
                    if (additionalPixelsActualCount > 0) {
                        // This block of code is effectively to synchronize
                        // access to pixelsList between (this) producer and consumer.
                        print("replenishPixelsListAsync.additional-actual-count: [\(additionalPixelsActualCount)] to [\(self._pixelsList!.count)]")
                        self._pixelsList!.append(contentsOf: additionalPixelsProbableList.prefix(additionalPixelsActualCount))
                        print("replenishPixelsListAsync.additional-append-done: [\(additionalPixelsActualCount)] to [\(self._pixelsList!.count)]")
                    }
                }
            }
            print("_replenishPixelsListAsync.end")
        }
    }
/*
    private func _replenishPixelsListAsync() {
        DispatchQueue.global(qos: .background).async {
            print("_replenishPixelsListAsync.start")
            // This block of code runs OFF of the main thread (i.e. in the background).
            if (self._pixelsList!.count < self._pixelsListMax) {
                var additionalPixelsList: [[UInt8]] = []
                for _ in 0..<(self._pixelsListMax - self._pixelsList!.count) {
                    print("_replenishPixelsListAsync.create-pixelmap-todo")
                    let pixelMap = PixelMap(self._pixelsWidth, self._pixelsHeight,
                                            scale: self._scale, mode: self._mode, producer: false)
                    print("_replenishPixelsListAsync.create-pixelmap-done")
                    pixelMap.randomize()
                    additionalPixelsList.append(pixelMap._pixels)
                }
                print("_replenishPixelsListAsync.append-todo: [\(additionalPixelsList.count)] for [\(self._pixelsList!.count)]")
                if (additionalPixelsList.count > 0) {
                    self._pixelsListAccessQueue!.async {
                        // This block of code is effectively to synchronize
                        // access to pixelsList between (this) producer and consumer.
                        print("_replenishPixelsListAsync.append: [\(additionalPixelsList.count)] to [\(self._pixelsList!.count)]")
                        self._pixelsList!.append(contentsOf: additionalPixelsList)
                        print("_replenishPixelsListAsync.append-done: [\(additionalPixelsList.count)] to [\(self._pixelsList!.count)]")
                    }
                }
            }
            print("_replenishPixelsListAsync.done")
        }
    }
*/

    // TODO
    // Need to go back to this ... do not call into (non-static) PixelMap stuff from producer ...

    static func maybe_randomize(_ pixels: inout [UInt8], _ pixelsWidth: Int, _ pixelsHeight: Int,
                           width: Int, height: Int, scale: Int, mode: ColorMode)
    {
        for y in 0..<height {
            for x in 0..<width {
                if (mode == ColorMode.monochrome) {
                    let value: UInt8 = UInt8.random(in: 0...1) * 255
                    PixelMap.maybe_write(&pixels, pixelsWidth, pixelsHeight,
                                    x: x, y: y, scale: scale, red: value, green: value, blue: value)
                }
                else if (mode == ColorMode.grayscale) {
                    let value = UInt8.random(in: 0...255)
                    PixelMap.maybe_write(&pixels, pixelsWidth, pixelsHeight,
                                    x: x, y: y, scale: scale, red: value, green: value, blue: value)
                }
                else {
                    let rgb = UInt32.random(in: 0...0xFFFFFF)
                    let red = UInt8((rgb >> 16) & 0xFF)
                    let green = UInt8((rgb >> 8) & 0xFF)
                    let blue = UInt8(rgb & 0xFF)
                    PixelMap.maybe_write(&pixels, pixelsWidth, pixelsHeight,
                                     x: x, y: y, scale: scale, red: red, green: green, blue: blue)
                }
            }
        }
    }

    static func maybe_write(_ pixels: inout [UInt8], _ pixelsWidth: Int, _ pixelsHeight: Int,
                       x: Int, y: Int, scale: Int,
                       red: UInt8, green: UInt8, blue: UInt8, transparency: UInt8 = 255)
    {
        for dy in 0..<scale {
            for dx in 0..<scale {
                let ix = x * scale + dx
                let iy = y * scale + dy
                let i = (iy * pixelsWidth + ix) * ScreenDepth
                if ((ix < pixelsWidth) && (i < pixels.count)) {
                    pixels[i] = red
                    pixels[i + 1] = green
                    pixels[i + 2] = blue
                    pixels[i + 3] = transparency
                }
            }
        }
    }
}

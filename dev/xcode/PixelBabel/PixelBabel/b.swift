
import Foundation

// Simulate a Pixel data type (could be a 2D array or any structure)
typealias Pixel = UInt8

class PixelProducer
{
    private let _pixelsListMax: Int = 5
    private var _pixelsList: [[Pixel]] = []
    private let _pixelsListDispatchQueue = DispatchQueue(label: "com.pixelproducer.access", qos: .background)
    
    init(produce: Bool = false) {
        if (produce) {
            self._pixelsListDispatchQueue.async {
                let initialPixels = self.generatePixels()
                self._pixelsList.append(initialPixels)
            }
            self.producePixelList()
        }
    }

    private func generatePixels() -> [Pixel] {
        let pixels = Array(0..<100).map { UInt8($0) } // Array(0..<100)  // Simulating 100 pixels as an example
        print("GENERATE-PIXELS: [\(pixels.count)]")
        return pixels
    }
    
    func producePixelList() {
        DispatchQueue.global(qos: .background).async {
            // This block of code runs OFF of the main thread (i.e. in the background).
            if (self._pixelsList.count < self._pixelsListMax) {
                var additionalPixelsList: [[Pixel]] = []
                for _ in 0..<(self._pixelsListMax - self._pixelsList.count) {
                    let additionalPixels = self.generatePixels()
                    additionalPixelsList.append(additionalPixels)
                }
                if (additionalPixelsList.count > 0) {
                    // This block of code is effectively to synchronize access
                    // to pixelsList between (this) producer and consumer.
                    self._pixelsListDispatchQueue.async {
                        print("PRODUCE-PIXELS: [\(additionalPixelsList.count)] to [\(self._pixelsList.count)]")
                        self._pixelsList.append(contentsOf: additionalPixelsList)
                    }
                }
            }
        }
    }
    
    func consumePixelList() -> [Pixel]? {
        var removedPixelList: [Pixel]?
        _pixelsListDispatchQueue.sync {
            if (!self._pixelsList.isEmpty) {
                removedPixelList = self._pixelsList.removeFirst()
            }
        }
        return removedPixelList
    }
}

// Simulate a simple run of producing and consuming pixel lists
let producer = PixelProducer(produce: true)

// Simulate the "tap" behavior (call producePixelList and consumePixelList multiple times)
for _ in 1...10 {
    // Simulate a tap which consumes a pixel list (prints the pixel list)
    if let list = producer.consumePixelList() {
        print("CONSUME-PIXELS: [\(list.count)]")
    } else {
        print("NO-PIXELS-AVAILABLE: []")
    }
    
    // After consuming, produce a new pixel list in the background
    //producer.producePixelList()
    
    // Wait a bit before consuming again (simulate time for background work to happen)
    Thread.sleep(forTimeInterval: 1)
}

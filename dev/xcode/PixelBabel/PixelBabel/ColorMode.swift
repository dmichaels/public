import Foundation
import SwiftUI

enum ColorMode: String, CaseIterable, Identifiable {
    case monochrome = "Monochrome"
    case grayscale = "Grayscale"
    case color = "Color"
    var id: String { self.rawValue }
}

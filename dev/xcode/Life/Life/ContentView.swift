
import SwiftUI

// Model
struct GridPoint: Hashable {
    let x: Int
    let y: Int
}

class GameOfLife: ObservableObject {
    @Published var aliveCells: Set<GridPoint> = []
    
    func toggleCell(at point: GridPoint) {
        if aliveCells.contains(point) {
            aliveCells.remove(point)
        } else {
            aliveCells.insert(point)
        }
    }
    
    func isAlive(_ point: GridPoint) -> Bool {
        aliveCells.contains(point)
    }
    
    func step() {
        var newAliveCells: Set<GridPoint> = []
        var neighborCounts: [GridPoint: Int] = [:]
        
        for cell in aliveCells {
            for dx in -1...1 {
                for dy in -1...1 {
                    if dx != 0 || dy != 0 {
                        let neighbor = GridPoint(x: cell.x + dx, y: cell.y + dy)
                        neighborCounts[neighbor, default: 0] += 1
                    }
                }
            }
        }
        
        for (point, count) in neighborCounts {
            if count == 3 || (count == 2 && aliveCells.contains(point)) {
                newAliveCells.insert(point)
            }
        }
        
        aliveCells = newAliveCells
    }
}

// Settings Model
class Settings: ObservableObject {
    @Published var showGrid = true
}

struct ContentView: View {
    @StateObject private var game = GameOfLife()
    @StateObject private var settings = Settings()
    @State private var offset = CGSize.zero
    @State private var lastDragPosition: CGSize = .zero
    @State private var scale: CGFloat = 20.0 // Default scale for cells
    @State private var scaleFactor: CGFloat = 1.0 // Track scale factor for smooth zoom

    @State private var isPlaying = false
    @State private var timer: Timer? = nil
    
    @State private var lastTapLocation: CGPoint? = nil // To track tap location
    @State private var dragThreshold: CGFloat = 5.0 // Minimum drag distance before considered a drag
    
    var body: some View {
        NavigationView {
            GeometryReader { geometry in
                Canvas { context, size in
                    // Calculate grid size based on view's geometry and zoom scale
                    let columns = Int(size.width / scale) + 2
                    let rows = Int(size.height / scale) + 2
                    
                    // Account for offset (scrolling) and zoom factor
                    let xOffset = Int(offset.width / scale)
                    let yOffset = Int(offset.height / scale)

                    // Draw grid if enabled
                    if settings.showGrid {
                        let gridPath = Path { path in
                            for col in 0..<columns {
                                let x = CGFloat(col) * scale - offset.width.truncatingRemainder(dividingBy: scale)
                                path.move(to: CGPoint(x: x, y: 0))
                                path.addLine(to: CGPoint(x: x, y: size.height))
                            }
                            for row in 0..<rows {
                                let y = CGFloat(row) * scale - offset.height.truncatingRemainder(dividingBy: scale)
                                path.move(to: CGPoint(x: 0, y: y))
                                path.addLine(to: CGPoint(x: size.width, y: y))
                            }
                        }
                        context.stroke(gridPath, with: .color(.gray.opacity(0.3)), lineWidth: 0.5)
                    }

                    // Draw cells
                    for point in game.aliveCells {
                        let x = CGFloat(point.x - xOffset) * scale
                        let y = CGFloat(point.y - yOffset) * scale
                        let rect = CGRect(x: x, y: y, width: scale, height: scale)
                        context.fill(Path(rect), with: .color(.black))
                    }
                }
                .gesture(DragGesture(minimumDistance: 0)
                    .onChanged { value in
                        print("drag: \(value)")
                        // Handle dragging the view
                        let dragDistance = value.translation
                        if abs(dragDistance.width) > dragThreshold || abs(dragDistance.height) > dragThreshold {
                            // Update the offset to drag the view
                            offset = CGSize(width: lastDragPosition.width + dragDistance.width,
                                            height: lastDragPosition.height + dragDistance.height)
                        } else {
                            // Consider it a tap for cell toggle
                            let location = value.startLocation
                            let x = Int((location.x + offset.width) / scale)
                            let y = Int((location.y + offset.height) / scale)
                            game.toggleCell(at: GridPoint(x: x, y: y))
                        }
                    }
                    .onEnded { _ in
                        lastDragPosition = offset
                    }
                )
                .gesture(MagnificationGesture()
                    .onChanged { value in
                        print("mag: \(value)")
                        // Apply smooth zooming
                        let newScale = scaleFactor * value
                        // Scale with smooth damping effect
                        let newScaleValue = max(5, min(80, scale * newScale / scaleFactor))
                        scale = newScaleValue
                        scaleFactor = value // Update scale factor for the next gesture
                    }
                    .onEnded { _ in
                        // Reset scale factor after zoom ends
                        scaleFactor = 1.0
                    }
                )
                .navigationTitle("Game of Life")
                .toolbar {
                    ToolbarItem(placement: .navigationBarTrailing) {
                        NavigationLink(destination: SettingsView(settings: settings)) {
                            Image(systemName: "gear")
                        }
                    }
                    ToolbarItem(placement: .bottomBar) {
                        HStack {
                            Button("Step") {
                                game.step()
                            }
                            Button(action: togglePlayPause) {
                                Text(isPlaying ? "Pause" : "Play")
                            }
                        }
                    }
                }
            }
        }
        .onChange(of: isPlaying) { newValue in
            if newValue {
                startGameLoop()
            } else {
                stopGameLoop()
            }
        }
    }
    
    private func togglePlayPause() {
        isPlaying.toggle()
    }
    
    private func startGameLoop() {
        timer?.invalidate() // Cancel any existing timer
        timer = Timer.scheduledTimer(withTimeInterval: 0.5, repeats: true) { _ in
            game.step()
        }
    }
    
    private func stopGameLoop() {
        timer?.invalidate()
        timer = nil
    }
}

struct SettingsView: View {
    @ObservedObject var settings: Settings
    
    var body: some View {
        Form {
            Toggle("Show Grid", isOn: $settings.showGrid)
        }
        .navigationTitle("Settings")
    }
}

class AppSettings: ObservableObject {
    @Published var dummy: Int = 0
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
            .environmentObject(AppSettings())  // Inject the AppSettings here for the preview
    }
}

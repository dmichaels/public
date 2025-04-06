import SwiftUI

// Model for each cell in the grid
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
}

struct ContentView: View {
    @State private var isTapped: Bool = false
    @GestureState private var longPress: LongPressGesture = LongPressGesture ()
        .updating($longPress)
    var body: some View {
        VStack {
            Circle()
                .fill(isTapped ? .red : .teal)
                .frame(width: 200, height: 200)
                .gesture(TapGesture()
                    .onEnded { _ in
                        print("onTapGesture")
                        isTapped.toggle()
                    }
                    .updating($gestureState)
                )
        }
        /*
            .onTapGesture(count: 3) { location in
                print("onTapGesture: \(location)")
                isTapped.toggle()
            }
         */
    }
}

struct XContentView: View {
    @StateObject private var game = GameOfLife()
    @State private var offset = CGSize.zero
    @State private var lastDragPosition: CGSize = .zero
    @State private var scale: CGFloat = 10.0 // Cell size (10x10)
    
    @State private var dragThreshold: CGFloat = 5.0 // Minimum drag distance to start dragging
    @State private var isDragging = false
    @State private var dragStartLocation: CGPoint? = nil // To track the start location of drag
    
    var body: some View {
        GeometryReader { geometry in
            Canvas { context, size in
                // Number of columns and rows based on size and scale (cell size)
                let columns = Int(size.width / scale)
                let rows = Int(size.height / scale)
                
                // Draw the grid
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
                
                // Draw cells
                for point in game.aliveCells {
                    let x = CGFloat(point.x) * scale - offset.width
                    let y = CGFloat(point.y) * scale - offset.height
                    let rect = CGRect(x: x, y: y, width: scale, height: scale)
                    context.fill(Path(rect), with: .color(.black))
                }
            }
            .frame(width: geometry.size.width, height: geometry.size.height)
            // Add the drag gesture to move the grid
            .gesture(DragGesture(minimumDistance: 0)
                .onChanged { value in
                    print("DragGesture: \(value)")
                    // If we are already dragging, update offset
                    if let startLocation = dragStartLocation {
                        let dragDistance = value.translation
                        if abs(dragDistance.width) > dragThreshold || abs(dragDistance.height) > dragThreshold {
                            // Invert the drag translation to correct the direction
                            offset = CGSize(width: lastDragPosition.width - dragDistance.width,
                                             height: lastDragPosition.height - dragDistance.height)
                            isDragging = true
                        }
                    } else {
                        // Save the initial drag start location for comparison
                        dragStartLocation = value.startLocation
                    }
                }
                .onEnded { _ in
                    lastDragPosition = offset
                    dragStartLocation = nil // Reset drag start location
                    isDragging = false
                }
            )
            // Add tap gesture to toggle cells
            .simultaneousGesture(DragGesture()
                .onEnded { location in
                    print("TapGesture: \(location)")
                    // Calculate the tap location in the local coordinate space
                    let localTapLocation = geometry.frame(in: .local).origin
                    print("TapGesture.location: \(localTapLocation)")
                    // Convert the tap location to grid coordinates
                    let tapX = Int((localTapLocation.x + offset.width) / scale)
                    let tapY = Int((localTapLocation.y + offset.height) / scale)
                    print("TapGesture.xy: \(tapX) \(tapY)")
                    // Toggle the cell at the calculated position
                    game.toggleCell(at: GridPoint(x: tapX, y: tapY))
                    /*
                    let location = geometry.frame(in: .local)
                    print("TapGesture.location: \(location) - \(isDragging)")
                    guard !isDragging else { return } // Prevent tapping while dragging
                    let x = Int((location.minX + offset.width) / scale)
                    let y = Int((location.minY + offset.height) / scale)
                    print("PRINT")
                    game.toggleCell(at: GridPoint(x: x, y: y))
                    // game.toggleCell(at: GridPoint(x: 0, y: 0))
                     */
                }
            )
        }
        .navigationTitle("Conway's Game of Life")
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}

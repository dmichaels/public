
import SwiftUI

struct Point: Hashable {
    var x: Int
    var y: Int
}

struct ContentView: View {
    @State private var cells: Set<Point> = []
    @State private var isRunning = false
    @State private var timer: Timer?
    
    var body: some View {
        VStack {
            ZStack {
                LifeGridView(cells: $cells)
                    .border(Color.gray)
            }
            .frame(maxWidth: .infinity, maxHeight: .infinity)
            .background(Color.white)
            
            HStack {
                Button(isRunning ? "Pause" : "Play") {
                    isRunning.toggle()
                    if isRunning {
                        timer = Timer.scheduledTimer(withTimeInterval: 0.2, repeats: true) { _ in
                            cells = nextGeneration(from: cells)
                        }
                    } else {
                        timer?.invalidate()
                    }
                }
                Button("Clear") {
                    isRunning = false
                    timer?.invalidate()
                    cells.removeAll()
                }
            }
            .padding()
        }
    }
    
    func nextGeneration(from current: Set<Point>) -> Set<Point> {
        var neighborCounts: [Point: Int] = [:]
        
        for cell in current {
            for dx in -1...1 {
                for dy in -1...1 {
                    if dx == 0 && dy == 0 { continue }
                    let neighbor = Point(x: cell.x + dx, y: cell.y + dy)
                    neighborCounts[neighbor, default: 0] += 1
                }
            }
        }
        
        var nextGen: Set<Point> = []
        
        for (point, count) in neighborCounts {
            if count == 3 || (count == 2 && current.contains(point)) {
                nextGen.insert(point)
            }
        }
        
        return nextGen
    }
}

struct LifeGridView: UIViewRepresentable {
    @Binding var cells: Set<Point>
    
    func makeUIView(context: Context) -> LifeGridUIView {
        let view = LifeGridUIView()
        view.cells = cells
        view.onToggleCell = { point in
            if view.cells.contains(point) {
                view.cells.remove(point)
            } else {
                view.cells.insert(point)
            }
            self.cells = view.cells
        }
        return view
    }
    
    func updateUIView(_ uiView: LifeGridUIView, context: Context) {
        uiView.cells = cells
        uiView.setNeedsDisplay()
    }
}

class LifeGridUIView: UIView {
    var cells: Set<Point> = []
    var cellSize: CGFloat = 20.0
    var offset: CGPoint = .zero
    var scale: CGFloat = 1.0
    
    var onToggleCell: ((Point) -> Void)?
    
    private var lastPanLocation: CGPoint = .zero
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        backgroundColor = .white
        setupGestures()
    }
    
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        backgroundColor = .white
        setupGestures()
    }
    
    func setupGestures() {
        let pan = UIPanGestureRecognizer(target: self, action: #selector(handlePan(_:)))
        addGestureRecognizer(pan)
        
        let pinch = UIPinchGestureRecognizer(target: self, action: #selector(handlePinch(_:)))
        addGestureRecognizer(pinch)
        
        let tap = UITapGestureRecognizer(target: self, action: #selector(handleTap(_:)))
        addGestureRecognizer(tap)
    }
    
    override func draw(_ rect: CGRect) {
        guard let context = UIGraphicsGetCurrentContext() else { return }
        
        context.setFillColor(UIColor.white.cgColor)
        context.fill(bounds)
        
        context.setStrokeColor(UIColor.lightGray.cgColor)
        context.setLineWidth(0.5)
        
        let visibleXRange = Int((offset.x / (cellSize * scale)) - 1)...Int(((offset.x + bounds.width) / (cellSize * scale)) + 1)
        let visibleYRange = Int((offset.y / (cellSize * scale)) - 1)...Int(((offset.y + bounds.height) / (cellSize * scale)) + 1)
        
        for point in cells {
            if visibleXRange.contains(point.x) && visibleYRange.contains(point.y) {
                let rect = CGRect(
                    x: CGFloat(point.x) * cellSize * scale - offset.x,
                    y: CGFloat(point.y) * cellSize * scale - offset.y,
                    width: cellSize * scale,
                    height: cellSize * scale
                )
                context.setFillColor(UIColor.black.cgColor)
                context.fill(rect)
            }
        }
    }
    
    @objc func handlePan(_ gesture: UIPanGestureRecognizer) {
        let translation = gesture.translation(in: self)
        if gesture.state == .changed {
            offset.x -= translation.x
            offset.y -= translation.y
            gesture.setTranslation(.zero, in: self)
            setNeedsDisplay()
        }
    }
    
    @objc func handlePinch(_ gesture: UIPinchGestureRecognizer) {
        if gesture.state == .changed {
            scale *= gesture.scale
            scale = max(0.1, min(10.0, scale))
            gesture.scale = 1.0
            setNeedsDisplay()
        }
    }
    
    @objc func handleTap(_ gesture: UITapGestureRecognizer) {
        let location = gesture.location(in: self)
        let x = Int((location.x + offset.x) / (cellSize * scale))
        let y = Int((location.y + offset.y) / (cellSize * scale))
        onToggleCell?(Point(x: x, y: y))
        setNeedsDisplay()
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

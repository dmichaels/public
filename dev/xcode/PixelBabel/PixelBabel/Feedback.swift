import SwiftUI
import AudioToolbox
import CoreHaptics
import AVFoundation

struct Feedback
{
    private var _settings: AppSettings
    private var _hapticEngine: CHHapticEngine?

    init(_ settings: AppSettings) {
        self._settings = settings
        // self._prepareHaptics() // xyzzy
    }
    
    mutating func _prepareHaptics() {
        print("PREP-HAPTICS")
        do {
            print("PREP-HAPTICS-A")
            self._hapticEngine = try CHHapticEngine()
            print("PREP-HAPTICS-B")
            try self._hapticEngine?.start()
            print("PREP-HAPTICS-C")
        } catch {
            print("Haptics not supported.")
        }
    }

    func playClickSound() {
        do {
            let session = AVAudioSession.sharedInstance()
            try session.setCategory(.playback, mode: .default, options: .mixWithOthers)
            try session.setActive(true)
            AudioServicesPlaySystemSound(1104)
        } catch {
            print("Error setting audio session: \(error)")
        }
    }
    
    func triggerHaptic() {
        if (self._settings.soundEnabled) {
            self.playClickSound()
        }
        if (self._settings.hapticEnabled) {
            let sharpTap = CHHapticEvent(eventType: .hapticTransient, parameters: [], relativeTime: 0)
            do {
                let pattern = try CHHapticPattern(events: [sharpTap], parameters: [])
                let player = try self._hapticEngine?.makePlayer(with: pattern)
                try player?.start(atTime: 0)
            } catch {
                print("Failed to play haptic.")
            }
        }
    }
}

import SwiftUI
import AudioToolbox
import CoreHaptics
import AVFoundation

struct SettingsView: View {
    @EnvironmentObject var settings: AppSettings
    @Binding var showSettings: Bool
    @State private var selectedMode: ColorMode = .color

    var body: some View {
        VStack(spacing: 2) {

            Spacer()
            HStack {
                Text("Color Mode")
                    .bold()
                    .lineLimit(1)
                    //.frame(width: 100, alignment: .leading)
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .padding(.leading)
                Spacer()
                Picker("Color Mode", selection: $selectedMode) {
                    ForEach(ColorMode.allCases) { mode in
                        Text(mode.rawValue)
                            .lineLimit(1)
                            .truncationMode(.tail)
                            .tag(mode)
                    }
                }
                .pickerStyle(MenuPickerStyle())
                .frame(width: 200, alignment: .trailing)
                .padding(.trailing)
                .lineLimit(1)
                .onChange(of: selectedMode) { newMode in
                    settings.colorMode = newMode
                }
            }
            
            HStack {
                Text("Sounds")
                    .bold()
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .padding(.leading)
                Toggle("", isOn: $settings.soundEnabled)
                    .labelsHidden()
                    .padding(.trailing, 30)
            }.padding(.top, 2)
            
            HStack {
                Text("Haptics")
                    .bold()
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .padding(.leading)
                Toggle("", isOn: $settings.hapticEnabled)
                    .labelsHidden()
                    .padding(.trailing, 30)
            }.padding(.top, 5)

            Divider()
                .frame(height: 3)
                .background(Color.gray.opacity(0.3))
                .padding(.horizontal)
                // .padding(.vertical, 20)
                .padding(.top, 20)
                .padding(.bottom, 10)
            HStack {
                Text("Pixel Size")
                    .bold()
                    .frame(width: 100, alignment: .leading)
                    .padding(.leading)
                Spacer()
                Text("\(settings.pixelSize)")
                    .monospacedDigit()
                    .frame(width: 50, alignment: .trailing)
                    .padding(.trailing)
            }
            Slider(
                value: Binding(
                    get: { Double(settings.pixelSize) },
                    set: { settings.pixelSize = Int($0) }
                ),
                in: 1...50,
                step: 1
            )
            .padding(.horizontal)
            .padding(.top, 0)
            .onChange(of: settings.pixelSize) { newValue in
                settings.pixelSize = newValue
            }

            Spacer()
            Spacer()
            Spacer()
        }
        .frame(maxWidth: .infinity, maxHeight: .infinity)
        .background(Color.white)
        .ignoresSafeArea()
        .gesture(
            DragGesture()
                .onEnded { value in
                    if value.translation.width > 100 {
                        withAnimation(.easeInOut(duration: 0.3)) {
                            showSettings = false
                        }
                    }
                }
        )
        .onAppear {
            selectedMode = settings.colorMode
        }
    }
}

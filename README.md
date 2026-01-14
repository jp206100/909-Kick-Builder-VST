# 909 Kick Builder VST

A professional-grade VST3 instrument plugin for designing kick drums, combining classic TR-909 samples with synthesis capabilities and advanced transient shaping.

## Features

### Current Status: ✅ Initial Structure Complete

- **Dual Sound Engine**: Sample-based and 909-style synthesis
- **Professional Processing**: Transient shaping, tonal control, Clip to Zero, saturation
- **Modern UI**: Futuristic Apple-inspired interface (2035 aesthetic)
- **Real-time Visualization**: Waveform display, spectrum analyzer, ADSR envelope display
- **Preset Management**: Save and recall custom kick sounds
- **Universal Binary**: Supports both Intel and Apple Silicon Macs

## Quick Start

### For Users

1. **Download the latest release** from the Releases page
2. **Copy** `909 Kick Builder.vst3` to `~/Library/Audio/Plug-Ins/VST3/`
3. **Launch** your DAW (Ableton Live, Logic Pro, FL Studio, etc.)
4. **Load** the plugin on a MIDI track
5. **Trigger** kicks with any MIDI note

### For Developers

See [docs/BUILD_INSTRUCTIONS.md](docs/BUILD_INSTRUCTIONS.md) for detailed build instructions.

**Quick build:**
```bash
# Generate Xcode project
cmake -B Builds/MacOSX -G Xcode \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0

# Build
cmake --build Builds/MacOSX --config Release

# Install
cp -r "Builds/MacOSX/909KickBuilder_artefacts/Release/VST3/909 Kick Builder.vst3" \
  ~/Library/Audio/Plug-Ins/VST3/
```

## System Requirements

- **macOS**: 11.0 (Big Sur) or later
- **Architectures**: Intel x86_64 and Apple Silicon arm64 (Universal Binary)
- **Plugin Format**: VST3
- **DAWs**: Logic Pro, Ableton Live, FL Studio, Bitwig, Reaper, and more

## Usage

### Basic Workflow

1. **Select a Sound Source**:
   - **Sample Mode**: Choose from 5 curated 909 kick samples
   - **Synthesis Mode**: Design from scratch using 909-style synthesis

2. **Shape the Sound**:
   - **ADSR Envelope**: Control attack, decay, sustain, and release
   - **Transient Shaper**: Enhance or reduce attack and sustain
   - **Tonal Controls**: Adjust body tone and click character

3. **Add Punch**:
   - **Clip to Zero**: Professional limiting technique for maximum density
   - **Saturation**: Add harmonic warmth
   - **Sub Generator**: Reinforce low-end

4. **Save as Preset**: Store your custom kicks for future use

### Parameter Guide

#### ADSR Envelope
- **Attack** (0-50ms): How quickly the kick reaches full volume
- **Decay** (10-2000ms): How long the kick body lasts
- **Sustain** (0-100%): Body level after initial decay (usually 0% for kicks)
- **Release** (10-500ms): Tail fade-out time

#### Transient Shaping
- **Attack** (-12dB to +12dB): Enhance or reduce the initial transient
- **Sustain** (-12dB to +12dB): Control body level independently
- **Punch** (0-100%): Combined transient enhancement

#### Clip to Zero
- **Enabled**: Turn clipping on/off
- **Mode**: Hard (digital) or Soft (saturation-style) clipping
- **Amount** (0-100%): How much clipping is applied
- **Auto Gain**: Automatically normalize after clipping

## Architecture

```
909KickBuilder/
├── Source/
│   ├── Audio/           # Sound generation (Sample & Synth engines)
│   ├── Processing/      # Effects chain (Transient, Clip, Saturation, etc.)
│   ├── UI/              # User interface components
│   ├── Presets/         # Preset management
│   └── Utils/           # Helper functions and constants
├── Assets/
│   └── Samples/         # 909 kick drum samples
└── docs/                # Documentation
```

## Development Roadmap

### Phase 1: Core Audio Engine ✅ (Current)
- [x] Basic plugin structure
- [x] Sample engine
- [x] Synthesis engine
- [x] ADSR envelope
- [x] MIDI handling

### Phase 2: Processing Chain (In Progress)
- [x] Transient shaper (basic)
- [x] Tonal shaping (filters)
- [x] Clip to Zero
- [x] Saturation
- [x] Sub generator
- [ ] Phase rotation (advanced)

### Phase 3: User Interface
- [x] Basic layout
- [x] Parameter controls
- [x] Waveform display
- [ ] Spectrum analyzer (FFT)
- [ ] ADSR envelope display
- [ ] Custom knobs with modern styling
- [ ] Meters and visualizations

### Phase 4: Presets & Polish
- [ ] Preset manager implementation
- [ ] 20-30 factory presets
- [ ] User preset save/load
- [ ] Performance optimization
- [ ] Final testing

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

This project is licensed under the **GPL-3.0 License** (JUCE GPL).

For commercial use with a closed-source license, you must obtain a commercial JUCE license from [juce.com](https://juce.com).

## Credits

- **JUCE Framework**: [juce.com](https://juce.com)
- **909 Samples**: Placeholder samples included (replace with licensed samples)
- **Architecture**: Inspired by classic drum machines and modern plugin design

## Support

- **Issues**: Report bugs on [GitHub Issues](https://github.com/yourusername/909-Kick-Builder-VST/issues)
- **Discussions**: Ask questions in [Discussions](https://github.com/yourusername/909-Kick-Builder-VST/discussions)
- **Documentation**: Full spec in [docs/](docs/)

## Changelog

### Version 0.1.0 (2026-01-14)
- Initial project structure
- Basic audio engine (sample & synthesis)
- ADSR envelope
- Processing chain stubs
- Basic UI layout

---

**Built with ❤️ for electronic music producers**

*909 Kick Builder - Professional kick drum design for the modern DAW*

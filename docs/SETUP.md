# Development Setup

## First Time Setup

After cloning this repository, you need to clone the JUCE framework:

```bash
# Clone JUCE (v7.0.12) into the project root
git clone --depth 1 --branch 7.0.12 https://github.com/juce-framework/JUCE.git
```

Alternatively, you can use a different JUCE version by cloning the main branch:

```bash
git clone https://github.com/juce-framework/JUCE.git
```

## Verify Setup

Your directory structure should look like this:

```
909-Kick-Builder-VST/
├── JUCE/                  ← JUCE framework (cloned separately)
├── Source/                ← Plugin source code
├── Assets/                ← Samples and presets
├── CMakeLists.txt         ← Build configuration
└── README.md
```

## Next Steps

See [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) for build instructions.

cmake_minimum_required(VERSION 3.22)

# Set macOS deployment target BEFORE project() to avoid SDK version being used
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version" FORCE)

project(909_KICK_BUILDER VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE subdirectory
add_subdirectory(JUCE)

# Create binary data target for samples
juce_add_binary_data(KickBuilderAssets
    SOURCES
        Assets/Samples/909_classic.wav
        Assets/Samples/909_deep.wav
        Assets/Samples/909_punchy.wav
        Assets/Samples/909_vintage.wav
        Assets/Samples/909_modern.wav
)

# Create the plugin target
juce_add_plugin(909KickBuilder
    COMPANY_NAME "OpenSource909"
    PLUGIN_MANUFACTURER_CODE Os09
    PLUGIN_CODE 9Kb1
    FORMATS VST3 Standalone
    PRODUCT_NAME "909 Kick Builder"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
    VST3_CATEGORIES "Instrument" "Synth"
    AU_MAIN_TYPE kAudioUnitType_MusicDevice
)

# Generate JuceHeader.h and add sources
juce_generate_juce_header(909KickBuilder)

target_sources(909KickBuilder
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
        Source/Audio/SampleEngine.cpp
        Source/Audio/SynthEngine.cpp
        Source/Audio/ADSREnvelope.cpp
        Source/Audio/MIDIHandler.cpp
        Source/Processing/TransientShaper.cpp
        Source/Processing/TonalShaper.cpp
        Source/Processing/ClipToZero.cpp
        Source/Processing/Saturation.cpp
        Source/Processing/SubGenerator.cpp
        Source/Processing/PhaseRotator.cpp
        Source/UI/FuturisticLookAndFeel.cpp
        Source/UI/WaveformDisplay.cpp
        Source/UI/SpectrumAnalyzer.cpp
        Source/UI/ADSRDisplay.cpp
        Source/UI/VintageKnob.cpp
        Source/UI/LevelMeter.cpp
        Source/UI/PresetBrowser.cpp
        Source/Presets/PresetManager.cpp
        Source/Utils/Constants.h
        Source/Utils/DSPHelpers.h
)

# Compiler definitions
target_compile_definitions(909KickBuilder
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_USE_CUSTOM_PLUGIN_STANDALONE_APP=0
        JUCE_MODAL_LOOPS_PERMITTED=1
)

# Link JUCE modules
target_link_libraries(909KickBuilder
    PRIVATE
        KickBuilderAssets
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# macOS specific settings
if(APPLE)
    set_target_properties(909KickBuilder PROPERTIES
        XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET "11.0"
        XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ""
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
    )
endif()

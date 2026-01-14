/*
  ==============================================================================

    Constants.h
    Global constants and configuration for 909 Kick Builder

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Constants
{
    // Plugin info
    constexpr const char* PLUGIN_NAME = "909 Kick Builder";
    constexpr const char* PLUGIN_VERSION = "1.0.0";
    constexpr const char* MANUFACTURER = "OpenSource909";

    // Audio settings
    constexpr int MAX_POLYPHONY = 1;  // Monophonic
    constexpr int MAX_MIDI_CHANNEL = 16;

    // Sample rates supported
    constexpr double MIN_SAMPLE_RATE = 44100.0;
    constexpr double MAX_SAMPLE_RATE = 96000.0;

    // Buffer sizes
    constexpr int MIN_BLOCK_SIZE = 32;
    constexpr int MAX_BLOCK_SIZE = 8192;

    // Parameter ranges
    namespace Params
    {
        // ADSR Envelope
        constexpr float ATTACK_MIN = 0.0f;      // ms
        constexpr float ATTACK_MAX = 50.0f;     // ms
        constexpr float ATTACK_DEFAULT = 1.0f;  // ms

        constexpr float DECAY_MIN = 10.0f;      // ms
        constexpr float DECAY_MAX = 2000.0f;    // ms
        constexpr float DECAY_DEFAULT = 300.0f; // ms

        constexpr float SUSTAIN_MIN = 0.0f;     // level (0-1)
        constexpr float SUSTAIN_MAX = 1.0f;
        constexpr float SUSTAIN_DEFAULT = 0.0f; // Kick drums typically have 0 sustain

        constexpr float RELEASE_MIN = 10.0f;    // ms
        constexpr float RELEASE_MAX = 500.0f;   // ms
        constexpr float RELEASE_DEFAULT = 50.0f;// ms

        // Transient Shaper
        constexpr float TRANSIENT_ATTACK_MIN = -12.0f;    // dB
        constexpr float TRANSIENT_ATTACK_MAX = 12.0f;     // dB
        constexpr float TRANSIENT_ATTACK_DEFAULT = 0.0f;  // dB

        constexpr float TRANSIENT_SUSTAIN_MIN = -12.0f;   // dB
        constexpr float TRANSIENT_SUSTAIN_MAX = 12.0f;    // dB
        constexpr float TRANSIENT_SUSTAIN_DEFAULT = 0.0f; // dB

        constexpr float PUNCH_MIN = 0.0f;       // %
        constexpr float PUNCH_MAX = 100.0f;     // %
        constexpr float PUNCH_DEFAULT = 0.0f;   // %

        // Tonal Shaping
        constexpr float PITCH_MIN = -12.0f;     // semitones
        constexpr float PITCH_MAX = 12.0f;      // semitones
        constexpr float PITCH_DEFAULT = 0.0f;   // semitones

        constexpr float BODY_TONE_MIN = 50.0f;  // Hz
        constexpr float BODY_TONE_MAX = 500.0f; // Hz
        constexpr float BODY_TONE_DEFAULT = 200.0f; // Hz

        constexpr float CLICK_TONE_MIN = 1000.0f;   // Hz
        constexpr float CLICK_TONE_MAX = 8000.0f;   // Hz
        constexpr float CLICK_TONE_DEFAULT = 3000.0f; // Hz

        // Clip to Zero
        constexpr float CLIP_AMOUNT_MIN = 0.0f;     // %
        constexpr float CLIP_AMOUNT_MAX = 100.0f;   // %
        constexpr float CLIP_AMOUNT_DEFAULT = 0.0f; // %

        // Saturation
        constexpr float SATURATION_DRIVE_MIN = 0.0f;    // %
        constexpr float SATURATION_DRIVE_MAX = 100.0f;  // %
        constexpr float SATURATION_DRIVE_DEFAULT = 0.0f;// %

        constexpr float SATURATION_MIX_MIN = 0.0f;      // %
        constexpr float SATURATION_MIX_MAX = 100.0f;    // %
        constexpr float SATURATION_MIX_DEFAULT = 100.0f;// %

        // Sub Generator
        constexpr float SUB_LEVEL_MIN = 0.0f;       // %
        constexpr float SUB_LEVEL_MAX = 100.0f;     // %
        constexpr float SUB_LEVEL_DEFAULT = 0.0f;   // %

        constexpr float SUB_DECAY_MIN = 10.0f;      // ms
        constexpr float SUB_DECAY_MAX = 1000.0f;    // ms
        constexpr float SUB_DECAY_DEFAULT = 400.0f; // ms

        // Phase
        constexpr float PHASE_MIN = 0.0f;       // degrees
        constexpr float PHASE_MAX = 360.0f;     // degrees
        constexpr float PHASE_DEFAULT = 0.0f;   // degrees

        // Output
        constexpr float OUTPUT_GAIN_MIN = -24.0f;   // dB
        constexpr float OUTPUT_GAIN_MAX = 12.0f;    // dB
        constexpr float OUTPUT_GAIN_DEFAULT = 0.0f; // dB

        // Synth engine specific
        constexpr float SYNTH_BODY_PITCH_MIN = 30.0f;   // Hz
        constexpr float SYNTH_BODY_PITCH_MAX = 80.0f;   // Hz
        constexpr float SYNTH_BODY_PITCH_DEFAULT = 50.0f; // Hz

        constexpr float SYNTH_CLICK_PITCH_MIN = 1000.0f;    // Hz
        constexpr float SYNTH_CLICK_PITCH_MAX = 8000.0f;    // Hz
        constexpr float SYNTH_CLICK_PITCH_DEFAULT = 3000.0f;// Hz

        constexpr float SYNTH_NOISE_AMOUNT_MIN = 0.0f;      // %
        constexpr float SYNTH_NOISE_AMOUNT_MAX = 100.0f;    // %
        constexpr float SYNTH_NOISE_AMOUNT_DEFAULT = 30.0f; // %

        constexpr float SYNTH_PITCH_ENV_DEPTH_MIN = 0.0f;   // octaves
        constexpr float SYNTH_PITCH_ENV_DEPTH_MAX = 4.0f;   // octaves
        constexpr float SYNTH_PITCH_ENV_DEPTH_DEFAULT = 2.0f; // octaves

        constexpr float SYNTH_PITCH_ENV_TIME_MIN = 1.0f;    // ms
        constexpr float SYNTH_PITCH_ENV_TIME_MAX = 500.0f;  // ms
        constexpr float SYNTH_PITCH_ENV_TIME_DEFAULT = 50.0f; // ms
    }

    // UI Colors (Futuristic Apple aesthetic)
    namespace Colors
    {
        const juce::Colour BACKGROUND { 0xFF0A0A0A };           // Deep black
        const juce::Colour PANEL { 0x40FFFFFF };                // Frosted glass
        const juce::Colour ACCENT_BLUE { 0xFF0A84FF };          // Electric blue
        const juce::Colour ACCENT_GREEN { 0xFF32D74B };         // Neon green
        const juce::Colour TEXT_PRIMARY { 0xFFFFFFFF };         // White
        const juce::Colour TEXT_SECONDARY { 0xFFE5E5E7 };       // Light gray
        const juce::Colour KNOB_TRACK { 0x40FFFFFF };           // Knob track
        const juce::Colour KNOB_FILL { 0xFF0A84FF };            // Knob fill (blue)
        const juce::Colour METER_GREEN { 0xFF32D74B };          // Meter green
        const juce::Colour METER_YELLOW { 0xFFFFD60A };         // Meter yellow
        const juce::Colour METER_RED { 0xFFFF453A };            // Meter red
        const juce::Colour WAVEFORM { 0xFF0A84FF };             // Waveform color
        const juce::Colour SPECTRUM_START { 0xFF0A84FF };       // Spectrum gradient start
        const juce::Colour SPECTRUM_END { 0xFF32D74B };         // Spectrum gradient end
    }

    // UI Dimensions
    namespace UI
    {
        constexpr int PLUGIN_WIDTH = 900;
        constexpr int PLUGIN_HEIGHT = 700;

        constexpr int KNOB_SIZE = 70;
        constexpr int SLIDER_WIDTH = 50;
        constexpr int SLIDER_HEIGHT = 200;

        constexpr int WAVEFORM_HEIGHT = 200;
        constexpr int SPECTRUM_HEIGHT = 100;
        constexpr int ADSR_DISPLAY_HEIGHT = 120;

        constexpr int MARGIN = 16;
        constexpr int PADDING = 8;
        constexpr int CORNER_RADIUS = 8;

        constexpr float KNOB_LINE_THICKNESS = 3.0f;
        constexpr float METER_SMOOTHING = 0.3f;

        constexpr int UI_REFRESH_RATE_HZ = 60;
    }

    // DSP Constants
    namespace DSP
    {
        constexpr float PI = 3.14159265358979323846f;
        constexpr float TWO_PI = 6.28318530717958647692f;

        constexpr float SMOOTHING_TIME_MS = 50.0f;  // Parameter smoothing
        constexpr float ENVELOPE_SMOOTHING_MS = 5.0f; // Envelope smoothing

        constexpr float MIN_FREQUENCY = 20.0f;  // Hz
        constexpr float MAX_FREQUENCY = 20000.0f; // Hz

        constexpr float DB_FLOOR = -100.0f;  // Minimum dB value
        constexpr float EPSILON = 1e-9f;     // Small value to prevent division by zero

        // FFT settings for spectrum analyzer
        constexpr int FFT_ORDER = 11;  // 2^11 = 2048 samples
        constexpr int FFT_SIZE = 1 << FFT_ORDER;
    }

    // Sample IDs
    enum class SampleID
    {
        Classic = 0,
        Deep,
        Punchy,
        Vintage,
        Modern,
        NumSamples
    };

    // Engine mode
    enum class EngineMode
    {
        Sample = 0,
        Synthesis
    };

    // Saturation character
    enum class SaturationCharacter
    {
        Clean = 0,
        Warm,
        Aggressive
    };

    // Clip mode
    enum class ClipMode
    {
        Hard = 0,
        Soft
    };
}

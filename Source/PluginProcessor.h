/*
  ==============================================================================

    PluginProcessor.h
    Main audio processor for 909 Kick Builder VST

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Utils/Constants.h"
#include "Utils/DSPHelpers.h"

// Forward declarations
class SampleEngine;
class SynthEngine;
class ADSREnvelope;
class TransientShaper;
class TonalShaper;
class ClipToZero;
class Saturation;
class SubGenerator;
class PhaseRotator;

//==============================================================================
/**
 * Main audio processor for 909 Kick Builder
 */
class NineZeroNineAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    NineZeroNineAudioProcessor();
    ~NineZeroNineAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Public API for UI
    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }

    // Get latest waveform for display
    void getLatestWaveform(juce::AudioBuffer<float>& bufferToFill);

    // Get latest audio for spectrum analysis
    void getLatestAudioForFFT(juce::AudioBuffer<float>& bufferToFill);

    // Get current output level (for metering)
    float getCurrentOutputLevel() const { return currentOutputLevel.load(); }

    // Get clip to zero gain reduction (for metering)
    float getClipGainReduction() const { return clipGainReduction.load(); }

private:
    //==============================================================================
    // Audio Processor Value Tree State (parameter management)
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // Audio engine components (will be created in .cpp)
    std::unique_ptr<SampleEngine> sampleEngine;
    std::unique_ptr<SynthEngine> synthEngine;
    std::unique_ptr<ADSREnvelope> adsrEnvelope;

    // Processing chain
    std::unique_ptr<TransientShaper> transientShaper;
    std::unique_ptr<TonalShaper> tonalShaper;
    std::unique_ptr<Saturation> saturation;
    std::unique_ptr<SubGenerator> subGenerator;
    std::unique_ptr<ClipToZero> clipToZero;
    std::unique_ptr<PhaseRotator> phaseRotator;

    // Audio buffers for visualization
    juce::AudioBuffer<float> waveformBuffer;
    juce::AudioBuffer<float> fftBuffer;
    juce::CriticalSection waveformLock;
    juce::CriticalSection fftLock;

    // Metering
    std::atomic<float> currentOutputLevel { 0.0f };
    std::atomic<float> clipGainReduction { 0.0f };

    // State
    std::atomic<bool> isNoteOn { false };
    std::atomic<int> currentVelocity { 127 };

    double currentSampleRate = 44100.0;

    // Helper methods
    void triggerKick(int velocity);
    void updateParameters();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NineZeroNineAudioProcessor)
};

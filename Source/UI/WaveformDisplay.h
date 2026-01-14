/*
  ==============================================================================

    WaveformDisplay.h
    Real-time waveform visualizer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class WaveformDisplay : public juce::Component
{
public:
    WaveformDisplay();
    ~WaveformDisplay() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setBuffer(const juce::AudioBuffer<float>& buffer);

private:
    juce::AudioBuffer<float> displayBuffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};

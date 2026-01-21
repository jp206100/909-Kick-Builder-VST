/*
  ==============================================================================

    WaveformDisplay.h
    Vintage CRT oscilloscope-style waveform visualizer

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

    // Clip visualization
    void setClipEnabled(bool enabled);
    void setClipAmount(float amount);
    void setClipGainReduction(float gainReductionDb);

private:
    void drawCRTBezel(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawGrid(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawWaveformWithGlow(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawClipThreshold(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawScanlines(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawVignette(juce::Graphics& g, juce::Rectangle<float> bounds);

    juce::AudioBuffer<float> displayBuffer;

    // Clip visualization state
    bool clipEnabled = false;
    float clipAmount = 0.0f;
    float clipGainReduction = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};

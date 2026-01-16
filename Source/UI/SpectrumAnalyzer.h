/*
  ==============================================================================

    SpectrumAnalyzer.h
    Vintage VU meter-style spectrum analyzer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SpectrumAnalyzer : public juce::Component
{
public:
    SpectrumAnalyzer();
    ~SpectrumAnalyzer() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Update spectrum data from FFT magnitudes
    void updateSpectrum(const float* magnitudes, int numMagnitudes);

private:
    void drawMeterBezel(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawFrequencyBands(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawLEDSegments(juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour colour);
    void drawScaleMarkings(juce::Graphics& g, juce::Rectangle<float> bounds);

    static constexpr int numBands = 16;
    float bandLevels[numBands];
    float peakHoldLevels[numBands];
    int peakHoldTimers[numBands];

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzer)
};

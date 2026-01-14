/*
  ==============================================================================

    SpectrumAnalyzer.h
    FFT spectrum analyzer

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

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyzer)
};

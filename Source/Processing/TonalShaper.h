/*
  ==============================================================================

    TonalShaper.h
    Body/Click tone filtering

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class TonalShaper
{
public:
    TonalShaper();
    ~TonalShaper();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setBodyTone(float frequency);
    void setClickTone(float frequency);

private:
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> lowPassFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highPassFilter;

    double sampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TonalShaper)
};

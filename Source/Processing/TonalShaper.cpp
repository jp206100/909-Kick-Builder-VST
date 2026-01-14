/*
  ==============================================================================

    TonalShaper.cpp

  ==============================================================================
*/

#include "TonalShaper.h"

TonalShaper::TonalShaper() {}
TonalShaper::~TonalShaper() {}

void TonalShaper::prepare(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;

    lowPassFilter.prepare(spec);
    highPassFilter.prepare(spec);

    setBodyTone(200.0f);
    setClickTone(3000.0f);
}

void TonalShaper::process(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    // Simplified: bypass for now
    // TODO: Implement proper filtering
}

void TonalShaper::reset()
{
    lowPassFilter.reset();
    highPassFilter.reset();
}

void TonalShaper::setBodyTone(float frequency)
{
    *lowPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, frequency);
}

void TonalShaper::setClickTone(float frequency)
{
    *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, frequency);
}

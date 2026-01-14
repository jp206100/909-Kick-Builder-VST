/*
  ==============================================================================

    SubGenerator.cpp

  ==============================================================================
*/

#include "SubGenerator.h"
#include "../Utils/Constants.h"

SubGenerator::SubGenerator() {}
SubGenerator::~SubGenerator() {}

void SubGenerator::prepare(double newSampleRate, int samplesPerBlock)
{
    sampleRate = newSampleRate;
}

void SubGenerator::trigger(float fundamentalFrequency)
{
    subFrequency = fundamentalFrequency / 2.0f; // One octave below
    phase = 0.0f;
    samplePosition = 0;
    isActive = true;
}

void SubGenerator::process(juce::AudioBuffer<float>& buffer)
{
    if (!isActive || level <= 0.0f)
        return;

    const float decayRate = 1000.0f / decay;

    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        const float t = samplePosition / static_cast<float>(sampleRate);
        const float env = std::exp(-decayRate * t);

        if (env < 0.001f)
        {
            isActive = false;
            break;
        }

        const float phaseDelta = subFrequency / static_cast<float>(sampleRate);
        phase += phaseDelta;
        if (phase >= 1.0f)
            phase -= 1.0f;

        const float subSample = std::sin(phase * Constants::DSP::TWO_PI) * env * level;

        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            buffer.addSample(ch, i, subSample);
        }

        ++samplePosition;
    }
}

void SubGenerator::reset()
{
    isActive = false;
    phase = 0.0f;
    samplePosition = 0;
}

void SubGenerator::setLevel(float amount)
{
    level = juce::jlimit(0.0f, 1.0f, amount / 100.0f);
}

void SubGenerator::setDecay(float milliseconds)
{
    decay = juce::jlimit(10.0f, 1000.0f, milliseconds);
}

/*
  ==============================================================================

    Saturation.cpp

  ==============================================================================
*/

#include "Saturation.h"
#include "../Utils/DSPHelpers.h"

Saturation::Saturation() {}
Saturation::~Saturation() {}

void Saturation::prepare(double sampleRate, int samplesPerBlock) {}
void Saturation::reset() {}

void Saturation::process(juce::AudioBuffer<float>& buffer)
{
    if (drive <= 0.0f)
        return;

    // Simplified saturation: just apply soft clipping
    const float driveGain = 1.0f + (drive * 2.0f);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        float* data = buffer.getWritePointer(ch);

        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            const float dry = data[i];
            const float driven = data[i] * driveGain;
            const float wet = DSPHelpers::softClip(driven) / driveGain;

            data[i] = dry + mix * (wet - dry);
        }
    }
}

void Saturation::setDrive(float amount)
{
    drive = juce::jlimit(0.0f, 1.0f, amount / 100.0f);
}

void Saturation::setCharacter(Constants::SaturationCharacter newCharacter)
{
    character = newCharacter;
}

void Saturation::setMix(float amount)
{
    mix = juce::jlimit(0.0f, 1.0f, amount / 100.0f);
}

/*
  ==============================================================================

    TransientShaper.cpp

  ==============================================================================
*/

#include "TransientShaper.h"

TransientShaper::TransientShaper() {}
TransientShaper::~TransientShaper() {}

void TransientShaper::prepare(double sampleRate, int samplesPerBlock) {}

void TransientShaper::process(juce::AudioBuffer<float>& buffer)
{
    // Simplified: Just apply overall gain for now
    // TODO: Implement proper envelope-based attack/sustain shaping
    const float totalGain = attackGain * (1.0f + punchAmount);
    buffer.applyGain(totalGain);
}

void TransientShaper::reset() {}

void TransientShaper::setAttackGain(float dB)
{
    attackGain = juce::Decibels::decibelsToGain(dB);
}

void TransientShaper::setSustainGain(float dB)
{
    sustainGain = juce::Decibels::decibelsToGain(dB);
}

void TransientShaper::setPunch(float amount)
{
    punchAmount = amount / 100.0f;
}

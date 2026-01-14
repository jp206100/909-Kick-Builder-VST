/*
  ==============================================================================

    PhaseRotator.h
    Phase rotation processor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class PhaseRotator
{
public:
    PhaseRotator();
    ~PhaseRotator();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setPhase(float degrees);

private:
    float phaseDegrees = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PhaseRotator)
};

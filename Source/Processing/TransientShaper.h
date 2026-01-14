/*
  ==============================================================================

    TransientShaper.h
    Attack/Sustain transient shaping processor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class TransientShaper
{
public:
    TransientShaper();
    ~TransientShaper();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setAttackGain(float dB);
    void setSustainGain(float dB);
    void setPunch(float amount);

private:
    float attackGain = 1.0f;
    float sustainGain = 1.0f;
    float punchAmount = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransientShaper)
};

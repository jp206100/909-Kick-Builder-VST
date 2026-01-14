/*
  ==============================================================================

    ADSREnvelope.h
    ADSR envelope generator

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ADSREnvelope
{
public:
    ADSREnvelope();
    ~ADSREnvelope();

    void prepare(double sampleRate);
    void trigger();
    void release();
    float getNextValue();
    bool isActive() const;

    void setAttack(float milliseconds);
    void setDecay(float milliseconds);
    void setSustain(float level);
    void setRelease(float milliseconds);

private:
    juce::ADSR adsr;
    juce::ADSR::Parameters params;
    double sampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSREnvelope)
};

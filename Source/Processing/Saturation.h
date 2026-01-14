/*
  ==============================================================================

    Saturation.h
    Harmonic saturation processor

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/Constants.h"

class Saturation
{
public:
    Saturation();
    ~Saturation();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setDrive(float amount);
    void setCharacter(Constants::SaturationCharacter character);
    void setMix(float amount);

private:
    float drive = 0.0f;
    float mix = 1.0f;
    Constants::SaturationCharacter character = Constants::SaturationCharacter::Clean;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Saturation)
};

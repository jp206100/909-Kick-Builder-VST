/*
  ==============================================================================

    ClipToZero.h
    Clip to Zero processor for punch and density

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/Constants.h"

class ClipToZero
{
public:
    ClipToZero();
    ~ClipToZero();

    void prepare(double sampleRate, int samplesPerBlock);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setEnabled(bool enabled);
    void setMode(Constants::ClipMode mode);
    void setAmount(float amount);
    void setAutoGain(bool enabled);

    float getGainReduction() const { return gainReduction; }

private:
    bool isEnabled = false;
    Constants::ClipMode clipMode = Constants::ClipMode::Soft;
    float clipAmount = 0.0f;
    bool autoGainEnabled = true;
    float gainReduction = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ClipToZero)
};

/*
  ==============================================================================

    ADSRDisplay.h
    ADSR envelope visualizer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ADSRDisplay : public juce::Component
{
public:
    ADSRDisplay();
    ~ADSRDisplay() override;

    void paint(juce::Graphics& g) override;

    // Update ADSR parameters for visualization
    void setAttack(float attackMs);
    void setDecay(float decayMs);
    void setSustain(float sustainLevel);
    void setRelease(float releaseMs);

private:
    void drawEnvelope(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawGrid(juce::Graphics& g, juce::Rectangle<float> bounds);

    float attack = 10.0f;   // ms
    float decay = 200.0f;   // ms
    float sustain = 0.5f;   // 0-1
    float release = 500.0f; // ms

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRDisplay)
};

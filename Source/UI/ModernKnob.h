/*
  ==============================================================================

    ModernKnob.h
    Custom rotary knob component

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ModernKnob : public juce::Component
{
public:
    ModernKnob();
    ~ModernKnob() override;

    void paint(juce::Graphics& g) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModernKnob)
};

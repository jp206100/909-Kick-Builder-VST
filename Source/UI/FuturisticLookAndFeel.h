/*
  ==============================================================================

    FuturisticLookAndFeel.h
    Custom LookAndFeel for futuristic Apple aesthetic

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/Constants.h"

class FuturisticLookAndFeel : public juce::LookAndFeel_V4
{
public:
    FuturisticLookAndFeel();
    ~FuturisticLookAndFeel() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FuturisticLookAndFeel)
};

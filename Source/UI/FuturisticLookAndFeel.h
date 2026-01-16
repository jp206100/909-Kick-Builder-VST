/*
  ==============================================================================

    FuturisticLookAndFeel.h
    Custom LookAndFeel for vintage TR-909 hardware aesthetic

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/Constants.h"

/**
 * VintageLookAndFeel - Custom styling for 1980s hardware aesthetic
 * Features:
 * - 3D beveled buttons with depth
 * - Engraved text labels
 * - Brushed metal backgrounds
 * - Roland-style combo boxes
 * - Hardware panel textures
 */
class VintageLookAndFeel : public juce::LookAndFeel_V4
{
public:
    VintageLookAndFeel();
    ~VintageLookAndFeel() override;

    // Button rendering with 3D hardware style
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                            const juce::Colour& backgroundColour,
                            bool shouldDrawButtonAsHighlighted,
                            bool shouldDrawButtonAsDown) override;

    // ComboBox with vintage dropdown styling
    void drawComboBox(juce::Graphics& g, int width, int height,
                     bool isButtonDown, int buttonX, int buttonY,
                     int buttonW, int buttonH, juce::ComboBox& box) override;

    // Toggle button with LED indicator
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                         bool shouldDrawButtonAsHighlighted,
                         bool shouldDrawButtonAsDown) override;

    // Label with engraved text effect
    void drawLabel(juce::Graphics& g, juce::Label& label) override;

private:
    void drawBeveledRectangle(juce::Graphics& g, juce::Rectangle<float> bounds,
                             bool isPressed, const juce::Colour& baseColour);
    void drawEngravedText(juce::Graphics& g, const juce::String& text,
                         juce::Rectangle<float> bounds, juce::Justification justification);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VintageLookAndFeel)
};

/*
  ==============================================================================

    ModernKnob.h
    Vintage hardware-style 3D skeuomorphic knob with realistic rendering

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/**
 * VintageKnob - Photorealistic 3D knob component inspired by 1980s hardware
 * Features:
 * - 3D rendering with depth, shadows, and highlights
 * - Textured grip with radial lines
 * - White position indicator pointer
 * - Smooth rotation animation
 * - Value display below knob
 */
class VintageKnob : public juce::Slider
{
public:
    VintageKnob();
    ~VintageKnob() override;

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

    // Set label text displayed above knob
    void setLabel(const juce::String& labelText);

    // Enable/disable value text display below knob
    void setShowValue(bool shouldShow);

private:
    void drawKnobBody(juce::Graphics& g, juce::Rectangle<float> bounds, float angle);
    void drawKnobPointer(juce::Graphics& g, juce::Rectangle<float> bounds, float angle);
    void drawKnobGripTexture(juce::Graphics& g, juce::Rectangle<float> bounds);
    void drawValueLabel(juce::Graphics& g, juce::Rectangle<float> bounds);

    juce::String labelText;
    bool showValue = true;
    juce::Point<float> lastMousePos;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VintageKnob)
};

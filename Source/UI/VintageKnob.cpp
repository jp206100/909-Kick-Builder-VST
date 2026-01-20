/*
  ==============================================================================

    VintageKnob.cpp
    Vintage hardware-style 3D skeuomorphic knob implementation

  ==============================================================================
*/

#include "VintageKnob.h"
#include "../Utils/Constants.h"

VintageKnob::VintageKnob()
{
    setSliderStyle(juce::Slider::RotaryVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    setRotaryParameters(juce::MathConstants<float>::pi * 1.2f,
                        juce::MathConstants<float>::pi * 2.8f,
                        true);
    setVelocityBasedMode(true);
    setVelocityModeParameters(0.5, 1, 0.09, false);
    setRange(0.0, 1.0, 0.01);
    setValue(0.5);
}

VintageKnob::~VintageKnob()
{
}

void VintageKnob::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Reserve space for label above and value below
    float labelHeight = 16.0f;
    float valueHeight = showValue ? 18.0f : 0.0f;
    float spacing = 4.0f;

    // Draw label above knob
    if (labelText.isNotEmpty())
    {
        auto labelBounds = bounds.removeFromTop(labelHeight);
        g.setColour(Constants::Colors::TEXT_STENCIL);
        g.setFont(juce::FontOptions("Arial", 11.0f, juce::Font::bold));
        g.drawText(labelText, labelBounds, juce::Justification::centred);
        bounds.removeFromTop(spacing);
    }

    // Reserve space for value at bottom
    auto valueBounds = bounds.removeFromBottom(valueHeight + spacing);

    // Calculate knob bounds (square, centered)
    float knobSize = juce::jmin(bounds.getWidth(), bounds.getHeight());
    auto knobBounds = bounds.withSizeKeepingCentre(knobSize, knobSize);

    // Calculate rotation angle based on value
    float angle = juce::jmap(static_cast<float>(getValue()),
                            static_cast<float>(getMinimum()),
                            static_cast<float>(getMaximum()),
                            getRotaryParameters().startAngleRadians,
                            getRotaryParameters().endAngleRadians);

    // Draw the knob components
    drawKnobBody(g, knobBounds, angle);
    drawKnobGripTexture(g, knobBounds);
    drawKnobPointer(g, knobBounds, angle);

    // Draw value text below
    if (showValue)
    {
        drawValueLabel(g, valueBounds);
    }
}

void VintageKnob::drawKnobBody(juce::Graphics& g, juce::Rectangle<float> bounds, float angle)
{
    (void)angle; // Unused - knob body doesn't rotate
    auto centre = bounds.getCentre();
    float radius = bounds.getWidth() * 0.42f;

    // Draw shadow below knob for depth
    {
        juce::Path shadowPath;
        shadowPath.addEllipse(centre.x - radius * 1.05f,
                             centre.y - radius * 0.95f + 3.0f,
                             radius * 2.1f, radius * 2.1f);
        g.setColour(Constants::Colors::KNOB_SHADOW.withAlpha(0.6f));
        g.fillPath(shadowPath);
    }

    // Draw main knob body with radial gradient for 3D effect
    {
        juce::ColourGradient gradient(
            Constants::Colors::KNOB_HIGHLIGHT, centre.x - radius * 0.3f, centre.y - radius * 0.3f,
            Constants::Colors::KNOB_BODY, centre.x + radius * 0.5f, centre.y + radius * 0.5f,
            true);
        gradient.addColour(0.4, Constants::Colors::KNOB_BODY);
        gradient.addColour(0.8, Constants::Colors::KNOB_SHADOW.brighter(0.1f));

        g.setGradientFill(gradient);
        g.fillEllipse(centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f);
    }

    // Draw outer rim highlight
    {
        g.setColour(Constants::Colors::KNOB_HIGHLIGHT.withAlpha(0.3f));
        float rimRadius = radius * 0.95f;
        juce::Path rimPath;
        rimPath.addCentredArc(centre.x, centre.y, rimRadius, rimRadius,
                             0.0f, -juce::MathConstants<float>::pi * 0.6f,
                             -juce::MathConstants<float>::pi * 0.2f, true);
        g.strokePath(rimPath, juce::PathStrokeType(2.0f));
    }

    // Draw inner circle for depth
    {
        float innerRadius = radius * 0.75f;
        g.setColour(Constants::Colors::KNOB_TRACK);
        g.fillEllipse(centre.x - innerRadius, centre.y - innerRadius,
                     innerRadius * 2.0f, innerRadius * 2.0f);
    }
}

void VintageKnob::drawKnobGripTexture(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    auto centre = bounds.getCentre();
    float radius = bounds.getWidth() * 0.42f;

    // Draw radial grip lines around the edge
    int numGripLines = 24;
    float gripRadius = radius * 0.85f;
    float gripLength = radius * 0.15f;

    g.setColour(Constants::Colors::KNOB_GRIP.withAlpha(0.6f));

    for (int i = 0; i < numGripLines; ++i)
    {
        float angle = (i / (float)numGripLines) * juce::MathConstants<float>::twoPi;

        float x1 = centre.x + std::cos(angle) * (gripRadius - gripLength);
        float y1 = centre.y + std::sin(angle) * (gripRadius - gripLength);
        float x2 = centre.x + std::cos(angle) * gripRadius;
        float y2 = centre.y + std::sin(angle) * gripRadius;

        g.drawLine(x1, y1, x2, y2, 1.0f);
    }
}

void VintageKnob::drawKnobPointer(juce::Graphics& g, juce::Rectangle<float> bounds, float angle)
{
    auto centre = bounds.getCentre();
    float radius = bounds.getWidth() * 0.42f;
    float pointerLength = radius * 0.65f;
    float pointerWidth = 3.0f;

    // Calculate pointer end position
    float pointerX = centre.x + std::cos(angle - juce::MathConstants<float>::halfPi) * pointerLength;
    float pointerY = centre.y + std::sin(angle - juce::MathConstants<float>::halfPi) * pointerLength;

    // Draw pointer shadow
    g.setColour(juce::Colours::black.withAlpha(0.4f));
    g.drawLine(centre.x + 1, centre.y + 1, pointerX + 1, pointerY + 1, pointerWidth);

    // Draw main pointer
    g.setColour(Constants::Colors::KNOB_POINTER);
    g.drawLine(centre.x, centre.y, pointerX, pointerY, pointerWidth);

    // Draw pointer cap (small circle at center)
    float capRadius = 4.0f;
    g.fillEllipse(centre.x - capRadius, centre.y - capRadius, capRadius * 2.0f, capRadius * 2.0f);
}

void VintageKnob::drawValueLabel(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    // Format value for display
    juce::String valueText;
    double val = getValue();
    double range = getMaximum() - getMinimum();

    if (range >= 1000.0)
        valueText = juce::String(val, 0);
    else if (range >= 10.0)
        valueText = juce::String(val, 1);
    else
        valueText = juce::String(val, 2);

    // Draw value text
    g.setColour(Constants::Colors::TEXT_PRIMARY);
    g.setFont(juce::FontOptions("Courier New", 12.0f, juce::Font::bold));
    g.drawText(valueText, bounds, juce::Justification::centred);
}

void VintageKnob::mouseDown(const juce::MouseEvent& event)
{
    juce::Slider::mouseDown(event);
    lastMousePos = event.position;
}

void VintageKnob::mouseDrag(const juce::MouseEvent& event)
{
    juce::Slider::mouseDrag(event);
    repaint();
}

void VintageKnob::mouseUp(const juce::MouseEvent& event)
{
    juce::Slider::mouseUp(event);
    repaint();
}

void VintageKnob::setLabel(const juce::String& text)
{
    labelText = text;
    repaint();
}

void VintageKnob::setShowValue(bool shouldShow)
{
    showValue = shouldShow;
    repaint();
}

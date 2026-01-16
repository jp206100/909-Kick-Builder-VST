/*
  ==============================================================================

    FuturisticLookAndFeel.cpp
    Vintage TR-909 hardware aesthetic implementation

  ==============================================================================
*/

#include "FuturisticLookAndFeel.h"

VintageLookAndFeel::VintageLookAndFeel()
{
    // Set base colors for vintage aesthetic
    setColour(juce::ResizableWindow::backgroundColourId, Constants::Colors::BACKGROUND);
    setColour(juce::Label::textColourId, Constants::Colors::TEXT_PRIMARY);
    setColour(juce::TextButton::buttonColourId, Constants::Colors::BUTTON_UP);
    setColour(juce::TextButton::textColourOffId, Constants::Colors::TEXT_PRIMARY);
    setColour(juce::ComboBox::backgroundColourId, Constants::Colors::PANEL_DARK_GRAY);
    setColour(juce::ComboBox::textColourId, Constants::Colors::TEXT_PRIMARY);
    setColour(juce::ComboBox::outlineColourId, Constants::Colors::METAL_SHADOW);
}

VintageLookAndFeel::~VintageLookAndFeel()
{
}

void VintageLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                             const juce::Colour& backgroundColour,
                                             bool shouldDrawButtonAsHighlighted,
                                             bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(2.0f);

    // Determine button state color
    juce::Colour buttonColour = shouldDrawButtonAsDown ? Constants::Colors::BUTTON_DOWN
                                                       : Constants::Colors::BUTTON_UP;

    if (shouldDrawButtonAsHighlighted && !shouldDrawButtonAsDown)
        buttonColour = buttonColour.brighter(0.1f);

    // Draw 3D beveled button
    drawBeveledRectangle(g, bounds, shouldDrawButtonAsDown, buttonColour);
}

void VintageLookAndFeel::drawBeveledRectangle(juce::Graphics& g, juce::Rectangle<float> bounds,
                                              bool isPressed, const juce::Colour& baseColour)
{
    float cornerSize = 2.0f;

    // Draw main button body
    g.setColour(baseColour);
    g.fillRoundedRectangle(bounds, cornerSize);

    // Draw 3D bevel effect
    if (!isPressed)
    {
        // Top-left highlight
        g.setColour(Constants::Colors::BUTTON_HIGHLIGHT);
        g.drawLine(bounds.getX() + cornerSize, bounds.getY(),
                  bounds.getRight() - cornerSize, bounds.getY(), 1.5f);
        g.drawLine(bounds.getX(), bounds.getY() + cornerSize,
                  bounds.getX(), bounds.getBottom() - cornerSize, 1.5f);

        // Bottom-right shadow
        g.setColour(Constants::Colors::METAL_SHADOW);
        g.drawLine(bounds.getX() + cornerSize, bounds.getBottom(),
                  bounds.getRight() - cornerSize, bounds.getBottom(), 1.5f);
        g.drawLine(bounds.getRight(), bounds.getY() + cornerSize,
                  bounds.getRight(), bounds.getBottom() - cornerSize, 1.5f);
    }
    else
    {
        // Inverted shadow when pressed
        g.setColour(Constants::Colors::METAL_SHADOW);
        g.drawLine(bounds.getX() + cornerSize, bounds.getY(),
                  bounds.getRight() - cornerSize, bounds.getY(), 1.5f);
        g.drawLine(bounds.getX(), bounds.getY() + cornerSize,
                  bounds.getX(), bounds.getBottom() - cornerSize, 1.5f);
    }
}

void VintageLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height,
                                      bool isButtonDown, int buttonX, int buttonY,
                                      int buttonW, int buttonH, juce::ComboBox& box)
{
    auto bounds = juce::Rectangle<float>(0, 0, width, height).reduced(1.0f);

    // Draw background with metal texture
    g.setColour(Constants::Colors::PANEL_DARK_GRAY);
    g.fillRoundedRectangle(bounds, 3.0f);

    // Draw 3D border
    drawBeveledRectangle(g, bounds, isButtonDown, Constants::Colors::PANEL_MEDIUM_GRAY);

    // Draw dropdown arrow
    auto arrowBounds = juce::Rectangle<float>(buttonX, buttonY, buttonW, buttonH).reduced(4.0f);
    juce::Path arrowPath;
    arrowPath.addTriangle(arrowBounds.getX(), arrowBounds.getY(),
                         arrowBounds.getRight(), arrowBounds.getY(),
                         arrowBounds.getCentreX(), arrowBounds.getBottom());

    g.setColour(Constants::Colors::TEXT_PRIMARY);
    g.fillPath(arrowPath);
}

void VintageLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                         bool shouldDrawButtonAsHighlighted,
                                         bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();

    // Draw LED indicator on the left
    float ledSize = 12.0f;
    auto ledBounds = bounds.removeFromLeft(ledSize + 8).withSizeKeepingCentre(ledSize, ledSize);

    // LED background (bezel)
    g.setColour(Constants::Colors::METAL_SHADOW);
    g.fillEllipse(ledBounds.expanded(2.0f));

    // LED light
    bool isOn = button.getToggleState();
    juce::Colour ledColour = isOn ? Constants::Colors::LED_ON : Constants::Colors::LED_OFF;

    g.setColour(ledColour);
    g.fillEllipse(ledBounds);

    // LED glow effect when on
    if (isOn)
    {
        g.setColour(ledColour.withAlpha(0.3f));
        g.fillEllipse(ledBounds.expanded(4.0f));
    }

    // Draw button text
    g.setColour(Constants::Colors::TEXT_PRIMARY);
    g.setFont(juce::Font("Arial", 13.0f, juce::Font::bold));
    g.drawText(button.getButtonText(), bounds, juce::Justification::centredLeft);
}

void VintageLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    auto bounds = label.getLocalBounds().toFloat();

    // Draw engraved text effect for labels
    if (!label.isBeingEdited())
    {
        g.setColour(Constants::Colors::TEXT_ENGRAVED);
        g.setFont(label.getFont());
        g.drawText(label.getText(), bounds.translated(0, 1), label.getJustificationType());

        g.setColour(label.findColour(juce::Label::textColourId));
        g.drawText(label.getText(), bounds, label.getJustificationType());
    }
}

void VintageLookAndFeel::drawEngravedText(juce::Graphics& g, const juce::String& text,
                                         juce::Rectangle<float> bounds, juce::Justification justification)
{
    // Draw shadow (engraved effect)
    g.setColour(Constants::Colors::TEXT_ENGRAVED);
    g.drawText(text, bounds.translated(0, 1), justification);

    // Draw main text
    g.setColour(Constants::Colors::TEXT_STENCIL);
    g.drawText(text, bounds, justification);
}

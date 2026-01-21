/*
  ==============================================================================

    ADSRDisplay.cpp

  ==============================================================================
*/

#include "ADSRDisplay.h"
#include "../Utils/Constants.h"

ADSRDisplay::ADSRDisplay() {}
ADSRDisplay::~ADSRDisplay() {}

void ADSRDisplay::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(4.0f);

    // Dark background with subtle gradient
    juce::ColourGradient bg(
        Constants::Colors::PANEL_DARK_GRAY.darker(0.3f), bounds.getCentreX(), bounds.getY(),
        Constants::Colors::PANEL_DARK_GRAY.darker(0.5f), bounds.getCentreX(), bounds.getBottom(),
        false);
    g.setGradientFill(bg);
    g.fillRoundedRectangle(bounds, 4.0f);

    // Draw inset border (hardware style)
    g.setColour(Constants::Colors::METAL_SHADOW);
    g.drawRoundedRectangle(bounds, 4.0f, 2.0f);
    g.setColour(Constants::Colors::PANEL_LIGHT_GRAY.withAlpha(0.3f));
    g.drawRoundedRectangle(bounds.reduced(1.0f), 4.0f, 1.0f);

    auto displayBounds = bounds.reduced(12.0f);

    // Draw grid
    drawGrid(g, displayBounds);

    // Draw envelope
    drawEnvelope(g, displayBounds);

    // Draw label
    g.setColour(Constants::Colors::TEXT_STENCIL);
    g.setFont(juce::FontOptions(10.0f, juce::Font::bold));
    g.drawText("ADSR ENVELOPE", bounds.removeFromTop(20).reduced(8.0f, 4.0f), juce::Justification::centredLeft);
}

void ADSRDisplay::drawGrid(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    g.setColour(Constants::Colors::GRID_LINE.withAlpha(0.15f));

    // Horizontal grid lines
    for (int i = 1; i < 4; ++i)
    {
        float y = bounds.getY() + (bounds.getHeight() * i / 4.0f);
        g.drawLine(bounds.getX(), y, bounds.getRight(), y, 1.0f);
    }

    // Vertical grid lines
    for (int i = 1; i < 4; ++i)
    {
        float x = bounds.getX() + (bounds.getWidth() * i / 4.0f);
        g.drawLine(x, bounds.getY(), x, bounds.getBottom(), 1.0f);
    }
}

void ADSRDisplay::drawEnvelope(juce::Graphics& g, juce::Rectangle<float> bounds)
{
    juce::Path envelopePath;

    // Calculate time proportions (normalize to total time)
    const float totalTime = attack + decay + 100.0f + release; // 100ms hold for sustain visibility
    const float attackProportion = attack / totalTime;
    const float decayProportion = decay / totalTime;
    const float sustainProportion = 100.0f / totalTime;
    const float releaseProportion = release / totalTime;

    const float width = bounds.getWidth();
    const float height = bounds.getHeight();
    const float bottom = bounds.getBottom();

    // Start at bottom left
    envelopePath.startNewSubPath(bounds.getX(), bottom);

    // Attack phase - rise to peak
    float attackEndX = bounds.getX() + (width * attackProportion);
    envelopePath.lineTo(attackEndX, bounds.getY());

    // Decay phase - fall to sustain level
    float decayEndX = attackEndX + (width * decayProportion);
    float sustainY = bottom - (height * sustain);
    envelopePath.lineTo(decayEndX, sustainY);

    // Sustain phase - hold at sustain level
    float sustainEndX = decayEndX + (width * sustainProportion);
    envelopePath.lineTo(sustainEndX, sustainY);

    // Release phase - fall back to zero
    float releaseEndX = sustainEndX + (width * releaseProportion);
    envelopePath.lineTo(releaseEndX, bottom);

    // Draw glow effect
    g.setColour(Constants::Colors::CRT_GLOW.withAlpha(0.3f));
    g.strokePath(envelopePath, juce::PathStrokeType(6.0f));

    g.setColour(Constants::Colors::CRT_GLOW.withAlpha(0.5f));
    g.strokePath(envelopePath, juce::PathStrokeType(3.0f));

    // Draw main line
    g.setColour(Constants::Colors::CRT_PHOSPHOR);
    g.strokePath(envelopePath, juce::PathStrokeType(2.0f));

    // Draw filled area under envelope
    juce::Path fillPath = envelopePath;
    fillPath.lineTo(releaseEndX, bottom);
    fillPath.lineTo(bounds.getX(), bottom);
    fillPath.closeSubPath();

    g.setColour(Constants::Colors::CRT_PHOSPHOR.withAlpha(0.1f));
    g.fillPath(fillPath);

    // Draw stage labels
    g.setColour(Constants::Colors::TEXT_SECONDARY);
    g.setFont(juce::FontOptions(9.0f, juce::Font::plain));

    float labelY = bounds.getBottom() + 4.0f;
    g.drawText("A", juce::Rectangle<float>(bounds.getX(), labelY, attackEndX - bounds.getX(), 12.0f), juce::Justification::centred);
    g.drawText("D", juce::Rectangle<float>(attackEndX, labelY, decayEndX - attackEndX, 12.0f), juce::Justification::centred);
    g.drawText("S", juce::Rectangle<float>(decayEndX, labelY, sustainEndX - decayEndX, 12.0f), juce::Justification::centred);
    g.drawText("R", juce::Rectangle<float>(sustainEndX, labelY, releaseEndX - sustainEndX, 12.0f), juce::Justification::centred);
}

void ADSRDisplay::setAttack(float attackMs)
{
    attack = juce::jmax(1.0f, attackMs);
    repaint();
}

void ADSRDisplay::setDecay(float decayMs)
{
    decay = juce::jmax(1.0f, decayMs);
    repaint();
}

void ADSRDisplay::setSustain(float sustainLevel)
{
    sustain = juce::jlimit(0.0f, 1.0f, sustainLevel);
    repaint();
}

void ADSRDisplay::setRelease(float releaseMs)
{
    release = juce::jmax(1.0f, releaseMs);
    repaint();
}

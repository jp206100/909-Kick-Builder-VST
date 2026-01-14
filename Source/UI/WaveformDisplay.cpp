/*
  ==============================================================================

    WaveformDisplay.cpp

  ==============================================================================
*/

#include "WaveformDisplay.h"
#include "../Utils/Constants.h"

WaveformDisplay::WaveformDisplay()
{
    displayBuffer.setSize(1, 1024);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(Constants::Colors::BACKGROUND);

    g.setColour(Constants::Colors::WAVEFORM);

    // Simple waveform drawing (placeholder)
    if (displayBuffer.getNumSamples() > 0)
    {
        juce::Path waveformPath;
        const int width = getWidth();
        const int height = getHeight();
        const float midY = height / 2.0f;

        waveformPath.startNewSubPath(0, midY);

        for (int i = 0; i < width; ++i)
        {
            const int sampleIndex = juce::jmap(i, 0, width, 0, displayBuffer.getNumSamples());
            if (sampleIndex < displayBuffer.getNumSamples())
            {
                const float sample = displayBuffer.getSample(0, sampleIndex);
                const float y = midY - (sample * midY);
                waveformPath.lineTo(i, y);
            }
        }

        g.strokePath(waveformPath, juce::PathStrokeType(2.0f));
    }
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::setBuffer(const juce::AudioBuffer<float>& buffer)
{
    displayBuffer.makeCopyOf(buffer, true);
    repaint();
}

/*
  ==============================================================================

    SubGenerator.h
    Sub-bass generator (octave below)

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SubGenerator
{
public:
    SubGenerator();
    ~SubGenerator();

    void prepare(double sampleRate, int samplesPerBlock);
    void trigger(float fundamentalFrequency);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();

    void setLevel(float amount);
    void setDecay(float milliseconds);

private:
    double sampleRate = 44100.0;
    float level = 0.0f;
    float decay = 400.0f;

    bool isActive = false;
    float phase = 0.0f;
    float subFrequency = 30.0f;
    int samplePosition = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SubGenerator)
};

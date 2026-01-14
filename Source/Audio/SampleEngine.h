/*
  ==============================================================================

    SampleEngine.h
    Sample playback engine for 909 kicks

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/Constants.h"

class SampleEngine
{
public:
    SampleEngine();
    ~SampleEngine();

    void prepare(double sampleRate, int samplesPerBlock);
    void trigger(int velocity);
    void renderNextBlock(juce::AudioBuffer<float>& buffer, int startSample, int numSamples);
    bool isActive() const { return isPlaying; }
    void setCurrentSample(Constants::SampleID sampleID);

private:
    void loadSamples();

    std::array<juce::AudioBuffer<float>, static_cast<size_t>(Constants::SampleID::NumSamples)> samples;
    Constants::SampleID currentSampleID = Constants::SampleID::Classic;

    double sampleRate = 44100.0;
    bool isPlaying = false;
    int playbackPosition = 0;
    float velocity = 1.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SampleEngine)
};

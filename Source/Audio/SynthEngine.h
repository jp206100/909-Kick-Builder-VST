/*
  ==============================================================================

    SynthEngine.h
    909-style synthesis engine

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/Constants.h"

class SynthEngine
{
public:
    SynthEngine();
    ~SynthEngine();

    void prepare(double sampleRate, int samplesPerBlock);
    void trigger(int velocity);
    void renderNextBlock(juce::AudioBuffer<float>& buffer, int startSample, int numSamples);
    bool isActive() const { return isPlaying; }

    // Parameters
    void setBodyPitch(float frequency);
    void setClickPitch(float frequency);
    void setNoiseAmount(float amount);
    void setPitchEnvDepth(float depth);
    void setPitchEnvTime(float time);

private:
    double sampleRate = 44100.0;
    bool isPlaying = false;
    int samplePosition = 0;
    float velocity = 1.0f;

    // Synthesis parameters
    float bodyPitch = 50.0f;
    float clickPitch = 3000.0f;
    float noiseAmount = 0.3f;
    float pitchEnvDepth = 2.0f; // octaves
    float pitchEnvTime = 50.0f; // ms

    // Oscillator state
    float bodyPhase = 0.0f;
    juce::Random random;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthEngine)
};

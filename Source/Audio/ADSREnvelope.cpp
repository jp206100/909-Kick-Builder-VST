/*
  ==============================================================================

    ADSREnvelope.cpp
    ADSR envelope generator

  ==============================================================================
*/

#include "ADSREnvelope.h"

ADSREnvelope::ADSREnvelope()
{
    params.attack = 0.001f;
    params.decay = 0.3f;
    params.sustain = 0.0f;
    params.release = 0.05f;
}

ADSREnvelope::~ADSREnvelope()
{
}

void ADSREnvelope::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;
    adsr.setSampleRate(sampleRate);
}

void ADSREnvelope::trigger()
{
    adsr.setParameters(params);
    adsr.noteOn();
}

void ADSREnvelope::release()
{
    adsr.noteOff();
}

float ADSREnvelope::getNextValue()
{
    return adsr.getNextSample();
}

bool ADSREnvelope::isActive() const
{
    return adsr.isActive();
}

void ADSREnvelope::setAttack(float milliseconds)
{
    params.attack = milliseconds / 1000.0f;
}

void ADSREnvelope::setDecay(float milliseconds)
{
    params.decay = milliseconds / 1000.0f;
}

void ADSREnvelope::setSustain(float level)
{
    params.sustain = juce::jlimit(0.0f, 1.0f, level);
}

void ADSREnvelope::setRelease(float milliseconds)
{
    params.release = milliseconds / 1000.0f;
}

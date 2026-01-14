/*
  ==============================================================================

    PluginProcessor.cpp
    Main audio processor implementation

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

// Include all components
#include "Audio/SampleEngine.h"
#include "Audio/SynthEngine.h"
#include "Audio/ADSREnvelope.h"
#include "Audio/MIDIHandler.h"
#include "Processing/TransientShaper.h"
#include "Processing/TonalShaper.h"
#include "Processing/ClipToZero.h"
#include "Processing/Saturation.h"
#include "Processing/SubGenerator.h"
#include "Processing/PhaseRotator.h"

//==============================================================================
NineZeroNineAudioProcessor::NineZeroNineAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#else
    :
#endif
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    // Initialize audio engine components
    sampleEngine = std::make_unique<SampleEngine>();
    synthEngine = std::make_unique<SynthEngine>();
    adsrEnvelope = std::make_unique<ADSREnvelope>();

    // Initialize processing chain
    transientShaper = std::make_unique<TransientShaper>();
    tonalShaper = std::make_unique<TonalShaper>();
    saturation = std::make_unique<Saturation>();
    subGenerator = std::make_unique<SubGenerator>();
    clipToZero = std::make_unique<ClipToZero>();
    phaseRotator = std::make_unique<PhaseRotator>();

    // Initialize buffers
    waveformBuffer.setSize(2, 4096);
    fftBuffer.setSize(2, 4096);
}

NineZeroNineAudioProcessor::~NineZeroNineAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout NineZeroNineAudioProcessor::createParameterLayout()
{
    using namespace Constants::Params;

    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Engine Mode
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "engine_mode", "Engine Mode",
        juce::StringArray{"Sample", "Synthesis"}, 0));

    // Sample Selection
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "sample_select", "Sample",
        juce::StringArray{"Classic", "Deep", "Punchy", "Vintage", "Modern"}, 0));

    // ADSR Envelope
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "attack", "Attack", ATTACK_MIN, ATTACK_MAX, ATTACK_DEFAULT));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "decay", "Decay", DECAY_MIN, DECAY_MAX, DECAY_DEFAULT));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "sustain", "Sustain", SUSTAIN_MIN, SUSTAIN_MAX, SUSTAIN_DEFAULT));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "release", "Release", RELEASE_MIN, RELEASE_MAX, RELEASE_DEFAULT));

    // Transient Shaping
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "transient_attack", "Transient Attack",
        TRANSIENT_ATTACK_MIN, TRANSIENT_ATTACK_MAX, TRANSIENT_ATTACK_DEFAULT));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "transient_sustain", "Transient Sustain",
        TRANSIENT_SUSTAIN_MIN, TRANSIENT_SUSTAIN_MAX, TRANSIENT_SUSTAIN_DEFAULT));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "punch", "Punch", PUNCH_MIN, PUNCH_MAX, PUNCH_DEFAULT));

    // Tonal Shaping
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "pitch", "Pitch", PITCH_MIN, PITCH_MAX, PITCH_DEFAULT));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "body_tone", "Body Tone", BODY_TONE_MIN, BODY_TONE_MAX, BODY_TONE_DEFAULT));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "click_tone", "Click Tone", CLICK_TONE_MIN, CLICK_TONE_MAX, CLICK_TONE_DEFAULT));

    // Clip to Zero
    layout.add(std::make_unique<juce::AudioParameterBool>("clip_enabled", "Clip Enabled", false));
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "clip_mode", "Clip Mode", juce::StringArray{"Hard", "Soft"}, 1));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "clip_amount", "Clip Amount", CLIP_AMOUNT_MIN, CLIP_AMOUNT_MAX, CLIP_AMOUNT_DEFAULT));
    layout.add(std::make_unique<juce::AudioParameterBool>("clip_auto_gain", "Auto Gain", true));

    // Saturation
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "saturation_drive", "Saturation Drive",
        SATURATION_DRIVE_MIN, SATURATION_DRIVE_MAX, SATURATION_DRIVE_DEFAULT));
    layout.add(std::make_unique<juce::AudioParameterChoice>(
        "saturation_character", "Saturation Character",
        juce::StringArray{"Clean", "Warm", "Aggressive"}, 0));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "saturation_mix", "Saturation Mix",
        SATURATION_MIX_MIN, SATURATION_MIX_MAX, SATURATION_MIX_DEFAULT));

    // Sub Generator
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "sub_level", "Sub Level", SUB_LEVEL_MIN, SUB_LEVEL_MAX, SUB_LEVEL_DEFAULT));
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "sub_decay", "Sub Decay", SUB_DECAY_MIN, SUB_DECAY_MAX, SUB_DECAY_DEFAULT));

    // Phase
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "phase", "Phase", PHASE_MIN, PHASE_MAX, PHASE_DEFAULT));

    // Output
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        "output_gain", "Output Gain",
        OUTPUT_GAIN_MIN, OUTPUT_GAIN_MAX, OUTPUT_GAIN_DEFAULT));

    return layout;
}

//==============================================================================
const juce::String NineZeroNineAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NineZeroNineAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NineZeroNineAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NineZeroNineAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NineZeroNineAudioProcessor::getTailLengthSeconds() const
{
    return 2.0; // 2 seconds for long kick tails
}

int NineZeroNineAudioProcessor::getNumPrograms()
{
    return 1;
}

int NineZeroNineAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NineZeroNineAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NineZeroNineAudioProcessor::getProgramName (int index)
{
    return {};
}

void NineZeroNineAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NineZeroNineAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    // Prepare audio engines
    sampleEngine->prepare(sampleRate, samplesPerBlock);
    synthEngine->prepare(sampleRate, samplesPerBlock);
    adsrEnvelope->prepare(sampleRate);

    // Prepare processing chain
    transientShaper->prepare(sampleRate, samplesPerBlock);
    tonalShaper->prepare(sampleRate, samplesPerBlock);
    saturation->prepare(sampleRate, samplesPerBlock);
    subGenerator->prepare(sampleRate, samplesPerBlock);
    clipToZero->prepare(sampleRate, samplesPerBlock);
    phaseRotator->prepare(sampleRate, samplesPerBlock);

    // Resize buffers
    waveformBuffer.setSize(2, samplesPerBlock);
    fftBuffer.setSize(2, samplesPerBlock * 2);
}

void NineZeroNineAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NineZeroNineAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // Only mono/stereo output
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    // Input and output layout must match
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NineZeroNineAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't contain input data
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Clear buffer (we're a synth)
    buffer.clear();

    // Update parameters from APVTS
    updateParameters();

    // Process MIDI
    processMIDI(midiMessages);

    // Generate audio from active engine
    const int engineMode = static_cast<int>(apvts.getRawParameterValue("engine_mode")->load());

    if (engineMode == 0) // Sample mode
    {
        sampleEngine->renderNextBlock(buffer, 0, buffer.getNumSamples());
    }
    else // Synthesis mode
    {
        synthEngine->renderNextBlock(buffer, 0, buffer.getNumSamples());
    }

    // Trigger sub generator if needed
    // (This is simplified - should be triggered alongside main kick)

    // Apply ADSR envelope
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* data = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            data[sample] *= adsrEnvelope->getNextValue();
        }
    }

    // Processing chain
    transientShaper->process(buffer);
    tonalShaper->process(buffer);
    saturation->process(buffer);
    subGenerator->process(buffer);
    clipToZero->process(buffer);
    phaseRotator->process(buffer);

    // Apply output gain
    const float outputGain = juce::Decibels::decibelsToGain(
        apvts.getRawParameterValue("output_gain")->load());
    buffer.applyGain(outputGain);

    // Update metering
    currentOutputLevel.store(DSPHelpers::getPeakLevel(buffer));
    clipGainReduction.store(clipToZero->getGainReduction());

    // Copy to waveform buffer for visualization (thread-safe)
    {
        juce::ScopedTryLock lock(waveformLock);
        if (lock.isLocked())
        {
            waveformBuffer.makeCopyOf(buffer, true);
        }
    }
}

void NineZeroNineAudioProcessor::processMIDI(juce::MidiBuffer& midiMessages)
{
    for (const auto metadata : midiMessages)
    {
        const auto message = metadata.getMessage();

        if (message.isNoteOn())
        {
            triggerKick(message.getVelocity());
        }
    }
}

void NineZeroNineAudioProcessor::triggerKick(int velocity)
{
    const int engineMode = static_cast<int>(apvts.getRawParameterValue("engine_mode")->load());

    if (engineMode == 0) // Sample mode
    {
        sampleEngine->trigger(velocity);
    }
    else // Synthesis mode
    {
        synthEngine->trigger(velocity);
    }

    // Trigger ADSR envelope
    adsrEnvelope->trigger();

    // Trigger sub generator (use fundamental from synth or sample)
    subGenerator->trigger(50.0f); // Simplified: use fixed frequency for now
}

void NineZeroNineAudioProcessor::updateParameters()
{
    // Update ADSR
    adsrEnvelope->setAttack(apvts.getRawParameterValue("attack")->load());
    adsrEnvelope->setDecay(apvts.getRawParameterValue("decay")->load());
    adsrEnvelope->setSustain(apvts.getRawParameterValue("sustain")->load());
    adsrEnvelope->setRelease(apvts.getRawParameterValue("release")->load());

    // Update Sample Engine
    const int sampleID = static_cast<int>(apvts.getRawParameterValue("sample_select")->load());
    sampleEngine->setCurrentSample(static_cast<Constants::SampleID>(sampleID));

    // Update Transient Shaper
    transientShaper->setAttackGain(apvts.getRawParameterValue("transient_attack")->load());
    transientShaper->setSustainGain(apvts.getRawParameterValue("transient_sustain")->load());
    transientShaper->setPunch(apvts.getRawParameterValue("punch")->load());

    // Update Tonal Shaper
    tonalShaper->setBodyTone(apvts.getRawParameterValue("body_tone")->load());
    tonalShaper->setClickTone(apvts.getRawParameterValue("click_tone")->load());

    // Update Clip to Zero
    clipToZero->setEnabled(apvts.getRawParameterValue("clip_enabled")->load() > 0.5f);
    clipToZero->setMode(static_cast<Constants::ClipMode>(
        static_cast<int>(apvts.getRawParameterValue("clip_mode")->load())));
    clipToZero->setAmount(apvts.getRawParameterValue("clip_amount")->load());
    clipToZero->setAutoGain(apvts.getRawParameterValue("clip_auto_gain")->load() > 0.5f);

    // Update Saturation
    saturation->setDrive(apvts.getRawParameterValue("saturation_drive")->load());
    saturation->setCharacter(static_cast<Constants::SaturationCharacter>(
        static_cast<int>(apvts.getRawParameterValue("saturation_character")->load())));
    saturation->setMix(apvts.getRawParameterValue("saturation_mix")->load());

    // Update Sub Generator
    subGenerator->setLevel(apvts.getRawParameterValue("sub_level")->load());
    subGenerator->setDecay(apvts.getRawParameterValue("sub_decay")->load());

    // Update Phase Rotator
    phaseRotator->setPhase(apvts.getRawParameterValue("phase")->load());
}

void NineZeroNineAudioProcessor::getLatestWaveform(juce::AudioBuffer<float>& bufferToFill)
{
    juce::ScopedTryLock lock(waveformLock);
    if (lock.isLocked())
    {
        bufferToFill.makeCopyOf(waveformBuffer, true);
    }
}

void NineZeroNineAudioProcessor::getLatestAudioForFFT(juce::AudioBuffer<float>& bufferToFill)
{
    juce::ScopedTryLock lock(fftLock);
    if (lock.isLocked())
    {
        bufferToFill.makeCopyOf(fftBuffer, true);
    }
}

//==============================================================================
bool NineZeroNineAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* NineZeroNineAudioProcessor::createEditor()
{
    return new NineZeroNineAudioProcessorEditor (*this);
}

//==============================================================================
void NineZeroNineAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void NineZeroNineAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NineZeroNineAudioProcessor();
}

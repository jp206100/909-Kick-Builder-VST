/*
  ==============================================================================

    MIDIHandler.h
    MIDI input handling

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MIDIHandler
{
public:
    MIDIHandler();
    ~MIDIHandler();

    struct MIDIEvent
    {
        enum class Type { NoteOn, NoteOff };
        Type type;
        int noteNumber;
        int velocity;
        int samplePosition;
    };

    void processM

IDIBuffer(const juce::MidiBuffer& midiBuffer, std::vector<MIDIEvent>& events);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MIDIHandler)
};

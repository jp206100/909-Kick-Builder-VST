/*
  ==============================================================================

    MIDIHandler.cpp
    MIDI input handling

  ==============================================================================
*/

#include "MIDIHandler.h"

MIDIHandler::MIDIHandler()
{
}

MIDIHandler::~MIDIHandler()
{
}

void MIDIHandler::processMIDIBuffer(const juce::MidiBuffer& midiBuffer, std::vector<MIDIEvent>& events)
{
    events.clear();

    for (const auto metadata : midiBuffer)
    {
        const auto message = metadata.getMessage();
        const int samplePosition = metadata.samplePosition;

        if (message.isNoteOn())
        {
            MIDIEvent event;
            event.type = MIDIEvent::Type::NoteOn;
            event.noteNumber = message.getNoteNumber();
            event.velocity = message.getVelocity();
            event.samplePosition = samplePosition;
            events.push_back(event);
        }
        else if (message.isNoteOff())
        {
            MIDIEvent event;
            event.type = MIDIEvent::Type::NoteOff;
            event.noteNumber = message.getNoteNumber();
            event.velocity = 0;
            event.samplePosition = samplePosition;
            events.push_back(event);
        }
    }
}

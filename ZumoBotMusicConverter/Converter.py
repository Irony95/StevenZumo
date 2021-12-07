from mido import MidiFile

# ------------------------example----------------------------------------------------------
# const char rhapsody[] PROGMEM = "O6 T40 L16 d#<b<f#<d#<f#<bd#f#"
#   "T80 c#<b-<f#<c#<f#<b-c#8"
#   "T180 d#b<f#d#f#>bd#f#c#b-<f#c#f#>b-c#8 c>c#<c#>c#<b>c#<c#>c#c>c#<c#>c#<b>c#<c#>c#"
#   "c>c#<c#>c#<b->c#<c#>c#c>c#<c#>c#<b->c#<c#>c#"
#   "c>c#<c#>c#f>c#<c#>c#c>c#<c#>c#f>c#<c#>c#"
#   "c>c#<c#>c#f#>c#<c#>c#c>c#<c#>c#f#>c#<c#>c#d#bb-bd#bf#d#c#b-ab-c#b-f#d#";


fileName = "simple.mid";
musicTrack = 1;
metaDataTrack = 0;
mid = MidiFile(fileName, clip=True);
print(f"the track selected has {len(mid.tracks[musicTrack])} number of Messages.");


NOTES = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
OCTAVES = list(range(11))
NOTES_IN_OCTAVE = len(NOTES)

output = "";
def number_to_note(number: int) -> tuple:
    octave = number // NOTES_IN_OCTAVE
    assert octave in OCTAVES, "error with notes"
    assert 0 <= number <= 127, "error with notes"
    note = NOTES[number % NOTES_IN_OCTAVE]

    return note, octave


#get the home octave
homeOctave = -1
for note in list(mid.tracks[musicTrack]):
    if (note.type == "note_on"):
        homeOctave = number_to_note(note.note)[1];
        output += f"O{homeOctave} "
        break;

#get metaData for the track
if (metaDataTrack != -1):
    for metaMessage in list(mid.tracks[metaDataTrack]):
        if (metaMessage.type == "time_signature"):
            output += f"L{metaMessage.notated_32nd_notes_per_beat} "
        elif (metaMessage.type == "set_tempo"):
            output += f"T{int(metaMessage.tempo/60000)} "

for note in list(mid.tracks[musicTrack]):
    if (note.type == "note_on"):
        n = number_to_note(note.note);
        noteKey = n[0].lower();
        octave = n[1];
        if (octave > homeOctave):
            output += ">";
        elif (octave < homeOctave):
            output += "<";
        output += noteKey;

print(output)



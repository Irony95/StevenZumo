from mido import MidiFile

fileName = "gio.mid";
mid = MidiFile(fileName, clip=True);
print(mid.tracks[3]);
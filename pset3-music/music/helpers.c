// Helper functions for music

#include <cs50.h>
#include <string.h>
#include <math.h>

#include "helpers.h"

const int MIN_FRACTION = 8; // Eighth note, 1/8
const int BASE_FREQUENCY = 440;
const string BASE_NOTE = "A4";
const char SHARP_SIGN = '#';

// Number of semitones relatively to a base note, starting from C
const int SEMITONES[7] = {-9, -7, -5, -4, -2, 0, 2};
const char SEMITONES_NOTES[7] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    // Parse units and units number from the fraction
    int units_num = atoi(&fraction[0]);
    int units = atoi(&fraction[2]);

    return MIN_FRACTION / units * units_num;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    if (strcmp(note, BASE_NOTE) == 0)
    {
        return BASE_FREQUENCY;
    }

    // Parse accidental and octave
    char accidental = '\0';
    int octave;

    if (strlen(note) == 3)
    {
        accidental = note[1];
        octave = atoi(&note[2]);
    }
    else
    {
        octave = atoi(&note[1]);
    }

    // Calculate number of semitones from the BASE_NOTE
    int semitones = 0;

    // Number of semitones by a note
    int notes_number = sizeof(SEMITONES_NOTES);
    for (int i = 0; i < notes_number; i ++)
    {
        if (SEMITONES_NOTES[i] == note[0])
        {
            semitones = SEMITONES[i];
            break;
        }
    }

    // Number of semitones by an octave
    int base_octave = atoi(&BASE_NOTE[strlen(BASE_NOTE) - 1]);
    semitones += (octave - base_octave) * 12;

    // Number of semitones by accidental
    if (accidental != '\0')
    {
        semitones += (accidental == SHARP_SIGN) ? 1 : -1;
    }

    // Calculate frequency
    float shift = powf(2, fabs(semitones / 12.0));
    return round(semitones > 0 ? (BASE_FREQUENCY * shift) : (BASE_FREQUENCY / shift));
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    return strlen(s) == 0;
}

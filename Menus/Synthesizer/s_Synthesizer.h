#pragma once

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define MAX_VOLUME      0.01
#define MAX_AMPLITUDE   0.01
#define MAX_NOTE_BUFFER 8

struct Oscilator {
    //Store the Type. (Int)
    //Potential Types:  
        // WAVEFORM_SINE              0
        // WAVEFORM_SAWTOOTH          1
        // WAVEFORM_SQUARE            2
        // WAVEFORM_TRIANGLE          3
        // WAVEFORM_ARBITRARY         4
        // WAVEFORM_PULSE             5
        // WAVEFORM_SAWTOOTH_REVERSE  6
        // WAVEFORM_SAMPLE_HOLD       7
        // WAVEFORM_TRIANGLE_VARIABLE 8
    int type = 0;
    //Store the Level (0-100)
    int level = 100;
    //Store the detune (-12 - +12)
    int detune = 0;
    //Store the Octave. (0 - 8)
    int octave = 4;
    //Store Whether it's on or off. (Bool, true = on)
    bool state = true;
};

class s_Synthesizer {
    private:     
        AudioSynthWaveformSine LFO_MODULATION;          //xy=126.99996185302734,222.00000381469727

        //2 Diiferent oscilators, but with 3 note capacities.
        AudioSynthWaveformModulated OSC0_0;
        AudioSynthWaveformModulated OSC0_1;
        AudioSynthWaveformModulated OSC0_2;
        AudioSynthWaveformModulated OSC1_0;
        AudioSynthWaveformModulated OSC1_1;
        AudioSynthWaveformModulated OSC1_2;

        //Mixers for the OSCS
        AudioMixer4 OSC0_MIXER;
        AudioMixer4 OSC1_MIXER;
        AudioMixer4 OSC_MIXER;

        //Filters in order.
        AudioFilterStateVariable HIGH_PASS_FILTER;        //xy=653.0889129638672,199.4222230911255
        AudioFilterStateVariable LOW_PASS_FILTER;        //xy=652.977783203125,248.08886337280273
        
        //Volume Envelope
        AudioEffectEnvelope ADSR_AMPLITUDE;      //xy=1045.6443405151367,226.6444435119629

        //Audio Output
        AudioOutputI2S i2s;           //xy=1220.2222061157227,228.00001335144043

        AudioConnection patchCord1 = AudioConnection(OSC0_0, 0, i2s, 0);
        AudioConnection patchCord2 = AudioConnection(OSC0_0, 0, i2s, 1);

        // AudioConnection patchCord1 = AudioConnection(LFO_MODULATION, 0, OSC1, 0);
        // AudioConnection patchCord2 = AudioConnection(LFO_MODULATION, 0, OSC2, 0);
        // AudioConnection patchCord3 = AudioConnection(OSC2, 0, OSC_MIXER, 1);
        // AudioConnection patchCord4 = AudioConnection(OSC1, 0, OSC_MIXER, 0);
        // AudioConnection patchCord5 = AudioConnection(OSC_MIXER, 0, HIGH_PASS_FILTER, 0);
        // AudioConnection patchCord6 = AudioConnection(HIGH_PASS_FILTER, 2, LOW_PASS_FILTER, 0);
        // AudioConnection patchCord8 = AudioConnection(LOW_PASS_FILTER, 0, ADSR_AMPLITUDE, 0);
        // AudioConnection patchCord10 = AudioConnection(ADSR_AMPLITUDE, 0, i2s, 0);
        // AudioConnection patchCord11 = AudioConnection(ADSR_AMPLITUDE, 0, i2s, 1);
        
        AudioControlSGTL5000 sgtl5000;     //xy=73.66674041748047,288.6666536331177
        // GUItool: end automatically generated code

        
    public:
        //Representatives of the main oscilators.
        //Directly access these to change aspects about the oscilators
        Oscilator osc_0;
        Oscilator osc_1;
        //Store at most 8 notes, play at most 3, in hz.
        byte notes[MAX_NOTE_BUFFER];
        int notesFull = 0;

        s_Synthesizer(){
            sgtl5000.enable();
            sgtl5000.volume(MAX_VOLUME);
        }

        //Sets up the Synthesizer
        void Setup(){
            OSC0_0.begin(WAVEFORM_SAWTOOTH);
            // OSC0_1.begin(WAVEFORM_SAWTOOTH);
            // OSC0_2.begin(WAVEFORM_SAWTOOTH);
            OSC0_0.amplitude(MAX_AMPLITUDE);
            // OSC0_1.amplitude(MAX_AMPLITUDE);
            // OSC0_2.amplitude(MAX_AMPLITUDE);
            OSC0_0.frequency(0);
            // OSC0_1.frequency(0);
            // OSC0_2.frequency(0);

            // OSC1_0.begin(WAVEFORM_SAWTOOTH);
            // OSC1_1.begin(WAVEFORM_SAWTOOTH);
            // OSC1_2.begin(WAVEFORM_SAWTOOTH);
            // OSC1_0.amplitude(MAX_AMPLITUDE);
            // OSC1_1.amplitude(MAX_AMPLITUDE);
            // OSC1_2.amplitude(MAX_AMPLITUDE);
            // OSC1_0.frequency(0);
            // OSC1_1.frequency(0);
            // OSC1_2.frequency(0);
        }

        void Play(){
            updatePitch();
        }

        //Pitch Of the Oscilators
        void updatePitch(){
            int notePosOffset = 0;
            if(notesFull > 3) notePosOffset = notesFull-3;
            OSC0_0.frequency(getFrequency(notes[0 + notePosOffset]));
            // OSC0_1.frequency(getFrequency(notes[1 + notePosOffset]));
            // OSC0_2.frequency(getFrequency(notes[2 + notePosOffset]));

            OSC1_0.frequency(getFrequency(notes[0 + notePosOffset]));
            // OSC1_1.frequency(getFrequency(notes[1 + notePosOffset]));
            // OSC1_2.frequency(getFrequency(notes[2 + notePosOffset]));
        }

        //MIDI note to Frequency
        double getFrequency(double note){
            return (440.0 * pow(2.0, ((double)(note-69)/12.0)));
        }

        //WE NEED A MIDI THINGY HERE.
        //Function Name TBD, actually plays the sound.
        void AddNote(byte pitch){
            notes[notesFull] = pitch;
            notesFull++;
        }
        void RemoveNote(byte pitch){
            int i = 0;
            while(i < MAX_NOTE_BUFFER){
                if(notes[i] == pitch) break;
                i++;
            }
            notes[i] = 0;
            notesFull--;
            while(i < (MAX_NOTE_BUFFER - 1) && notes[i+1] != 0){
                notes[i] = notes[i+1];
                i++;
                if(i == (MAX_NOTE_BUFFER-1) ){
                    notes[i] = 0;
                }
            }
        }

        void SavePreset(){

        }
        void LoadPreset(){

        }
};
#pragma once

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define MAX_VOLUME      1
#define MAX_AMPLITUDE   0.1 //THIS is the one we need to worry abt
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
    float type = 0;

    //Store the Level (0.0-1.0)
    double level = 1.0;

    //Store the detune (-12 - +12)
    int detune = 0;

    //Store the Octave. (-4 - 4)
    int octave = 0;

    //Store Whether it's on or off. (Bool, true = on)
    bool state = true;
};

//struct Note
//Stores both oscilators, its mixer, and adsr.
struct Note {
    AudioSynthWaveformModulated osc0;
    AudioSynthWaveformModulated osc1;

    AudioMixer4 mixer;
    AudioEffectEnvelope adsr;

    //Store the original pitch of the note. (0-127)
    byte pitch = 0;

    public: 
        void adjustLevel(double l0, double l1){
            mixer.gain(0, l0);
            mixer.gain(1, l1);
        }

        void adjustADSR(int a, int d, double s, int r){
            adsr.attack(a);
            adsr.decay(d);
            adsr.sustain(s);
            adsr.release(r);
        }
};



class s_Synthesizer {
    private:
        //Circular Array of notes
        Note notes[MAX_NOTE_BUFFER];
        int notesFull = 0;
        int onIndex = 0; //Should always be ahead of, or equal to off index.

        AudioSynthWaveformSine LFO_MODULATION;          //xy=126.99996185302734,222.00000381469727

        //Final Mixer
        AudioMixer4 MIXER1;
        AudioMixer4 MIXER2;
        AudioMixer4 FINAL_MIXER;

        //Filters in order.
        AudioFilterStateVariable HIGH_PASS_FILTER;
        AudioFilterStateVariable LOW_PASS_FILTER;

        //Audio Output
        AudioOutputI2S i2s;           //xy=1220.2222061157227,228.00001335144043

        //Create Patch Cords connecting each of the notes in the array to its own mixer and adsr.
        //There should be a more efficent way of doing this...
        AudioConnection patchCord1 = AudioConnection(notes[0].osc0, 0, notes[0].mixer, 0);
        AudioConnection patchCord2 = AudioConnection(notes[0].osc1, 0, notes[0].mixer, 1);
        AudioConnection patchCord3 = AudioConnection(notes[0].mixer, notes[0].adsr);
        AudioConnection patchCord4 = AudioConnection(notes[1].osc0, 0, notes[1].mixer, 0);
        AudioConnection patchCord5 = AudioConnection(notes[1].osc1, 0, notes[1].mixer, 1);
        AudioConnection patchCord6 = AudioConnection(notes[1].mixer, notes[1].adsr);
        AudioConnection patchCord7 = AudioConnection(notes[2].osc0, 0, notes[2].mixer, 0);
        AudioConnection patchCord8 = AudioConnection(notes[2].osc1, 0, notes[2].mixer, 1);
        AudioConnection patchCord9 = AudioConnection(notes[2].mixer, notes[2].adsr);
        AudioConnection patchCord10 = AudioConnection(notes[3].osc0, 0, notes[3].mixer, 0);
        AudioConnection patchCord11 = AudioConnection(notes[3].osc1, 0, notes[3].mixer, 1);
        AudioConnection patchCord12 = AudioConnection(notes[3].mixer, notes[3].adsr);
        AudioConnection patchCord13 = AudioConnection(notes[4].osc0, 0, notes[4].mixer, 0);
        AudioConnection patchCord14 = AudioConnection(notes[4].osc1, 0, notes[4].mixer, 1);
        AudioConnection patchCord15 = AudioConnection(notes[4].mixer, notes[4].adsr);
        AudioConnection patchCord16 = AudioConnection(notes[5].osc0, 0, notes[5].mixer, 0);
        AudioConnection patchCord17 = AudioConnection(notes[5].osc1, 0, notes[5].mixer, 1);
        AudioConnection patchCord18 = AudioConnection(notes[5].mixer, notes[5].adsr);
        AudioConnection patchCord19 = AudioConnection(notes[6].osc0, 0, notes[6].mixer, 0);
        AudioConnection patchCord20 = AudioConnection(notes[6].osc1, 0, notes[6].mixer, 1);
        AudioConnection patchCord21 = AudioConnection(notes[6].mixer, notes[6].adsr);
        AudioConnection patchCord22 = AudioConnection(notes[7].osc0, 0, notes[7].mixer, 0);
        AudioConnection patchCord23 = AudioConnection(notes[7].osc1, 0, notes[7].mixer, 1);
        AudioConnection patchCord24 = AudioConnection(notes[7].mixer, notes[7].adsr);

        AudioConnection PatchCord31 = AudioConnection(notes[0].adsr, 0, MIXER1, 0);
        AudioConnection PatchCord32 = AudioConnection(notes[1].adsr, 0, MIXER1, 1);
        AudioConnection PatchCord33 = AudioConnection(notes[2].adsr, 0, MIXER1, 2);
        AudioConnection PatchCord34 = AudioConnection(notes[3].adsr, 0, MIXER1, 3);
        AudioConnection PatchCord35 = AudioConnection(notes[4].adsr, 0, MIXER2, 0);
        AudioConnection PatchCord36 = AudioConnection(notes[5].adsr, 0, MIXER2, 1);
        AudioConnection PatchCord37 = AudioConnection(notes[6].adsr, 0, MIXER2, 2);
        AudioConnection PatchCord38 = AudioConnection(notes[7].adsr, 0, MIXER2, 3);

        //Attach LFO to each oscilator
        AudioConnection patchCord40 = AudioConnection(LFO_MODULATION, 0, notes[0].osc0, 0);
        AudioConnection patchCord41 = AudioConnection(LFO_MODULATION, 0, notes[0].osc1, 0);
        AudioConnection patchCord42 = AudioConnection(LFO_MODULATION, 0, notes[1].osc0, 0);
        AudioConnection patchCord43 = AudioConnection(LFO_MODULATION, 0, notes[1].osc1, 0);
        AudioConnection patchCord44 = AudioConnection(LFO_MODULATION, 0, notes[2].osc0, 0);
        AudioConnection patchCord45 = AudioConnection(LFO_MODULATION, 0, notes[2].osc1, 0);
        AudioConnection patchCord46 = AudioConnection(LFO_MODULATION, 0, notes[3].osc0, 0);
        AudioConnection patchCord47 = AudioConnection(LFO_MODULATION, 0, notes[3].osc1, 0);
        AudioConnection patchCord48 = AudioConnection(LFO_MODULATION, 0, notes[4].osc0, 0);
        AudioConnection patchCord49 = AudioConnection(LFO_MODULATION, 0, notes[4].osc1, 0);
        AudioConnection patchCord50 = AudioConnection(LFO_MODULATION, 0, notes[5].osc0, 0);
        AudioConnection patchCord51 = AudioConnection(LFO_MODULATION, 0, notes[5].osc1, 0);
        AudioConnection patchCord52 = AudioConnection(LFO_MODULATION, 0, notes[6].osc0, 0);
        AudioConnection patchCord53 = AudioConnection(LFO_MODULATION, 0, notes[6].osc1, 0);
        AudioConnection patchCord54 = AudioConnection(LFO_MODULATION, 0, notes[7].osc0, 0);
        AudioConnection patchCord55 = AudioConnection(LFO_MODULATION, 0, notes[7].osc1, 0);

        AudioConnection patchCord61 = AudioConnection(MIXER1, 0, FINAL_MIXER, 0);
        AudioConnection patchCord62 = AudioConnection(MIXER2, 0, FINAL_MIXER, 1);
        AudioConnection patchCord63 = AudioConnection(FINAL_MIXER, 0, HIGH_PASS_FILTER, 0);
        AudioConnection patchCord64 = AudioConnection(HIGH_PASS_FILTER, 2, LOW_PASS_FILTER, 0);
        AudioConnection patchCord65 = AudioConnection(LOW_PASS_FILTER, 0, i2s, 0);
        AudioConnection patchCord66 = AudioConnection(LOW_PASS_FILTER, 0, i2s, 1);
 
        AudioControlSGTL5000 sgtl5000;  
        // GUItool: end automatically generated code

    public:
        //Representatives of the main oscilators.
        //Directly access these to change aspects about the oscilators
        Oscilator osc_0;
        Oscilator osc_1;

        int Osc0PrevType = 2;
        int Osc1PrevType = 1;

        int bend;
        int bendRange = 2;

        int attack = 100;           //in ms
        int decay = 100;            //in ms
        double sustain = 0.75;       //from 0.0 - 1.0
        int release = 100;          //in ms

        s_Synthesizer(){
            osc_0.level = 0.4;
            osc_0.octave = 0;
            osc_0.type = WAVEFORM_TRIANGLE;

            osc_1.level = 0.6;
            osc_1.octave = -2;
            osc_1.type = WAVEFORM_SINE;
        }

        //Sets up the Synthesizer
        void Setup(){
            sgtl5000.enable();
            sgtl5000.volume(MAX_VOLUME);

            updateType(osc_0.type, 0);
            updateType(osc_1.type, 1);

            for (int i = 0; i < MAX_NOTE_BUFFER; i++)
            {
                notes[i].osc0.amplitude(MAX_AMPLITUDE);
                notes[i].osc1.amplitude(MAX_AMPLITUDE);
            }

            updateADSR(attack, decay, sustain, release);
            updateLevel(osc_0.level, osc_1.level);

            MIXER1.gain(0, 1.0);
            MIXER1.gain(1, 1.0);
            MIXER1.gain(2, 1.0);
            MIXER1.gain(3, 1.0);

            MIXER2.gain(0, 1.0);
            MIXER2.gain(1, 1.0);
            MIXER2.gain(2, 1.0);
            MIXER2.gain(3, 1.0);

            FINAL_MIXER.gain(0, 0.5);
            FINAL_MIXER.gain(1, 0.5);

            LFO_MODULATION.phase(0);
            LFO_MODULATION.frequency(0);

            LOW_PASS_FILTER.frequency(10000);
            LOW_PASS_FILTER.resonance(5.0);
            HIGH_PASS_FILTER.frequency(15.625);
            HIGH_PASS_FILTER.resonance(5.0);
        }

        //Main Update loop of the Oscilators
        void Play(){
            updateLevel(osc_0.level, osc_1.level);

            //Update Type of Oscilators
            if(osc_0.type != Osc0PrevType) updateType(osc_0.type, 0);
            if(osc_1.type != Osc1PrevType) updateType(osc_1.type, 1);

            //Try to Attempt to normalize waves :)

            //Iterate through array, for each note which state is on, Update the Frequency.
            for(int i = 0; i < MAX_NOTE_BUFFER; i++){
                if(notes[i].adsr.isActive()){
                    notes[i].osc0.frequency(
                        getFrequency( notes[i].pitch 
                        + (osc_0.octave * 12) 
                        + osc_0.detune
                        + (getBend()*bendRange) ) 
                    );
                    notes[i].osc1.frequency( 
                        getFrequency( notes[i].pitch 
                        + (osc_1.octave * 12) 
                        + osc_1.detune
                        + (getBend()*bendRange) ) 
                    );
                }
            }
        }
        
        //Updates type of selected oscilator
        void updateType(int type, int oscilator){
            switch (oscilator)
            {
                case 0:
                    for (int i = 0; i < MAX_NOTE_BUFFER; i++)
                    {
                        notes[i].osc0.begin(type);
                    }
                    Osc0PrevType = type;
                    break;
                case 1:
                    for (int i = 0; i < MAX_NOTE_BUFFER; i++)
                    {
                        notes[i].osc1.begin(type);
                    }
                    Osc1PrevType = type;
                    break;
                default:
                    break;
            }
        }
        void updateADSR(int attack, int decay, double sustain, int release){
            for (int i = 0; i < MAX_NOTE_BUFFER; i++)
            {
                notes[i].adjustADSR(attack, decay, sustain, release);
            }
        }
        void updateLevel(double level0, double level1){
            for(int i = 0; i < MAX_NOTE_BUFFER; i++){
                notes[i].adjustLevel(level0, level1);
            }
        }
        void updateLowPassFilterFrequency(float freq){
            freq = max(freq, 2.0f); //we don't want the frequency to be 0.
            LOW_PASS_FILTER.frequency(10000 * freq / 127);
        }
        void updateLowPassFilterResonance(float Q){
            LOW_PASS_FILTER.resonance((4.3 * Q / 127) + 0.7);
        }
        void updateHighPassFilterFrequency(float freq){
            freq = max(freq, 2.0f); //we don't want the frequency to be 0.
            HIGH_PASS_FILTER.frequency(10000 * (1 - (freq / 127)) );
        }
        void updateHighPassFilterResonance(float Q){
            HIGH_PASS_FILTER.resonance((4.3 * Q / 127) + 0.7);
        }

        //MIDI note to Frequency
        float getFrequency(float note){
            if (note == 0) return 0;
            return (440.0F * pow(2.0, ((float)(note-69)/12.0F)));
        }
        float getBend(){
            return ( (float) bend / 0x1FFF );
        }

        void AddNote(byte pitch){
            //Find a note with a state that's off.
            while(notes[onIndex].adsr.isActive() && notesFull < MAX_NOTE_BUFFER){
                Serial.println("Finding note...");
                onIndex = (onIndex + 1) % MAX_NOTE_BUFFER;
            }
            //If the array is full, replace the first note.
            notes[onIndex].pitch = pitch;
            notes[onIndex].adsr.noteOn();

            //Wheh the array is full and then empties, wacky things happen, find a fix eventually!

            Serial.print("Added - ");
            for(Note note : notes){
                if(note.adsr.isActive())
                    Serial.print(note.pitch);
                else
                    Serial.print("X");
                Serial.print(" ");
            }
            Serial.println();

            onIndex = (onIndex + 1) % MAX_NOTE_BUFFER;
            notesFull = min(notesFull+1, MAX_NOTE_BUFFER);

            Serial.print("Notes Full: ");
            Serial.println(notesFull);
        }
        void RemoveNote(byte pitch){
            
            //Find where in buffer the pitch(es) are
            for (int i = 0; i < MAX_NOTE_BUFFER; i++){
                //If the pitch is found, and the note is on, turn it off.
                if(notes[i].pitch == pitch && notes[i].adsr.isActive()) {
                    //Remove it from notes, and turn off the correct ADSR
                    notes[i].adsr.noteOff();

                    Serial.print("Removed - ");
                    for(Note note : notes){
                        if(note.adsr.isSustain())
                            Serial.print(note.pitch);
                        else
                            Serial.print("X");
                        Serial.print(" ");
                    }
                    Serial.println();

                    notesFull--;

                    Serial.print("Notes Full:");
                    Serial.println(notesFull);
                }
            }
        }

        void updateLFO(double freq, float depth){
            //Iterate through the notes list, adjust frequencyModulation for each osilator
            for (int i = 0; i < MAX_NOTE_BUFFER; i++)
            {
                notes[i].osc0.frequencyModulation(depth);
                notes[i].osc1.frequencyModulation(depth);
            }
            LFO_MODULATION.frequency(freq/8.0);

            if(freq == 0)
                LFO_MODULATION.phase(0);
        }

        void SavePreset(){

        }
        void LoadPreset(){

        }
};
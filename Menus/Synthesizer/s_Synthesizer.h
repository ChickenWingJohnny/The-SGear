#pragma once

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define MAX_VOLUME      0.01
#define MAX_AMPLITUDE   0.01
#define MAX_NOTE_PLAY   4
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

class s_Synthesizer {
    private:     
        AudioSynthWaveformSine LFO_MODULATION;          //xy=126.99996185302734,222.00000381469727

        //2 Diiferent oscilators, but with 4 note capacities.
        AudioSynthWaveformModulated OSC0_0;
        AudioSynthWaveformModulated OSC0_1;
        AudioSynthWaveformModulated OSC0_2;
        AudioSynthWaveformModulated OSC0_3;

        AudioSynthWaveformModulated OSC1_0;
        AudioSynthWaveformModulated OSC1_1;
        AudioSynthWaveformModulated OSC1_2;
        AudioSynthWaveformModulated OSC1_3;

        //Volume Envelope
        AudioEffectEnvelope ADSR_AMPLITUDE_0;
        AudioEffectEnvelope ADSR_AMPLITUDE_1;
        AudioEffectEnvelope ADSR_AMPLITUDE_2;      
        AudioEffectEnvelope ADSR_AMPLITUDE_3;         

        //Mixers for the OSC's Notes
        AudioMixer4 OSC0_MIXER;
        AudioMixer4 OSC1_MIXER;
        AudioMixer4 OSC2_MIXER;
        AudioMixer4 OSC3_MIXER;

        //Final Mixer
        AudioMixer4 OSC_MIXER;

        //Filters in order.
        AudioFilterStateVariable HIGH_PASS_FILTER;
        AudioFilterStateVariable LOW_PASS_FILTER;

        //Audio Output
        AudioOutputI2S i2s;           //xy=1220.2222061157227,228.00001335144043

        
        AudioConnection patchCord1 = AudioConnection(OSC0_0, 0, OSC0_MIXER, 0);
        AudioConnection patchCord2 = AudioConnection(OSC1_0, 0, OSC0_MIXER, 1);
        AudioConnection patchCord3 = AudioConnection(OSC0_MIXER, ADSR_AMPLITUDE_0);
        AudioConnection patchCord4 = AudioConnection(OSC0_1, 0, OSC1_MIXER, 0);
        AudioConnection patchCord5 = AudioConnection(OSC1_1, 0, OSC1_MIXER, 1);
        AudioConnection patchCord6 = AudioConnection(OSC1_MIXER, ADSR_AMPLITUDE_1);
        AudioConnection patchCord7 = AudioConnection(OSC0_2, 0, OSC2_MIXER, 0);
        AudioConnection patchCord8 = AudioConnection(OSC1_2, 0, OSC2_MIXER, 1);
        AudioConnection patchCord9 = AudioConnection(OSC2_MIXER, ADSR_AMPLITUDE_2);
        AudioConnection patchCord10 = AudioConnection(OSC0_3, 0, OSC3_MIXER, 0);
        AudioConnection patchCord11 = AudioConnection(OSC1_3, 0, OSC3_MIXER, 1);
        AudioConnection patchCord12 = AudioConnection(OSC3_MIXER, ADSR_AMPLITUDE_3);

        AudioConnection patchCord13 = AudioConnection(ADSR_AMPLITUDE_0, 0, OSC_MIXER, 0);
        AudioConnection patchCord14 = AudioConnection(ADSR_AMPLITUDE_1, 0, OSC_MIXER, 1);
        AudioConnection patchCord15 = AudioConnection(ADSR_AMPLITUDE_2, 0, OSC_MIXER, 2);
        AudioConnection patchCord16 = AudioConnection(ADSR_AMPLITUDE_3, 0, OSC_MIXER, 3);

        // AudioConnection patchCord1 = AudioConnection(LFO_MODULATION, 0, OSC1, 0);
        // AudioConnection patchCord2 = AudioConnection(LFO_MODULATION, 0, OSC2, 0);
        // AudioConnection patchCord5 = AudioConnection(OSC_MIXER, 0, HIGH_PASS_FILTER, 0);
        // AudioConnection patchCord6 = AudioConnection(HIGH_PASS_FILTER, 2, LOW_PASS_FILTER, 0);
        // AudioConnection patchCord8 = AudioConnection(LOW_PASS_FILTER, 0, i2s, 0);

        

        AudioConnection patchCord20 = AudioConnection(OSC_MIXER, 0, i2s, 0);
        AudioConnection patchCord21 = AudioConnection(OSC_MIXER, 0, i2s, 1);

        
        
        AudioControlSGTL5000 sgtl5000;     //xy=73.66674041748047,288.6666536331177
        // GUItool: end automatically generated code

        
    public:
        //Representatives of the main oscilators.
        //Directly access these to change aspects about the oscilators
        Oscilator osc_0;
        bool note0Active = false;
        bool note1Active = false;
        bool note2Active = false;
        bool note3Active = false;
        Oscilator osc_1;
        

        //Store at most 8 notes, play at most 3, in hz.
        byte notes[MAX_NOTE_BUFFER];
        int notesFull = 0;

        int bend;
        int bendRange = 2;

        int attack = 25;           //in ms
        int decay = 25;            //in ms
        double sustain = 0.8;       //from 0.0 - 1.0
        int release = 25;          //in ms

        s_Synthesizer(){
            sgtl5000.enable();
            sgtl5000.volume(MAX_VOLUME);

            osc_0.level = 0.50;
            osc_0.octave = 0;
            osc_0.type = WAVEFORM_SQUARE;

            osc_1.level = 0.50;
            osc_1.octave = -2;
            osc_1.type = WAVEFORM_SAWTOOTH;
        }

        //Sets up the Synthesizer
        void Setup(){
            OSC0_0.begin(osc_0.type);
            OSC0_1.begin(osc_0.type);
            OSC0_2.begin(osc_0.type);
            OSC0_3.begin(osc_0.type);
            OSC0_0.amplitude(MAX_AMPLITUDE);
            OSC0_1.amplitude(MAX_AMPLITUDE);
            OSC0_2.amplitude(MAX_AMPLITUDE);
            OSC0_3.amplitude(MAX_AMPLITUDE);

            OSC1_0.begin(osc_1.type);
            OSC1_1.begin(osc_1.type);
            OSC1_2.begin(osc_1.type);
            OSC1_3.begin(osc_1.type);
            OSC1_0.amplitude(MAX_AMPLITUDE);
            OSC1_1.amplitude(MAX_AMPLITUDE);
            OSC1_2.amplitude(MAX_AMPLITUDE);
            OSC1_3.amplitude(MAX_AMPLITUDE);

            ADSR_AMPLITUDE_0.attack(attack);
            ADSR_AMPLITUDE_0.decay(decay);
            ADSR_AMPLITUDE_0.sustain(sustain);
            ADSR_AMPLITUDE_0.release(release);
            ADSR_AMPLITUDE_1.attack(attack);
            ADSR_AMPLITUDE_1.decay(decay);
            ADSR_AMPLITUDE_1.sustain(sustain);
            ADSR_AMPLITUDE_1.release(release);
            ADSR_AMPLITUDE_2.attack(attack);
            ADSR_AMPLITUDE_2.decay(decay);
            ADSR_AMPLITUDE_2.sustain(sustain);
            ADSR_AMPLITUDE_2.release(release);
            ADSR_AMPLITUDE_3.attack(attack);
            ADSR_AMPLITUDE_3.decay(decay);
            ADSR_AMPLITUDE_3.sustain(sustain);
            ADSR_AMPLITUDE_3.release(release);

            OSC0_MIXER.gain(0, osc_0.level);
            OSC0_MIXER.gain(1, osc_1.level);
            OSC1_MIXER.gain(0, osc_0.level);
            OSC1_MIXER.gain(1, osc_1.level);
            OSC2_MIXER.gain(0, osc_0.level);
            OSC2_MIXER.gain(1, osc_1.level);
            OSC3_MIXER.gain(0, osc_0.level);
            OSC3_MIXER.gain(1, osc_1.level);

            OSC_MIXER.gain(0, 1.00);
            OSC_MIXER.gain(1, 1.00);
            OSC_MIXER.gain(2, 1.00);
            OSC_MIXER.gain(3, 1.00);
        }

        //Main Update loop of the Oscilators
        void Play(){
            int notePosOffset = 0;
            if(notesFull > MAX_NOTE_PLAY) notePosOffset = notesFull-3;

            // OSC0_MIXER.gain(0, osc_0.level);
            // OSC0_MIXER.gain(1, osc_1.level);
            // OSC1_MIXER.gain(0, osc_0.level);
            // OSC1_MIXER.gain(1, osc_1.level);
            // OSC2_MIXER.gain(0, osc_0.level);
            // OSC2_MIXER.gain(1, osc_1.level);
            // OSC3_MIXER.gain(0, osc_0.level);
            // OSC3_MIXER.gain(1, osc_1.level);

            //Attempt to normalize waves :)
            double gainFactor = 1.0/notesFull;
            if(notesFull > 0){
                OSC_MIXER.gain(0, gainFactor);
                OSC_MIXER.gain(1, gainFactor);
                OSC_MIXER.gain(2, gainFactor);
                OSC_MIXER.gain(3, gainFactor);
            }

            if(notes[0 + notePosOffset] != 0){
                OSC0_0.frequency(
                    getFrequency( notes[0 + notePosOffset] 
                        + (osc_0.octave * 12) 
                        + (getBend()*bendRange) ) 
                );
                OSC1_0.frequency( 
                    getFrequency( notes[0 + notePosOffset] 
                    + (osc_1.octave * 12) 
                    + (getBend()*bendRange) 
                    ) 
                );
            }
            if(notes[1 + notePosOffset] != 0){
                OSC0_1.frequency(
                    getFrequency( notes[1 + notePosOffset] 
                    + (osc_0.octave * 12) 
                    + (getBend()*bendRange) ) 
                );
                OSC1_1.frequency( 
                    getFrequency( notes[1 + notePosOffset] 
                    + (osc_1.octave * 12) 
                    + (getBend()*bendRange) ) 
                );
            }
            if(notes[2 + notePosOffset] != 0){
                OSC0_2.frequency(
                    getFrequency( notes[2 + notePosOffset] 
                    + (osc_0.octave * 12) 
                    + (getBend()*bendRange) ) 
                );
                OSC1_2.frequency( 
                    getFrequency( notes[2 + notePosOffset] 
                    + (osc_1.octave * 12) 
                    + (getBend()*bendRange) ) 
                );
            }
            if(notes[3 + notePosOffset] != 0){
                OSC0_3.frequency(
                    getFrequency( notes[3 + notePosOffset] 
                    + (osc_0.octave * 12) 
                    + (getBend()*bendRange) ) 
                );
                OSC1_3.frequency( 
                    getFrequency( notes[3 + notePosOffset] 
                    + (osc_1.octave * 12) 
                    + (getBend()*bendRange) ) 
                );
            }
        }

        void updateADSR(int attack, int decay, double sustain, int release){
            
        }

        //MIDI note to Frequency
        float getFrequency(float note){
            if (note == 0) return 0;
            return (440.0F * pow(2.0, ((float)(note-69)/12.0F)));
        }
        float getBend(){
            return ( (float) bend / 0x1FFF );
        }

        //WE NEED A MIDI THINGY HERE.
        //Function Name TBD, actually plays the sound.
        void AddNote(byte pitch){
            notes[notesFull] = pitch;

            Serial.print("Added - ");
            for(byte note : notes){
                Serial.print(note);
                Serial.print(" ");
            }
            Serial.println();

            if(notesFull < MAX_NOTE_PLAY){
                if(!note0Active){
                    ADSR_AMPLITUDE_0.noteOn();
                    note0Active = true;
                } else if(!note1Active){
                    ADSR_AMPLITUDE_1.noteOn();
                    note1Active = true;
                } else if(!note2Active){
                    ADSR_AMPLITUDE_2.noteOn();
                    note2Active = true;
                } else if(!note3Active){
                    ADSR_AMPLITUDE_3.noteOn();
                    note3Active = true;
                }
            }
            
            // Serial.print("OSCS Active: ");
            // if(note0Active) Serial.print("0 ");
            // if(note1Active) Serial.print("1 ");
            // if(note2Active) Serial.print("2 ");
            // if(note3Active) Serial.print("3 ");
            // Serial.println();

            notesFull++;

            // Serial.print("Notes Full: ");
            // Serial.println(notesFull);
        }
        void RemoveNote(byte pitch){
            int i = 0;
            //Find where in buffer the pitch is
            while(i < MAX_NOTE_BUFFER){
                if(notes[i] == pitch) break;
                i++;
            }
            //Remove it from notes, and turn off the correct ADSR
            notes[i] = 0;

            // Serial.print("Removed - ");
            // for(byte note : notes){
            //     Serial.print(note);
            //     Serial.print(" ");
            // }
            // Serial.println();

            //And shift the rest back.
            notesFull--;
            while(i < (MAX_NOTE_BUFFER - 1) && notes[i+1] != 0){
                notes[i] = notes[i+1];
                i++;
                notes[i] = 0;
            }

            // Serial.print("Shifted - ");
            // for(byte note : notes){
            //     Serial.print(note);
            //     Serial.print(" ");
            // }
            // Serial.println();

            // Serial.print("Notes Full:");
            // Serial.println(notesFull);

            //And turn off Oscilators if applicable
            if(notesFull < MAX_NOTE_PLAY){
                switch (notesFull){
                    case 0:
                        ADSR_AMPLITUDE_0.noteOff();
                        ADSR_AMPLITUDE_0.noteOff();
                        note0Active = false;
                        break;
                    case 1:
                        ADSR_AMPLITUDE_1.noteOff();
                        ADSR_AMPLITUDE_1.noteOff();
                        note1Active = false;
                        break;
                    case 2:
                        ADSR_AMPLITUDE_2.noteOff();
                        ADSR_AMPLITUDE_2.noteOff();
                        note2Active = false;
                        break;
                    case 3:
                        ADSR_AMPLITUDE_3.noteOff();
                        ADSR_AMPLITUDE_3.noteOff();
                        note3Active = false;
                        break;
                    default:
                        break;
                }
            }

            // Serial.print("OSCS Active: ");
            // if(note0Active) Serial.print("0 ");
            // if(note1Active) Serial.print("1 ");
            // if(note2Active) Serial.print("2 ");
            // if(note3Active) Serial.print("3 ");
            // Serial.println();
        }

        void SavePreset(){

        }
        void LoadPreset(){

        }
};
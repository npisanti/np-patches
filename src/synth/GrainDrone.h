
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"
#include "../util/SampleControl.h"

namespace np { namespace synth {

class GrainDrone {
    
public:
    void resize( int w, int h );
    ofParameterGroup & setup( int w, int h, std::string name="graindrone", bool guicontrol=true );
    void draw( int x, int y) ;

    void addSample( util::SampleControl & sample );

    pdsp::Patchable& in_dry_control();
    pdsp::Patchable& in_resonator_control( int i );
    pdsp::Patchable& in_position();
    pdsp::Patchable& in_fb();
    pdsp::Patchable& out_L();
    pdsp::Patchable& out_R();
    
    float meter_cutoff() const;
        
    pdsp::GrainCloud        cloud;  
    ofParameterGroup        parameters;

    std::vector<pdsp::Parameter>    resonators_control;    
    pdsp::Parameter            dry_control;
    pdsp::Parameter            positionControl;
    pdsp::Parameter            densityControl;
    pdsp::Parameter            fader;        
    pdsp::Parameter            resonatorsFBControl;
    
    void smoothing( float ms );
    
    void sample( int select ); // select sample
    void pitch( int newpitch ); // set pitch
   
   
    ofParameterGroup & label( std::string name );
    
private:

    std::vector<pdsp::PatchNode> sendnode;
    pdsp::PatchNode              drynode;

    std::vector<util::SampleControl*> samples;

    pdsp::DBtoLin       dBtoLin;
    
    pdsp::PatchNode     fbInput;

    pdsp::Parameter        positionJitterControl;
    pdsp::Parameter        lengthControl;

    pdsp::Parameter        distanceJitterControl;
    pdsp::Parameter        pitchJitterControl;
    pdsp::Parameter        selectSampleControl;
    
    pdsp::Parameter        pitchControl;
    
    pdsp::Parameter            resonatorsPitchMaster;
    std::vector<pdsp::Parameter>    resonatorsPitchControls;

    pdsp::Parameter            resonatorsLRSpreadControl;

    pdsp::Parameter            resonatorsDampControl;
   
    int                     grainVoices;

    std::vector<pdsp::CombFilter> resonators;
    std::vector<pdsp::Amp>        resonators_sends;

    pdsp::Amp               dryL;
    pdsp::Amp               dryR;

    pdsp::Amp               voiceAmpL;
    pdsp::Amp               voiceAmpR;
    
    pdsp::LFOPhazor         lfoPhazor;
    pdsp::CheapTri          lfo;
    pdsp::Amp               pitchModAmt;
    pdsp::Parameter            pitchModControl;
    pdsp::Parameter            pitchModSpeed;

    bool showLabel;

    int uiWidth;
    int uiHeight;
  
    int grainW;
    int grainH;
    
};

}} // end namespaces

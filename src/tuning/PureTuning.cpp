
// PureTuning.cpp
// Nicola Pisanti, GPLv3 License, 2016

#include "PureTuning.h"

ofParameterGroup & np::tuning::PureTuning::setup( int oscsNum, std::string name ) {
    
    ratios.resize( oscsNum );
    
    parameters.setName( name );
    parameters.add( masterPitchControl.set( "master pitch",  54,   24,    96  ) );

    int index = 0;
    
    for( RatioUI & r : ratios ){
        r.setBasePitch( masterPitchControl );  
        string label = "o";
        label += ofToString(index+1);
        label += " octave";
        parameters.add( r.octave.set( label, 1, 1, 8 ) );
        label = "o";
        label += ofToString(index+1);
        label += " numerator";
        parameters.add( r.numerator.set( label, 1, 1, 23 ) );
        label = "o";
        label += ofToString(index+1);
        label += " denominator";
        parameters.add( r.denominator.set( label, 1, 1, 23 ) );
        index++;
    }
    
    masterPitchControl.addListener( this, &PureTuning::updateAll);

    return parameters;
}


ofParameterGroup & np::tuning::PureTuning::label( std::string name ){
    parameters.setName(name);
    return parameters;
}

void np::tuning::PureTuning::updateAll( int & value ) {
    int dummy = 0;
    for( RatioUI & r : ratios ) r.ratioChange( dummy );
}

pdsp::Patchable & np::tuning::PureTuning::out_pitch(int voice) {
    return ratios[voice].pitch;
}

void np::tuning::PureTuning::smoothing( float ms ) {
    for( RatioUI & r : ratios ) r.pitch.enableSmoothing( ms );
}

float np::tuning::PureTuning::pitch( int voice )const{
    return ratios[voice].pitch.get();
}

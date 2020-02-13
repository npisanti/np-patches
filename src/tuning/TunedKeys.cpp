
#include "TunedKeys.h"

void np::tuning::TunedKeys::setup( pdsp::midi::Keys & k ){
    this->keys = &k;
    notes.resize(12);
    
    parameters.setName( "keys pitches" );

    for( size_t i=0; i<notes.size(); ++i ){
        parameters.add( notes[i].pitch.set("k "+ofToString(i)+" pitch", i, -24, 24) );
        parameters.add( notes[i].cents.set("k "+ofToString(i)+" cents", 0.0f, -1.0f, 1.0f) );
        
        notes[i].pitch.addListener(this, &np::tuning::TunedKeys::updateTuningI);
        notes[i].cents.addListener(this, &np::tuning::TunedKeys::updateTuningF);
    }
}


void np::tuning::TunedKeys::updateTuning(){

    for( int n=0; n<128; n++ ){
        keys->setTuningByPitch( n, ((n/12) * 12) + notes[n%12].pitch + notes[n%12].cents );
    }

}

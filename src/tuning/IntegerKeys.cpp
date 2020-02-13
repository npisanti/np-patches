
#include "IntegerKeys.h"

void np::tuning::IntegerKeys::setup( pdsp::midi::Keys & k ){
    this->keys = &k;
    notes.resize(12);
    
    parameters.setName( "keys frequencies" );
    parameters.add( baseNote.set("base note", 12, 12, 24 ) );
    
    for( size_t i=0; i<notes.size(); ++i ){
        parameters.add( notes[i].numerator.set("k "+ofToString(i)+" numerator", 12+i, 1, 128) );
        parameters.add( notes[i].denominator.set("k "+ofToString(i)+" denominator", 12, 1, 128) );
        
        notes[i].numerator.addListener(this, &np::tuning::IntegerKeys::updateTuning);
        notes[i].denominator.addListener(this, &np::tuning::IntegerKeys::updateTuning);
    }
    baseNote.addListener(this, &np::tuning::IntegerKeys::updateTuning);
}


void np::tuning::IntegerKeys::updateTuning( int & value ){

    for( int i=0; i<12; ++i ){
        double f = pdsp::p2f( baseNote );
        f *= double( notes[i].numerator );
        f /= double( notes[i].denominator );
        
        f *= 0.5;
        
        for( int n=i; n<128; n+=12 ){
            keys->setTuning( n, f );
            f *= 2.0;
        }
    }
}

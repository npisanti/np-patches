
// BasiVerb.cpp
// Nicola Pisanti, MIT License, 2018

#include "BasiVerb.h"

void np::effect::BasiVerb::patch(){
    
    addModuleInput( "signal", lowcut );
    addModuleOutput( "signal", gain.ch(0) );
    addModuleOutput( "0", gain.ch(0) );
    addModuleOutput( "1", gain.ch(1));

    lowcut >> reverb;
	reverb.ch(0) >> gain.ch(0);
    reverb.ch(1) >> gain.ch(1);
	
	timeControl 		>> reverb.in_time();
    densityControl 		>> reverb.in_density();
    dampingControl 		>> reverb.in_damping();
    hiCutControl 		>> reverb.in_hi_cut();
    modFreqControl 		>> reverb.in_mod_freq();
    modAmountControl 	>> reverb.in_mod_amount();
    lowCutControl       >> lowcut.in_freq();
    
    parameters.setName( "reverb" );
    parameters.add( gain.set("reverb gain", -9, -48, 12 ) );
    parameters.add( lowCutControl.set("low cut freq", 100, 20, 1000 ) );
    parameters.add( timeControl.set("rt60", 3.33f, 0.05f, 20.0f ) );
    parameters.add( densityControl.set("density", 0.5f, 0.0f, 1.0f ) );
    parameters.add( dampingControl.set("damping", 0.5f, 0.0f, 1.0f ) );
    parameters.add( hiCutControl.set("high cut freq", 5000, 3000, 20000 ) );
    parameters.add( modFreqControl.set("mod speed (hz)", 0.2f, 0.01f, 1.25f));
    parameters.add( modAmountControl.set("mod depth (ms)", 0.8f, 0.0f, 3.0f));
}

ofParameterGroup & np::effect::BasiVerb::label( string name ){
    parameters.setName( name );
    return parameters;
}

float np::effect::BasiVerb::meter_lfo() const {
    return reverb.meter_lfo();
}

pdsp::Patchable & np::effect::BasiVerb::in_signal() {
    return reverb.in_signal();
}

pdsp::Patchable & np::effect::BasiVerb::ch( size_t index ) {
    pdsp::wrapChannelIndex( index );
    switch( index ){
        case 0: default: return out("0"); break;
        case 1:          return out("1"); break;
    }
}

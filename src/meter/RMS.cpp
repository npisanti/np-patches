
#include "RMS.h"

float np::meter::RMS::meter() const { 
    return envFollower.meter_output(); 
}

ofParameterGroup & np::meter::RMS::label( string name ){
    parameters.setName( name );
    return parameters;
}

void np::meter::RMS::patch( ){
    
    addModuleInput("signal", rms ); 
    addModuleOutput("signal", envFollower );

    rms.set( 50.0f );
    
    rms >> gain >> envFollower;

    attackControl  >> envFollower.in_attack();
    releaseControl >> envFollower.in_release();
        
    parameters.setName( "rms envelope" );
    parameters.add( gain.set( "band input gain", 0, -48, 36 ));  
    parameters.add( attackControl.set( "attack ms", 5, 1, 80 ));
    parameters.add( releaseControl.set( "release ms", 20, 1, 500 ));
    
}

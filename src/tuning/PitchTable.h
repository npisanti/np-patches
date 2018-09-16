
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

// pitch table 

namespace np { namespace tuning  {

class PitchTable {

public: 
    PitchTable(){}
    PitchTable( const PitchTable & other ) { }
    
    ofParameterGroup & setup( int size, std::string name="pitch table" );

    void smoothing( float ms );

    size_t size() const;
    
    void set( int i, float pitch );

    std::vector<pdsp::Parameter> pitch;
    
    ofParameterGroup parameters;    

    ofParameterGroup & label( string name );

};

}} // end namespaces

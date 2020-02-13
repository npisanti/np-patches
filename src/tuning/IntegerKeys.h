
#pragma once

#include "ofxPDSP.h"

namespace np{ namespace tuning {

class IntegerKeys{
    struct Note{
        ofParameter<int> numerator;
        ofParameter<int> denominator;
    };

public:
    void setup( pdsp::midi::Keys & k );

    ofParameterGroup parameters;
    ofParameter<int> baseNote;
    std::vector<Note> notes;
    
private:

    pdsp::midi::Keys * keys;
    
    void updateTuning( int & value );
    
};
    
}}

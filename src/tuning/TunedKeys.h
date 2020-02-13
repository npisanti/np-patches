
#pragma once

#include "ofxPDSP.h"

namespace np{ namespace tuning {

class TunedKeys{
    struct Note{
        ofParameter<int> pitch;
        ofParameter<float> cents;
    };

public:
    void setup( pdsp::midi::Keys & k );

    ofParameterGroup parameters;
    ofParameter<int> baseNote;
    std::vector<Note> notes;
    
private:

    pdsp::midi::Keys * keys;
    
    void updateTuning();
    void updateTuningI( int & value ){ updateTuning(); }
    void updateTuningF( float & value ){ updateTuning(); }
    
};
    
}}

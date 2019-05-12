
#pragma once

#include "ofParameter.h"
#include "ofParameterGroup.h"
#include "sequencer/Sequence.h"


namespace np { namespace sequence {
    
    
class Tracker : public pdsp::Sequence{

    struct Step {
        float value;
        float chance;
    };

public:
    Tracker();
    Tracker(  const Tracker & other ){ ofLogError()<<"do not copy around sequence::Tracker"; }

    void load( std::string filepath, bool autoreload=true );
    void setMaxSteps( int max );
    ofParameterGroup & label( std::string name ){ parameters.setName( name); return parameters; }

    void setInterval( float time ){ interval=time; }
    
    ofParameterGroup parameters;
        ofParameter<int>    division;
        ofParameter<int>    steps;
        ofParameter<int>    start;
        ofParameter<bool>   regenerate; // if this parameter is true regenerate the pattern at the next iteration (and then set himself to false again)
        ofParameter<bool>   autogenerate; // regenerate the pattern after <repeat> iterations
        ofParameter<int>    repeat; // how many times the pattern will be repeated before being generate again when autogenerate is true
        
private:

    void loadFile();

    float interval = 0.05f;   
    
#if defined(__linux__) || defined(OF_TARGET_OSX) || defined(OF_TARGET_MINGW) || defined(OF_TARGET_WINVS)
    
    void watch();

    time_t writeTimestamp = 0.0f;
    float timePassed = 0.0f;;
    
    void checkFile(ofEventArgs &args);
#endif

    std::string path;

    std::vector<std::vector<std::vector<Step>>> values;
    std::atomic<int> index;
    
    bool bLoaded;
    
};

typedef Tracker PTracker;

}} // end namespaces

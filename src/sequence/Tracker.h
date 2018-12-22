
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

#if !defined(__ANDROID__) && !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_OS_IPHONE)
#include "ofxWatchFile.h"
#endif 


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



    ofParameterGroup parameters;
        ofParameter<int>    division;
        ofParameter<int>    steps;
        ofParameter<int>    start;
        ofParameter<bool>   regenerate; // if this parameter is true regenerate the pattern at the next iteration (and then set himself to false again)
        ofParameter<bool>   autogenerate; // regenerate the pattern after <repeat> iterations
        ofParameter<int>    repeat; // how many times the pattern will be repeated before being generate again when autogenerate is true
        
private:

    void loadFile();


#if !defined(__ANDROID__) && !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_OS_IPHONE)
    ofxWatchFile watcher;
    void onFileChange( ofFile &file ){
        loadFile();
    }
#endif

    std::string path;

    std::vector<std::vector<std::vector<Step>>> values;
    std::atomic<int> index;
    
    bool bLoaded;
    
};

typedef Tracker PTracker;

}} // end namespaces

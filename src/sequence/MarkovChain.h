
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

#if !defined(__ANDROID__) && !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_OS_IPHONE)
#include "ofxWatchFile.h"
#endif 

namespace np { namespace sequence  {
    
class MarkovChain : public pdsp::Sequence {
    
    struct Chain {        
        int first;
        std::vector<std::vector<float>> chances;
        std::vector<std::vector<float>> messages;
        size_t nodes;
    };
    
public:
    MarkovChain();

    void load ( std::string filepath, bool autoreload=true ); 

    ofParameter<int> division;
    ofParameter<int> steps;
    
    ofParameter<bool> regenerate;
    ofParameter<bool> autogenerate;
    ofParameter<int>  repeat;  

    ofParameter<bool> active;

    ofParameterGroup parameters;
    
    const int               getStep() const;
    const int               getState() const;
    const std::vector<int>  getStates() const;
    const int               getSize() const;

    void draw ( int x, int y ); // this is still not thread-safe, but it works well on desktop

private:
    void loadFile();

#if !defined(__ANDROID__) && !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_OS_IPHONE)
    ofxWatchFile watcher;
    void onFileChange( ofFile &file ){
        loadFile();
    }
#endif 
   
    std::string path;
   
    double div;
    double length;
    int step;

    std::atomic<int> meter_step;
    std::atomic<int> meter_state;
    
    std::atomic<int> index;
    std::vector<Chain> chains;
    
    
    int  state;
    std::vector<int>    states;

    bool run;
    
    bool bLoaded;
};
    
}} // end namespaces 

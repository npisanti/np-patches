
#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

namespace np { namespace sequence {

class Wolfram : public pdsp::Sequence {

// ----------------------------------------------------------------------------
class Automaton1D  {
    
public:

    void setup(int w, int h, int cells, int generations, int rule, bool dsp=true );
        
    void resizeGraphics(int w, int h);

    void advance() noexcept;
        
    void draw();
    void draw( int x, int y);
    
    void setRule( int rule ) noexcept;
    int  getRule( ) const;
    
    void initRandom( float density ) noexcept;
    void initCanonical() noexcept;
    void clear() noexcept;

    int cell( const int & x, const int & y ) noexcept;
    
    const int generations() const;
    const int cells() const;
    
 
private:

    vector<vector<int>> CA;    
   
    int xCells;
    int generationsNum;

    int rule;
    vector<int> wrules;
    
    int caGenerationIndex;

    float cellW;
    float cellH;
    ofFbo gridFbo;
    ofFbo cellsFbo;
    atomic<bool> updateCellsFbo;
    
    bool dsp;
};
    
    
// ----------------------------------------------------------------------------
public:
    Wolfram();

    void setup( int maxSteps, int maxOutputs, int generations, int caSide, int totalHeight, std::string name="wolfram sequence" );
    
    void draw(int x, int y);

    ofParameter<int>     threshold;
    ofParameter<int>     activeOuts;
    ofParameter<int>     steps;
    ofParameter<int>     rule;
    ofParameter<int>     division;
    ofParameter<float>   seedsDensity;
    ofParameter<bool>    reverse;

    ofParameter<float>   gateLen;
    ofParameter<bool>    regenerate;
    ofParameter<bool>    gateOff;
            
    vector<float>   stepbars;    
    Automaton1D     ca;

    ofParameterGroup parameters;
    
    ofParameterGroup & label( std::string name );

private:

    atomic<int>     actionCode; // 0 advance | 1 clear | 2 random | 3 random & fill

    ofFbo   stepbarsFbo;

    int     cah;
    int     caSide;
    int     barHeight;
    
    float   gate;

    int     storedRule;

    int     activeOutsStored;     

    int     maxSteps; // number of steps for each output
    int     maxOutputs;
    
    int     thresholdStored;
    
    double  sequenceLength; // division * steps
    float   scaling;

    vector<float> values; // support structure for calculating linear max

};

}} // end namespaces

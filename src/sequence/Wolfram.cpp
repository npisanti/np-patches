
// Wolfram.cpp
// Nicola Pisanti, MIT License, 2018

#include "Wolfram.h"


void np::sequence::Wolfram::setup( int maxSteps, int maxOutputs, int generations, int caSide, int totalHeight, std::string name) {
    
    this->caSide = caSide;
    int caw = caSide * maxOutputs * maxSteps;
    this->cah = caSide * generations;
    this->maxSteps = maxSteps;
    this->maxOutputs = maxOutputs;
    this->bars = ((double) maxSteps) / division;

    barHeight = totalHeight - cah - 40;
    
    values.resize(maxOutputs);
    stepbars.resize(maxOutputs*maxSteps);

    pdsp::Sequence::label = name;
    parameters.setName( name );
    parameters.add( this->rule.set( "rule", 60, 0, 150 ));
    parameters.add( this->steps.set( "steps", 16, 0, maxSteps ));
    parameters.add( this->division.set( "division", 16, 0, 32 ));
    parameters.add( this->activeOuts.set( "active outs", maxOutputs, 0, maxOutputs ));
    parameters.add( this->threshold.set( "threshold", 4, 0, 8 ));
    parameters.add( this->seedsDensity.set( "seeds density", 0.33f, 0.0f, 1.0f ));
    parameters.add( this->reverse.set( "reverse steps", false) );
    parameters.add( this->gateOff.set( "send gate off", false) );
    parameters.add( this->gateLen.set( "gate len", 0.5f, 0.001f, 0.999f) );
    parameters.add( this->regenerate.set( "regenerate", false) );


    this->storedRule = rule;
    actionCode = 3; // init

    this->thresholdStored = -1;
    activeOutsStored = maxOutputs;
 
    gate = 0.5f;
    
    ca.setup( caw, cah, maxSteps * maxOutputs, generations, rule );
    
    stepbarsFbo.allocate(caw+2, barHeight + 20 + 2);
    stepbarsFbo.begin();
        ofClear(0, 0, 0, 0);
    stepbarsFbo.end();
}


ofParameterGroup & np::sequence::Wolfram::label( std::string name ){
    parameters.setName( name );
    pdsp::Sequence::label = name;
    return parameters;
}


np::sequence::Wolfram::Wolfram(){
    
    // it is useful to have this inside the constructor
    // otherwise it could happen that the lambda take references not to the internal members
    // but to to arguments of the setup function, giving an UNDEFINED behavior
    
    code = [&] () noexcept {
        
        int nextRule = rule;
        if(nextRule != storedRule){
            ca.setRule( nextRule );
            storedRule = nextRule;
        }
        
        int newCode = actionCode;
        
        if(regenerate){
            newCode = 3;
            regenerate = false;
        }
        
        switch( newCode ){
            
            case 0: 
                ca.advance();
                break;

            case 1:
                ca.clear();
                actionCode = 0;
                break;
                
            case 2:
                ca.initRandom(seedsDensity);
                actionCode = 0;
                break;
                
            case 3:
                ca.initRandom(seedsDensity);
                for(int i=0; i<ca.generations(); ++i) ca.advance();
                actionCode = 0;
                break;
                
            default: break;
        }
        
        int nextThreshold = threshold;
        if(nextThreshold != thresholdStored){
            thresholdStored = nextThreshold;
            float gen = ca.generations();
            scaling = gen / (float) (gen-thresholdStored);
        }
    
        if(activeOutsStored != activeOuts){
            activeOutsStored = activeOuts;
        } 
        
        gate = gateLen;

        // wolfram to seqs ---------------------------------
        int b;
        if(reverse){
            b = stepbars.size()-1;
        }else{
            b=0;
        }
        for(int x=0; x < ca.cells(); ++x){

            int sum = -thresholdStored;
            for(int y = 0; y<ca.generations(); ++y){
                sum += ca.cell(x, y);
            }
            if(sum<0) sum = 0;            
          
            float intensity = (float)sum / (float)ca.generations(); 
            intensity *= scaling;
            if(intensity > 1.0f) intensity = 1.0f;

            stepbars[b] = intensity;
            
            if(reverse){
                b--;
            }else{
                b++;
            }
        }
        
        
        // seqs array to messages ---------------------------
        bars = double(steps) / double(division);
        steplen = 1.0 / double(division);
        
        begin();       
            for(int x=0; x<steps; ++x){
                
                for(int out=0; out < activeOutsStored; ++out) {
                    values[out] = stepbars[ (out*steps) + x ];
                }
                
                for(int out=0; out < activeOutsStored; ++out){
                    if(values[out]!=0.0f){
                         message( (double)(x), values[out], out);               // gate on
                         if(gateOff) message( (double)(x) + gate, 0.0f, out);   // gate off
                    }
                }
            }
        end();
    };    
    
}


void np::sequence::Wolfram::draw(int x, int y) {
    
    // update stepbars graphics
    stepbarsFbo.begin();
        ofClear(0, 0, 0, 0);
        ofTranslate(1,1);

        float playhead = meter_percent(); 
        
        int sequenceSteps = steps;
        
        for(int x =0; x<maxOutputs*maxSteps; ++x){
            ofSetColor( 100 );
            ofNoFill();
            ofDrawRectangle(caSide*x, 0, caSide, barHeight); 
            
            if( (x < sequenceSteps*activeOutsStored ) &&
                ((int) (playhead * sequenceSteps)) == x%sequenceSteps ){
                ofSetColor( 255 ); 
            }
            ofFill();
            
            float height = stepbars[x] * barHeight;
            float yy = barHeight - height;
            ofDrawRectangle(caSide*x, yy, caSide, height); 
       
        }
        
        ofSetColor( 255 ); 
        ofTranslate(0, barHeight);  
        
        // draw the playhead and envelope meters

        ofNoFill();
        float playheadW = caSide*sequenceSteps;
        
        for(int i=0; i<maxOutputs; ++i){
            
            if( i < activeOutsStored) {
                ofSetColor( 255 ); 
                
                float x = playhead * playheadW;
                ofDrawLine(x, 0, x, 20);                                 
            }
            
            string label = "out ";
            label+= to_string(i);
            ofDrawBitmapString(label, 5, 15);

            ofSetColor( 100 );
            ofDrawRectangle( 0, 0, playheadW, 20 );
            
            ofTranslate(playheadW,0);
        }
    stepbarsFbo.end();
    
    // draw everything
    ofPushMatrix();
        ofTranslate(x, y);
        
        ca.draw(0, 0);
        
        ofTranslate(0, cah+20);
        
        stepbarsFbo.draw(0, 0);
    
    ofPopMatrix();
}


//-----------------------------------------------------------------------------
void np::sequence::Wolfram::Automaton1D::setup(int w, int h, int cells, int generationsNum, int rule, bool dsp ){

    xCells = cells;
    this->generationsNum = generationsNum;
    this->dsp = dsp;
    
    wrules.resize(8);
    setRule( rule );

    caGenerationIndex = 0;
    CA.resize(generationsNum); // 8 different generatiosn
    for( vector<int> & v: CA ){
        v.resize(cells); 
    }
    
    resizeGraphics( w, h );
}

void np::sequence::Wolfram::Automaton1D::resizeGraphics(int w, int h) {
    gridFbo.allocate(w, h, GL_RGBA);
    gridFbo.begin();
        ofClear(255,255,255, 0);
        ofTranslate( 1, 1);
        cellW = (float)(w-2) / (float)xCells; 
        cellH = (float)(h-2) / (float)generationsNum; 
        ofSetColor(100);
        ofNoFill();
        for(int y = 0; y<generationsNum; ++y){
            for(int x = 0; x<xCells; ++x){
                ofDrawRectangle( x*cellW, y*cellH, cellW, cellH);
            }
        }
        
    gridFbo.end();
    
    cellsFbo.allocate(w, h, GL_RGBA);
    cellsFbo.begin();
        ofClear(255,255,255, 0);
    cellsFbo.end();
  
    updateCellsFbo = true;
    
}
    
int np::sequence::Wolfram::Automaton1D::cell( const int & x, const int & y ) noexcept {
    return CA[y][x];
}

void np::sequence::Wolfram::Automaton1D::setRule( int rule ) noexcept { // init wolfram rules from number bits
    this->rule = rule;
    int rulebits = rule;
    for(int i=0; i<8; ++i){ // inits the rules
        wrules[i] = rulebits & 0x0001;  // take just the lowest bit
        rulebits = rulebits>>1;             // bitshift by one
    }
    //ruleChanged = false;
}

void np::sequence::Wolfram::Automaton1D::advance() noexcept{ // update the automaton
    
    int oldGeneration = caGenerationIndex;
    caGenerationIndex--;
    if(caGenerationIndex < 0){
        caGenerationIndex = generationsNum - 1;
    }
    
    int max = (int) (CA[caGenerationIndex].size());
    for(int x = 0; x < max; ++x){
        int xprev = x-1;
        if (xprev==-1) xprev = max-1;
        int xnext = x+1;
        if(xnext==max) xnext = 0;
        
        int cellRule = CA[oldGeneration][xprev] * 4 
                    + CA[oldGeneration][x] * 2 
                    + CA[oldGeneration][xnext];
                    
        CA[caGenerationIndex][x] = wrules[cellRule]; 
    }     
    updateCellsFbo = true;
    
}

void np::sequence::Wolfram::Automaton1D::draw(){
        
    if( updateCellsFbo ){
        cellsFbo.begin();
            ofClear(255,255,255, 0);
            ofTranslate(1,1);
            ofFill();
            ofSetColor(255);
            int genIndex = caGenerationIndex;
            for(int y = 0; y<generationsNum; ++y){
                for(int x = 0; x<xCells; ++x){
                    if(CA[genIndex][x]==1){
                        ofDrawRectangle( x*cellW, y*cellH, cellW, cellH);
                    }
                }
                genIndex++; 
                if(genIndex==generationsNum) genIndex = 0;
            }        

        cellsFbo.end();

        updateCellsFbo = false;
    }

    cellsFbo.draw(0, 0);
    gridFbo.draw( 0, 0);
}

void np::sequence::Wolfram::Automaton1D::initRandom( float density ) noexcept{
    for( vector<int> & v: CA ){
        for( int i=0; i < (int)v.size(); ++i){
            v[i] = 0;
        }
    }        
    
    if(dsp){ // used in the dsp loop
        for( int x=0; x < (int)(CA[caGenerationIndex].size()); ++x ){
            // pdspChange(float value) controls the chance of having an alive cell
            if(x%2==0){
                CA[caGenerationIndex][x] = pdsp::chance(density) ? 1 : 0;                     
            }else{
                CA[caGenerationIndex][x] = 0;          
            }
        }        
    }else{ // used inside the update() loop
        for( int x=0; x < (int)(CA[caGenerationIndex].size()); ++x ){
            // pdspChange(float value) controls the chance of having an alive cell
            if(x%2==0){
                if(ofRandomuf() < density ){
                    CA[caGenerationIndex][x] = 1;
                }else{
                    CA[caGenerationIndex][x] = 0;
                }                 
            }else{
                CA[caGenerationIndex][x] = 0;          
            }
        }        
    }
    // always also make the first active
    CA[caGenerationIndex][0] = 1;   
    updateCellsFbo = true;
}

void np::sequence::Wolfram::Automaton1D::clear() noexcept{
    for( vector<int> & v: CA ){
        for( int i=0; i < (int)v.size(); ++i){
            v[i] = 0;
        }
    }    
    updateCellsFbo = true;    
}


void np::sequence::Wolfram::Automaton1D::initCanonical() noexcept{
    initRandom( 0.25f );
}


const int np::sequence::Wolfram::Automaton1D::generations() const {
    return generationsNum;
}

const int np::sequence::Wolfram::Automaton1D::cells() const {
    return xCells;
}


void np::sequence::Wolfram::Automaton1D::draw( int x, int y) {
    ofPushMatrix();
    ofTranslate( x, y );
        draw();
    ofPopMatrix();
}
    
int np::sequence::Wolfram::Automaton1D::getRule( ) const {
    return rule;
}
    



// MarkovChain.cpp
// Nicola Pisanti, MIT License, 2018

#include "MarkovChain.h"

#if defined(__linux__) || defined(OF_TARGET_OSX) || defined(OF_TARGET_MINGW) || defined(OF_TARGET_WINVS)
    #include <boost/filesystem.hpp>
#endif 

np::sequence::MarkovChain::MarkovChain(){
    
    chains.resize(12); // 12 chains buffer
    
    for( auto & chain : chains ){
        chain.first = 0;
        chain.messages.reserve(12);
        for ( auto & m : chain.messages ){
            m.reserve(8);
        }
        chain.chances.reserve(12);
        for( auto & p : chain.chances ){
            p.reserve(12);
        }
        chain.nodes = 0;
    }
    // just one message going to itself
    chains[0].messages.resize(1);
    chains[0].messages[0].resize(1);
    chains[0].messages[0][0] = 0.0f;
    chains[0].chances.resize(1);
    chains[0].chances[0].resize(1);
    chains[0].chances[0][0] = 1.0f;    
    chains[0].nodes = 1;    
    
    index = 0;
    
    step = 0;
    states.resize( 64 );
    for(auto & value : states ) value = 0;
    meter_step = step;
    meter_state = 0;
    bLoaded = false;

    std::string name = "markov chain"; 
    parameters.setName( name );
    pdsp::Sequence::label = name;
    
    parameters.add( this->steps.set( "steps", 16, 0, 64 ));
    parameters.add( this->division.set( "division", 16, 0, 32 ));
    parameters.add( this->autogenerate.set( "autogenerate", false ) );
    parameters.add( this->repeat.set( "ag counter", 1, 1, 16) );
    parameters.add( this->regenerate.set( "regenerate", true) );
    parameters.add( this->active.set( "active", true) );
    this->div = division ;


    run = active;
    
    code = [&] () noexcept {

            if( counter()==0 ) step=0;
            
            if( regenerate || (autogenerate && (counter()==0 || counter()>=int(repeat*states.size()) ) ) ){
                step = 0;
                div = division;
                length = 1.0 / (double)division;  
                states.resize(steps);
                resetCount();
                regenerate = false;
            }

            if( counter()%(int)div == 0 ){ // update each bar
                run = active;
            }
            
            int read = index;
            
            steplen = 1.0 / double(div);
            bars = length;
            begin();
                // markov chain code ------------------------
                if( step==counter() ){   // step is == counter only once until resetCount() is called  
                    if(step == 0 ) { 
                        state = chains[read].first;
                    }else{
                        // markov chain next
                        float outcome = pdsp::urand();
                        float target = 0.0f;
                        int next = chains[read].nodes -1;
                        
                        for( size_t i=0; i<chains[read].nodes; ++i ){
                            target += chains[read].chances[state][i];
                            if( outcome < target ){
                                next = i;
                                break;
                            }
                        }
                        state = next;
                    }
                    states[step] = state;
                }else{
                    state = states[step]; // we reiterate on already generated values
                }
                 
                // --------------------------------------------
                if( run ){
                    // write messages                    
                    for(int i=0; i<(int)chains[read].messages[state].size(); ++i){
                        out( i );
                        bang( chains[read].messages[state][i] );
                    }
                    
                    meter_step = step;
                    meter_state = state;
                }
                
                step++;
                if(step == (int)states.size()) step = 0;
            end();
     };
    
}


void np::sequence::MarkovChain::load ( std::string filepath, bool autoreload ) {
    
    path = filepath;
    auto v = ofSplitString(path, "/" );
    string name = v[v.size()-1];
    
    parameters.setName(name);
    pdsp::Sequence::label = name;
    
#if !defined(__ANDROID__) && !defined(TARGET_IPHONE_SIMULATOR) && !defined(TARGET_OS_IPHONE)
    if ( autoreload ){
        watch();
    }else{
        loadFile();
    }
#else
    loadFile();
#endif
}


const int np::sequence::MarkovChain::getStep() const {
    return meter_step;
}

const int np::sequence::MarkovChain::getState() const {
    return meter_state;
}

const std::vector<int> np::sequence::MarkovChain::getStates() const {
    return states;
}

const int np::sequence::MarkovChain::getSize() const {
    return states.size();
}

void np::sequence::MarkovChain::draw ( int x, int y ) {
    std::string theorem = parameters.getName();
    theorem +=": ";
    for (size_t i=0; i<states.size(); i++){
        theorem += ofToString( states[i] );
    }
    ofDrawBitmapString(theorem, x, y );   
}


// -------------------- file loading routines ------------------------

void np::sequence::MarkovChain::loadFile() {    
	
	ofFile file( path );
	
	if(!file.exists()){
		ofLogError("The file " + path + " is missing");
        return;
	}
    
    ofBuffer buffer(file);

    int countLines = 0;

	for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        if( line[0] != '#' ){
            countLines++; // true line 
        }
    }
    
    size_t write = index+1;
    if(write>=chains.size()){ write = 0; }
    chains[write].first = 0;
    chains[write].messages.clear();
    chains[write].chances.clear();
        
    int mode = 0;
        
	//Read file line by line	
	for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        line += " "; // to be sure that parsing is completed 
        
        if( line[0] != '#' && line.length()>1 ){ // line is not a comment

            if( line[0] == '(' ){
                if(line[1] == 'f'){
                    mode = 0;
                }
                if(line[1] == 'm'){
                    mode = 1;
                }
                if(line[1] == 'c'){
                    mode = 2;
                }
            }else{
                
                string toParse = "";
                bool parsing = false;            
                bool firstvalue = true;                
                
                for(size_t l=0; l<line.length(); ++l){
                    
                    switch( line[l] ){
                        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.': 
                            toParse += (char) line[l];
                            if(!parsing) parsing = true;
                        break;
                        
                        case ',': 
                            toParse += ".";
                            if(!parsing) parsing = true;
                        break;
                        
                        case '-': case 'o': case '~': case '|':
                            if(!parsing) parsing = true;
                            toParse = "0";
                        break;
                        
                        default:
                            if(parsing){
                                switch( mode ){
                                    case 0: // set up first
                                        chains[write].first = ofToInt( toParse );
                                    break;
                                    
                                    case 1: // set up messages
                                        if(firstvalue){
                                            chains[write].messages.emplace_back();
                                            chains[write].messages.back().clear();
                                            firstvalue = false;
                                        }
                                        chains[write].messages.back().push_back( ofToFloat(toParse));
                                    break;
                                    
                                    case 2: // set up chances
                                        if(firstvalue){
                                            chains[write].chances.emplace_back();
                                            chains[write].chances.back().clear();
                                            firstvalue = false;
                                        }
                                        chains[write].chances.back().push_back( ofToFloat(toParse));
                                    break;
                                }
                                
                                toParse = "";
                                parsing = false;
                            }
                        break;
                    }
                }   
            }
            
        } // end parsing line
        

	}// end reading lines 

    // now do a sanity check of the written chain 
    chains[write].nodes = chains[write].chances.size();
    for( size_t i=chains[write].messages.size(); i<chains[write].nodes; ++i ){
        chains[write].messages.emplace_back();
        chains[write].messages.back().clear();
    }
    // makes the chances a perfect square
    for( size_t i=0; i<chains[write].chances.size(); ++i){
        for( size_t k=chains[write].chances[i].size(); k<chains[write].nodes; ++k ){
            chains[write].chances[i].push_back( 0.0f );
        }
    }

    // now normalize chances
    for( size_t i=0; i<chains[write].chances.size(); ++i){
        float sum = 0.0f;
        for( size_t k=0; k<chains[write].chances[i].size(); ++k ){
            sum += chains[write].chances[i][k];
        }
        if( sum!=0.0f && sum!=1.0 ){
            float divide = 1.0f/sum;
            for( size_t k=0; k<chains[write].chances[i].size(); ++k ){
                chains[write].chances[i][k] *= divide;
            }
        }
    }    

    bLoaded = true;

    index = write;
}

#if defined(__linux__) || defined(OF_TARGET_OSX) || defined(OF_TARGET_MINGW) || defined(OF_TARGET_WINVS)

void np::sequence::MarkovChain::watch(){
    ofFile file( path );
    
    if( file.exists() ){
        ofAddListener( ofEvents().update, this, &np::sequence::MarkovChain::checkFile );
        writeTimestamp = boost::filesystem::last_write_time( path );    
        loadFile();
    }
}

void np::sequence::MarkovChain::checkFile(ofEventArgs &args){
    timePassed += ofGetLastFrameTime();
    if( timePassed > interval ){
        time_t checkTimestamp = boost::filesystem::last_write_time( path );
        if( checkTimestamp != writeTimestamp){
            writeTimestamp = checkTimestamp;
            loadFile();
            ofLogNotice() << "["<<path<<"] file reloaded";
        }
    }
}

#endif

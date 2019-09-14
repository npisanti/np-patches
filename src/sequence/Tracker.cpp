
#include "Tracker.h"
#include "ofMain.h"

#if defined(__linux__) || defined(OF_TARGET_OSX) || defined(OF_TARGET_MINGW) || defined(OF_TARGET_WINVS)
    #include <boost/filesystem.hpp>
#endif 

np::sequence::Tracker::Tracker() {
    
    parameters.setName("ptracker");
    parameters.add( division.set("division", 16, 1, 32) );
    parameters.add( steps.set("steps", 16, 2, 64) );
    parameters.add( start.set("step start", 0, 0, 64) );
        
    parameters.add( autogenerate.set( "autogenerate", true ) );
    parameters.add( repeat.set( "ag counter", 1, 1, 16) );
    parameters.add( regenerate.set( "regenerate", false) );    
    
    bLoaded = false;
    // we already get some memory, for avoid allocating during audio processing
    values.resize( 12 ); // 12 buffer for editing
    for( auto & a : values ){
        a.resize(128);
        for( auto & b : a ){
            b.resize(16);
            for( auto & v : b ){
                v.value = -1.0f;
                v.chance = 1.0f;
            }
        }
    }
    
    index = 0;

    code = [&] () noexcept {
        if( autogenerate && (counter() >= repeat) ){ resetCount(); } 
        if( regenerate ){ resetCount(); }
        
        if( regenerate || (autogenerate && counter() == 0) ){

            double length = (double)steps / division;
            int read = index; // memorize the index from which we read, as index can be changed by another thread
            
            bars = length;
            double d = division;
            this->begin();
                if( !values.empty() && bLoaded ){
                    for( int i=start; i<(int)values[read].size() && i<(steps+start); ++i ) {
                        for( size_t o=0; o<values[read][i].size() && i<(steps+start); ++o ) {
                            if( ( values[read][i][o].chance>=1.0f || pdsp::chance(values[read][i][o].chance))
                                 && values[read][i][o].value >= 0.0f ){
                                this->out(o).delay((i-start)/d).bang( values[read][i][o].value );
                            } 
                        }                        
                    }
                }
            this->end();
            
            regenerate = false;
        }
    };
}

void np::sequence::Tracker::setMaxSteps( int max ){
    steps.set("steps", steps, 2, max );
    start.set("step start", start, 0, max );
}

void np::sequence::Tracker::load( std::string filepath, bool autoreload ) {
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

void np::sequence::Tracker::loadFile() {
    
	//Load file placed at the given path
	ofFile file( path );
	
	if(!file.exists()){
		ofLogError("The file " + path + " is missing");
        bLoaded = false;
        return;
	}
    
    ofBuffer buffer(file);

    int write = index + 1; // we operate on the next vector of the circular buffer
    if (write>=(int)values.size()){ write=0; }
    int trueLines = 0;

	for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        if( line[0] != '#' ){
            trueLines++;
        }
    }
    start.setMax( trueLines-1 );
    
    values[write].resize(trueLines);
    for( auto & a : values[write] ){
        a.resize(64);
        for( auto & val : a ){
            val.value = -1.0f;
            val.chance = 1.0f;
        }
    }

    int i = 0;
	//Read file line by line
	for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        line += " "; // to be sure that parsing is completed 

        if( line[0] != '#' && line.length()>1 ){
            int o=0;
            
            string parsedValue = "";
            bool bParsingValue = false;            

            string parsedChance = "";
            bool bParsingChance = false;            

            for( size_t l=0; l<line.length(); ++l ){
                switch( line[l] ){
                    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': case '.': 
                        if( bParsingChance ){
                            parsedChance += (char) line[l];
                        }else{
                            parsedValue += (char) line[l];
                            if(!bParsingValue) bParsingValue = true;
                        }
                    break;
                    
                    case 'o': 
                        if( bParsingChance ){
                            parsedChance += '0';
                        }else{
                            parsedValue += '0';
                            if(!bParsingValue) bParsingValue = true;
                        }
                    break;
                    
                    case 'x':
                       if(bParsingValue){
                            values[write][i][o].value = ofToFloat( parsedValue );
                            parsedValue = "";                            
                            bParsingValue = false;
                            parsedChance = "";
                            bParsingChance = true;
                        }
                    break;
                    
                    case '-': case '~': case '|': case '_': 
                        values[write][i][o].value = -1.0f;
                        o++;
                    break;
                    
                    default:
                        if(bParsingValue){
                            values[write][i][o].value = ofToFloat( parsedValue );
                            o++;
                            parsedValue = "";
                            bParsingValue = false;
                        }
                        if(bParsingChance){
                            values[write][i][o].chance = ofToFloat( parsedChance );
                            o++;
                            bParsingChance = false;
                        }
                    break;
                }
            }
            i++;
        }
	}
    
    index = write; // everything done, update the index
    bLoaded = true;
}


#if defined(__linux__) || defined(OF_TARGET_OSX) || defined(OF_TARGET_MINGW) || defined(OF_TARGET_WINVS)

void np::sequence::Tracker::watch(){
    ofFile file( path );
    
    if( file.exists() ){
        ofAddListener( ofEvents().update, this, &np::sequence::Tracker::checkFile );
        writeTimestamp = boost::filesystem::last_write_time( path );    
        loadFile();
    }
}

void np::sequence::Tracker::checkFile(ofEventArgs &args){
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


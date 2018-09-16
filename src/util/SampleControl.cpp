
// SampleControl.cpp
// Nicola Pisanti, MIT License, 2018

#include "SampleControl.h"

int np::util::SampleControl::number = 0;

void np::util::SampleControl::patch (){
    parameters.setName( "please label me " + ofToString(number++));
    parameters.add( loadButton.set( "load", false ));
    parameters.add( sampleName.set("sample", "no sample"));
    parameters.add( samplePath.set("path", "no path"));    
    
    normalize = true;
    
    loadButton.addListener(this, &SampleControl::loadButtonCall );
    samplePath.addListener(this, &SampleControl::sampleChangedCall );
}


ofParameterGroup & np::util::SampleControl::label( std::string name ){
    parameters.setName( name );
    return parameters;
}

void np::util::SampleControl::loadButtonCall( bool & value ) {
   
    if(value){
        
        //Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("select an audio sample"); 
        
        //Check if the user opened a file
        if (openFileResult.bSuccess){
            
            string path = openFileResult.getPath();
            
            samplePath = path;
        
            ofLogVerbose("file loaded");
            
        }else {
            ofLogVerbose("User hit cancel");
        }
        
        loadButton = false;
    }
}

void np::util::SampleControl::sampleChangedCall( string & value ) {
    
    ofLogVerbose( "loading sample " + value );
    loadSample( samplePath );    

    auto v = ofSplitString(samplePath, "/" );
    sampleName = v[v.size()-1];
    
}
    
void np::util::SampleControl::loadSample( string path ) {
    sample.load( path );
    if (normalize){
        sample.normalize();
    }
    graphics.setWaveform(sample, 0, ofColor(90), 1024, 512);
}

void np::util::SampleControl::load( string path ) {
    samplePath = path;
}

bool np::util::SampleControl::loading() const {
    return loadButton;
}

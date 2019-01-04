
#include "StereoDelay.h"

void np::effect::StereoDelay::patch() {
    
    delay0.setMaxTime(3000.0f);
    delay1.setMaxTime(3000.0f);
    
    channels.resize(2);
    
    addModuleInput("0", channels[0] );
    addModuleInput("1", channels[1] );
    addModuleOutput("0", channels[0] );
    addModuleOutput("1", channels[1] );

    channels[0].input >> inputFader.ch(0) >> delay0 >> outputFader.ch(0) >> channels[0].output;
    channels[1].input >> inputFader.ch(1) >> delay1 >> outputFader.ch(1) >> channels[1].output;
    
    speed >> phazor.in_freq() >> LFO >> modAmt;
  
                                    modAmt >> delay0.in_time();
    time * (1.f/16.f) >> lDelayTimeControl >> delay0.in_time();
                            modAmt * -1.0f >> delay1.in_time();
    time * (1.f/16.f) >> rDelayTimeControl >> delay1.in_time();

    lFeedbackControl >> delay0.in_feedback();
    rFeedbackControl >> delay1.in_feedback();
    dampingControl >> delay0.in_damping();
    dampingControl >> delay1.in_damping();

    parameters.setName("stereo delay");

    parameters.add( inputFader.set("input gain", -15, -48, 12) );
    
    parameters.add( lDelayTimeControl.set("L 16th time", 4, 1, 24) );
    parameters.add( lFeedbackControl.set("L feedback", 0.4f, 0.0f, 1.0f));
    parameters.add( rDelayTimeControl.set("R 16th time", 6, 1, 24) );
    parameters.add( rFeedbackControl.set("R feedback", 0.4f, 0.0f, 1.0f));   
    parameters.add( dampingControl.set("damping", 0.5f, 0.0f, 1.0f));   
   
    parameters.add(speed.set("mod speed (hz)", 0.25f, 0.05f, 4.0f));
    parameters.add(modAmt.set("mod depth (ms)", 0.0f, 0.0f, 8.0f));
       
    parameters.add( outputFader.set("output gain", 0, -48, 12) );

    inputFader.enableSmoothing(50.f);
    outputFader.enableSmoothing(50.f);
    lDelayTimeControl.enableSmoothing( 500.f );
    lDelayTimeControl.enableSmoothing( 500.f );
    lFeedbackControl.enableSmoothing(50.f);
    rFeedbackControl.enableSmoothing(50.f);
    modAmt.enableSmoothing( 50.f );
}


ofParameterGroup & np::effect::StereoDelay::label( string name ){
    parameters.setName( name );
    return parameters;
}


pdsp::Patchable & np::effect::StereoDelay::ch( size_t index ) {
    pdsp::wrapChannelIndex( index );
    return channels[index];
}

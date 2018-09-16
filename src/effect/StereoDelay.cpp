
#include "StereoDelay.h"


void np::effect::StereoDelay::patch() {
    
    addModuleInput("0", input.in_0() );
    addModuleInput("1", input.in_1() );
    addModuleOutput("0", output.out_0() );
    addModuleOutput("1", output.out_1() );

    input.out_L() >> ldelay >> output.in_L();
    input.out_R() >> rdelay >> output.in_R();


    speed >> phazor.in_freq() >> LFO >> modAmt;
    
                                    modAmt >> ldelay.in_time();
    time * (1.f/16.f) >> lDelayTimeControl >> ldelay.in_time();
                            modAmt * -1.0f >> rdelay.in_time();
    time * (1.f/16.f) >> rDelayTimeControl >> rdelay.in_time();


    lFeedbackControl >> ldelay.in_feedback();
    rFeedbackControl >> rdelay.in_feedback();
    dampingControl >> ldelay.in_damping();
    dampingControl >> rdelay.in_damping();

    parameters.setName("stereo delay");

    parameters.add( input.set("input gain", -15, -48, 12) );
    
    parameters.add( lDelayTimeControl.set("L 16th time", 4, 1, 24) );
    parameters.add( lFeedbackControl.set("L feedback", 0.4f, 0.0f, 1.0f));
    parameters.add( rDelayTimeControl.set("R 16th time", 6, 1, 24) );
    parameters.add( rFeedbackControl.set("R feedback", 0.4f, 0.0f, 1.0f));   
    parameters.add( dampingControl.set("damping", 0.5f, 0.0f, 1.0f));   
   
    parameters.add(speed.set("mod speed (hz)", 0.25f, 0.05f, 4.0f));
    parameters.add(modAmt.set("mod depth (ms)", 0.0f, 0.0f, 8.0f));
       
    parameters.add( output.set("output gain", 0, -48, 12) );

    input.enableSmoothing(50.f);
    output.enableSmoothing(50.f);
    lDelayTimeControl.enableSmoothing(450.f);
    lDelayTimeControl.enableSmoothing(450.f);
    lFeedbackControl.enableSmoothing(50.f);
    rFeedbackControl.enableSmoothing(50.f);

}


ofParameterGroup & np::effect::StereoDelay::label( string name ){
    parameters.setName( name );
    return parameters;
}


pdsp::Patchable & np::effect::StereoDelay::in_L() {
    return in("0");
}

pdsp::Patchable & np::effect::StereoDelay::in_R() {
    return in("1");
}

pdsp::Patchable & np::effect::StereoDelay::out_L() {
    return out("0");
}

pdsp::Patchable & np::effect::StereoDelay::out_R() {
    return out("1");
}



// Euclidean.cpp
// Nicola Pisanti, MIT License, 2018

#include "Euclidean.h"


np::sequence::Euclidean::Euclidean(){

    //values.reserve(256);    
    values.resize(256);
    for(float & value : values ) value = 0.0f;

    string name = "euclidean sequence";
    pdsp::Sequence::label = name;
    parameters.setName( name );
    
    parameters.add( this->steps.set( "steps", 16, 1, 64 ));
    parameters.add( this->division.set( "division", 16, 1, 32 ));
    parameters.add( this->x.set( "x", 0.8f, 0.0f, 1.0f ) );
    parameters.add( this->xJit.set( "x jit", 0.2f, 0.0f, 1.0f ) );
    
    parameters.add( this->y.set( "y", 0.2f, 0.0f, 1.0f ) );
    parameters.add( this->yJit.set( "y jit", 0.1f, 0.0f, 1.0f ) );

    parameters.add( this->ghostsChance.set( "ghost chance", 0.0f, 0.0f, 1.0f ) ); 
    parameters.add( this->ghostsDynMin.set( "ghost min dyn", 0.1f, 0.0f, 1.0f) );
    parameters.add( this->ghostsDynMax.set( "ghost max dyn", 0.4f, 0.0f, 1.0f) );
    parameters.add( this->seqShift.set( "shift", 0, -6, 6 ) );
    
    parameters.add( this->autogenerate.set( "autogenerate", false ) );
    parameters.add( this->repeat.set( "ag counter", 1, 1, 16) );
    parameters.add( this->regenerate.set( "regenerate", true) );    
    
    parameters.add( this->gate.set("gate", 0.5f, 0.0001f, 1.0f ) );
    
    lastX = lastY = -1;
    
    code = [&] () noexcept {
        
        if( autogenerate && (counter() >= repeat) ){ resetCount(); } 
        
        if( regenerate ){ resetCount(); }
        
        if( regenerate || (autogenerate && counter() == 0) ){

            double g = gate;    

            // all this code to avoid repetition of same loop if possible -----
            int newXMin = (x-xJit)*steps;
            int newXMax = (x+xJit)*steps;
            int newYMin = (y-yJit)*steps;
            int newYMax = (y+yJit)*steps;

            newXMin = (newXMin > 0) ? newXMin : 0;            
            newYMin = (newYMin > 0) ? newYMin : 0;            
            int clip = steps;
            newXMax = (newXMax<clip) ? newXMax : clip;
            newYMax = (newYMax<clip) ? newYMax : clip;
            
            int m, n;
            if( newXMin == newXMax && newYMin == newXMax ){
                m = newXMin;
                n = newYMin;
            }else if( newXMin == newXMax ){
                m = newXMin;
                n = lastY;
                while( n==lastY ){
                    n = pdsp::dice( newYMin, newYMax+1 );
                }
            }else if( newYMin == newYMax ){
                m = lastX;
                while( m==lastX ){
                    m = pdsp::dice( newXMin, newXMax+1 );
                }
                n = newYMin;
            }else{
                m = lastX;
                n = lastY;
                while( m==lastX && n==lastY ){
                    m = pdsp::dice( newXMin, newXMax+1 );
                    n = pdsp::dice( newYMin, newYMax+1 );
                }
            }
            lastX = m;
            lastY = n;
            // end of code for avoiding repetitions ---------------------------
            // even like this in rare cases the same 4/4 pattern is repeated
            // probably some combination produces the same pattern
            // like this it's cheaper on the CPU, so it's fine
                
            cword( values, m, n, true);
            
            float gmin = ghostsDynMin;
            float gadd = ghostsDynMax - gmin; 
            ghost_chance( values, gmin + pdsp::urand()*gadd, ghostsChance);
            
            shift( values, seqShift );
            
            bars = double(steps) / double(division);

            double d = division;
            begin();
                for( size_t i=0; i<values.size(); ++i){
                    if(values[i]>0.0f){
                        delay( i / d ).bang( values[i] );
                        delay( (i+g) / d ).bang( 0.0f );
                    }
                }
            end();
            
            regenerate = false;
        }
        
    };    
    
}

ofParameterGroup & np::sequence::Euclidean::label( string name ){
    pdsp::Sequence::label = name;
    parameters.setName( name );
    return parameters;
}

   
// ------------------ rhytm generation methods --------------------------------
int np::sequence::Euclidean::gcd( int x, int y ) {
       if( x < y )
          std::swap( x, y );

       while( y > 0 )
       {
          int f = x % y;
          x = y;
          y = f;
       }
       return x;
 }  

void np::sequence::Euclidean::cword ( std::vector<float> & vect, int x, int y, bool upper) {

    // clean vector
    for( float & value : vect ) value = 0.0f;
        
    int max = vect.size();
    
    if ( y > x ) y = x;
    
    
    int div = gcd( y, x );
    
    y = y / div;
    x = x/ div;

    int i = 0;
            
    int a = y;
    int b = x;    
    
    if ( upper ){
        vect[i] = 1.0f;
    }
    i++;
    
    while ( a != b && i < max ){
        if ( a > b ){
            vect[i] = 1.0f;
            b = b + x;
        }else{
            vect[i] = 0.0f;
            a = a + y;
        }
        i++;
    }
    
    if ( i<max ){
        if ( ! upper ){
            vect[i] = 1.0f;
        }
    }
    
    
    for ( int k = 1; k<div; k++){
        
        for (int j = 0; j<i; ++j){
            vect[ j + k*i ] = vect[j];
        }
        
    }
    
}
 

void np::sequence::Euclidean::shift ( std::vector<float> & vect, int shiftAmount) {
    if(shiftAmount!=0){
        if( shiftAmount < 0 ){
            shiftAmount = vect.size() + shiftAmount;
        }
        std::rotate(vect.begin(), vect.begin()+shiftAmount, vect.end());        
    }
}


void np::sequence::Euclidean::ghost_all ( std::vector<float> & vect, float dynamic ) {
    
    for( size_t i=0; i<vect.size(); ++i){
        if( vect[i] == 0.0f ){
            vect[i] = dynamic;
        }
    } 
    
}


void np::sequence::Euclidean::ghost_chance ( std::vector<float> & vect, float dynamic, float chance) {
    
    bool fill;
    if ( vect[0] == 0.0f ){
        fill = pdsp::chance(chance);        
    }else{
        fill = false;
    }

    for( size_t i=0; i<vect.size(); ++i){
        if( vect[i] == 0 ) {
            if ( fill ){
                vect[i] = dynamic;
            }
        }else{
            bool yes = pdsp::chance( chance );
            fill = fill ? false : yes;
        }
    }    
    
}


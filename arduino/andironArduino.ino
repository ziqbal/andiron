
/* notes */



/* macros */



/* header */


/***************************************\
* andironArduino (C)2014 zaf.io         *
* Zafar Iqbal <mail@zaf.io>             *
* BUILD Tue, 23 Dec 2014 14:04:00 +0000 *
\***************************************/


/* vars */


const String _vars_box = "1" ;
const String _vars_release = "1" ;

unsigned long _vars_frame = 0 ;

void _vars_loop( ) {

	_vars_frame ++ ;

	if( ( _vars_frame % 200000 ) == 0 ) {

      _data_transmit( F( "#" ) , F( "H" ) ) ;  

      return ;

	}    	
	
}




/* init */



void _init_setup( ) {

	randomSeed( analogRead( 0 ) ) ;

}

/* eeprominit */


#include <EEPROM.h>

unsigned long _eeprominit_init ;

void _eeprominit_setup( ) {

	unsigned int ee1 , ee2 ; 

	ee1 = EEPROM.read( 0 ) ;

	ee2 = EEPROM.read( 1 ) ;

	ee2 ++ ;

	if( ee2 == 256 ) {

		ee2 = 0 ;

		ee1 ++ ;

		if( ee1 == 256 ) ee1 = 0 ;

	}

	EEPROM.write( 0 , ee1 ) ;

	EEPROM.write( 1 , ee2 ) ;

	_eeprominit_init = ( ee1 << 8 ) + ee2 ;	

/* 
  EEPROM.write(0, 255);
  EEPROM.write(1, 255);
*/ 	

}



/* serial */


void _serial_setup( ) {

	Serial.begin( 9600 ) ;	

}



int _serial_stream[ ] = { -1 , -1 , -1 , -1 , -1 } ;
byte _serial_sp = 0 ; 
byte _serial_e = 0 ; 
byte _serial_a = 0 ; 
byte _serial_v = 0 ; 

void _serial_loop( ) {

	
/* Macro Start 100 */

static unsigned long _serial_Trigger = 100 ;	
static unsigned long _serial_Counter = 0 ;

if( ++ _serial_Counter < _serial_Trigger ) return ;

_serial_Counter = 0 ;

/* Macro End 100 */


	if( Serial.available( ) > 0 ) {

		int incomingByte = Serial.read( ) ;

		if ( incomingByte == -1 ) return ;

		_serial_stream[ _serial_sp++ ] = incomingByte ;

		if( _serial_sp == 5 ) _serial_sp = 0 ;

		/*
		for( int i = 0 ; i < 5 ; i++ ) {
			Serial.print(_serial_stream[ i ]);
			Serial.print(" ");
		}
		Serial.print(" -> ");
		Serial.print(_serial_sp);
		Serial.println(" ");
		*/


		if( ! _serial_decode( ) ) return ; 

		/*
		Serial.print( " !!! " ) ;
		Serial.print( _serial_e , DEC ) ;
		Serial.print( " " );
		Serial.print(_serial_a,DEC);
		Serial.print(" ");
		Serial.print(_serial_v,DEC);
		Serial.println(" ");		
		*/

		// 1 1 X -> DigitalOutput HIGH PinX 
		if( _serial_e == 1  && _serial_a == 1 ) {

			pinMode( _serial_v , OUTPUT );
			digitalWrite( _serial_v , HIGH); 

		}

		// 1 0 X -> DigitalOutput LOW PinX 
		if( _serial_e == 1  && _serial_a == 0 ) {
			
			pinMode( _serial_v , OUTPUT );
			digitalWrite( _serial_v , LOW ); 

		}		


	}	

}

bool _serial_decode( ) {

	int sp;
	int v1 , v2 , v3 , v4 ;

	sp = _serial_sp - 1 ;
	if( sp == -1 ) sp = 4 ;
	v1 = _serial_stream[ sp ] ;
	if( v1 == -1 ) return( false ) ;

	sp--  ;
	if( sp == -1 ) sp = 4 ;
	v2 = _serial_stream[ sp ] ;
	if( v2 == -1 ) return( false ) ;

	sp-- ;
	if( sp == -1 ) sp = 4 ;
	v3 = _serial_stream[ sp ] ;
	if( v3 == -1 ) return( false ) ;

	sp-- ;
	if( sp == -1 ) sp = 4 ;
	v4 = _serial_stream[ sp ] ;
	if( v4 == -1 ) return( false ) ;

	int ck;

	ck = ( v2 + v3 + v4 ) & 255 ;

	/*
	Serial.print( " ### " ) ;
	Serial.println( ck , DEC ) ;
	*/

	if( ck == v1 ) {

		sp = _serial_sp - 1 ;
		if( sp == -1 ) sp = 4 ;
		_serial_stream[ sp ] = -1 ;

		sp-- ;
		if( sp == -1 ) sp = 4 ;
		_serial_v = _serial_stream[ sp ] ;

		sp-- ;
		if( sp == -1 ) sp = 4 ;
		_serial_a = _serial_stream[ sp ] ;

		sp-- ;
		if( sp == -1 ) sp = 4 ;
		_serial_e = _serial_stream[ sp ] ;

		return( true ) ;

	}

	return( false ) ;

}


/* pre */


void _pre_setup( ) {

	delay( 1000 ) ;

	_data_transmit( F( "#" ) , F( "B" ) ) ;

}

void _pre_loop( ) {

	
/* Macro Start 0 */

static unsigned long _pre_Trigger = 0 ;	
static unsigned long _pre_Counter = 0 ;

if( ++ _pre_Counter < _pre_Trigger ) return ;

_pre_Counter = 0 ;

/* Macro End 0 */


	//////

	
}




/* data */


void _data_transmit( String key , String val ) {

  String data = key
                + F( "," )
                + val
                + F( "," )
                + _vars_box
                + F( "," )
                + _vars_release
                + F( "," )
                + String( _eeprominit_init , HEX )
                + F( "," )
                + String( _vars_frame , HEX )
                + F( "," )
                + String( millis( ) , HEX ) ;
  
  int checksum = 0 ;
  
  int dataLen = data.length( ) ;

  for( int i = 0 ; i < dataLen ; i ++ ) {

    checksum += data.charAt( i ) ;

  }

  checksum = checksum % 10 ;
  
  Serial.println( data + F( "," ) + checksum ) ;

}




/* blank */


void _blank_setup( ) {

}

void _blank_loop( ) {

	
/* Macro Start 0 */

static unsigned long _blank_Trigger = 0 ;	
static unsigned long _blank_Counter = 0 ;

if( ++ _blank_Counter < _blank_Trigger ) return ;

_blank_Counter = 0 ;

/* Macro End 0 */


	//////
	
}


void setup ( ) {

	_init_setup();
	_eeprominit_setup();
	_serial_setup();
	_pre_setup();
	_blank_setup();
  
}

void loop ( ) {
  
	_vars_loop();
	_serial_loop();
	_pre_loop();
	_blank_loop();

}




/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "viewninja.h"
#include "target.h"

#include <bu/plugger.h>

#include <bu/sio.h>
using namespace Bu;

PluginInterface3( pluginViewNinja, ninja, ViewNinja, View,
        "Ingwie Phoenix", 0, 1 );

#define ESC             "\x1b"

#define C_RESET         ESC "[0m"
#define C_RED           ESC "[31m"
#define C_GREEN         ESC "[32m"
#define C_YELLOW        ESC "[33m"
#define C_BLUE          ESC "[34m"
#define C_MAGENTA       ESC "[35m"
#define C_CYAN          ESC "[36m"
#define C_WHITE         ESC "[37m"
#define C_DEFAULT       ESC "[39m"

#define C_BR_RED        ESC "[1;31m"
#define C_BR_GREEN      ESC "[1;32m"
#define C_BR_YELLOW     ESC "[1;33m"
#define C_BR_BLUE       ESC "[1;34m"
#define C_BR_MAGENTA    ESC "[1;35m"
#define C_BR_CYAN       ESC "[1;36m"
#define C_BR_WHITE      ESC "[1;37m"

#define L_CLEAR			ESC "[K"


ViewNinja::ViewNinja() :
    bFirst( true ),
    bDisped( false ),
    bDispedTrg( false ),
    iDepth( 0 ),
    iTotal( 0 ),
    iCurrent( 0 ),
    needNewLine( false ),
    targedProcessed( true )
{
}

ViewNinja::~ViewNinja()
{
    if( bDisped == false ) {
        sio << "Nothing to be done." << sio.nl;
    } //else sio << sio.nl;
}

void ViewNinja::beginAction( const Bu::String &/*sAction*/ ) {
}

void ViewNinja::endAction() {
}

void ViewNinja::skipTarget(
	const Bu::String &sProfile,
    const Target &rTarget 
) {
    this->iCurrent++;
}

void ViewNinja::beginTarget( 
	const Bu::String &sProfile,
    const Target &rTarget
) {
    if( this->iDepth == 0 ) {
        bDispedTrg = false;
        this->iTotal = rTarget.getDepCount();
        this->iCurrent = 0;
        this->sTopTarget = rTarget.getOutputList().first();
    }
    this->iDepth++;

    sCurProfile = sProfile;
}

void ViewNinja::drawTargetHdr(
	const Bu::String &sProfile,
    const Target &rTarget
) {}

void ViewNinja::processTarget(
	const Bu::String &sProfile,
    const Target &rTarget
) {
    this->iCurrent++;

    sio << C_BR_CYAN << sProfile << "|" << sTopTarget << " "
        << C_BR_WHITE << "[" << C_BR_GREEN 
        << this->iCurrent << "/" << this->iTotal
        << C_BR_WHITE << "] " << C_BR_MAGENTA
        << rTarget.getDisplay() << C_BR_WHITE
        << ": " << rTarget.getOutputList().first() << C_RESET << sio.flush;
        
    bDisped = true;
}

void ViewNinja::endTarget() {
    this->iDepth--;
	if(iTotal == iCurrent) sio << sio.nl;
	else sio << '\r' << L_CLEAR << sio.flush;
}

void ViewNinja::buildRequires( const Target &rTarget ) { 
/* Not needed */ }

void ViewNinja::cmdStarted( const Bu::String &/*sCmd*/ ) {
}

void ViewNinja::cmdFinished(
	const Bu::String &sStdOut,
    const Bu::String &sStdErr, 
    long /*iExit*/
) {
	if( sStdOut.isSet() ||  sStdErr.isSet() ) sio << sio.nl;
	
	// stdOut output
    if( sStdOut.isSet() ) {
        Bu::String::const_iterator b;
        b = sStdOut.begin();
        while( b )
        {
            Bu::String::const_iterator e, max;
            max = b + 78;
            for( e = b; e != max && *e != '\n'; e++ ) { } 
            sio << C_BR_GREEN << "| " << C_RESET << String( b, e ) << sio.nl;
            b = e;
            if( *b == '\n' )
                b++;
        }
        sio << C_BR_GREEN << "\\-----" << C_RESET << sio.nl;
    }
    
    // stdErr output
    if( sStdErr.isSet() ) {
        Bu::String::const_iterator b;
        b = sStdErr.begin();
        while( b )
        {
            Bu::String::const_iterator e, max;
            max = b + 78;
            for( e = b; e != max && *e != '\n'; e++ ) { } 
            sio << C_BR_RED << "| " << C_RESET  << String( b, e ) << sio.nl;
            b = e;
            if( *b == '\n' )
                b++;
        }
        sio << C_BR_RED << "\\-----" << C_RESET << sio.nl;
    }
    bDisped = true;
}

void ViewNinja::checkBegin(Bu::String what) {
	config_what = what;
	sio << C_BR_WHITE << "--" << C_BR_GREEN << " Checking for: " << what << "..." << sio.flush;
}
void ViewNinja::checkEnd() { sio << C_DEFAULT << sio.nl; config_what.clear(); }
void ViewNinja::checkFail() {
	sio << "\r" << C_BR_RED << "!" << " Checking for: " << config_what << " FAILED" << sio.flush;
}
void ViewNinja::checkSuccess() {
	sio << "\r" << C_BR_WHITE << ">" << C_BR_CYAN << " Checking for: " << config_what << " OK" << sio.flush;
}

void ViewNinja::userError( const Bu::String &sMsg )
{
    sio << C_BR_RED << "Error: " << sMsg << C_RESET << sio.nl;
    bDisped = true;
}

void ViewNinja::userWarning( const Bu::String &sMsg )
{
    sio << C_BR_YELLOW << "Warning: " << sMsg << C_RESET << sio.nl;
    bDisped = true;
}

void ViewNinja::userNotice( const Bu::String &sMsg )
{
    sio << C_BR_GREEN << "Notice: " << sMsg << C_RESET << sio.nl;
    bDisped = true;
}

void ViewNinja::sysError( const Bu::String &sMsg )
{
    sio << C_BR_RED << sMsg << C_RESET << sio.nl;
    bDisped = true;
}

void ViewNinja::sysWarning( const Bu::String &sMsg )
{
    sio << C_BR_YELLOW << sMsg << C_RESET << sio.nl;
    bDisped = true;
}

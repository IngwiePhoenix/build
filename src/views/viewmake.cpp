/*
 * Copyright (C) 2007-2014 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#include "viewmake.h"
#include "target.h"

#include <bu/plugger.h>

#include <bu/sio.h>
using namespace Bu;

PluginInterface3( pluginViewMake, make, ViewMake, View,
        "Mike Buland", 0, 1 );

ViewMake::ViewMake()
{
}

ViewMake::~ViewMake()
{
}

void ViewMake::beginAction( const Bu::String &/*sAction*/ )
{
}

void ViewMake::endAction()
{
}

void ViewMake::skipTarget( const Bu::String &/*sProfile*/,
        const Target &/*rTarget*/ )
{
}

void ViewMake::beginTarget( const Bu::String &/*sProfile*/,
        const Target &/*rTarget*/ )
{
}

void ViewMake::processTarget( const Bu::String &/*sProfile*/,
        const Target &/*rTarget*/ )
{
}

void ViewMake::endTarget()
{
}

void ViewMake::buildRequires( const Target &/*rTarget*/ )
{
}

void ViewMake::cmdStarted( const Bu::String &sCmd )
{
    sio << sCmd << sio.nl;
}

void ViewMake::cmdFinished( const Bu::String &sStdOut,
        const Bu::String &sStdErr, long /*iExit*/ )
{
    sio << sStdOut << sStdErr;
}

// Going for the autoconf style :)
void ViewMake::checkBegin(Bu::String what) {
	sio << "checking for: " << what << "... " << sio.flush;
}
void ViewMake::checkEnd() { sio << sio.nl; }
void ViewMake::checkFail() { sio << "FAILED" << sio.flush; }
void ViewMake::checkSuccess() { sio << "OK" << sio.flush; }

void ViewMake::userError( const Bu::String &sMsg )
{
    sio << "Error: " << sMsg << sio.nl;
}

void ViewMake::userWarning( const Bu::String &sMsg )
{
    sio << "Warning: " << sMsg << sio.nl;
}

void ViewMake::userNotice( const Bu::String &sMsg )
{
    sio << "Notice: " << sMsg << sio.nl;
}

void ViewMake::sysError( const Bu::String &sMsg )
{
    sio << sMsg << sio.nl;
}

void ViewMake::sysWarning( const Bu::String &sMsg )
{
    sio << sMsg << sio.nl;
}


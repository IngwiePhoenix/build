#include "viewdefault.h"
#include "target.h"

#include <bu/plugger.h>

#include <bu/sio.h>
using namespace Bu;

PluginInterface3( pluginViewDefault, default, ViewDefault, View,
		"Mike Buland", 0, 1 );

#define ESC				"\x1b"

#define C_RESET			ESC "[0m"
#define	C_RED			ESC "[31m"
#define C_GREEN			ESC "[32m"
#define C_YELLOW		ESC "[33m"
#define C_BLUE			ESC "[34m"
#define C_MAGENTA		ESC "[35m"
#define C_CYAN			ESC "[36m"
#define C_WHITE			ESC "[37m"
#define C_DEFAULT		ESC "[39m"

#define	C_BR_RED		ESC "[1;31m"
#define C_BR_GREEN		ESC "[1;32m"
#define C_BR_YELLOW		ESC "[1;33m"
#define C_BR_BLUE		ESC "[1;34m"
#define C_BR_MAGENTA	ESC "[1;35m"
#define C_BR_CYAN		ESC "[1;36m"
#define C_BR_WHITE		ESC "[1;37m"

ViewDefault::ViewDefault() :
	bFirst( true ),
	bDisped( false ),
	bDispedTrg( false ),
	iDepth( 0 ),
	iTotal( 0 ),
	iCurrent( 0 )
{
}

ViewDefault::~ViewDefault()
{
	if( bDisped == false )
	{
		sio << "Nothing to be done." << sio.nl << sio.nl;
	}
}

void ViewDefault::beginAction( const Bu::FString &/*sAction*/ )
{
}

void ViewDefault::endAction()
{
}

void ViewDefault::skipTarget( const Bu::FString &/*sProfile*/,
		const Target &/*rTarget*/ )
{
	iCurrent++;
}

void ViewDefault::beginTarget( const Bu::FString &/*sProfile*/,
		const Target &rTarget )
{
	if( iDepth == 0 )
	{
		bDispedTrg = false;
		iTotal = rTarget.getDepCount();
		iCurrent = 0;
		if( bFirst == false )
		{
			sio << sio.nl;
		}
		bFirst = false;
	}
	iDepth++;
}

void ViewDefault::processTarget( const Bu::FString &sProfile,
		const Target &rTarget )
{
	if( bDispedTrg == false )
	{
		bDispedTrg = true;
		sio << C_BR_WHITE << " --- " << C_BR_CYAN << sProfile << " "
			<< rTarget.getOutputList().first() << C_BR_WHITE << " --- "
			<< C_RESET << sio.nl;
	}
	iCurrent++;

	int iPct = (iTotal>0)?(iCurrent*100/iTotal):(100);
	sio << C_BR_WHITE << "[" << C_BR_GREEN << Fmt(3) << iPct
		<< "%" << C_BR_WHITE << "] " << C_BR_MAGENTA
		<< Fmt(10) << rTarget.getDisplay() << C_BR_WHITE
		<< ": " << rTarget.getOutputList().first() << C_RESET << sio.nl;

	bDisped = true;
}

void ViewDefault::endTarget()
{
	iDepth--;
}

void ViewDefault::buildRequires( const Target &rTarget )
{
	int iPct = (iTotal>0)?(iCurrent*100/iTotal):(100);
	sio << C_BR_WHITE << "[" << C_BR_GREEN << Fmt(3) << iPct
		<< "%" << C_BR_WHITE << "] " << C_BR_MAGENTA
		<< Fmt(10) << "deps" << C_BR_WHITE
		<< ": " << rTarget.getOutputList().first() << C_RESET << sio.nl;
	bDisped = true;
}

void ViewDefault::cmdStarted( const Bu::FString &/*sCmd*/ )
{
}

void ViewDefault::cmdFinished( const Bu::FString &sStdOut,
	const Bu::FString &sStdErr, long /*iExit*/ )
{
	if( sStdOut )
	{
		Bu::FString::const_iterator b;
		b = sStdOut.begin();
		while( b )
		{
			Bu::FString::const_iterator e, max;
			max = b + 78;
			for( e = b; e != max && *e != '\n'; e++ ) { } 
			sio << C_BR_GREEN << "| " << C_RESET << FString( b, e ) << sio.nl;
			b = e;
			if( *b == '\n' )
				b++;
		}
		sio << C_BR_GREEN << "\\-----" << C_RESET << sio.nl;
	}
	if( sStdErr )
	{
		Bu::FString::const_iterator b;
		b = sStdErr.begin();
		while( b )
		{
			Bu::FString::const_iterator e, max;
			max = b + 78;
			for( e = b; e != max && *e != '\n'; e++ ) { } 
			sio << C_BR_RED << "| " << C_RESET  << FString( b, e ) << sio.nl;
			b = e;
			if( *b == '\n' )
				b++;
		}
		sio << C_BR_RED << "\\-----" << C_RESET << sio.nl;
	}
	//sio << C_BR_WHITE << "[" << C_BR_GREEN << sStdOut << C_BR_WHITE << "]" << sio.nl;
	//sio << C_BR_WHITE << "[" << C_BR_RED << sStdErr << C_BR_WHITE << "]" << sio.nl;
	bDisped = true;
}

void ViewDefault::userError( const Bu::FString &sMsg )
{
	sio << C_BR_RED << "Error: " << sMsg << C_RESET << sio.nl;
	bDisped = true;
}

void ViewDefault::userWarning( const Bu::FString &sMsg )
{
	sio << C_BR_YELLOW << "Warning: " << sMsg << C_RESET << sio.nl;
	bDisped = true;
}

void ViewDefault::userNotice( const Bu::FString &sMsg )
{
	sio << C_BR_GREEN << "Notice: " << sMsg << C_RESET << sio.nl;
	bDisped = true;
}

void ViewDefault::sysError( const Bu::FString &sMsg )
{
	sio << C_BR_RED << sMsg << C_RESET << sio.nl;
	bDisped = true;
}

void ViewDefault::sysWarning( const Bu::FString &sMsg )
{
	sio << C_BR_YELLOW << sMsg << C_RESET << sio.nl;
	bDisped = true;
}

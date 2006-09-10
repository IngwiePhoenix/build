#include "build.h"

subExceptionDef( BuildException );

Build::Build() :
	pStrProc( NULL )
{
}

Build::~Build()
{
}

void Build::setStringProc( StringProc *pStrProc )
{
	delete this->pStrProc;
	this->pStrProc = pStrProc;
}

std::string Build::replVars( const std::string &sSrc, const std::string &sCont, VarMap *mExtra )
{
	if( pStrProc == NULL )
		throw BuildException(
			"No valid string processor was registered with the Build object."
			);

	return pStrProc->replVars( sSrc, sCont, mExtra );
}

void Build::execAction( const std::string &sWhat )
{
	if( mAction.find( sWhat ) == mAction.end() )
		throw BuildException(
			"No action matches %s, check your build.conf.",
			sWhat.c_str()
			);

	Action *pAct = mAction[sWhat];

	for( pAct->begin(); !pAct->isEnded(); pAct->next() )
	{
		if( mTarget.find( pAct->getWhat() ) == mTarget.end() )
			throw BuildException(
				"No target matches %s in action %s.",
				pAct->getWhat().c_str(),
				sWhat.c_str()
				);
		Target *pTarget = mTarget[pAct->getWhat()];
		switch( pAct->getAct() )
		{
			case Action::actCheck:
				pTarget->check( *this );
				break;

			case Action::actClean:
				pTarget->clean( *this );
				break;
		}
	}

	return;
}

void Build::addTarget( Target *pTarget )
{
	TargetMap::iterator i = mTarget.find( pTarget->getName() );
	if( i == mTarget.end() )
	{
		mTarget[pTarget->getName()] = pTarget;
	}
	else
	{
		throw BuildException("Merging targets isn't working yet.");
	}
}

void Build::addRequires( const std::string &who, const std::string &what )
{
	mRequires[who].push_back( what );
}

void Build::addRule( Rule *pRule )
{
	mRule[pRule->getName()] = pRule;
}

Rule *Build::getRule( const std::string &name )
{
	if( mRule.find( name ) == mRule.end() )
		throw BuildException("No rule named %s found.", name.c_str() );

	return mRule[name];
}

void Build::addAction( Action *pAction )
{
	mAction[pAction->getName()] = pAction;
}

void Build::set( const std::string &cont, const std::string &var, const std::string &val )
{
	if( cont == "" )
	{
		mVars[var] = val;
	}
	else
	{
		mContVars[cont][var] = val;
	}
}

void Build::setAdd( const std::string &cont, const std::string &var, const std::string &val )
{
	if( cont == "" )
	{
		mVars[var] = getVar( cont, var, NULL ) + " " + val;
	}
	else
	{
		mContVars[cont][var] = getVar( cont, var, NULL ) + " " + val;
	}
}

std::string Build::getVar( const std::string &cont, const std::string &var, VarMap *mExtra )
{
	if( mExtra != NULL )
	{
		if( mExtra->find(var) == mExtra->end() )
		{
			return getVar( cont, var, NULL );
		}
		return (*mExtra)[var];
	}
	
	if( cont == "" )
	{
		if( mVars.find(var) == mVars.end() )
		{
			if( getenv( var.c_str() ) == NULL )
			{
				mVars[var] = "";
			}
			else
			{
				mVars[var] = getenv( var.c_str() );
			}
		}
		return mVars[var];
	}
	else
	{
		if( mContVars[cont].find(var) == mContVars[cont].end() )
		{
			mContVars[cont][var] = getVar( "", var, NULL );
		}
		return mContVars[cont][var];
	}
}

void Build::debugDump()
{
	printf("Requires:\n");
	for( ReqMap::iterator i = mRequires.begin(); i != mRequires.end(); i++ )
	{
		printf("  %s: ", (*i).first.c_str() );

		for( StringList::iterator j = (*i).second.begin();
			 j != (*i).second.end(); j++ )
		{
			if( j != (*i).second.begin() )
				printf(", ");
			printf("%s", (*j).c_str() );
		}
		printf("\n");
	}

	printf("Targets:\n");
	for( TargetMap::iterator i = mTarget.begin(); i != mTarget.end(); i++ )
	{
		printf("  %s:\n", (*i).first.c_str() );
		printf("    Rule:  %s\n", (*i).second->getRule().c_str() );
		printf("    Input: ");
		for( StringList::iterator j = (*i).second->getInput().begin();
			 j != (*i).second->getInput().end(); j++ )
		{
			if( j != (*i).second->getInput().begin() )
				printf(", ");
			printf("%s", (*j).c_str() );
		}
		printf("\n");
	}

	printf("Global Variables:\n");
	for( VarMap::iterator i = mVars.begin(); i != mVars.end(); i++ )
	{
		printf("  \"%s\" = \"%s\"\n", (*i).first.c_str(), (*i).second.c_str() );
	}

	printf("Context Variables:\n");
	for( ContextMap::iterator i = mContVars.begin(); i != mContVars.end(); i++ )
	{
		printf("  %s:\n", (*i).first.c_str() );
		
		for( VarMap::iterator j = (*i).second.begin();
			 j != (*i).second.end(); j++ )
		{
			printf("    \"%s\" = \"%s\"\n",
				(*j).first.c_str(),
				(*j).second.c_str()
				);
		}
	}

	printf("Rules:\n");
	for( RuleMap::iterator i = mRule.begin(); i != mRule.end(); i++ )
	{
		printf("  %s:\n", (*i).first.c_str() );
		printf("    Matches:  func\n");
		printf("    Filters:  %d\n", (*i).second->getFilterList().size() );
		printf("    Performs: %d\n", (*i).second->getPerformList().size() );
		printf("    Produces:\n");
		printf("    Requires:\n");
	}

	printf("Actions:\n");
	for( ActionMap::iterator i = mAction.begin(); i != mAction.end(); i++ )
	{
		printf("  %s: ", (*i).first.c_str() );
		for( (*i).second->begin(); !(*i).second->isEnded(); (*i).second->next() )
		{
			printf("%d:%s ", (*i).second->getAct(), (*i).second->getWhat().c_str() );
		}
		printf("\n");
	}
}


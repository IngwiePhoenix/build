#include "profile.h"
#include "ast.h"
#include "astbranch.h"
#include "astleaf.h"
#include "condition.h"

#include "conditionfiletime.h"
#include "conditionalways.h"
#include "conditionnever.h"

#include <bu/sio.h>
using namespace Bu;

Profile::Profile( const class AstBranch *pRoot ) :
	pRoot( pRoot ),
	pCond( NULL ),
	pAst( NULL )
{
	sName = dynamic_cast<const AstLeaf *>(
		(*pRoot->getBranchBegin()).first()
		)->getStrValue();

	setCondition();
}

Profile::Profile( const Profile &rSrc ) :
	sName( rSrc.sName ),
	pRoot( rSrc.pRoot ),
	pCond( rSrc.pCond->clone() ),
	pAst( NULL )
{
}

Profile::~Profile()
{
	delete pAst;
	pAst = NULL;
}

const Bu::FString &Profile::getName() const
{
	return sName;
}

const AstBranch *Profile::getRoot() const
{
	return pRoot;
}

const Condition *Profile::getCond() const
{
	return pCond;
}

bool Profile::shouldExec( class Runner &r, class Target &rTarget ) const
{
	return pCond->shouldExec( r, rTarget );
}

Profile *Profile::genDefaultClean()
{
	Ast *pAst = new Ast();
	pAst->addNode( AstNode::typeProfile );
		pAst->openBranch();
			pAst->addNode( AstNode::typeString, "clean" );
		pAst->openBranch();
		 	pAst->addNode( AstNode::typeCondition, "always" );		
			pAst->addNode( AstNode::typeFunction );
				pAst->openBranch();
					pAst->addNode( AstNode::typeString, "unlink" );
						pAst->openBranch();
							pAst->addNode( AstNode::typeVariable, "OUTPUT" );
					pAst->closeNode();
			pAst->closeNode();
	//pAst->closeNode();
	Profile *pRet = new Profile(
		dynamic_cast<const AstBranch *>(*pAst->getNodeBegin())
		);
	pRet->pAst = pAst;

	return pRet;
}

void Profile::setCondition()
{
	for( AstBranch::NodeList::const_iterator i =
		 (*(pRoot->getBranchBegin()+1)).begin(); i; i++ )
	{
		if( (*i)->getType() == AstNode::typeCondition )
		{
			Bu::FString sCond = dynamic_cast<const AstLeaf *>(*i)->getStrValue();
			if( sCond == "filetime" )
			{
				delete pCond;
				pCond = new ConditionFileTime();
			}
			else if( sCond == "always" )
			{
				delete pCond;
				pCond = new ConditionAlways();
			}
			else if( sCond == "never" )
			{
				delete pCond;
				pCond = new ConditionNever();
			}
		}
	}

	if( pCond == NULL )
	{
		// The default condition
		pCond = new ConditionFileTime();
	}
}


#ifndef REG_EXP_H
#define REG_EXP_H

#include <string>
#include <stdint.h>
#include <regex.h>
#include <utility>
#include "staticstring.h"

class RegExp
{
public:
	RegExp();
	RegExp( const char *sSrc );
	virtual ~RegExp();

	void compile( const char *sSrc );
	int getNumSubStrings();
	bool execute( const char *sSrc );
	std::pair<int,int> getSubStringRange( int nIndex );
	std::string getSubString( int nIndex );
	const char *getSource()
	{
		return sSrc;
	}

private:
	StaticString sSrc;
	StaticString sTest;
	regex_t re;
	bool bCompiled;
	int nSubStr;
	regmatch_t *aSubStr;
};

#endif
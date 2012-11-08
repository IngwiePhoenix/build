/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef CONDITION_FILE_EXISTS_H
#define CONDITION_FILE_EXISTS_H

#include "condition.h"

class ConditionFileExists : public Condition
{
public:
    ConditionFileExists();
    virtual ~ConditionFileExists();

    virtual bool shouldExec( class Runner &r, class Target &rTarget );
    virtual Condition *clone();
};

#endif

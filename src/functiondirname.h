/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_DIR_NAME_H
#define FUNCTION_DIR_NAME_H

#include "function.h"

class FunctionDirName : public Function
{
public:
    FunctionDirName();
    virtual ~FunctionDirName();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );

};

#endif

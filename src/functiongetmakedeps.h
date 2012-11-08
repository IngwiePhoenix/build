/*
 * Copyright (C) 2007-2012 Xagasoft, All rights reserved.
 *
 * This file is part of the Xagasoft Build and is released under the
 * terms of the license contained in the file LICENSE.
 */

#ifndef FUNCTION_GET_MAKE_DEPS_H
#define FUNCTION_GET_MAKE_DEPS_H

#include "function.h"

class FunctionGetMakeDeps : public Function
{
public:
    FunctionGetMakeDeps();
    virtual ~FunctionGetMakeDeps();

    virtual Bu::String getName() const;
    virtual Variable call( Variable &input, VarList lParams );
};

#endif

/*
  Copyright 2013--2018 James E. McClure, Virginia Polytechnic & State University

  This file is part of the Open Porous Media project (OPM).
  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef included_Utilities
#define included_Utilities

#include <cstdarg>
#include <vector>

#include "StackTrace/Utilities.h"


namespace Utilities {


// Functions inherited from StackTrace::Utilities
using StackTrace::Utilities::abort;
using StackTrace::Utilities::cause_segfault;
using StackTrace::Utilities::clearErrorHandlers;
using StackTrace::Utilities::exec;
using StackTrace::Utilities::getMemoryUsage;
using StackTrace::Utilities::getSystemMemory;
using StackTrace::Utilities::setAbortBehavior;
using StackTrace::Utilities::setErrorHandlers;
using StackTrace::Utilities::tick;
using StackTrace::Utilities::time;
using StackTrace::Utilities::sleep_ms;
using StackTrace::Utilities::sleep_s;


//! std::string version of sprintf
inline std::string stringf( const char *format, ... );


//! Factor a number into it's prime factors
std::vector<int> factor(size_t number);


//! Null use function
void nullUse( void* );


} // namespace Utilities


#include "common/UtilityMacros.h"


// stringf
inline std::string Utilities::stringf( const char *format, ... )
{
    va_list ap;
    va_start( ap, format );
    char tmp[4096];
    vsprintf( tmp, format, ap );
    va_end( ap );
    return std::string( tmp );
}


#endif

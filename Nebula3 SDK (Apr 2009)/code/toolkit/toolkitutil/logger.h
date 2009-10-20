#pragma once
//------------------------------------------------------------------------------
/**
    @class ToolkitUtil::Logger
    
    A simple logger for error messages and warnings.
    
    (C) 2008 Radon Labs GmbH
*/
#include "util/string.h"
#include "util/array.h"

//------------------------------------------------------------------------------
namespace ToolkitUtil
{
class Logger
{
public:
    /// constructor
    Logger();
    /// destructor
    ~Logger();
    
    /// put a formatted error message
    void Error(const char* msg, ...);
    /// put a formatted warning message
    void Warning(const char* msg, ...);
    /// put a formatted message
    void Print(const char* msg, ...);

private:
    Util::Array<Util::String> messages;
};

} // namespace ToolkitUtil
//------------------------------------------------------------------------------


    
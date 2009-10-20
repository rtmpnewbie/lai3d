//------------------------------------------------------------------------------
//  logger.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "toolkitutil/logger.h"

namespace ToolkitUtil
{
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
Logger::Logger()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Logger::~Logger()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
void
Logger::Error(const char* msg, ...)
{
    va_list argList;
    va_start(argList, msg);
    String str;
    str.FormatArgList(msg, argList);
    this->messages.Append(String("[ERROR] ") + str);
    n_printf(str.AsCharPtr());
    va_end(argList);
}
     
//------------------------------------------------------------------------------
/**
*/
void
Logger::Warning(const char* msg, ...)
{
    va_list argList;
    va_start(argList, msg);
    String str;
    str.FormatArgList(msg, argList);
    this->messages.Append(String("[WARNING] ") + str);
    n_printf(str.AsCharPtr());
    va_end(argList);
}

//------------------------------------------------------------------------------
/**
*/
void
Logger::Print(const char* msg, ...)
{
    va_list argList;
    va_start(argList, msg);
    String str;
    str.FormatArgList(msg, argList);
    this->messages.Append(str);
    n_printf(str.AsCharPtr());
    va_end(argList);
}

} // namespace ToolkitUtil

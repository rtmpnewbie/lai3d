#pragma once
//------------------------------------------------------------------------------
/**
    @file core/debug.h

    Nebula debug macros.
  
    n_assert()  - the vanilla assert() Macro
    n_verify()  - like assert() except that the statement in parens is simply evaluated, unchecked, if __NEBULA_NO_ASSERT is set
    n_assert2() - an assert() plus a message from the programmer
*/
#include "core/config.h"

void __cdecl n_printf(const char *, ...) __attribute__((format(printf,1,2)));
void __cdecl n_error(const char*, ...) __attribute__((noreturn)) __attribute__((format(printf,1,2)));
void __cdecl n_dbgout(const char*, ...) __attribute__((format(printf,1,2)));
void __cdecl n_warning(const char*, ...) __attribute__((format(printf,1,2)));
void __cdecl n_confirm(const char*, ...) __attribute__((format(printf,1,2)));
void n_sleep(double);
void n_barf(const char *, const char *, int) __attribute__((noreturn));
void n_barf2(const char*, const char*, const char*, int) __attribute__((noreturn));

// backward compatibility
#define n_message n_confirm

#if __NEBULA_NO_ASSERT__
#define n_assert(exp)
#define n_verify(exp) (exp)
#define n_verify2(exp,imsg) (exp)
#define n_assert2(exp, msg)
#define n_dxtrace(hr, msg)
#else
#define n_assert(exp) { if (!(exp)) n_barf(#exp,__FILE__,__LINE__); }
#define n_assert2(exp, msg) { if (!(exp)) n_barf2(#exp,msg,__FILE__,__LINE__); }
#endif

#if __WIN32__
// dx9 specific: check HRESULT and display DX9 specific message box
#define n_dxtrace(hr, msg) { if (FAILED(hr)) DXTrace(__FILE__,__LINE__,hr,msg,true); }
#endif
//------------------------------------------------------------------------------

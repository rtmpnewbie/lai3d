//------------------------------------------------------------------------------
//  win32registry.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "system/win32/win32registry.h"

namespace System
{

//------------------------------------------------------------------------------
/**
    Convert a RootKey value into a Win32 key handle.
*/
HKEY
Win32Registry::RootKeyToWin32KeyHandle(RootKey rootKey)
{
    switch (rootKey)
    {
        case ClassesRoot:   return HKEY_CLASSES_ROOT;
        case CurrentUser:   return HKEY_CURRENT_USER;
        case LocalMachine:  return HKEY_LOCAL_MACHINE;
        case Users:         return HKEY_USERS;
    }
    // can't happen
    n_error("Can't happen!"); 
    return 0;
}

//------------------------------------------------------------------------------
/**
    Return true if a specific entry exists in the registry. To check only
    for the existence of a key without the contained value, pass an 
    empty 'name' string.
*/
bool
Win32Registry::Exists(RootKey rootKey, const Util::String& key, const Util::String& name)
{
    n_assert(key.IsValid());
    HKEY win32RootKey = RootKeyToWin32KeyHandle(rootKey);
    HKEY hKey = 0;
    LONG res = RegOpenKeyEx(win32RootKey,       // hKey
                            key.AsCharPtr(),    // lpSubKey
                            0,                  // ulOptions (reserved)
                            KEY_READ,           // samDesired
                            &hKey);
    if (ERROR_SUCCESS != res)
    {
        // key does not exist
        return false;
    }
    if (name.IsValid())
    {
        res = RegQueryValueEx(hKey,             // hKey
                              name.AsCharPtr(), // lpValueName
                              NULL,             // lpReserved
                              NULL,             // lpType
                              NULL,             // lpData
                              NULL);            // lpcbData
        RegCloseKey(hKey);
        return (ERROR_SUCCESS == res);
    }
    else
    {
        // key exists, value name was empty
        RegCloseKey(hKey);
        return true;
    }
}

//------------------------------------------------------------------------------
/**
    Set a key value in the registry. This will create the key if it doesn't
    exist.
*/
bool
Win32Registry::Write(RootKey rootKey, const Util::String& key, const Util::String& name, const Util::String& value)
{
    n_assert(key.IsValid());
    n_assert(name.IsValid());

    HKEY win32RootKey = RootKeyToWin32KeyHandle(rootKey);
    HKEY hKey = 0;
    LONG res = RegCreateKeyEx(win32RootKey,     // hKey
                              key.AsCharPtr(),  // lpSubKey
                              0,                // Reserved
                              NULL,             // lpClass
                              REG_OPTION_NON_VOLATILE,  // dwOptions
                              KEY_ALL_ACCESS,   // samDesired
                              NULL,             // lpSecurityAttribute
                              &hKey,            // phkResult
                              NULL);            // lpdwDisposition
    if (ERROR_SUCCESS == res)
    {
        res = RegSetValueEx(hKey,               // hKey
                            name.AsCharPtr(),   // lpValueName
                            0,                  // Reserved
                            REG_SZ,             // dwType (normal string)
                            (const BYTE*) value.AsCharPtr(),    // lpData
                            value.Length() + 1);                // cbData
        RegCloseKey(hKey);
        return (ERROR_SUCCESS == res);
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**
    Get a string value from the registry. Fails hard if the key doesn't exists
    (use the Exists() method to make sure that the key exists!).
*/
Util::String
Win32Registry::ReadString(RootKey rootKey, const Util::String& key, const Util::String& name)
{
    n_assert(key.IsValid());
    HKEY win32RootKey = RootKeyToWin32KeyHandle(rootKey);
    HKEY hKey = 0;
    LONG res = RegOpenKeyEx(win32RootKey,       // hKey
                            key.AsCharPtr(),    // lpSubKey
                            0,                  // ulOptions (reserved)
                            KEY_READ,           // samDesired
                            &hKey);
    n_assert(ERROR_SUCCESS == res);

    // HACK: don't accept data > 1 KByte
    char buf[1024];
    DWORD bufSize = sizeof(buf);
    res = RegQueryValueEx(hKey,                // hKey
                          name.AsCharPtr(),    // lpValueName
                          NULL,                // lpReserved
                          NULL,                // lpType
                          (BYTE*)buf,          // lpData
                          &bufSize);           // lpcbData
    n_assert(ERROR_SUCCESS == res);
    buf[bufSize - 1] = 0;
    Util::String returnString = buf;
	RegCloseKey(hKey);
    return returnString;
}

//------------------------------------------------------------------------------
/**
    Get an int value from the registry. Fails hard if the key doesn't exists
    (use the Exists() method to make sure that the key exists!).
*/
int
Win32Registry::ReadInt(RootKey rootKey, const Util::String& key, const Util::String& name)
{
    n_assert(key.IsValid());
    HKEY win32RootKey = RootKeyToWin32KeyHandle(rootKey);
    HKEY hKey = 0;
    LONG res = RegOpenKeyEx(win32RootKey,       // hKey
                            key.AsCharPtr(),    // lpSubKey
                            0,                  // ulOptions (reserved)
                            KEY_READ,           // samDesired
                            &hKey);
    n_assert(ERROR_SUCCESS == res);

	DWORD dwType = REG_DWORD;
	DWORD dwSize = sizeof(DWORD);
	DWORD result;

	res = RegQueryValueEx(hKey,
						  name.AsCharPtr(), 
						  NULL, 
						  &dwType, 
						  (BYTE*)&result,
						  &dwSize);
    n_assert(ERROR_SUCCESS == res);
	RegCloseKey(hKey);
	return result;
}


//------------------------------------------------------------------------------
/**
    This deletes a complete registry key with all its values.
*/
bool
Win32Registry::Delete(RootKey rootKey, const Util::String& key)
{
    n_assert(key.IsValid());
    HKEY win32RootKey = RootKeyToWin32KeyHandle(rootKey);
    LONG res = RegDeleteKey(win32RootKey,       // hKey
                            key.AsCharPtr());   // lpSubKey
    return (ERROR_SUCCESS == res);
}

} // namespace System

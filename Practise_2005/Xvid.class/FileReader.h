#pragma once

#define BUFFER_SIZE (2*1024*1024)

class CFileReader
{
public:
public:
	CFileReader();
	~CFileReader();

	bool ReadVideoFile(const char* szFileName);
private:
	unsigned char* m_pMP4Buffer;
	int            m_nUsefulBytes;
};

inline CFileReader* GetFileReader()
{
	extern CFileReader* g_pReader;
	return g_pReader;
}
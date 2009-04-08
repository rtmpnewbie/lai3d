#include "stdafx.h"

#include "FileReader.h"
#include <stdlib.h>

CFileReader* g_pReader = new CFileReader();

CFileReader::CFileReader()
: m_pMP4Buffer(NULL)
, m_nUsefulBytes(0)
{
	g_pReader = this;
}

CFileReader::~CFileReader()
{

}

bool CFileReader::ReadVideoFile( const char* szFileName )
{
	FILE* pFile = ::fopen(szFileName, "rb");
	if (NULL == pFile)
	{
		return false;
	}
	m_pMP4Buffer = (unsigned char *) ::malloc(BUFFER_SIZE);
	unsigned char *mp4_ptr    = m_pMP4Buffer;

	m_nUsefulBytes = fread(m_pMP4Buffer, 1, BUFFER_SIZE, pFile);
	return true;
}
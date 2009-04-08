#include "stdafx.h"
#include "Decoder.h"

CDecoder::CDecoder()
: m_vdo_dec(NULL)
{
	m_vdo_dec = new CXvidDec() ;
	m_vdo_dec->AttachCaller(320, 240, this) ; 
	m_vdo_dec->Open() ; 
}

CDecoder::~CDecoder()
{
	m_vdo_dec->Close();
	delete m_vdo_dec;
	m_vdo_dec = NULL;
}

void CDecoder::PostDecHandler( unsigned char * image, int used_bytes )
{
	// 仿照DoRenderSample写这里,每帧调用Decode的时候就会自动调用这里，填充texture的内容
}

void CDecoder::Decode( unsigned char* xvid, int xvid_len )
{
	if (m_vdo_dec)
	{
		m_vdo_dec->Decode(xvid, xvid_len);
	}
}
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
	// ����DoRenderSampleд����,ÿ֡����Decode��ʱ��ͻ��Զ�����������texture������
}

void CDecoder::Decode( unsigned char* xvid, int xvid_len )
{
	if (m_vdo_dec)
	{
		m_vdo_dec->Decode(xvid, xvid_len);
	}
}
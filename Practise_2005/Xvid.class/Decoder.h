#pragma once

#include "XvidDec.h"

class CDecoder : public CXvidDecHandler
{
public:
	CDecoder();
	~CDecoder();

	virtual void PostDecHandler(unsigned char * image, int used_bytes); 

	void  Decode(unsigned char* xvid, int xvid_len);
private:
	CXvidDec *  m_vdo_dec;
};
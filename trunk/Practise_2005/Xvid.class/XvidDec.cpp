#include "StdAfx.h"
#include ".\xviddec.h"
#include "xvid.h"

CXvidDec::CXvidDec() 
{
	m_width = 0 ; 
	m_height = 0 ; 
	m_image = NULL ; 
	m_dec_handle = NULL ; 
	m_dec_caller = NULL ; 
}

CXvidDec::~CXvidDec()
{
	if(m_image) free(m_image) ; 
	m_image = NULL ; 
}

void CXvidDec::AttachCaller(int width, int height, CXvidDecHandler * dec_caller) 
{
	m_width = width ;
	m_height = height ; 
	m_dec_caller = dec_caller ; 
	if((m_width > 0) && (m_height > 0)) 
	{
		int image_len = m_width * m_height * 3 ; 
		m_image = (unsigned char *)malloc(image_len) ; 
		memset(m_image, 0, image_len) ; 
		CXvidDec::XVID_GLOBAL_INIT() ; 
	}
}

bool CXvidDec::Close()
{
	int xerr = 0 ; 
	/* Destroy the encoder instance */
	xerr = xvid_decore(m_dec_handle, XVID_ENC_DESTROY, NULL, NULL); 
	return (xerr) ? false : true ; 
}

void CXvidDec::XVID_GLOBAL_INIT()
{
	/*------------------------------------------------------------------------
	* XviD core initialization
	*----------------------------------------------------------------------*/
	xvid_gbl_init_t xvid_gbl_init;
	memset(&xvid_gbl_init, 0, sizeof(xvid_gbl_init));
	xvid_gbl_init.version = XVID_VERSION;
	xvid_gbl_init.cpu_flags = XVID_CPU_FORCE | XVID_CPU_ASM ; // force to use asm optimized routine
	/* Initialize XviD core -- Should be done once per __process__ */
	xvid_global(NULL, XVID_GBL_INIT, &xvid_gbl_init, NULL);
}

bool CXvidDec::Open()
{
	if(!m_dec_caller) return false ; 
	static xvid_dec_create_t xvid_dec_create ;
	int ret = 0; 
	/*------------------------------------------------------------------------
	* XviD encoder initialization
	*----------------------------------------------------------------------*/

	memset(&xvid_dec_create, 0, sizeof(xvid_dec_create_t));
	xvid_dec_create.version = XVID_VERSION;
	/* Width and Height of input frames */
	xvid_dec_create.width = m_width ;
	xvid_dec_create.height = m_height ;
	ret = xvid_decore(NULL, XVID_DEC_CREATE, &xvid_dec_create, NULL) ;
	m_dec_handle = xvid_dec_create.handle;
	return true;
}

void CXvidDec::Decode(unsigned char * xvid, int xvid_len)
{
	int ret = 0;
	ret = dec_core(xvid, m_image, xvid_len); 
	if (ret > 0)
		m_dec_caller->PostDecHandler(m_image, ret) ;  
}

/* raw xvid_encode procedure  */
int CXvidDec::dec_core(unsigned char *bitstream,unsigned char *image, int bs_size)
{
	int ret;
	xvid_dec_frame_t xvid_dec_frame;
	/* Reset all structures */
	memset(&xvid_dec_frame, 0, sizeof(xvid_dec_frame_t));

	/* Set version */
	xvid_dec_frame.version = XVID_VERSION;
	//xvid_dec_stats->version = XVID_VERSION;

	/* No general flags to set */
	xvid_dec_frame.general          = 0;

	/* Input stream */
	xvid_dec_frame.bitstream        = bitstream;
	xvid_dec_frame.length           = bs_size;

	/* Output frame structure */
	xvid_dec_frame.output.plane[0]  = image;
	xvid_dec_frame.output.stride[0] = m_width*3;
	xvid_dec_frame.output.csp = XVID_CSP_BGR;
	ret = xvid_decore(m_dec_handle, XVID_DEC_DECODE, &xvid_dec_frame, NULL);
	return(ret);    
}

#include "stdafx.h"

/************************************************************************/
/*
* Copyright (C) Meizu Technology Corporation Zhuhai China
* All rights reserved.
* 中国珠海, 魅族科技有限公司, 版权所有.
*
* This file is a part of the Meizu Foundation Classes library.
* Author:    Michael
* Create on: 2008-12-1
*/
/************************************************************************/

// The steps for running this sample code:
// Use Visual Studio 2005/2008 to create a smart device project，
// Choose M8SDK,
// Choose empty project,
// Add a sample.cpp file to the project,
// Copy this peace of code into the sample.cpp,
// Now, you can build the project and try to run it in M8 emulator or device.

// include the MZFC library header file
#include <mzfc_inc.h>

// This sample shows in a MZFC application:
// application's creation & initialization,
// window's creation & initialization，
// using button control, and process its command.

// ID of the button in the window
#define MZ_IDC_TESTBTN1  101

// Main window derived from CMzWndEx
class CSample1MainWnd: public CMzWndEx
{
	MZ_DECLARE_DYNAMIC(CSample1MainWnd);
public:
	// A button control in the window
	UiButton m_btn;

protected:
	// Initialization of the window (dialog)
	virtual BOOL OnInitDialog()
	{
		// Must all the Init of parent class first!
		if (!CMzWndEx::OnInitDialog())
		{
			return FALSE;
		}

		// Then init the controls & other things in the window
		m_btn.SetButtonType(MZC_BUTTON_GREEN);
		m_btn.SetPos(100,250,280,100);
		m_btn.SetID(MZ_IDC_TESTBTN1);
		m_btn.SetText(L"草泥马之歌");
		m_btn.SetTextColor(RGB(255,255,255));

		// Add the control into the window.
		AddUiWin(&m_btn);

		return TRUE;
	}

	// override the MZFC command handler
	virtual void OnMzCommand(WPARAM wParam, LPARAM lParam)
	{
		UINT_PTR id = LOWORD(wParam);
		switch(id)
		{
		case MZ_IDC_TESTBTN1:
			{
				if(1 == MzMessageBoxEx(m_hWnd, L"You have pressed Exit button, Really want exit?", L"Exit", MB_YESNO, false))
					PostQuitMessage(0);
			}
			break;
		}
	}
};

MZ_IMPLEMENT_DYNAMIC(CSample1MainWnd)

// Application class derived from CMzApp
class CSample1App: public CMzApp
{
public:
	// The main window of the app.
	CSample1MainWnd m_MainWnd;

	// Initialization of the application
	virtual BOOL Init()
	{
		// Init the COM relative library.
		CoInitializeEx(0, COINIT_MULTITHREADED);

		// Create the main window
		RECT rcWork = MzGetWorkArea();
		m_MainWnd.Create(rcWork.left,rcWork.top,RECT_WIDTH(rcWork),RECT_HEIGHT(rcWork), 0, 0, 0);
		m_MainWnd.Show();

		// return TRUE means init success.
		return TRUE;
	}
};

// The global variable of the application.
CSample1App theApp;


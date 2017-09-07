
// FractalDisplay.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FractalDisplay.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFractalDisplayApp

BEGIN_MESSAGE_MAP(CFractalDisplayApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFractalDisplayApp construction

CFractalDisplayApp::CFractalDisplayApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CFractalDisplayApp object

CFractalDisplayApp theApp;

// CFractalDisplayApp initialization

BOOL CFractalDisplayApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CMainFrame* pMainFrame = new CMainFrame();
	// create window, including register window
	pMainFrame->CreateEx(0, _T("MAINFRAMECLASS"), _T("Fractal Display"), WS_POPUPWINDOW | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, CRect(0, 0, 960, 910), NULL, NULL, NULL);
	// for conveneince, link pMainFrame to theApp
	m_pMainWnd = pMainFrame;
	// show and update the window
	pMainFrame->CenterWindow();
	pMainFrame->ShowWindow(SW_SHOW);
	pMainFrame->UpdateWindow();

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return TRUE;
}


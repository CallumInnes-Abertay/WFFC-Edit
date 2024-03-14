#pragma once
#include <afxwin.h>
#include <afxext.h>


// CChildView window

class CChildRender : public CWnd
{
	// Construction
public:
	CChildRender();

	// Attributes
public:
	// Operations
public:
	// Overrides
protected:
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	// Implementation
public:
	~CChildRender() override;

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

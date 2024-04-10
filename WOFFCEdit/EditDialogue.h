#pragma once
#include <memory>

#include "afxdialogex.h"
#include "resource.h"
#include "afxwin.h"
#include "SceneObject.h"
#include <vector>

// EditDialogue dialog

class ObjectHandler;

class EditDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(EditDialogue)

public:
	EditDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph);
	// modal // takes in out scenegraph in the constructor
	EditDialogue(CWnd* pParent = nullptr);
	~EditDialogue() override;
	void SetObjectData(std::vector<SceneObject>* SceneGraph, int* Selection);
	//passing in pointers to the data the class will operate on.

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override; // DDX/DDV support
	afx_msg void End(); //kill the dialogue

	std::vector<SceneObject>* m_sceneGraph;
	int* m_currentSelection;


	DECLARE_MESSAGE_MAP()

public:
	// Control variable for more efficient access of the listbox
	CListBox m_listBox;
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonApply();

	std::shared_ptr<ObjectHandler> object_handler;
};


INT_PTR CALLBACK SelectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

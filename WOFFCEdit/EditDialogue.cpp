// EditDialogue.cpp : implementation file
//

#include "EditDialogue.h"
#include "DisplayObject.h"

#include "stdafx.h"

// EditDialogue dialog

IMPLEMENT_DYNAMIC(EditDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(EditDialogue, CDialogEx)
	ON_COMMAND(IDOK, &EditDialogue::End) //ok button
	ON_BN_CLICKED(IDOK, &EditDialogue::OnBnClickedOk)
	ON_BN_CLICKED(ID_BUTTON_APPLY, &EditDialogue::OnBnClickedButtonApply)
END_MESSAGE_MAP()


EditDialogue::EditDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph) //constructor used in modal
	: CDialogEx(IDD_EDIT_DIALOG, pParent)
{
	m_sceneGraph = SceneGraph;
}

EditDialogue::EditDialogue(CWnd* pParent) //constructor used in modeless
	: CDialogEx(IDD_EDIT_DIALOG, pParent)
{
}

EditDialogue::~EditDialogue()
{
}

///pass through pointers to the data in the tool we want to manipulate
void EditDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph, int* selection)
{
	m_sceneGraph = SceneGraph;
	m_currentSelection = selection;
}


void EditDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DisplayObject newObjectParam;

	DDX_Text(pDX, IDC_EDIT_X_POSITION, newObjectParam.m_position.x);
	DDX_Text(pDX, IDC_EDIT_Y_POSITION, newObjectParam.m_position.y);
	DDX_Text(pDX, IDC_EDIT_Z_POSITION, newObjectParam.m_position.z);

	DDX_Text(pDX, IDC_EDIT_X_SCALE, newObjectParam.m_scale.x);
	DDX_Text(pDX, IDC_EDIT_Y_SCALE, newObjectParam.m_scale.y);
	DDX_Text(pDX, IDC_EDIT_Z_SCALE, newObjectParam.m_scale.z);


	DDX_Text(pDX, IDC_EDIT_X_ROTATION, newObjectParam.m_orientation.x);
	DDX_Text(pDX, IDC_EDIT_Y_ROTATION, newObjectParam.m_orientation.y);
	DDX_Text(pDX, IDC_EDIT_Z_ROTATION, newObjectParam.m_orientation.z);
}


void EditDialogue::End()
{
	DestroyWindow();
	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

BOOL EditDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//uncomment for modal only
	/*	//roll through all the objects in the scene graph and put an entry for each in the listbox
		int numSceneObjects = m_sceneGraph->size();
		for (size_t i = 0; i < numSceneObjects; i++)
		{
			//easily possible to make the data string presented more complex. showing other columns.
			std::wstring listBoxEntry = std::to_wstring(m_sceneGraph->at(i).ID);
			m_listBox.AddString(listBoxEntry.c_str());
		}*/

	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void EditDialogue::PostNcDestroy()
{
}

void EditDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void EditDialogue::OnBnClickedButtonApply()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
}

// SelectDialogue.cpp : implementation file
//

#include "stdafx.h"
#include "SelectDialogue.h"

#include "ObjectHandler.h"

// SelectDialogue dialog

IMPLEMENT_DYNAMIC(SelectDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(SelectDialogue, CDialogEx)
	ON_WM_CLOSE(&SelectDialogue::End)
	ON_COMMAND(IDOK, &SelectDialogue::End) //ok button
	ON_BN_CLICKED(IDOK, &SelectDialogue::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &SelectDialogue::Select) //listbox

END_MESSAGE_MAP()


SelectDialogue::SelectDialogue(CWnd* pParent, std::vector<SceneObject>* SceneGraph) //constructor used in modal
	: CDialogEx(IDD_SELECT_DIALOG, pParent)
{
	m_sceneGraph = SceneGraph;
}

SelectDialogue::SelectDialogue(CWnd* pParent) //constructor used in modeless
	: CDialogEx(IDD_SELECT_DIALOG, pParent)
{
}

SelectDialogue::~SelectDialogue()
{
}

///pass through pointers to the data in the tool we want to manipulate
void SelectDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph)
{
	m_sceneGraph = SceneGraph;

	//roll through all the objects in the scene graph and put an entry for each in the listbox
	int numSceneObjects = ObjectHandler::Instance().GetDisplayObjects()->size();
	for (int i = 0; i < numSceneObjects; i++)
	{
		//easily possible to make the data string presented more complex. showing other columns.in
		int id = ((*ObjectHandler::Instance().GetDisplayObjects())[i].m_ID);
		std::wstring listBoxEntry = std::to_wstring(id);
		m_listBox.AddString(listBoxEntry.c_str());
	}
}


void SelectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
}

void SelectDialogue::End()
{
	ObjectHandler::Instance().m_isEditing = false;
	DestroyWindow();
	//destory the window properly.  INcluding the links and pointers created.  THis is so the dialogue can start again. 
}

//BOOL SelectDialogue::PreTranslateMessage(MSG* pMsg)
//{
//	if (pMsg->message == WM_LBUTTONDOWN && // Check for left mouse button click
//		GetDlgItem(IDCANCEL) == GetFocus())   // Check if close button has focus
//	{#
//		ObjectHandler::Instance().m_isEditing = false;
//		End();  // Call your End function to close the window and stop editing
//		return TRUE;  // Message handled, don't call default processing
//	}
//
//	return CDialog::PreTranslateMessage(pMsg);  // Pass other messages to base class
//}

void SelectDialogue::Select()
{
	int index = m_listBox.GetCurSel();
	CString currentSelectionValue;

	m_listBox.GetText(index, currentSelectionValue);

	m_currentSelection = _ttoi(currentSelectionValue);

	// If something is already selected
	if (!ObjectHandler::Instance().m_selectedObjects.empty())
	{
		//Then removed that texture
		ObjectHandler::Instance().RemoveAllTextureChanges();
	}

	//If it's already selected
	if (std::find(ObjectHandler::Instance().m_selectedObjects.begin(),
	              ObjectHandler::Instance().m_selectedObjects.end(),
	              m_currentSelection) != ObjectHandler::Instance().m_selectedObjects.end())
	{
		bool wasDuplicated = false;
		if (!ObjectHandler::Instance().m_selectedObjects.empty())
		{
			wasDuplicated = RemoveIntFromVector(ObjectHandler::Instance().m_selectedObjects, m_currentSelection);
		}
		// If the selected object wasn't already in the vector, then add it and change the texture appropriately.
		if (m_currentSelection != -1 && !wasDuplicated)
		{
			ObjectHandler::Instance().m_selectedObjects.push_back(m_currentSelection);

			ObjectHandler::Instance().MultiTextureChange();
			return;
		}
		//If it was, then it's already removed, and thus should have a deselected texture.
		if (m_currentSelection != -1 && wasDuplicated)
		{
			ObjectHandler::Instance().RemoveTextureChange(m_currentSelection);
		}
	}
	//And set a new selection and then add a selected texture to it.
	ObjectHandler::Instance().m_selectedObjects.push_back(m_currentSelection);
	ObjectHandler::Instance().MultiTextureChange();
}

BOOL SelectDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ObjectHandler::Instance().m_isEditing = true;


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

void SelectDialogue::PostNcDestroy()
{
	ObjectHandler::Instance().m_isEditing = false;
}


// SelectDialogue message handlers callback   - We only need this if the dailogue is being setup-with createDialogue().  We are doing
//it manually so its better to use the messagemap
/*INT_PTR CALLBACK SelectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		//	EndDialog(hwndDlg, wParam);
			DestroyWindow(hwndDlg);
			return TRUE;
			

		case IDCANCEL:
			EndDialog(hwndDlg, wParam);
			return TRUE;
			break;
		}
	}
	
	return INT_PTR();
}*/


void SelectDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}

void SelectDialogue::OnClose()
{
	End();
}

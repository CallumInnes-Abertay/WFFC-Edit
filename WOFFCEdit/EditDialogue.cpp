// EditDialogue.cpp : implementation file
//

#include "EditDialogue.h"

#include <regex>

#include "DisplayObject.h"
#include "ObjectHandler.h"

#include "stdafx.h"

// EditDialogue dialog

IMPLEMENT_DYNAMIC(EditDialogue, CDialogEx)

//Message map.  Just like MFCMAIN.cpp.  This is where we catch button presses etc and point them to a handy dandy method.
BEGIN_MESSAGE_MAP(EditDialogue, CDialogEx)
	ON_WM_CLOSE(&EditDialogue::End)
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
void EditDialogue::SetObjectData(std::vector<SceneObject>* SceneGraph, int selection)
{
	m_sceneGraph = SceneGraph;
	m_currentSelection = selection;
}


void EditDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	if (ObjectHandler::IsInstanceMade())
	{
		ObjectHandler::Instance().m_isEditing = true;
		newObjectParams = ObjectHandler::Instance().GetLastSelectedDisplayObject();
	}

	//Set position
	DDX_Text(pDX, IDC_EDIT_X_POSITION, newObjectParams.m_position.x);
	DDX_Text(pDX, IDC_EDIT_Y_POSITION, newObjectParams.m_position.y);
	DDX_Text(pDX, IDC_EDIT_Z_POSITION, newObjectParams.m_position.z);

	//Set scale
	DDX_Text(pDX, IDC_EDIT_X_SCALE, newObjectParams.m_scale.x);
	DDX_Text(pDX, IDC_EDIT_Y_SCALE, newObjectParams.m_scale.y);
	DDX_Text(pDX, IDC_EDIT_Z_SCALE, newObjectParams.m_scale.z);


	//Set rotation
	DDX_Text(pDX, IDC_EDIT_X_ROTATION, newObjectParams.m_orientation.x);
	DDX_Text(pDX, IDC_EDIT_Y_ROTATION, newObjectParams.m_orientation.y);
	DDX_Text(pDX, IDC_EDIT_Z_ROTATION, newObjectParams.m_orientation.z);

	newObjectParams.m_ID = m_currentSelection;
	if (ObjectHandler::Instance().m_objectHistory.empty())
	{
		//Set the initial value of the object before editing.
		//newObjectParams.m_ID = *m_currentSelection;
		ObjectHandler::Instance().m_objectHistory.push(newObjectParams);
	}
}

BOOL EditDialogue::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_CHAR)
	{
		const int controls[] = {
			IDC_EDIT_X_POSITION, IDC_EDIT_Y_POSITION, IDC_EDIT_Z_POSITION,
			IDC_EDIT_X_SCALE, IDC_EDIT_Y_SCALE, IDC_EDIT_Z_SCALE,
			IDC_EDIT_X_ROTATION, IDC_EDIT_Y_ROTATION, IDC_EDIT_Z_ROTATION
		};
		for (int id : controls)
		{
			if (pMsg->hwnd == GetDlgItem(id)->m_hWnd)
			{
				if (!(isdigit(pMsg->wParam) || pMsg->wParam == VK_BACK ||
					pMsg->wParam == '+' || pMsg->wParam == '-' || pMsg->wParam == '.' ||
					pMsg->wParam == VK_DELETE))
				{
					return TRUE; // Stop processing this message (ignore input)
				}
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void EditDialogue::OnKillfocusEdit(CEdit& controlEdit)
{
	UpdateData(TRUE); // Update variables bound to controls
	CString strValue;
	controlEdit.GetWindowText(strValue);
	if (!IsValidFloat(strValue))
	{
		AfxMessageBox(_T("Enter a valid floating point number!"));
		controlEdit.SetFocus(); // Set focus back to the control
	}
}


void EditDialogue::End()
{
	ObjectHandler::Instance().m_isEditing = false;
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


//When apply is clicked then update the selected object.
void EditDialogue::OnBnClickedButtonApply()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	if (ObjectHandler::IsInstanceMade())
	{
		ObjectHandler::Instance().SetDisplayObject(newObjectParams);
	}
	//Then add it to a history of objects to allow undo.
	ObjectHandler::Instance().m_objectHistory.push(newObjectParams);
}

void EditDialogue::OnClose()
{
	CDialogEx::OnCancel();
	End();
}

bool EditDialogue::IsValidFloat(const CString& strInput)
{
	std::wregex regPattern(L"^[-+]?[0-9]*\\.?[0-9]+$"); // Regex for floating-point numbers
	return std::regex_match(static_cast<LPCTSTR>(strInput), regPattern);
}

#include "MFCMain.h"


#include "resource.h"
#include "ObjectHandler.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	ON_COMMAND(ID_FILE_QUIT, &MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_EDIT_EDIT, &MFCMain::MenuEditEdit)
	ON_COMMAND(ID_EDIT_SPAWNOBJECT, &MFCMain::MenuEditSpawnObject)
	ON_COMMAND(ID_BUTTON40001, &MFCMain::ToolBarButton1)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	m_frame->Create(nullptr,
	                _T("World Of Flim-Flam Craft Editor"),
	                WS_OVERLAPPEDWINDOW,
	                CRect(100, 100, 1024, 768),
	                nullptr,
	                nullptr,
	                0,
	                nullptr
	);

	//show and set the window to run and update. 
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();


	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd(); //handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width = WindowRECT.Width();
	m_height = WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_width, m_height);

	return TRUE;
}

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, nullptr, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, nullptr, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			m_ToolSystem.UpdateInput(&msg);
		}
		else
		{
			std::wstring selectedObjectsString;
			selectedObjectsString = VectorToWideString(ObjectHandler::Instance().m_selectedObjects);
			std::wstring statusString = selectedObjectsString;
			m_ToolSystem.Tick(&msg);

			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);
		}
	}

	return static_cast<int>(msg.wParam);
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}


//Set up the spawn dialog
void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);		//create our dialoguebox //modal constructor
	//m_ToolSelectDialogue.DoModal();	// start it up modal

	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	m_ToolSelectDialogue.Create(IDD_SELECT_DIALOG); //Start up modeless
	m_ToolSelectDialogue.ShowWindow(SW_SHOW); //show modeless
	if (m_ToolSystem.m_selectedObject != nullptr)
	{
		m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph);
	}
	else
	{
		m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph);
	}
}

//Set up the edit dialog
void MFCMain::MenuEditEdit()
{
	//If the user hasn't selected an object to edit.
	if (ObjectHandler::Instance().m_selectedObjects.empty())
	{
		CString message;
		// Load the string from the string table.
		message.LoadString(IDS_NO_OBJECTS_SELECTED);

		// Display error message box with clear instructions and an appropriate icon
		AfxMessageBox(message, MB_ICONEXCLAMATION | MB_OK);
		// Can return early since no more is needed.
		return;
	}

	if (m_ToolSystem.m_selectedObject != nullptr)
	{
		m_ToolEditDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, ((*m_ToolSystem.m_selectedObject)[0]));
	}
	else
	{
		m_ToolEditDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, -1);
	}

	m_ToolEditDialogue.Create(IDD_EDIT_DIALOG); //Start up modeless
	m_ToolEditDialogue.ShowWindow(SW_SHOW); //show modeless
}

//Set up the spawn object dialog
void MFCMain::MenuEditSpawnObject()
{
	m_ToolSpawnObjectDialogue.Create(IDD_SPAWN_OBJECT_DIALOG); //Start up modeless
	m_ToolSpawnObjectDialogue.ShowWindow(SW_SHOW); //show modeless
}

void MFCMain::ToolBarButton1()
{
	m_ToolSystem.onActionSave();
}

std::wstring MFCMain::VectorToWideString(const std::vector<int>& vec)
{
	std::wstringstream ss;
	ss << L"The selected objects are:";

	// Iterate over the vector
	for (size_t i = 0; i < vec.size(); ++i)
	{
		// Add the current id to the string
		ss << vec[i];
		// Add a comma if it's not the last element
		if (i < vec.size() - 1)
		{
			ss << L",";
		}
	}

	return ss.str();
}


MFCMain::MFCMain()
{
}


MFCMain::~MFCMain()
{
}

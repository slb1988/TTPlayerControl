// TTPlayerControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TTPlayerControl.h"
#include "TTPlayerControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTTPlayerControlDlg 对话框




CTTPlayerControlDlg::CTTPlayerControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTTPlayerControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	parentHandler = listHandler = nextSongHandler = lastSongHandler = startSongHandler = stopSongHandler = exitSongHandler = NULL;
}

void CTTPlayerControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTTPlayerControlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_START_PAUSE, &CTTPlayerControlDlg::OnBnClickedStartPause)
	ON_BN_CLICKED(IDC_BTN_STOP, &CTTPlayerControlDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_PREV, &CTTPlayerControlDlg::OnBnClickedBtnPrev)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CTTPlayerControlDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_SHOW_WINDOW, &CTTPlayerControlDlg::OnBnClickedShowWindow)
	ON_BN_CLICKED(IDC_HIDE_WINDOW, &CTTPlayerControlDlg::OnBnClickedHideWindow)
	ON_BN_CLICKED(IDC_BTN_GETHANDLE, &CTTPlayerControlDlg::OnBnClickedBtnGethandle)
END_MESSAGE_MAP()


// CTTPlayerControlDlg 消息处理程序

BOOL CTTPlayerControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	HANDLE m_listenHandler = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ServerThread, this, 0, NULL);

	parentHandler = FindWindow(L"TTPlayer_PlayerWnd",NULL)->GetSafeHwnd();
	listHandler = FindWindow(L"TTPlayer_PlayListWnd",NULL)->GetSafeHwnd();
	if (parentHandler == NULL)
	{
		// 启动ttplayer
		ShellExecute(NULL, NULL,L"ttplayer.exe",NULL,NULL,SW_HIDE);
	}
	else
	{
		GetHandler();
	}

	while( parentHandler == NULL)
		parentHandler = FindWindow(L"TTPlayer_PlayerWnd",NULL)->GetSafeHwnd();

	while (exitSongHandler == NULL || nextSongHandler == NULL || lastSongHandler == NULL || startSongHandler == NULL || stopSongHandler == NULL)
		GetHandler();

	//FindWindow(parentHandler,NULL,"SkinButton",L"");
	//HWND handler = FindWindowEx(parentHandler,NULL,L"SkinButton",L"")->GetSafeHwnd();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTTPlayerControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTTPlayerControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTTPlayerControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTTPlayerControlDlg::SendMessageTT(HWND handler)
{
	::SendMessage(handler,WM_LBUTTONDOWN,NULL,NULL);
	::SendMessage(handler,WM_LBUTTONUP, NULL,NULL);
}

void CTTPlayerControlDlg::OnBnClickedStartPause()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessageTT(startSongHandler);
}

void CTTPlayerControlDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessageTT(stopSongHandler);
}

void CTTPlayerControlDlg::OnBnClickedBtnPrev()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessageTT(lastSongHandler);
}

void CTTPlayerControlDlg::OnBnClickedBtnNext()
{
	// TODO: 在此添加控件通知处理程序代码
	SendMessageTT(nextSongHandler);
}

void CTTPlayerControlDlg::OnBnClickedShowWindow()
{
	// TODO: 在此添加控件通知处理程序代码
	::ShowWindow(parentHandler, 1);
	::ShowWindow(listHandler, 1);
}

void CTTPlayerControlDlg::OnBnClickedHideWindow()
{
	// TODO: 在此添加控件通知处理程序代码
	::ShowWindow(parentHandler, 0);
	::ShowWindow(listHandler, 0);
}

void CTTPlayerControlDlg::GetHandler(void)
{
	listHandler = FindWindow(L"TTPlayer_PlayListWnd",NULL)->GetSafeHwnd();
	nextSongHandler = ::GetDlgItem(parentHandler, 32006);
	lastSongHandler = ::GetDlgItem(parentHandler, 32005);
	startSongHandler = ::GetDlgItem(parentHandler, 32000);
	stopSongHandler = ::GetDlgItem(parentHandler, 32002);
	exitSongHandler = ::GetDlgItem(parentHandler, 57665);
}

void CTTPlayerControlDlg::OnBnClickedBtnGethandle()
{
	// TODO: 在此添加控件通知处理程序代码
	GetHandler();
}

UINT CTTPlayerControlDlg::ServerThread(void* lpParam)
{
	CTTPlayerControlDlg *dlg = (CTTPlayerControlDlg*)lpParam;
	//dlg->ShowMessage(L"hello");

	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	//	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo * result = NULL;
	struct addrinfo hints;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0)
	{
		CString str;
		str.Format(L"WSAStartup failed with error: %d\n", iResult);
		//dlg->ShowMessage(str);
		AfxMessageBox(str);
		return -1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol =  IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//Resolve the server address and port
	iResult = getaddrinfo(NULL,DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		CString str;
		str.Format(L"getaddrinfo failed with error: %d\n", iResult);
		//dlg->ShowMessage(str);
		AfxMessageBox(str);
		return -1;
	} 

	//Creating a Socket for the server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		CString str;
		str.Format(L"Socket Failed with error: %ld\n", WSAGetLastError());
		AfxMessageBox(str);
		WSACleanup();
		return -1;
	}

	// Setup the TCP listening socket, Binding a Socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		CString str;
		str.Format(L"Bind failed with error: %d\n", WSAGetLastError());
		AfxMessageBox(str);
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}
	freeaddrinfo(result);

	//Listen on a socket
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		CString str;
		str.Format(L"Listen failed with error: %d\n", WSAGetLastError());
		AfxMessageBox(str);
		closesocket(ListenSocket);
		WSACleanup();
		return -1;
	}

	while (true)
	{
		//Accept a client socket
		SOCKET clientSocket = accept(ListenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			CString str;
			str.Format(L"Accept failed with error: %d\n", WSAGetLastError());
			AfxMessageBox(str);
			closesocket(ListenSocket);
			WSACleanup();
			return -1;
		}

		//初始化结构体传递参数给 用户线程进行处理
		CLIENT_INF *pInfo = new CLIENT_INF;
		memset(pInfo, 0, sizeof(CLIENT_INF));			//初始化pIOInfo
		memset(pInfo->Buffer, 0, DEFAULT_BUFLEN);
		//memset(pIOInfo->Overlapped, 0, sizeof(OVERLAPPED));		//初始化重叠结构
		pInfo->socket = clientSocket;
		pInfo->dwRecvLen = DEFAULT_BUFLEN;
		pInfo->dwSendLen = DEFAULT_BUFLEN;
		pInfo->process = dlg;
		// 		pInfo->RecBuf.len = DEFAULT_BUFLEN;
		// 		pInfo->RecBuf.buf = pIOInfo->Buffer;

		HANDLE  hThread;  
		hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClientThread, pInfo, 0, NULL);
		if (hThread == NULL)
		{
			AfxMessageBox(L"CreateThread error!");
			return -1;
		}
		//HANDLE m_listenHandler = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenSocket, this, 0, NULL);
		//closesocket(clientSocket);
	}

	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}

UINT CTTPlayerControlDlg::ClientThread(void* lpParam)
{
	CLIENT_INF * pInfo = (CLIENT_INF*)lpParam;
	int iResult = 0;
	int iSendResult = 0;


	do 
	{
		//这里存在Unicode 和 char * 转化的问题
		//CString str;
		iResult = recv(pInfo->socket, pInfo->Buffer, pInfo->dwRecvLen, 0);

		char *dst = (char*)malloc(22);
		pInfo->process->UnicodeToAnsi(pInfo->Buffer, iResult, dst);

		if (iResult > 0)
		{
			if ( !strncmp(dst, "start/pause", 11) )
				pInfo->process->OnBnClickedStartPause();
			else if ( !strncmp(dst, "prev", 4) )
				pInfo->process->OnBnClickedBtnPrev();
			else if ( !strncmp(dst, "next", 4) )
				pInfo->process->OnBnClickedBtnNext();
			else if ( !strncmp(dst, "stop", 4) )
				pInfo->process->OnBnClickedBtnStop();
			
			//Echo the buffer back to the sender
// 			iSendResult = send(pInfo->socket, pInfo->Buffer, iResult, 0);
// 			if (iSendResult == SOCKET_ERROR)
// 			{
// 				AfxMessageBox(L"send failed!");
// 				return -1;
// 			}
			//pInfo->process->OnBnClickedStartPause();
		}
		else if (iResult == 0)
		{	//AfxMessageBox(L"Connetion closing...\n");
		}
		else
		{
			AfxMessageBox(L"recv failed!");
			return -1;
		}
	} while (iResult > 0);

	closesocket(pInfo->socket);	
	return 0;
}

int CTTPlayerControlDlg::RecvMsg(void* lpParam)
{
	return 0;
}

char* CTTPlayerControlDlg::UnicodeToAnsi(char* src, int len, char * dst)
{
	char * dst2 = dst;
	for (int i = 0; i < len; i++, src++)
	{
		if (*src == NULL)
		{
			*(dst2++) = *(src+1);
		}
		//src++;
	}
	
	*(dst2+1) = NULL;

	return NULL;
}

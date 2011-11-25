// TTPlayerControlDlg.h : 头文件
//
#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"Ws2_32.lib")

#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "27000"

class CTTPlayerControlDlg;

typedef struct CLIENT_INF
{
	//OVERLAPPED Overlapped;			//IO重叠结果
	SOCKET socket;						//套接字
	char Buffer[DEFAULT_BUFLEN];				//用户数据缓冲区
	//	WSABUF	RecBuf;						//数据缓存区
	DWORD dwSendLen;					//发送数据长度
	DWORD dwRecvLen;					//接收数据长度
	CTTPlayerControlDlg * process;
	//NetEvent neType;					//网络事件
}*LPCLIENT_INF;


//与客户端通信消息列表
typedef struct SOCKET_INF
{
	SOCKET socket;
	int type;
	char Buffer[DEFAULT_BUFLEN];
	DWORD dwSendLen;
	DWORD dwRecvLen;
}*LPSOCKET_INF;

// CTTPlayerControlDlg 对话框
class CTTPlayerControlDlg : public CDialog
{
// 构造
public:
	CTTPlayerControlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TTPLAYERCONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	HWND parentHandler;
	HWND listHandler;
	HWND nextSongHandler;
	HWND lastSongHandler;
	HWND startSongHandler;
	HWND stopSongHandler;
	HWND exitSongHandler;

	bool bServerStop;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void SendMessageTT(HWND handler);
	afx_msg void OnBnClickedStartPause();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnPrev();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedShowWindow();
	afx_msg void OnBnClickedHideWindow();
	void GetHandler(void);
	afx_msg void OnBnClickedBtnGethandle();
	UINT static ServerThread(void* lpParam);
	UINT static ClientThread(void* lpParam);
	int RecvMsg(void* lpParam);
	char* UnicodeToAnsi(char* unStr, int , char *);
};


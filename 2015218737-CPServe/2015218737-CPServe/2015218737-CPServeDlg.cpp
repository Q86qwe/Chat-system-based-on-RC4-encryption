
// 2015218737-CPServeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "2015218737-CPServe.h"
#include "2015218737-CPServeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_PORT 2000//�����������˿�
SOCKET m_Listening;//����socket
HANDLE m_hListenThread;//�߳̾��
char iKey[256] = "justfortest";//��ʼ��Կ���ɱ䳤�����256
unsigned char sBox[256] = { 0 };//S-box
unsigned char s_Box[256] = { 0 };
// CAboutDlg dialog used for App About

void rc4_init(unsigned char *s, char *key, unsigned long Len) /*��ʼ��������sΪS-box��keyΪ��ʼ��Կ��LenΪ��ʼ��Կ�ĳ���*/
{
	int i = 0, j = 0;
	char k[256] = { 0 };//�����Կ��
	unsigned char tmp = 0;
	//cout << "��Կ���ȣ�" << Len<<endl;
	//��һ��	
	/***************************/
	//�˴���Ӷ������ʵ��S-box����Կ���ĳ�ʼ��
	for (; i < 256; i++)
	{
		s[i] = i;
		k[i] = key[i % Len];
	}
	/***************************/

	/*�ڶ��������ڴ���S-box������iѭ�����ڱ�֤S-boxÿλ����֤�õ�������key��ÿ���ֽڵ�������Ϊ�������������*/
	/***************************/
	//�˴���Ӷ������ʵ��S-box������
	for (i = 0; i < 256; i++)
	{
		j = (j + s[i] + k[i]) % 256;
		tmp = s[j];
		s[j] = s[i];
		s[i] = tmp;
	}
	/***************************/

}

void rc4_crypt(unsigned char*s, char*Data, unsigned long Len) /*�ӽ��ܣ�sΪS-box��DataΪ���ݣ�LenΪ���ݵĳ���*/
{
	int i = 0, j = 0, t = 0;
	unsigned long k = 0;
	unsigned char tmp;
	//cout << "���ĳ��ȣ�" << Len << endl;
	/***************************/
	//�˴���Ӷ������ӽ���
	for (; k < Len; k++)
	{
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
		t = (s[i] + s[j]) % 256;
		Data[k] ^= s[t];
	}


	/***************************/
}


char * CSt2char(CString str)
{
	//ע�⣺����n��len��ֵ��С��ͬ,n�ǰ��ַ�����ģ� len�ǰ��ֽڼ����
	int n = str.GetLength(); // n = 14, len = 18//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	char * buff = new char[len + 1]; //���ֽ�Ϊ��λ
	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP, 0, str, str.GetLength() + 1, buff, len + 1, NULL, NULL);
	buff[len + 1] = '\0'; //���ֽ��ַ���'/0'����
	return buff;
}
CString char2CSt(char * szBuf)
{
	//����char *�����С�����ֽ�Ϊ��λ��һ������ռ�����ֽ�
	int charLen = strlen(szBuf);
	//������ֽ��ַ��Ĵ�С�����ַ����㡣
	int len = MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, NULL, 0);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ķ��ֽ��ַ���С
	TCHAR *buf = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, szBuf, charLen, buf, len);
	buf[len] = '\0'; //����ַ�����β��ע�ⲻ��len+1
	//��TCHAR����ת��ΪCString
	CString pWideChar;
	pWideChar.Append(buf);
	//ɾ��������
	delete[]buf;
	return pWideChar;
}


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



// CMy2015218737CPServeDlg dialog

DWORD WINAPI ListenThread(LPVOID lpparam)
{
	int length = 0;//�����ַ�������
	CMy2015218737CPServeDlg* pDlg = (CMy2015218737CPServeDlg*)lpparam;
	if (pDlg == NULL) return 0;//��ȡ��ǰ���Ի����ָ��
	SOCKET ConnectSocket;//��¼��׽��������
	sockaddr_in ClientAddr;//��¼���ݰ���IP��ַ
	int nLen = sizeof(sockaddr);
	if (listen(m_Listening, 40) == SOCKET_ERROR)//��ʼ�����Ƿ��пͻ������ӡ�{
		return 0;

	char szBuf[MAX_PATH];//���ݻ�����
	memset(szBuf,0,MAX_PATH);//��ʼ��������
	char szBuf_key[20];//������Կ
	memset(szBuf_key, 0, 20);//��ʼ��������
	char szBuf_mi[200];//��������
	memset(szBuf_mi, 0, 200);//��ʼ��������

	while(1)
	{
		ConnectSocket = accept(m_Listening, (sockaddr*)&ClientAddr, &nLen);//����ֱ���пͻ�����
		//�ӣ���Ȼ���˷�CPU��Դ
		char *pAddrname = inet_ntoa(ClientAddr.sin_addr);//�õ��ͻ��˵�IP��ַ
		memset(szBuf, 0, MAX_PATH);//ÿ�ν�������ǰ��ջ�����
		if(recv(ConnectSocket, szBuf, sizeof szBuf, 0) != SOCKET_ERROR)
		{
			/*����A�ַ���ɶ���Կ��������Ϣ�ķֿ���ȡ*/
			length = strlen(szBuf);
			int i = 0,j = 0;
			for (; i < length; i++)
			{
				if (szBuf[i] != 'A')
					szBuf_mi[i] = szBuf[i];
				else
					break;
			}
			for (;i < length ;i++ )
				szBuf_key[j++] = szBuf[i];

			
			pDlg->m_get_mi.SetWindowTextW(char2CSt(szBuf_key));
			
			
			pDlg->m_encode.SetWindowTextW(char2CSt(szBuf_mi));
			rc4_init(sBox, szBuf_key, strlen(szBuf_key));
			
			for (int x = 0; x < 256; x++)
			{
				s_Box[x] = sBox[x];
			}
			rc4_crypt(s_Box, szBuf_mi, strlen(szBuf_mi));
			pDlg->m_edtMessage.SetWindowTextW(char2CSt(szBuf_mi));
			
			
			char buff[MAX_PATH] = "ţ��ţ��";
			strcat_s(buff, pAddrname);
			strcat_s(buff, "�� ");
			send(ConnectSocket, buff, sizeof buff, 0);
			
		}
	}
	return 0;
}

CMy2015218737CPServeDlg::CMy2015218737CPServeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy2015218737CPServeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy2015218737CPServeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_HOST, HostIP);
	DDX_Control(pDX, IDC_BUTTON_START, m_btnStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_btnStop);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_edtMessage);
	DDX_Control(pDX, IDC_EDIT_MESSAGE2, m_encode);
	DDX_Control(pDX, IDC_EDIT_get_mi, m_get_mi);
}

BEGIN_MESSAGE_MAP(CMy2015218737CPServeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMy2015218737CPServeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_START, &CMy2015218737CPServeDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMy2015218737CPServeDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CMy2015218737CPServeDlg message handlers

BOOL CMy2015218737CPServeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	WSADATA wsaData;//�ṹ���������溯��WSAStartup���ص�Windows Socket ��ʼ����Ϣ
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		AfxMessageBox(_T("WSAStartup error�� "));
		return FALSE;
	}//��һ��Ӧ�ó������WSAStartup����ʱ������ϵͳ���������Socket�汾��������Ӧ��Socket
	//�⣬Ȼ����ҵ���Socket�⵽��Ӧ�ó�����
		//��ʾҪʹ��1.1�汾��Socket
	GetLocalIP();//��ȡ IP ��ַ
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy2015218737CPServeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy2015218737CPServeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy2015218737CPServeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy2015218737CPServeDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	WSACleanup();
	CDialogEx::OnCancel();
}


void CMy2015218737CPServeDlg::GetLocalIP()
{
	char HostName[128];//��¼������
	char *IPAddress;//��¼IP��ַ
	if (gethostname(HostName, 128) == 0)//��ȡ�������ɹ�
	{
		struct hostent *pHost;
		pHost = gethostbyname(HostName);//����������������ȡIP��ַ
		IPAddress = pHost->h_addr_list[0];//ȡ��һ����ַ
		HostIP.SetAddress(*IPAddress, *(IPAddress + 1), *(IPAddress + 2), *(IPAddress + 3));//����IP��ַ
		
		UpdateData(FALSE);//ˢ�½���
	}
}


void CMy2015218737CPServeDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	m_btnStart.EnableWindow(FALSE);
	m_btnStop.EnableWindow(TRUE);

	sockaddr_in local;//ʹ��sockaddr_in�ṹָ��IP��ַ�Ͷ˿���Ϣ
	local.sin_family = AF_INET;
	local.sin_port = htons(DEFAULT_PORT);//���õĶ˿�ΪDEFAULT_PORT�������ͱ����������ֽ�˳��ת��������ֽ�˳��
	char IPAddress[MAX_PATH];//��¼IP��ַ
	BYTE add1, add2, add3, add4;
	HostIP.GetAddress(add1, add2, add3, add4);
	sprintf_s(IPAddress, "%d.%d.%d.%d", add1, add2, add3, add4);
	local.sin_addr.S_un.S_addr = inet_addr(IPAddress);//��һ�����ʮ���Ƶ�IPת����һ������������(u_long����), ���ص�������ʽ�������ֽ���
	m_Listening = socket(AF_INET, SOCK_STREAM, 0);//��ʼ��Socket������TCP��ʽ-SOCK_STREAM
	if (m_Listening == INVALID_SOCKET)//����ʧ��
	{
		AfxMessageBox(_T("��ʼ��socketʧ�ܣ� "));
		return;
	}
	if (bind(m_Listening, (LPSOCKADDR)& local, sizeof(local)) == SOCKET_ERROR)//��IP��ַ�Ͷ˿ڰ󶨵����������׽�����
	{
		AfxMessageBox(_T("��ʧ�ܣ� "));
		closesocket(m_Listening);
		return;
	}
	//���������߳�
	DWORD dwThreadID = 0;
	m_hListenThread = ::CreateThread(NULL, 0, ListenThread, this, 0, &dwThreadID);//ListenThreadΪ�̺߳����� thisΪ��ǰ���Ի����ָ��
}


void CMy2015218737CPServeDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	m_btnStart.EnableWindow(TRUE);
	m_btnStop.EnableWindow(FALSE);

	TerminateThread(m_hListenThread, 0);//�ر��߳�
	CloseHandle(m_hListenThread);//�ͷ���Դ
	shutdown(m_Listening, 2);//�رն�дͨ��
	closesocket(m_Listening);//�ͷ�socket
}

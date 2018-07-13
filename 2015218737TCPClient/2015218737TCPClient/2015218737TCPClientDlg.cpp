
// 2015218737TCPClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "2015218737TCPClient.h"
#include "2015218737TCPClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define DEFAULT_PORT 2000//�����������˿ڣ�ע����������˱���һ��
char iKey[256] = "Ajustfortest";//��ʼ��Կ���ɱ䳤�����256
unsigned char sBox[256] = { 0 };//S-box
unsigned char s_Box[256] = { 0 };


void get_rdom_key(char *k,int i)//���������Կ
{
	int x = 1;
	for (; x < i;x++)
		k[x] = 'a' + rand() % 26;
	
}

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

void rc4_crypt(unsigned char*s, char*Data, char*Data2, unsigned long Len) /*�ӽ��ܣ�sΪS-box��DataΪ���ݣ�LenΪ���ݵĳ���*/
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
		Data2[k] = Data[k];

	}


	/***************************/
}

char * CSt2char(CString str)
{
	//ע�⣺����n��len��ֵ��С��ͬ,n�ǰ��ַ�����ģ� len�ǰ��ֽڼ����
	int n = str.GetLength(); // n = 14, len = 18
	//��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
	int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL);
	//Ϊ���ֽ��ַ���������ռ䣬�����СΪ���ֽڼ���Ŀ��ֽ��ֽڴ�С
	char * buff = new char[len + 1]; //���ֽ�Ϊ��λ//���ֽڱ���ת���ɶ��ֽڱ���
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


// CAboutDlg dialog used for App About

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


// CMy2015218737TCPClientDlg dialog



CMy2015218737TCPClientDlg::CMy2015218737TCPClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy2015218737TCPClientDlg::IDD, pParent)
	, m_strSendMessage(_T(""))
	, m_encode(_T(""))
	, mikey_string(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy2015218737TCPClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_SERVER, ServerIP);

	//DDX_Text(pDX, IDC_STATIC_RECV, m_strRecvMessage);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strSendMessage);
	DDX_Text(pDX, IDC_EDIT_ENCODE, m_encode);
	//DDX_Control(pDX, IDC_EDIT_mikey, m_key);
	DDX_Text(pDX, IDC_EDIT_mikey, mikey_string);
}

BEGIN_MESSAGE_MAP(CMy2015218737TCPClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMy2015218737TCPClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CMy2015218737TCPClientDlg::OnBnClickedButtonSend)
	ON_EN_CHANGE(IDC_EDIT_MESSAGE, &CMy2015218737TCPClientDlg::OnEnChangeEditMessage)
	
	ON_EN_CHANGE(IDC_EDIT_ENCODE, &CMy2015218737TCPClientDlg::OnEnChangeEditEncode)
	//ON_BN_CLICKED(IDC_BUTTON_get_mi, &CMy2015218737TCPClientDlg::OnBnClickedButtongetmi)
END_MESSAGE_MAP()


// CMy2015218737TCPClientDlg message handlers

BOOL CMy2015218737TCPClientDlg::OnInitDialog()
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
	WSADATA wsaData;//�ṹ���������溯�� WSAStartup ���ص� Windows Socket ��ʼ����Ϣ
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		AfxMessageBox(_T("WSAStartup error�� "));
		return FALSE;
	}//��һ��Ӧ�ó������ WSAStartup ����ʱ�� ����ϵͳ��������� Socket �汾��������Ӧ�� Socket �⣬Ȼ����ҵ��� Socket �⵽��Ӧ�ó�����
	//��ʾҪʹ�� 1.1 �汾�� Socket
	
	ServerIP.SetAddress(192, 168, 43, 192);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy2015218737TCPClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMy2015218737TCPClientDlg::OnPaint()
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
HCURSOR CMy2015218737TCPClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy2015218737TCPClientDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	WSACleanup();//�ͷ���Դ
}


void CMy2015218737TCPClientDlg::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
	sockaddr_in local;
	local.sin_family = AF_INET;//������AF_INET,��ʾ��socket��Internet���н���ͨ��
	local.sin_port = htons(DEFAULT_PORT);//�˿ں�
	char IPAddress[MAX_PATH];
	BYTE add1, add2, add3, add4;
	ServerIP.GetAddress(add1, add2, add3, add4);
	sprintf_s(IPAddress, "%d.%d.%d.%d", add1, add2, add3, add4);
	local.sin_addr.S_un.S_addr = inet_addr(IPAddress);//��������IP��ַ��

	SOCKET socketTmp = socket(AF_INET, SOCK_STREAM, 0);///��ʼ��Socket����������˱���һ��if (connect(socketTmp, (LPSOCKADDR)& local, sizeof(local)) != 0)//���ӷ�����
	
	if (connect(socketTmp, (LPSOCKADDR)& local, sizeof(local)) != 0)//���ӷ�����
	{
		closesocket(socketTmp);
		AfxMessageBox(_T("���ӷ�����ʧ��"));
		return;
	}
	get_rdom_key(iKey, 12);//���������Կ

	char szText[MAX_PATH];//���ݻ�����
	memset(szText, 0, MAX_PATH);
	UpdateData(TRUE);
	rc4_init(sBox, iKey, strlen(iKey));
	for (int x = 0; x < 256; x++)
	{
		s_Box[x] = sBox[x];
	}
	rc4_crypt(sBox, CSt2char(m_strSendMessage), szText, strlen(CSt2char(m_strSendMessage)));
	m_encode = szText;//�����ĸ���ؼ�����
	mikey_string = iKey;//����Կ����ؼ�����
	strcat(szText,iKey);//��Կ������ƴ��
	send(socketTmp, szText, sizeof szText, 0);//������Ϣ
	memset(szText, 0, MAX_PATH);//��ջ�����
	
	UpdateData(FALSE);//ˢ�½���
	shutdown(socketTmp, 2);//�رն�дͨ��
	closesocket(socketTmp);
}

/*
void CMy2015218737TCPClientDlg::OnBnClickedButtongetmi()
{
	// TODO: Add your control notification handler code here
	sockaddr_in local;
	local.sin_family = AF_INET;//������AF_INET,��ʾ��socket��Internet���н���ͨ��
	local.sin_port = htons(DEFAULT_PORT);//�˿ں�
	char IPAddress[MAX_PATH];
	BYTE add1, add2, add3, add4;
	ServerIP.GetAddress(add1, add2, add3, add4);
	sprintf_s(IPAddress, "%d.%d.%d.%d", add1, add2, add3, add4);
	local.sin_addr.S_un.S_addr = inet_addr(IPAddress);//��������IP��ַ��

	SOCKET socketTmp = socket(AF_INET, SOCK_STREAM, 0);///��ʼ��Socket����������˱���һ��if (connect(socketTmp, (LPSOCKADDR)& local, sizeof(local)) != 0)//���ӷ�����
	if (connect(socketTmp, (LPSOCKADDR)& local, sizeof(local)) != 0)//���ӷ�����
	{
		closesocket(socketTmp);
		AfxMessageBox(_T("���ӷ�����ʧ��"));
		return;
	}

	get_rdom_key(iKey,11);
	send(socketTmp, iKey, sizeof iKey, 0);//������Ϣ
	mikey_string = iKey;
	UpdateData(FALSE);//ˢ�½���
}*/


void CMy2015218737TCPClientDlg::OnEnChangeEditMessage()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	
}




void CMy2015218737TCPClientDlg::OnEnChangeEditEncode()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

}


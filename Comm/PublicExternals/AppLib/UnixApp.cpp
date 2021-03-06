//==============================================================================
//                      UnixApp.cpp
//
//begin   : 2016-08-16
//describe:
//==============================================================================
#include "UnixApp.h"
#include "ProcLock.h"
#include "LocalDeviceConf.h"
#include "AppPath.h"

CUnixApp* pTheApp = NULL;


//==============================================================================
//                 
//                    CUnixApp
//                    unix（包括linux）下的应用程序的通用基类
//
//==============================================================================
CUnixApp::CUnixApp()
{
	assert(!pTheApp); // 只允许有一个应用程序实例对象
	pTheApp = this;
	m_argc = 0;
	
	m_bIsDevCfgLoaded = false;
	
	m_strAppName = "";
	m_strMainAppName = "";
	m_strService = "";
}

CUnixApp::~CUnixApp()
{
	for(int i=0; i<m_argc; i++)
	{
		delete [] m_argv[i];
	}
	delete [] m_argv;
}

string CUnixApp::ArgsToName(int argc, char **argv, char c)
{
	assert(argc > 0);
	string s = basename(argv[0]);
	for(int i=1; i<argc; i++)
	{
		s += c;
		s += argv[i];
	}
	return s;
}

void CUnixApp::SetArgs(int argc, char** argv)
{
	//SetArgs只能用一次
	assert(m_argc == 0);
	assert(argc > 0 && argv);

	m_argc = argc;
	m_argv = new char*[argc];
	assert(m_argv);
	for(int i=0; i<argc; i++)
	{
		m_argv[i] = new char[strlen(argv[i]) + 1];
		assert(m_argv[i]);
		strcpy(m_argv[i], argv[i]);
	}
}

char** CUnixApp::GetArgv()
{ 
	return m_argv; 
}

int CUnixApp::GetArgc() 
{ 
	return m_argc; 
}

string CUnixApp::GetAppName()
{
	return m_strAppName;
}

string CUnixApp::GetMainAppName()
{
	return m_strMainAppName;
}

string CUnixApp::GetServiceName()
{ 
	return m_strService; 
}

bool CUnixApp::IsRunAtCenter()
{
	if (!m_bIsDevCfgLoaded)
	{
		LoadDevInfo();
	}
	
	return m_objLocalAddr.GetUnitType() == ADDR_UNITTYPE_CENTER;
}

bool CUnixApp::IsRunAtStation()
{
	if (!m_bIsDevCfgLoaded)
	{
		LoadDevInfo();
	}
	
	return m_objLocalAddr.GetUnitType() == ADDR_UNITTYPE_STATION;
}

unsigned char CUnixApp::GetBureauId()
{
	if (!m_bIsDevCfgLoaded)
	{
		LoadDevInfo();
	}

	return m_objLocalAddr.GetBureauId();
}

unsigned short CUnixApp::GetStationId()
{
	if (!m_bIsDevCfgLoaded)
	{
		LoadDevInfo();
	}

	if (IsRunAtStation())
	{
		return m_objLocalAddr.GetUnitId();
	}
	else
	{
		return 0;
	}
}

CProtocolForward::CForwardAddr CUnixApp::GetLocalAddr()
{
	if (!m_bIsDevCfgLoaded)
	{
		LoadDevInfo();
	}

	return m_objLocalAddr;
}

string CUnixApp::GetServiceIp1()
{
	if (!m_bIsDevCfgLoaded)
	{
		LoadDevInfo();
	}

	return m_strServiceIp1;
}

string CUnixApp::GetServiceIp2()
{
	if (!m_bIsDevCfgLoaded)
	{
		LoadDevInfo();
	}

	return m_strServiceIp2;
}

bool CUnixApp::Init()
{
	int argc = m_argc;
	char** argv = m_argv;
	CheckHelpOptions(argc, argv);
	if (!GetOptions(argc, argv))
	{
		printf("Usage: %s %s\n", basename(argv[0]), GetAppUsage().c_str());
		return false;
	}

	if (!CAppPath::InitExeDir(argv[0]))
	{
		return false;
	}

	SetAppName(argc, argv);
	SetLogFile(argc, argv);	
	LogImportant("CUnixApp::Init(), app startup ...\n%s", GetAppVersion().c_str());

	SingleCheck(argc, argv);
	SigSet();

	if (!m_bIsDevCfgLoaded)
	{
		if (!LoadDevInfo())
		{
			LogImportant("CUnixApp::Init(), LoadDevInfo() fail!");
			return false;
		}
	}

	return true;
}

void CUnixApp::Run()
{
}

bool CUnixApp::Uninit()
{
	LogImportant("================================================================="\
		"==================CUnixApp::Uninit()============================="\
		"=================================================================");
	return true;
}

void CUnixApp::SigHandler(int sig)
{
	assert(CUnixApp::GetApp());
	char szAppName[FILE_NAME_LEN];
	strcpy(szAppName, CUnixApp::GetApp()->GetAppName().c_str());
	switch (sig)
	{
	case SIGTERM:
	case SIGINT:
	case SIGQUIT:
		exit(CUnixApp::GetApp()->Uninit());
	case SIGUSR1:
	case SIGUSR2:
		break;
	case SIGPIPE:
		break;
	case SIGHUP:
		break;
	default:
		break;
	}
}

CUnixApp* CUnixApp::GetApp()
{
	return pTheApp;
}

bool CUnixApp::LoadDevInfo()
{
	if (m_bIsDevCfgLoaded)
	{
		return true;
	}

	CLocalDeviceConf objLocalDevConf;
	char szErrInfo[1024];
	if (!objLocalDevConf.Load(szErrInfo, sizeof(szErrInfo)))
	{
		LogImportant("CUnixApp::LoadDevInfo(), %s!", szErrInfo);
		return false;
	}
	
	m_objLocalAddr.SetBureauId(objLocalDevConf.m_byBureauId);
	m_objLocalAddr.SetUnitType(objLocalDevConf.m_byUnitType);
	m_objLocalAddr.SetUnitId(objLocalDevConf.m_wUnitId);
	m_objLocalAddr.SetDevType(objLocalDevConf.m_byDevType);
	m_objLocalAddr.SetDevId(objLocalDevConf.m_wDevId);

// 	m_strServiceIp1 = pDevConf->m_strIp1;
// 	m_strServiceIp2 = pDevConf->m_strIp2;
	
	LogImportant("CUnixApp::LoadDevInfo(), ok, \nBureauId=%d; UnitType=%d; UnitId=%d; DevType=%d; DevId=%d; CltId=%d; \nIP1=%s; IP2=%s", 
		m_objLocalAddr.GetBureauId(), m_objLocalAddr.GetUnitType(), m_objLocalAddr.GetUnitId(), 
		m_objLocalAddr.GetDevType(), m_objLocalAddr.GetDevId(), m_objLocalAddr.GetCltId(),
		m_strServiceIp1.c_str(), m_strServiceIp2.c_str());
	
	m_bIsDevCfgLoaded = true;

	return true;
}

string CUnixApp::GetAppUsage()
{ 
	return ""; 
}

void CUnixApp::CheckHelpOptions(int argc, char **argv)
{
	if (argc > 1)
	{
		if (!strcmp(argv[1], "-h"))
		{
			printf("Usage: %s %s\n", basename(argv[0]), GetAppUsage().c_str());
			exit(Uninit());
		}
		else if (!strcmp(argv[1], "-v"))
		{
			printf("%s\n", GetAppVersion().c_str());
			exit(Uninit());
		}
	}
}

bool CUnixApp::GetOptions(int argc, char** argv)
{
	if (argc < 2)
	{
		return true;
	}

	m_strService = argv[1];
	argc = argc - 1;
	argv = argv + 1;
	if (argc < 2)
	{
		return true;
	}

	int opt = getopt(argc, argv, "-hv");
	if (opt == -1)
	{
		return true;
	}

	while (opt != -1)
	{
		switch (opt)
		{
		default:
			return true;
		}
		opt = getopt(argc, argv, "-hv");
	}
	return true;
}

void CUnixApp::SetAppName(int argc, char **argv)
{
	assert(argc > 0);
	
	m_strAppName = ArgsToName(argc, argv, ' ');
	m_strMainAppName = basename(argv[0]);
}

void CUnixApp::SetLogFile(int argc, char **argv)
{
	char szFileName[FILE_NAME_LEN];
	memset(szFileName, 0, FILE_NAME_LEN);

	// 生成日志文件路径
	string strLogPath = CAppPath::GetLogPath();
	if (strLogPath.size() > 0)
	{
		snprintf(szFileName, sizeof(szFileName), "%s", strLogPath.c_str());
	}
	
	// 设置日志文件前缀
	strcat(szFileName, m_strMainAppName.c_str());
	strcat(szFileName, "/");
	strcat(szFileName, m_strService.c_str());
	CLogger::SetLogFileBase(szFileName);

	// 设置主程序日志文件后缀
	CLogger::SetLogFileTagInfo("App");

	// 设置本类及其派生类的日志文件后缀，即：本类及其派生类的日志自动记录
	// 到名称为“日志文件前缀+日志文件后缀”的日志文件中
	CUnixApp::SetLogFileTag("App");	
}

void CUnixApp::SingleCheck(int argc, char** argv)
{
	static CProcLock locker;
	string lockfile = ArgsToName(argc, argv, '_') + ".lock";

	if (!locker.Lock(lockfile.c_str()))
	{
		LogImportant("[%s] has exist.", m_strAppName.c_str());
		exit(Uninit());
	}
}

void CUnixApp::SigSet()
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SigHandler;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);
	sigaction(SIGHUP, &sa, NULL);
	sigaction(SIGIO, &sa, NULL);
}

MY_FN_SIGHANDLER CUnixApp::GetSigHandler()
{ 
	return CUnixApp::SigHandler;
}


///////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	assert(pTheApp);
	if (!pTheApp)
	{
		return -1;
	}

	pTheApp->SetArgs(argc, argv);
	
	if (!pTheApp->Init())
	{
		pTheApp->Uninit();
		return -1;
	}
	
	pTheApp->Run();

	if (!pTheApp->Uninit())
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

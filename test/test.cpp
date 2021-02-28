// test.cpp : 定义应用程序的入口点。
//

#include "pch.h"
#include "framework.h"
#include "test.h"
#include <Windows.h>
#include "resource.h"
#include <tlhelp32.h>

#include <stdio.h>

#define WECHAT_PROCESS_NAME "WeChat.exe"

INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InjectDll();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
 
    DialogBox(hInstance, MAKEINTRESOURCE(ID_DIALOG1), NULL, &DialogProc);
    return 0;
}


INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_INITDIALOG)
    {
        //MessageBox(NULL, "首次加载", "标题", 0);
    }
    if (uMsg == WM_CLOSE)
    {
        //MessageBox(NULL, "关闭", "标题", 0);
        EndDialog(hwndDlg, NULL);
    }
    //点击事件
    if (uMsg == WM_COMMAND)
    {
        if (wParam == ID_INJDLL)
        {
            InjectDll();
            //MessageBox(NULL, "注入", "标题", 0);
        }
        if (wParam == ID_UNDLL)
        {
            MessageBox(NULL, "卸载", "标题", 0);
        }
    }
    return FALSE;
}
//1.微信进程句柄(写入内存)
//通过进程名称找到PID，然后PID打开微信进程获取进程句柄

//获取PID
DWORD ProcessNameFindPid(LPCSTR ProcName)
{
    //WeChat.exe
    //获取所有系统进程快照
    HANDLE ProcessAll = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    //找到相应ProcName的PID
    PROCESSENTRY32 processInfo = {0};
    processInfo.dwSize = sizeof(PROCESSENTRY32);
    do
    {
        if (strcmp(ProcName, processInfo.szExeFile) == 0)
        {
            return processInfo.th32ProcessID;
        }
    }
    while(Process32Next(ProcessAll, &processInfo));
    return 0;
    //获取进程句柄
}


//2.在微信内存空间申请内存
void InjectDll()
{
    //根据PID获取句柄
    DWORD PID = ProcessNameFindPid(WECHAT_PROCESS_NAME);

    if (PID == 0)
    {
        MessageBox(NULL, "没有找到微信进程", "标题", 0);
        return;
    }
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    if (hProcess == NULL)
    {
        MessageBox(NULL, "进程打开失败", "标题", 0);
        return;
    }

    char pathStr[100] = { "D://ADev//wechet//GetWechatInfo//Debug//GetWechatInfo.dll" };
    //在对应进程句柄内申请内存,可读可写
    LPVOID dllAddress =  VirtualAllocEx(hProcess, NULL, sizeof(pathStr), MEM_COMMIT, PAGE_READWRITE);
    if (dllAddress == NULL)
    {
        MessageBox(NULL, "内存分配失败", "标题", 0);
        return;
    }

    //写入dll地址到内存
    if (WriteProcessMemory(hProcess, dllAddress, pathStr, sizeof(pathStr), NULL) == 0)
    {
        MessageBox(NULL, "内存写入失败", "标题", 0);
        return;
    }

    char message[100] = { 0 };
    sprintf_s(message, "写入的地址为 %p", dllAddress);
    OutputDebugString(message);

    HMODULE k32 = GetModuleHandle("Kernel32.dll");
    LPVOID loadlibarayAdd = GetProcAddress(k32, "LoadLibraryA");
    
    HANDLE RemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadlibarayAdd, dllAddress, 0, NULL);
    if (RemoteThread == NULL)
    {
        MessageBox(NULL, "创建远程线程失败", "标题", 0);
        return;
    }
}


//3.写入DLL路径，远程线程执行LoadLibaray


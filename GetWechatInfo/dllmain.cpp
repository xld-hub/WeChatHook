// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include "resource.h"
#include <tchar.h>
//#include "GetWechatInfo.h"

INT_PTR  CALLBACK DialogProc(
    HWND hwndDlg,
    UINT uMsg,
    WPARAM wParam,
    LPARAM IParam
);

void readWechatData(HWND hwndDlg);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        
        DialogBox(hModule, MAKEINTRESOURCE(IDD_DIALOG1), NULL, &DialogProc);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

INT_PTR  CALLBACK DialogProc(
    HWND hwndDlg, 
    UINT uMsg, 
    WPARAM wParam, 
    LPARAM IParam
)
{
    //if (uMsg == WM_INITDIALOG)
    //{
    //    readWechatData(hwndDlg);
    //}
    //if (uMsg == WM_CLOSE)
    //{
    //    EndDialog(hwndDlg, 0);
    //}
    switch (uMsg)
    {
    case WM_INITDIALOG:
        readWechatData(hwndDlg);
        break;
    case WM_COMMAND:
        if (wParam == IDC_WRITE)
        {

        }
        if (wParam == IDC_READ)
        {

        }
        break;
    case WM_CLOSE:
        EndDialog(hwndDlg, 0);
        break;
    default:
        break;
    }

    return FALSE;
}

DWORD getWeChatAdd()
{
    HMODULE winAdd = LoadLibrary("WeChatWin.dll");
    return (DWORD)winAdd;
}

//WeChatWin.dll + 13972DC = 5cb172dc   微信昵称
//WeChatWin.dll + 1397310 = 5CB17310  手机号
//WeChatWin.dll + 1397264 = 5CB17264 微信id //系统分配的为指针，自定义的为变量


//读取内存数据
void readWechatData(HWND hwndDlg)
{
   
    DWORD wechatwinAdd = getWeChatAdd();
    
    char wechatID[100] = { 0 };

    DWORD pID = wechatwinAdd + 0x1397264;
    sprintf_s(wechatID, "%s", *(DWORD*)pID);

    //MessageBox(NULL, (LPSTR) * (DWORD*)pID, "title", 0);
    SetDlgItemText(hwndDlg, IDC_ID, wechatID);


    char number[100] = { 0 };
    sprintf_s(number, "%s", wechatwinAdd + 0x1397310);
    SetDlgItemText(hwndDlg, IDC_LOG, number);

    char name[100] = { 0 };
    sprintf_s(name, _T("%s"), (char*)wechatwinAdd + 0x13972DC);
    SetDlgItemText(hwndDlg, IDC_NAME, name);

}
//#include "pch.h"
//#include <windows.h>
//#include <stdio.h>
//#include "resource.h"
////��ȡ΢��dll��ַ
//DWORD getWeChatAdd()
//{
//	HMODULE winAdd = LoadLibrary(L"WeChatWin.dll");
//	return (DWORD)winAdd;
//}
//
////WeChatWin.dll + 13972DC = 5cb172dc   ΢���ǳ�
////WeChatWin.dll + 1397310 = 5CB17310  �ֻ���
////WeChatWin.dll + 1397264 = 5CB17264 ΢��id //ϵͳ�����Ϊָ�룬�Զ����Ϊ����
//
//
////��ȡ�ڴ�����
//void readWechatData(HWND hwndDlg)
//{
//	DWORD wechatwinAdd = getWeChatAdd();
//
//	char wechatID[100] = { 0 };
//
//	//MessageBox(hwndDlg, (LPCWSTR)hwndDlg, L"����", 0);
//
//	DWORD pID = wechatwinAdd + 0x1397264;
//	
//	sprintf_s(wechatID, "%s", *(DWORD * )pID);
//	SetDlgItemText(hwndDlg, IDC_ID, (LPCWSTR)wechatID);
//
//	char number[100] = { 0 };
//	sprintf_s(number, "%s", wechatwinAdd + 0x1397310);
//	SetDlgItemText(hwndDlg, IDC_LOG, (LPCWSTR)number);
//
//	char name[100] = { 0 };
//	sprintf_s(name, "%s", wechatwinAdd + 0x13972DC);
//	SetDlgItemText(hwndDlg, IDC_NAME, (LPCWSTR)name);
//
//}
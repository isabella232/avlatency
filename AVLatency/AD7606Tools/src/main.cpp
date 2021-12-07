
#include <stdio.h>
#include <Windows.h>


#include <ad7606.h>
#include "resource.h"

#define Log(format, ...) printf("[%s %d] " format, __FUNCTION__, __LINE__, ##__VA_ARGS__);

ADC_CONFIG myADCCfg;
BYTE byDevIndex = 0xFF;


void updatecontrol(void)
{
    char temp[20];

    //((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel((myADCCfg.byADCOptions & 0x20) >> 5);
    Log("(myADCCfg.byADCOptions & 0x20) >> 5 = %d\n", (myADCCfg.byADCOptions & 0x20) >> 5);
    //((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel((myADCCfg.byTrigOptions & 0x03));
    Log("(myADCCfg.byTrigOptions & 0x03) = %d\n", (myADCCfg.byTrigOptions & 0x03));
    //((CComboBox*)GetDlgItem(IDC_COMBO3))->SetCurSel((myADCCfg.byTrigOptions & 0xC0) >> 2);
    Log("(myADCCfg.byTrigOptions & 0xC0) >> 2 = %d\n", (myADCCfg.byTrigOptions & 0xC0) >> 2);
    //((CComboBox*)GetDlgItem(IDC_COMBO4))->SetCurSel((myADCCfg.byADCOptions & 0x10) >> 4);
    Log("(myADCCfg.byADCOptions & 0x10) >> 4 = %d\n", (myADCCfg.byADCOptions & 0x10) >> 4);
    //((CComboBox*)GetDlgItem(IDC_COMBO5))->SetCurSel((myADCCfg.byADCOptions & 0x07));
    Log("(myADCCfg.byADCOptions & 0x07) = %d\n", (myADCCfg.byADCOptions & 0x07));
    //sprintf_s(temp, "%d", myADCCfg.wTrigSize);
    //((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(CString(temp));
    Log("myADCCfg.wTrigSize = %d\n", myADCCfg.wTrigSize);
    //sprintf_s(temp, "%d", myADCCfg.dwMaxCycles);
    //((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(CString(temp));
    Log("myADCCfg.dwMaxCycles = %d\n", myADCCfg.dwMaxCycles);
    //sprintf_s(temp, "%d", myADCCfg.wPeriod);
    //((CEdit*)GetDlgItem(IDC_EDIT3))->SetWindowText(CString(temp));
    Log("myADCCfg.wPeriod = %d\n", myADCCfg.wPeriod);
    if (byDevIndex == 0xFF)
    {
        //SetWindowText(_T("AD7060--设备未连接"));
        //GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("连接"));
        Log("Device disconnected\n");
    }
    else
    {
        //GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("断开"));
    }
    if (myADCCfg.byTrigOptions & 0x80)
    {
        //GetDlgItem(IDC_BUTTON3)->SetWindowText(_T("停止采样"));
        Log("Sampling: true\n");
    }
    else
    {
        //GetDlgItem(IDC_BUTTON3)->SetWindowText(_T("连续采样"));
        Log("Sampling: false\n");
    }
}

void OpenDevice()
{
    char serial[15] = { 0 };
    BYTE byVerified;

    if (byDevIndex != 0xFF)
    {
        if (myADCCfg.byTrigOptions & 0x80)   //trig mode
        {
            myADCCfg.byTrigOptions &= 0x7F;

        }
        if (M3F20xm_CloseDevice(byDevIndex) == false)
        {
            Log("Close device error\n");
            return;
        }
        byDevIndex = 0xFF;
    }
    else
    {
        byDevIndex = M3F20xm_OpenDevice();
        if (byDevIndex == 0xFF)
        {
            Log("Open device error\n");
            return;
        }
        else
        {

            if (!M3F20xm_Verify(byDevIndex, &byVerified))
            {
                Log("Device verification error\n");
                M3F20xm_CloseDevice(byDevIndex);
                byDevIndex = 0xFF;
                return;
            }

            if (byVerified)
            {
                M3F20xm_ADCGetConfig(byDevIndex, &myADCCfg);
            }
            M3F20xm_GetSerialNo(byDevIndex, serial);
            Log("Get serial number [%s]\n", serial);

        }
    }

    updatecontrol();
}



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    printf("\n");

    AllocConsole();

    FILE* stream;
    freopen_s(&stream, "CONIN$", "r", stdin);
    freopen_s(&stream, "CONOUT$", "w", stdout);
    freopen_s(&stream, "CONOUT$", "w", stderr);

    OpenDevice();

    while (1)
    {
        Sleep(1000);
    }
	return 0;
}
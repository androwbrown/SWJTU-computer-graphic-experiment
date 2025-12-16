#include "InputDialog.h"
#include <vector>

#define IDC_INPUT_EDIT 101
#define IDC_PROMPT_TEXT 102
//弹出一个对话框，用于输入旋转角度
struct DialogData {
    const wchar_t* title;
    const wchar_t* prompt;
    std::wstring* out_text;
};


INT_PTR CALLBACK InputDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG: {
      
        DialogData* pData = (DialogData*)lParam;
        SetWindowTextW(hDlg, pData->title);
        SetDlgItemTextW(hDlg, IDC_PROMPT_TEXT, pData->prompt);
        SetWindowLongPtr(hDlg, DWLP_USER, (LONG_PTR)pData);
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK: {
            DialogData* pData = (DialogData*)GetWindowLongPtr(hDlg, DWLP_USER);
            if (pData && pData->out_text) {
                wchar_t buffer[256];
                GetDlgItemTextW(hDlg, IDC_INPUT_EDIT, buffer, 256);
                *pData->out_text = buffer;
            }
            EndDialog(hDlg, IDOK);
            return (INT_PTR)TRUE;
        }
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

bool GetInputString(HWND owner, const wchar_t* title, const wchar_t* prompt, std::wstring& out_text) {
    std::vector<BYTE> dlgTemplate;
    dlgTemplate.resize(4096); 
    auto align_dword = [](LPWORD& p) {
        ULONG_PTR ptr = (ULONG_PTR)p;
        ptr = (ptr + 3) & ~3;
        p = (LPWORD)ptr;
    };

    LPDLGTEMPLATEW lpdt = (LPDLGTEMPLATEW)dlgTemplate.data();

    lpdt->style = WS_POPUP | WS_BORDER | WS_SYSMENU | DS_MODALFRAME | WS_CAPTION | DS_CENTER;
    lpdt->dwExtendedStyle = 0;
    lpdt->cdit = 4; 
    lpdt->x = 0;
    lpdt->y = 0;
    lpdt->cx = 200;
    lpdt->cy = 70;

    LPWORD p = (LPWORD)(lpdt + 1);

    *p++ = 0; 
    *p++ = 0; 
    *p++ = 0; 

    align_dword(p);

    DLGITEMTEMPLATE* pItem = (DLGITEMTEMPLATE*)p;
    pItem->style = WS_CHILD | WS_VISIBLE | SS_LEFT;
    pItem->dwExtendedStyle = 0;
    pItem->x = 10;
    pItem->y = 10;
    pItem->cx = 180;
    pItem->cy = 10;
    pItem->id = IDC_PROMPT_TEXT;
    p = (LPWORD)(pItem + 1);
    *p++ = 0xFFFF; 
    *p++ = 0x0082; 
    *p++ = 0;
    *p++ = 0; 

    align_dword(p);

    pItem = (DLGITEMTEMPLATE*)p;
    pItem->style = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL;
    pItem->dwExtendedStyle = 0;
    pItem->x = 10;
    pItem->y = 25;
    pItem->cx = 180;
    pItem->cy = 14;
    pItem->id = IDC_INPUT_EDIT;
    p = (LPWORD)(pItem + 1);
    *p++ = 0xFFFF;
    *p++ = 0x0081; 
    *p++ = 0; 
    *p++ = 0; 

    align_dword(p);

    pItem = (DLGITEMTEMPLATE*)p;
    pItem->style = WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON;
    pItem->dwExtendedStyle = 0;
    pItem->x = 50;
    pItem->y = 50;
    pItem->cx = 50;
    pItem->cy = 14;
    pItem->id = IDOK;
    p = (LPWORD)(pItem + 1);
    *p++ = 0xFFFF;
    *p++ = 0x0080; // Button
    const wchar_t* okText = L"OK";
    memcpy(p, okText, (wcslen(okText) + 1) * sizeof(wchar_t));
    p += wcslen(okText) + 1;
    *p++ = 0; 

    align_dword(p);

    // 关闭键
    pItem = (DLGITEMTEMPLATE*)p;
    pItem->style = WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON;
    pItem->dwExtendedStyle = 0;
    pItem->x = 110;
    pItem->y = 50;
    pItem->cx = 50;
    pItem->cy = 14;
    pItem->id = IDCANCEL;
    p = (LPWORD)(pItem + 1);
    *p++ = 0xFFFF;
    *p++ = 0x0080; // Button
    const wchar_t* cancelText = L"Cancel";
    memcpy(p, cancelText, (wcslen(cancelText) + 1) * sizeof(wchar_t));
    p += wcslen(cancelText) + 1;
    *p++ = 0; 
    DialogData data;
    data.title = title;
    data.prompt = prompt;
    data.out_text = &out_text;

    INT_PTR result = DialogBoxIndirectParamW(GetModuleHandle(NULL), lpdt, owner, InputDialogProc, (LPARAM)&data);

    return (result == IDOK);
}


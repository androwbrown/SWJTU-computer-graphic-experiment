#pragma once
#include <windows.h>
#include <string>

// Function to show a simple input dialog and get a string from the user.
// Returns true if the user clicks OK, false otherwise.
// The entered text is returned in out_text.
bool GetInputString(HWND owner, const wchar_t* title, const wchar_t* prompt, std::wstring& out_text);

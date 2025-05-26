#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <wx/wx.h>
#include "OsFrame.h"
#include "VirtualOs.h"
#include <thread>

class VirtualOsApp : public wxApp {
public:
	bool OnInit() override;
};
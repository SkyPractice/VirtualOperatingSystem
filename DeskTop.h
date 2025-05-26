#pragma once
#include <wx/wx.h>
#include "OsAppIcon.h"
#include <vector>
#include "Interrupt.h"
#include "ProcessWindow.h"
#include <wx/event.h>
#include <algorithm>

class DeskTop : public wxPanel {

	std::vector<OsAppIcon*> icons;
	wxGridSizer* icon_sizer;

public:
	wxBitmap bmp;
	std::vector<Interrupt>* interrupt_vec;
	std::mutex* kernel_mut;
	
	std::vector<ProcessWindow*> process_windows;

	DeskTop(wxWindow* parent);
	void addIcon(OsAppIcon* icon);
	void init();
	void onPaint(wxPaintEvent& evt);

};
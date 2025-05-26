#define _CRT_SECURE_NO_WARNINGS
#include "OsAppIcon.h"

wxSize OsAppIcon::icon_size = wxSize(60, 80);

OsAppIcon::OsAppIcon(wxWindow* parent,
	std::string icon_path, std::string app_source_path, std::string app_name_arg,
	std::vector<Interrupt>* interrupts_arg, std::mutex* mut) : wxPanel(parent, wxID_ANY,
		wxDefaultPosition,
		icon_size), source_path(app_source_path), app_name(app_name_arg), interrupts(interrupts_arg),
		kernel_mut(mut){

	wxImage img;
	img.LoadFile(icon_path);

	icon = wxBitmap(img);

	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	this->SetBackgroundColour(wxTransparentColour);

	this->Bind(wxEVT_PAINT, &OsAppIcon::onDraw, this);
	this->Bind(wxEVT_LEFT_DOWN, &OsAppIcon::OnClick, this);
	this->Bind(wxEVT_SET_FOCUS, [&](wxFocusEvent& evt) {
		Refresh();
		});
	this->SetCursor(wxCursor(wxCURSOR_HAND));
	Refresh();
}

void OsAppIcon::OnClick(wxMouseEvent& evt) {

	std::lock_guard<std::mutex> locker(*kernel_mut);
	interrupts->push_back({ UserModeInterrupt, ProcessCreation, nullptr, this });

	evt.Skip();
}

void OsAppIcon::onDraw(wxPaintEvent& evt) {

	wxPaintDC dc(this);
	dc.Clear();
	wxGraphicsContext* ctx = wxGraphicsContext::Create(dc);
	
	if (ctx) {

		int x, y, w, h;
		this->GetPosition(&x, &y);
		this->GetSize(&w, &h);
		ctx->ClearRectangle(0, 0, w, h);

		wxFont fnt(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
		wxGraphicsFont font = ctx->CreateFont(fnt, *wxWHITE);

		wxDouble txt_width, txt_height;
		
		ctx->SetFont(font);
		ctx->GetTextExtent(app_name, &txt_width, &txt_height);
		ctx->DrawText(app_name, (w / 2) - (txt_width / 2), 45);
		ctx->DrawBitmap(icon, 10, 10, icon_size.GetX() - 20, 40);
		delete ctx;
	}

}
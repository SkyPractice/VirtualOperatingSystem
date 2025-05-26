#include "DeskTop.h"
#include "WindowCreationEvent.h"
#include "WindowTerminationEvent.h"

wxDECLARE_EVENT(MY_CUSTOM_EVENT, WindowCreationEvent);
wxDEFINE_EVENT(MY_CUSTOM_EVENT, WindowCreationEvent);

wxDECLARE_EVENT(WINDOW_TERMINATION_EVENT, WindowTerminationEvent); // ProcessTerminationEvent
wxDEFINE_EVENT(WINDOW_TERMINATION_EVENT, WindowTerminationEvent);

WindowCreationEvent::WindowCreationEvent(std::string name, Process * process,
	std::vector<Interrupt>* int_vec, std::mutex* kernel_mutex) :
	wxCommandEvent(MY_CUSTOM_EVENT, wxID_ANY),
	window_name(name), process_caller(process), interrupts_vec(int_vec), kernel_mut(kernel_mutex) {};

WindowTerminationEvent::WindowTerminationEvent(int id) : wxCommandEvent(WINDOW_TERMINATION_EVENT, wxID_ANY),
	window_id(id) {};

DeskTop::DeskTop(wxWindow* parent) : wxPanel(parent) {
	this->SetBackgroundStyle(wxBG_STYLE_PAINT);
	this->SetBackgroundColour(wxTransparentColour);
	wxImage img("wallpaper.jpg");
	bmp = wxBitmap(img);
	this->Bind(wxEVT_PAINT, &DeskTop::onPaint, this);

	this->Bind(MY_CUSTOM_EVENT, [=](WindowCreationEvent& evt) {
		ProcessWindow* process_window = new ProcessWindow(this, evt.window_name, evt.interrupts_vec,
			evt.kernel_mut, evt.process_caller);
		evt.process_caller->window = process_window;
		process_windows.push_back(process_window);
		evt.process_caller->suspended = false;
		this->Layout();
		this->Refresh();
		});

	this->Bind(WINDOW_TERMINATION_EVENT, [&](WindowTerminationEvent& evt) {
		auto itr = std::find_if(process_windows.begin(), process_windows.end(), [&](ProcessWindow* window)
			{ return window->id == evt.window_id; });

		if (itr != process_windows.end()) {

			this->RemoveChild(*itr);
			this->Layout();
			this->Refresh();
			
			delete (*itr)->process; // deleting the process automatically deletes the window

			process_windows.erase(itr);
		}
		});

	this->Bind(wxEVT_SIZE, [&](wxSizeEvent& s) {
		Refresh();
		s.Skip();
		});


}

void DeskTop::addIcon(OsAppIcon* icon) {
	icon->SetPosition(wxPoint(0, icons.size() * icon->GetSize().y + icons.size() * 20));
	icon->Refresh();
	icon->Layout();
	Refresh();
	icons.push_back(icon);
}
void DeskTop::init() {

	//addIcon(new OsAppIcon(this, "test_icon.png", "test_app.txt", "app", interrupt_vec, kernel_mut));
	//addIcon(new OsAppIcon(this, "test_icon.png", "test_app.txt", "app", interrupt_vec, kernel_mut));
	//addIcon(new OsAppIcon(this, "test_icon.png", "test_app.txt", "app", interrupt_vec, kernel_mut));
	for (const auto& ic : icons)
		ic->Refresh();
}

void DeskTop::onPaint(wxPaintEvent& evt) {
	wxAutoBufferedPaintDC dc(this);
	wxGraphicsContext* ctx = wxGraphicsContext::Create(dc);

	if (ctx) {
		int x, y, w, h;
		this->GetPosition(&x, &y);
		this->GetSize(&w, &h);
		ctx->SetBrush(*wxWHITE_BRUSH);
		ctx->DrawRectangle(0, 0, w, h);
		ctx->DrawBitmap(bmp, x, y, w, h);

		delete ctx;
	}
}

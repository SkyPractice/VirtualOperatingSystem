#include "VirtualOsApp.h"
#include "Kernel.h"

bool VirtualOsApp::OnInit() {

	OsFrame* frame = new OsFrame("VirtualOs", wxSize(600, 600));
	wxInitAllImageHandlers();


	VirtualOs* virtual_os = new VirtualOs(new DeskTop(frame));


	frame->ShowFullScreen(true);
	frame->Show();
	return true;
}
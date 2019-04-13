#include <Windows.h>
#include "pluginsdk/_plugins.h"


#define DLL_EXPORT       __declspec(dllexport)

#define PLUGIN_NAME          "Register History"
#define PLUGIN_VERSION       1

#define RECORD_START         0
#define RECORD_START_TITLE   "Record Start"

#define RECORD_STOP          1
#define RECORD_STOP_TITLE    "Record Stop"

#define RECORD_VIEW          2
#define RECORD_VIEW_TITLE    "History View"

#define COLUMN_SIZE          8


HWND g_hwndDlg; // global hwnd dialog

DWORD g_dwProcessId; // get process id

int g_record; // record state
int g_count; // record count

int g_hMenu;
int g_hMenuDisasm;
int g_hMenuDump;
int g_hMenuStack;
int g_iPluginHandle;


extern "C"
{

	DLL_EXPORT bool pluginit(PLUG_INITSTRUCT* initStruct);
	DLL_EXPORT bool plugstop(void);
	DLL_EXPORT void plugsetup(PLUG_SETUPSTRUCT* setupStruct);


	DLL_EXPORT void CBMENUENTRY(CBTYPE cbType, PLUG_CB_MENUENTRY* info);
	DLL_EXPORT void CBDEBUGEVENT(CBTYPE cbType, PLUG_CB_DEBUGEVENT* info);
	DLL_EXPORT void CBBREAKPOINT(CBTYPE cbType, PLUG_CB_BREAKPOINT* info);
	DLL_EXPORT void CBPAUSEDEBUG(CBTYPE cbType, void* reserved);

}


// Main
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved);
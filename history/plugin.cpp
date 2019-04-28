#include "plugin.h"
#include <iostream>
#include <sstream>
#include <string>


DLL_EXPORT bool pluginit(PLUG_INITSTRUCT * initStruct)
{
	initStruct->sdkVersion = PLUG_SDKVERSION;
	initStruct->pluginVersion = PLUGIN_VERSION;
	strcpy_s(initStruct->pluginName, 256, PLUGIN_NAME);

	g_iPluginHandle = initStruct->pluginHandle;

	g_record = 0; // default setting
	g_count = 0;

	return true;
}


DLL_EXPORT bool plugstop(void)
{
	_plugin_menuclear(g_hMenu);

	return true;
}


DLL_EXPORT void plugsetup(PLUG_SETUPSTRUCT * setupStruct)
{
	g_hwndDlg = setupStruct->hwndDlg;
	g_hMenu = setupStruct->hMenu;
	g_hMenuDisasm = setupStruct->hMenuDisasm;
	g_hMenuDump = setupStruct->hMenuDump;
	g_hMenuStack = setupStruct->hMenuStack;

	GuiReferenceInitialize(RECORD_VIEW_TITLE);

	// Add Column
	for (int i = 0; i < gui_column_list->length; i++)
		GuiReferenceAddColumn(COLUMN_DATA_SIZE, gui_column_list[i].c_str());

	_plugin_menuaddentry(g_hMenu, RECORD_START, RECORD_START_TITLE); // Start record
	_plugin_menuaddentry(g_hMenu, RECORD_STOP, RECORD_STOP_TITLE);   // Stop record
}



DLL_EXPORT void CBMENUENTRY(CBTYPE cbType, PLUG_CB_MENUENTRY * info)
{
	switch (info->hEntry)
	{
	case RECORD_START:
		g_record = 1;
		_plugin_logputs("Record start!");
		break;

	case RECORD_STOP:
		g_record = 0;
		_plugin_logputs("Record stop!");
		break;
	}
}


// call when pause debug
DLL_EXPORT void CBPAUSEDEBUG(CBTYPE cbType, void* reserved)
{
	if (g_record)
	{
		SELECTIONDATA sel; // save selection data
		REGDUMP dump; // save register data

		std::ostringstream out[10];

		GuiSelectionGet(GUI_DISASSEMBLY, &sel);
		DbgGetRegDumpEx(&dump, sizeof(dump)); // get data

		out[0] << g_count + 1;
		out[1] << sel.start;
		out[2] << dump.regcontext.cax;
		out[3] << dump.regcontext.cbx;
		out[4] << dump.regcontext.ccx;
		out[5] << dump.regcontext.cdx;
		out[6] << dump.regcontext.cbp;
		out[7] << dump.regcontext.csp;
		out[8] << dump.regcontext.csi;
		out[9] << dump.regcontext.cdi;

		GuiReferenceSetRowCount(g_count + 1); // Add Row

		for (int i = 0; i < 10; i++)
			GuiReferenceSetCellContent(g_count, i, _strdup(out[i].str().c_str()));

		GuiReferenceReloadData(); // reload data
		g_count += 1;             // next line
	}
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}
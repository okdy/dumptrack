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

	/* initialize record status and record count variable */
	g_record = 0;
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

	/* set Reference view title */
	GuiReferenceInitialize(RECORD_VIEW_TITLE);

	/* Add column to reference view */
	for (int i = 0; i < gui_column_list->length; i++)
		GuiReferenceAddColumn(COLUMN_DATA_SIZE, gui_column_list[i].c_str());

	/* add start, stop menu */
	_plugin_menuaddentry(g_hMenu, RECORD_START, RECORD_START_TITLE);
	_plugin_menuaddentry(g_hMenu, RECORD_STOP, RECORD_STOP_TITLE);
}



DLL_EXPORT void CBMENUENTRY(CBTYPE cbType, PLUG_CB_MENUENTRY * info)
{
	switch (info->hEntry)
	{
	case RECORD_START:
		g_record = 1;
		_plugin_logputs(RECORD_START_TITLE);
		break;

	case RECORD_STOP:
		g_record = 0;
		_plugin_logputs(RECORD_STOP_TITLE);
		break;
	}
}


/* This function call when x64dbg debugging stop
 */
DLL_EXPORT void CBPAUSEDEBUG(CBTYPE cbType, void* reserved)
{
	if (g_record)
	{
		/* Gui selection data and register dump variable */
		SELECTIONDATA selection;
		REGDUMP dump;

		std::ostringstream out[10];

		GuiSelectionGet(GUI_DISASSEMBLY, &selection);

		/* get register dump */
		DbgGetRegDumpEx(&dump, sizeof(dump));

		/* set register dump data to ostringstream */
		out[0] << g_count + 1;
		out[1] << selection.start;
		out[2] << dump.regcontext.cax;
		out[3] << dump.regcontext.cbx;
		out[4] << dump.regcontext.ccx;
		out[5] << dump.regcontext.cdx;
		out[6] << dump.regcontext.cbp;
		out[7] << dump.regcontext.csp;
		out[8] << dump.regcontext.csi;
		out[9] << dump.regcontext.cdi;

		GuiReferenceSetRowCount(g_count + 1);

		/* set data to cell */
		for (int i = 0; i < 10; i++)
			GuiReferenceSetCellContent(g_count, i, _strdup(out[i].str().c_str()));

		GuiReferenceReloadData();

		/* record count increase */
		g_count += 1;
	}
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}
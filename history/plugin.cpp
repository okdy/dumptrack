#include "plugin.h"
#include <stdio.h>


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
	GuiReferenceAddColumn(4, "id");
	GuiReferenceAddColumn(COLUMN_SIZE, "ADDRESS");
	GuiReferenceAddColumn(COLUMN_SIZE, "EAX");
	GuiReferenceAddColumn(COLUMN_SIZE, "EBX");
	GuiReferenceAddColumn(COLUMN_SIZE, "ECX");
	GuiReferenceAddColumn(COLUMN_SIZE, "EDX");
	GuiReferenceAddColumn(COLUMN_SIZE, "EBP");
	GuiReferenceAddColumn(COLUMN_SIZE, "ESP");
	GuiReferenceAddColumn(COLUMN_SIZE, "ESI");
	GuiReferenceAddColumn(COLUMN_SIZE, "EDI");

	GuiReferenceSetRowCount(50);

	_plugin_menuaddentry(g_hMenu, RECORD_START, RECORD_START_TITLE); // start record
	_plugin_menuaddentry(g_hMenu, RECORD_STOP, RECORD_STOP_TITLE); // stop record
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
	SELECTIONDATA sel; // save selection data
	REGDUMP dump; // save register data
	char data[16];

	GuiSelectionGet(GUI_DISASSEMBLY, &sel);
	DbgGetRegDumpEx(&dump, sizeof(dump)); // get data

	sprintf_s(data, "%d", g_count + 1);
	GuiReferenceSetCellContent(g_count, 0, data);
	sprintf_s(data, "%x", sel.start);
	GuiReferenceSetCellContent(g_count, 1, data);

	sprintf_s(data, "%x", dump.regcontext.cax);
	GuiReferenceSetCellContent(g_count, 2, data);
	sprintf_s(data, "%x", dump.regcontext.cbx);
	GuiReferenceSetCellContent(g_count, 3, data);
	sprintf_s(data, "%x", dump.regcontext.ccx);
	GuiReferenceSetCellContent(g_count, 4, data);
	sprintf_s(data, "%x", dump.regcontext.cdx);
	GuiReferenceSetCellContent(g_count, 5, data);
	sprintf_s(data, "%x", dump.regcontext.cbp);
	GuiReferenceSetCellContent(g_count, 6, data);
	sprintf_s(data, "%x", dump.regcontext.csp);
	GuiReferenceSetCellContent(g_count, 7, data);
	sprintf_s(data, "%x", dump.regcontext.csi);
	GuiReferenceSetCellContent(g_count, 8, data);
	sprintf_s(data, "%x", dump.regcontext.cdi);
	GuiReferenceSetCellContent(g_count, 9, data);

	GuiReferenceReloadData(); // reload data

	g_count += 1; // next line
}



BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{


	return TRUE;
}
/* -------------------------------------------------------------------------
//	FileName	��	yygame/yye_files_list.h
//	Creator		��	(wyk)
//	CreateTime	��	2014��4��21��18:12:07
//	Description	��	���ļ�������YY������ڰ�װ��ʹ�ù����п��ܻ��ڰ�װĿ¼�²����������ļ����ļ����б�
//��ǰĿ¼Ϊ %appdata%/duowan/YYExplorer/���汾�ţ�/ Ŀ¼�µ������ļ����ļ��С�
//�ڸ��ǰ�װ��ж�ء����º��һ�������������ʱ�Դ��жϾɰ汾��������Ƿ�Ϊ���������������ļ����ļ��У��Ӷ�ɾ��֮
//ע������˳��Ӧ��������Ŀ¼���ļ����������б�Ŀ¼��
// -----------------------------------------------------------------------*/
#ifndef YYE_FILES_LIST_H
#define YYE_FILES_LIST_H

//��װ��ж�ء����¹���list
const wchar_t* const yye_files_list_common[] = {
  L"Dictionaries\\en-US-3-0.bdic",
  L"Dictionaries",
  L"locales\\en-US.pak",
  L"locales\\zh-CN.pak",
  L"locales\\zh-TW.pak",
  L"locales",
  L"PepperFlash\\manifest.json",
  L"PepperFlash\\pepflashplayer.dll",
  L"PepperFlash",
  L"extensions\\yygame_broadcasting.crx",
  L"extensions\\yygame_broadcasting2.crx",
  L"extensions\\yygame_extension.crx",
  L"extensions\\yygame_notes.crx",
  L"extensions",
  L"plugins\\WebPlayer\\loader\\info.plist",
  L"plugins\\WebPlayer\\loader\\npUnity3D32.dll",
  L"plugins\\WebPlayer\\loader\\UnityWebPluginAX.ocx",
  L"plugins\\WebPlayer\\loader",
  L"plugins\\WebPlayer\\UnityBugReporter.exe",
  L"plugins\\WebPlayer\\UnityWebPlayerUpdate.exe",
  L"plugins\\WebPlayer",
  L"plugins\\npaliedit.dll",
  L"plugins\\npAliSSOLogin.dll",
  L"plugins\\np-mswmp.dll",
  L"plugins\\npyygameutils.dll",
  L"plugins",
  L"udb.dll",
  L"udblogin.log",
  L"D3DCompiler_43.dll",
  L"D3DCompiler_46.dll",
  L"chrome.dll",
  L"chrome_child.dll",
  L"chrome_100_percent.pak",
  L"ffmpegsumo.dll",
  L"libEGL.dll",
  L"libGLESv2.dll",
  L"resources.pak",
  L"StartInfo.db",
  L"StartInfoXp.db",
  L"StartInfoWin7.db",
  L"debug.log",
  L"update.log",
  L"uninst.exe",//����uninst.exe��Ϊ�汾Ŀ¼���ˣ�Ŀ¼�ڵĿ����ƶ�. by hzz 2014-11-12 11:05:48
  L"YYExplorer.exe",
  L"YYExplorerCrash.exe",
  L"YYExplorerSetting.exe",
  L"YYExplorerUplive.exe",
  L"YYExplorerUpliveTrigger.exe",
  L"..\\Old_YYExplorer.exe",
  L"..\\Backup_YYExplorer.exe",
  L"..\\debug.log",
  L"YYEDeskPopWtl.exe",
  L"chromium_red.ico",
  L"chromium.ico",
  L"tips_bg.png",
  L"pop.log",
  L"push_task_entities",
  L"url_destop.ico",
  L"video.ico",
  L"url_desktop_game.ico",
  NULL,
};

//��װ����
const wchar_t* const yye_files_list_extra_install[] = {
  //L"First Run",             //�����װ���������user dataѡ���ɾ������ļ�
  L"..\\YYExplorer.exe",
  L"..\\New_YYExplorer.exe",
  L"uninst.exe",
  L"..\\uninst.exe",
  NULL,
};

//ж�ض���
const wchar_t* const yye_files_list_extra_uninstall[] = {
  L"..\\YYExplorer.exe",
  L"..\\New_YYExplorer.exe",
  NULL,
};

//���¶���
const wchar_t* const yye_files_list_extra_update[] = {
  L"uninst.exe",
  NULL,
};

#endif

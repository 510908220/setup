/* -------------------------------------------------------------------------
//	FileName	：	yygame/yye_files_list.h
//	Creator		：	(wyk)
//	CreateTime	：	2014年4月21日18:12:07
//	Description	：	本文件保存了YY浏览器在安装或使用过程中可能会在安装目录下产生的所有文件或文件夹列表，
//当前目录为 %appdata%/duowan/YYExplorer/（版本号）/ 目录下的所有文件或文件夹。
//在覆盖安装、卸载、更新后第一次运行新浏览器时以此判断旧版本浏览器中是否为浏览器本身产生的文件或文件夹，从而删除之
//注：罗列顺序应先罗列子目录（文件），再罗列本目录名
// -----------------------------------------------------------------------*/
#ifndef YYE_FILES_LIST_H
#define YYE_FILES_LIST_H

//安装、卸载、更新共用list
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
  L"uninst.exe",//由于uninst.exe改为版本目录外了，目录内的可以移动. by hzz 2014-11-12 11:05:48
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

//安装额外
const wchar_t* const yye_files_list_extra_install[] = {
  //L"First Run",             //如果安装过程有清除user data选项，则删除这个文件
  L"..\\YYExplorer.exe",
  L"..\\New_YYExplorer.exe",
  L"uninst.exe",
  L"..\\uninst.exe",
  NULL,
};

//卸载额外
const wchar_t* const yye_files_list_extra_uninstall[] = {
  L"..\\YYExplorer.exe",
  L"..\\New_YYExplorer.exe",
  NULL,
};

//更新额外
const wchar_t* const yye_files_list_extra_update[] = {
  L"uninst.exe",
  NULL,
};

#endif

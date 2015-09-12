#ifndef LANG_STRING_H_
#define LANG_STRING_H_
//这个文件在每次构建的时候会重新生成一次。如要更改它，请更改.\chrome\trunk\src\dailybuild\genversionHFile.py文件。
//This file will be rebuilt by dailybuild.If you want to alter it,please alter .\chrome\trunk\src\dailybuild\genversionHFile.py file.
//last build date 2013.11.4
#define FILE_VERSION                2013,12,18,2348
#define PRODUCT_VERSION             0,1,313,0
#define STR_FILE_VERSION            "20131218.2348"
#define STR_PRODUCT_VERSION         "0.1.313.0"

#define RAW_VERSION                 STR_PRODUCT_VERSION
#define SETUP_VERSION               _TEXT(RAW_VERSION)
#define ENG_PRODUCT_NAME            _TEXT("YYExplorer")
#define UI_PRODUCT_NAME             _TEXT("YY浏览器")
#define UI_COMMON_UI_TITLE          _TEXT("[ProductName] 安装向导")

#define UI_FILE_DIR_INFO            _TEXT("请选择安装路径：")
#define UI_FILE_ERR_SPACE_NOT_ENOUGH    _TEXT("临时目录所在C盘空间不足，请将系统盘清理出至少100M空间后重试")
#define UI_FILE_ERR_INSTALL_SPACE_NOT_ENOUGH    _TEXT("安装目录所在磁盘空间不足，请清理出至少100M空间或更换磁盘后重试")
#define UI_FILE_ERR_PATH_TOOLONG    _TEXT("您选择的路径长度过长，请重新选择。")
#define UI_FILE_ERR_SYS_FORBID_PATH _TEXT("该目录为系统目录，请重新修改路径。")
#define UI_FILE_ERR_INVALID_PATH    _TEXT("该目录为无效目录，请重新修改路径。")
#define UI_FILE_ERR_VOID_PATH       _TEXT("安装路径不能为空。")

#define ALREADY_LARTEST_VERSION     _TEXT("检测到您已经安装了较新版本的[ProductName]，是否覆盖安装？")

#endif  //LANG_STRING_H_

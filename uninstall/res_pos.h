#ifndef RES_POS_H_
#define RES_POS_H_

const int BACKGROUND_SIZE_W = 600;
const int BACKGROUND_SIZE_HIDE_H = 346;
const int BACKGROUND_SIZE_SHOW_H = 557;

//关闭按钮
const int UNINSTALL_CLOSE_BTN_POS_X = 429;  //(BACKGROUND_SIZE_W - 14 - 9)
const int UNINSTALL_CLOSE_BTN_POS_Y = 13;
const int UNINSTALL_CLOSE_BTN_SIZE_W = 24;
const int UNINSTALL_CLOSE_BTN_SIZE_H = 24;
const int CLOSE_BTN_H_INTERVAL = 10;
const int CLOSE_BTN_V_INTERVAL = 4;
//dialog1
//const int LOGO_POS_X                           100
//const int LOGO_POS_Y                           70
//const int LOGO_SIZE_W                          253
//const int LOGO_SIZE_H                          96

//const int LOGO_UNINSTALLING_POS_X              100
//const int LOGO_UNINSTALLING_POS_Y              40
//一键卸载
const int UNINSTALL_BUTTON_POS_X = 144;
const int UNINSTALL_BUTTON_POS_Y = 226;
const int UNINSTALL_BUTTON_SIZE_W = 162;
const int UNINSTALL_BUTTON_SIZE_H = 50;

#define UNINSTALL_BUTTON_FONT_COLOR_SIZE_VISTA          _TEXT("Microsoft YaHei"),RGB(0x76,0x3c,0x00),23  //这样写宏定义不知道合适不
#define UNINSTALL_BUTTON_FONT_COLOR_SIZE_XP             _TEXT("Simsun"),RGB(0x76,0x3c,0x00),18  //这样写宏定义不知道合适不

  //YY浏览器正在运行，点击“立刻安装”将关闭浏览器并安装
  const int YYEXPLORER_RUNNING_STATIC_POS_X = 80;
  const int YYEXPLORER_RUNNING_STATIC_POS_Y = 251;
  const int YYEXPLORER_RUNNING_STATIC_SIZE_W = 350;
  const int YYEXPLORER_RUNNING_STATIC_SIZE_H = 18;
//同时删除。。。。
const int DELETE_PROFILE_CHECKBOX_POS_X = 20;
const int DELETE_PROFILE_CHECKBOX_POS_Y = 308;
const int DELETE_PROFILE_CHECKBOX_SIZE_W = 180;     //还要显示后面那些字
const int DELETE_PROFILE_CHECKBOX_SIZE_H = 14;
//dialog2
//默认字体颜色大小
#define PAGE_PROGRESS_DEFAULT_FONT_COLOR_SIZE_VISTA          _TEXT("Microsoft YaHei"),RGB(0x3d,0x3d,0x3d),18
#define PAGE_PROGRESS_DEFAULT_FONT_COLOR_SIZE_XP             _TEXT("SimSun"),RGB(0x3d,0x3d,0x3d),12
//正在卸载对话框
const int UNINSTALL_PROGRESS_DLG_SIZE_W = BACKGROUND_SIZE_W;
const int UNINSTALL_PROGRESS_DLG_SIZE_H = BACKGROUND_SIZE_HIDE_H;

//正在为您卸载YY浏览器n% static  新ui：正在卸载中...
const int UNINSTALLING_POS_X = 27;
const int UNINSTALLING_POS_Y = 187;
const int UNINSTALLING_SIZE_W = 180;
const int UNINSTALLING_SIZE_H = 16;
#define UNINSTALLING_STATIC_MODE  _TEXT("正在卸载中...")
//进度条
const int PROGRESS_POS_X = 27;
const int PROGRESS_POS_Y = 209;
const int PROGRESS_SIZE_W = 480;
const int PROGRESS_SIZE_H = 6;
//取消卸载按钮
const int CANCEL_UNINSTALL_BTN_POS_X = 144;
const int CANCEL_UNINSTALL_BTN_POS_Y = 254;
const int CANCEL_UNINSTALL_BTN_SIZE_W = 162;
const int CANCEL_UNINSTALL_BTN_SIZE_H = 50;
#define CANCEL_UNINSTALL_BUTTON_FONT_COLOR_SIZE_VISTA          _TEXT("Microsoft YaHei"),RGB(0x3d,0x3d,0x3d),20  //这样写宏定义不知道合适不
#define CANCEL_UNINSTALL_BUTTON_FONT_COLOR_SIZE_XP             _TEXT("Simsun"),RGB(0x3d,0x3d,0x3d),14  //这样写宏定义不知道合适不
//dialog3
//默认字体颜色大小
#define PAGE_FEEDBACK_DEFAULT_FONT_COLOR_SIZE_VISTA          _TEXT("Microsoft YaHei"),RGB(0x3d,0x3d,0x3d),18
#define PAGE_FEEDBACK_DEFAULT_FONT_COLOR_SIZE_XP             _TEXT("SimSun"),RGB(0x3d,0x3d,0x3d),12
//用户反馈对话框
const int FEEDBACK_DLG_SIZE_W = 600;
const int FEEDBACK_DLG_SIZE_H = 346;
  //卸载YY浏览器 static标题
  const int UNINSTALL_TITLE_POS_X = 25;
  const int UNINSTALL_TITLE_POS_Y = 40;
  const int UNINSTALL_TITLE_SIZE_W = 225;
  const int UNINSTALL_TITLE_SIZE_H = 23;
  #define SORRY_TITLE_STATIC_FONT_COLOR_SIZE_VISTA          _TEXT("Microsoft YaHei"),RGB(0x33,0x33,0x33),23
  #define SORRY_TITLE_STATIC_FONT_COLOR_SIZE_XP             _TEXT("SimSun"),RGB(0x33,0x33,0x33),23
#define ADVICE_IMPLY_XP             _TEXT("SimSun"),RGB(0x4c,0x4c,0x4c),18
#define ADVICE_IMPLY_VISTA             _TEXT("Microsoft YaHei"),RGB(0x4c,0x4c,0x4c),18
#define ADVICE_IMPLY_GROUP_XP             _TEXT("SimSun"),RGB(0x33,0x99,0xff),18
#define ADVICE_IMPLY_GROUP_VISTA             _TEXT("Microsoft YaHei"),RGB(0x33,0x99,0xff),18
  //分割符
  const int SEPARATOR_POS_X = 24;
  const int SEPARATOR_POS_Y = 46;
//您卸载的原因：
const int UNINSTALL_REASON_STATIC_POS_X = 29;
const int UNINSTALL_REASON_STATIC_POS_Y = 71;
const int UNINSTALL_REASON_STATIC_SIZE_W = 225;
const int UNINSTALL_REASON_STATIC_SIZE_H = 18;
#define UNINSTALL_REASON_FONT_COLOR_SIZE_VISTA          _TEXT("Microsoft YaHei"),RGB(0x37,0x37,0x37),20
#define UNINSTALL_REASON_FONT_COLOR_SIZE_XP             _TEXT("SimSun"),RGB(0x37,0x37,0x37),16
//原因 checkbox
const int UNINSTALL_REASON_CHECKBOX_POS_X = 62;
const int UNINSTALL_REASON_CHECKBOX1_POS_Y = 110;
const int UNINSTALL_REASON_CHECKBOX_SIZE_W = 360;
const int UNINSTALL_REASON_CHECKBOX_SIZE_H = 14;
const int UNINSTALL_REASON_CHECKBOX_DELTA_H = 12;
//其他 编辑框
const int OTHER_REASON_EDIT_POS_X = 61;
const int OTHER_REASON_EDIT_POS_Y = 339;
const int OTHER_REASON_EDIT_SIZE_W = 520;
const int OTHER_REASON_EDIT_SIZE_H = 130;
//其他原因(编辑框内灰字)
const int OTHER_REASON_STATIC_POS_X = (OTHER_REASON_EDIT_POS_X + 6);
const int OTHER_REASON_STATIC_POS_Y = (OTHER_REASON_EDIT_POS_Y + 6);
const int OTHER_REASON_STATIC_SIZE_W = 500;
const int OTHER_REASON_STATIC_SIZE_H = 60;
//您的联系方式：
const int CONTACT_TITLE_STATIC_POS_X = 29;
const int CONTACT_TITLE_STATIC_POS_Y = 414;
const int CONTACT_TITLE_STATIC_SIZE_W = 225;
const int CONTACT_TITLE_STATIC_SIZE_H = 18;
//联系方式 编辑框
const int CONTACT_EDIT_POS_X = 61;
const int CONTACT_EDIT_POS_Y = 444;
const int CONTACT_EDIT_SIZE_W = 361;
const int CONTACT_EDIT_SIZE_H = 25;
//请您留下YY、手机。。。(编辑框内灰字)
const int CONTACT_STATIC_POS_X = (CONTACT_EDIT_POS_X + 6);
const int CONTACT_STATIC_POS_Y = (CONTACT_EDIT_POS_Y + 6);
const int CONTACT_STATIC_SIZE_W = 300;
const int CONTACT_STATIC_SIZE_H = 16;
//提交反馈按钮
const int FEEDBACK_BTN_POS_X = 316;
const int FEEDBACK_BTN_POS_Y = 491;
const int FEEDBACK_BTN_SIZE_W = 160;
const int FEEDBACK_BTN_SIZE_H = 42;
#define FEEDBACK_BUTTON_FONT_COLOR_SIZE_VISTA          _TEXT("Microsoft YaHei"),RGB(0x3d,0x3d,0x3d),20  //这样写宏定义不知道合适不
#define FEEDBACK_BUTTON_FONT_COLOR_SIZE_XP             _TEXT("SimSun"),RGB(0x3d,0x3d,0x3d),16

#define FEEDBACK_URL                               L"http://baidu.com"

#endif
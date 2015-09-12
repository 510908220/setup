#ifndef RES_FILENAME_H_
#define RES_FILENAME_H_

const TCHAR YYEXPLORER_REG_PATH[]                     =  _TEXT("SOFTWARE\\duowan\\YYExplorer");
//#define GAMEPROXY_REG_PATH      _TEXT("SOFTWARE\\duowan\\gameproxy")

const TCHAR BACKGROUND_FILE_REAL[]                    = _TEXT("background.png");
const TCHAR BACKGROUND_FILE[]										      = _TEXT("broswer_setup_bg.png");
const TCHAR SETUP_PROGRESS_BACKGROUND_FILE1[]         = _TEXT("broswer_setup_bg.png");
const TCHAR SETUP_PROGRESS_BACKGROUND_FILE2[]         = _TEXT("broswer_setup_bg2.png");
const TCHAR SETUP_PROGRESS_BACKGROUND_FILE3[]         = _TEXT("broswer_setup_bg3.png");
const TCHAR BACKGROUND_SHOW_FILE[]							      = _TEXT("background_custom_lisence.png");
const TCHAR BACKGROUND_SHOW_LISENCE_FILE[]            = _TEXT("license_bg.png");  //��ʱ��ȫ�򿪵���
const TCHAR BACKGROUND_SHOW_CUSTOM_FILE[]             = _TEXT("background_custom.png");

const TCHAR LOGO_FILE[]                               = _TEXT("");

const TCHAR BINGO_FILE[]                              = _TEXT("bingo.png");

const TCHAR INSTALL_NORMAL_FILE[]                     = _TEXT("btn_setup.png");
const TCHAR INSTALL_HOVER_FILE[]                      = _TEXT("btn_setup_hover.png");
const TCHAR INSTALL_DOWN_FILE[]                       = _TEXT("btn_setup_pressed.png");
const TCHAR INSTALL_DISABLE_FILE[]                    = _TEXT("btn_setup_disabled.png");

const TCHAR CLOSE_NORMAL_FILE[]                       = _TEXT("browser_setup_close_normal.png");
const TCHAR CLOSE_HOVER_FILE[]                        = _TEXT("browser_setup_close_hover.png");
const TCHAR CLOSE_DOWN_FILE[]                         = _TEXT("browser_setup_close_pressed.png");
const TCHAR CLOSE_DISABLE_FILE[]                      = _TEXT("");

const TCHAR CHECK_NORMAL_FILE[]                       = _TEXT("btn_checkbox_true.png");
const TCHAR CHECK_HOVER_FILE[]				= _TEXT("btn_checkbox_true_hover.png");
const TCHAR CHECK_DWON_FILE[]         = _TEXT("btn_checkbox_true_pressed.png");
const TCHAR CHECK_DISABLE_FILE[]      = _TEXT("");

const TCHAR UNCHECK_NORMAL_FILE[]     = _TEXT("btn_checkbox.png");
const TCHAR UNCHECK_HOVER_FILE[]      = _TEXT("btn_checkbox_hover.png");
const TCHAR UNCHECK_DOWN_FILE[]       = _TEXT("btn_checkbox_pressed.png");
const TCHAR UNCHECK_DISABLE_FILE[]    = _TEXT("");

const TCHAR CUSTOM_HIDE_NORMAL_FILE[] = _TEXT("btn30_normal.png");
const TCHAR CUSTOM_HIDE_HOVER_FILE[]  = _TEXT("btn30_hover.png");
const TCHAR CUSTOM_HIDE_DOWN_FILE[]    = _TEXT("btn30_down.png");
const TCHAR CUSTOM_HIDE_DISABLE_FILE[] = _TEXT("btn30_disable.png");

const TCHAR CUSTOM_SHOW_NORMAL_FILE[] = _TEXT("btn30_normal.png");
const TCHAR CUSTOM_SHOW_HOVER_FILE[]  = _TEXT("btn30_hover.png");
const TCHAR CUSTOM_SHOW_DOWN_FILE[]    = _TEXT("btn30_down.png");
const TCHAR CUSTOM_SHOW_DISABLE_FILE[] = _TEXT("btn30_disable.png");

const TCHAR PAGE_CUSTOM_BACK_NORMAL_FILE[] = _TEXT("btn_back.png");
const TCHAR PAGE_CUSTOM_BACK_HOVER_FILE[]  = _TEXT("btn_back_hover.png");
const TCHAR PAGE_CUSTOM_BACK_DOWN_FILE[]    = _TEXT("btn_back_pressed.png");
const TCHAR PAGE_CUSTOM_BACK_DISABLE_FILE[] = _TEXT("");

const TCHAR PAGE_LICENCE_BACK_NORMAL_FILE[] = _TEXT("btn_back.png");
const TCHAR PAGE_LICENCE_BACK_HOVER_FILE[]  = _TEXT("btn_back_hover.png");
const TCHAR PAGE_LICENCE_BACK_DOWN_FILE[]    = _TEXT("btn_back_pressed.png");
const TCHAR PAGE_LICENCE_BACK_DISABLE_FILE[] = _TEXT("");

const TCHAR LEFT_ARROW_FILE[]               = _TEXT("back_left_arrow.png");
const TCHAR RIGHT_ARROW_FILE[]              = _TEXT("custom_right_arrow.png");

const TCHAR SETUP_PROGRESS_FILE[]     = _TEXT("broswer_setup_progress.png");
const TCHAR SETUP_PROGRESSBKG_FILE[]  = _TEXT("broswer_setup_progress_bg.png");

const TCHAR CHANGEPATH_NORMAL_FILE[]  = _TEXT("btn_update.png");
const TCHAR CHANGEPATH_HOVER_FILE[]   = _TEXT("btn_update_hover.png");
const TCHAR CHANGEPATH_DOWN_FILE[]    = _TEXT("btn_update_pressed.png");
const TCHAR CHANGEPATH_DISABLE_FILE[] = _TEXT("");

const TCHAR INSTALL_TITLE[]           = _TEXT("��װYY�����");
const TCHAR INSTALLING_STATIC_MODE[]  = _TEXT("����Ϊ����װYY�����");
const TCHAR YYEXPLORER_FIRST_RUN_WND_TITLE[]           = _TEXT("���µı�ǩҳ - YY�����");
const TCHAR YYEXPLORER_FIRST_RUN_WND_CLASS_NAME[]      = _TEXT("Chrome_WidgetWin_1");

const WCHAR kPinStrEn[] = L"Pin to Tas&kbar";  
const WCHAR kPinStrCh[] = L"������������(&K)";  
const WCHAR kUnPinStrEn[] = L"Unpin from Tas&kbar";
const WCHAR kUnPinStrCh[] = L"������������(&K)";  

typedef struct  __FONT_COLOR_SIZE
{
  WCHAR* szFont;
  COLORREF color;
  int nHeight;
} FONT_COLOR_SIZE;

//
const FONT_COLOR_SIZE _PAGE_INIT_DEFAULT_FONT_COLOR_SIZE_VISTA = 
{
  _TEXT("Microsoft YaHei"), 
  RGB(255,255,255), 
  12
};
const FONT_COLOR_SIZE _PAGE_INIT_DEFAULT_FONT_COLOR_SIZE_XP = 
{
  _TEXT("SimSun"), 
  RGB(255,255,255), 
  18
};
//#define PAGE_INIT_DEFAULT_FONT_COLOR_SIZE_VISTA             _TEXT("Microsoft YaHei"),RGB(255,255,255),12
//#define PAGE_INIT_DEFAULT_FONT_COLOR_SIZE_XP                _TEXT("SimSun"),RGB(255,255,255),18

//#define PAGE_INIT_BLUE_LINK_FONT_COLOR_SIZE_XP             _TEXT("Microsoft YaHei"),RGB(255,255,255),14      //����ģ����ֻ���ڴ��������
//#define PAGE_INIT_BLUE_LINK_FONT_COLOR_SIZE_VISTA          _TEXT("SimSun"),RGB(255,255,255),18

//����ÿ���ռ��λ�á�x��y�����Ͻǵ��λ�����꣬w��h�ǿ��
const int LOGO_POS_X                    = 98;
const int LOGO_POS_Y                    = 70;
const int LOGO_SIZE_W                   = 253;
const int LOGO_SIZE_H                   = 96;

const int PAGE_INIT_LOGO_POS_X           = 98;
const int PAGE_INIT_LOGO_POS_Y           = 70;
const int PAGE_CUSTOM_LOGO_POS_X         = 98;
const int PAGE_CUSTOM_LOGO_POS_Y         = 40;

const int BACKGROUND_SIZE_W             = 500;
const int BACKGROUND_SIZE_HIDE_H        = 400;
  const int BACKGROUND_SIZE_SHOW_H      =   630;        //licence���Զ���ѡ���ʾ
  const int BACKGROUND_SIZE_SHOW_LISENCE_H = 400;
  const int BACKGROUND_SIZE_SHOW_CUSTOM_H  = 450;

const int LICENCE_DLG_SIZE_W            = 500;
const int LICENCE_DLG_HIDE_SIZE_H       = 400;
  const int LICENCE_DLG_SHOW_LISENCE_SIZE_H      =  BACKGROUND_SIZE_SHOW_LISENCE_H;
  const int LICENCE_DLG_SHOW_CUSTOM_SIZE_H       =  450;
  const int LICENCE_DLG_SHOW_SIZE_H     =   630;

const int INSTALL_BUTTON_POS_X          = 170;
const int INSTALL_BUTTON_POS_Y          = 237;
const int INSTALL_BUTTON_SIZE_W         = 160;
const int INSTALL_BUTTON_SIZE_H         = 48;
//#define INSTALL_BUTTON_FONT_COLOR_SIZE_VISTA             _TEXT("Microsoft YaHei"),RGB(0x76,0x3c,0x00),23  //����д�궨�岻֪�����ʲ�
//#define INSTALL_BUTTON_FONT_COLOR_SIZE_XP          _TEXT("SimSuni"),RGB(0x76,0x3c,0x00),18
const int PAGE_CUSTOM_INSTALL_BUTTON_POS_X         =  145;
const int PAGE_CUSTOM_INSTALL_BUTTON_POS_Y         =  254;
const int PAGE_CUSTOM_INSTALL_BUTTON_SIZE_W        =  160;
const int PAGE_CUSTOM_INSTALL_BUTTON_SIZE_H        =  38;
const FONT_COLOR_SIZE PAGE_CUSTOM_INSTALL_BUTTON_FONT_COLOR_SIZE_VISTA = 
{
	_TEXT("Microsoft YaHei"), 
	RGB(0x76,0x3c,0x00), 
	20
};
const FONT_COLOR_SIZE PAGE_CUSTOM_INSTALL_BUTTON_FONT_COLOR_SIZE_XP = 
{
	_TEXT("SimSun"), 
	RGB(0x76,0x3c,0x00), 
	13
};
//#define PAGE_CUSTOM_INSTALL_BUTTON_FONT_COLOR_SIZE_VISTA             _TEXT("Microsoft YaHei"),RGB(0x76,0x3c,0x00),20
//#define PAGE_CUSTOM_INSTALL_BUTTON_FONT_COLOR_SIZE_XP          _TEXT("SimSun"),RGB(0x76,0x3c,0x00),13
//�����Ķ�������
const int LICENCE_CHECKBOX_POS_X       =  93;
const int LICENCE_CHECKBOX_POS_Y       =  364;
const int LICENCE_CHECKBOX_SIZE_W      =  105;           //��Ҫ��ʾ�������Ķ������ܡ�
const int LICENCE_CHECKBOX_SIZE_H      =  16;
//�û����Э��
const int LICENCE_STATIC_POS_X         =  (LICENCE_CHECKBOX_POS_X+LICENCE_CHECKBOX_SIZE_W+1);
const int LICENCE_STATIC_POS_Y         =  LICENCE_CHECKBOX_POS_Y;
const int LICENCE_STATIC_SIZE_W        =  150;
const int LICENCE_STATIC_SIZE_H        =  18;

const int CLOSE_BTN_POS_X              =  476;
const int CLOSE_BTN_POS_Y              =  12;
const int CLOSE_BTN_SIZE_W             =  12;
const int CLOSE_BTN_SIZE_H             =  12;

const int BACK_BUTTON_PAGE_LICENCE_POS_X          =  214;
const int BACK_BUTTON_PAGE_LICENCE_POS_Y          =  352;
const int BACK_BUTTON_PAGE_LICENCE_SIZE_W         =  74;
const int BACK_BUTTON_PAGE_LICENCE_SIZE_H         =  30;
//const FONT_COLOR_SIZE BACK_BUTTON_PAGE_LICENCE_FONT_COLOR_SIZE_XP = 
//{
//	_TEXT("SimSun"), 
//	RGB(0x37,0x37,0x37), 
//	12
//};
//const FONT_COLOR_SIZE BACK_BUTTON_PAGE_LICENCE_FONT_COLOR_SIZE_VISTA = 
//{
//	_TEXT("Microsoft YaHei"), 
//	RGB(0x37,0x37,0x37), 
//	18
//};
//#define BACK_BUTTON_PAGE_LICENCE_FONT_COLOR_SIZE_XP             _TEXT("SimSun"),RGB(0x37,0x37,0x37),12  //����д�궨�岻֪�����ʲ�
//#define BACK_BUTTON_PAGE_LICENCE_FONT_COLOR_SIZE_VISTA          _TEXT("Microsoft YaHei"),RGB(0x37,0x37,0x37),18  //����д�궨�岻֪�����ʲ�

const int BACK_BUTTON_PAGE_CUSTOM_POS_X          =  30;
const int BACK_BUTTON_PAGE_CUSTOM_POS_Y          =  292;
const int BACK_BUTTON_PAGE_CUSTOM_SIZE_W         =  56;
const int BACK_BUTTON_PAGE_CUSTOM_SIZE_H         =  30;
const FONT_COLOR_SIZE BACK_BUTTON_PAGE_CUSTOM_FONT_COLOR_SIZE_XP = 
{
	_TEXT("SimSun"), 
	RGB(0x37,0x37,0x37), 
	12
};
const FONT_COLOR_SIZE BACK_BUTTON_PAGE_CUSTOM_FONT_COLOR_SIZE_VISTA = 
{
	_TEXT("Microsoft YaHei"), 
	RGB(0x37,0x37,0x37), 
	18
};
//#define BACK_BUTTON_PAGE_CUSTOM_FONT_COLOR_SIZE_XP             _TEXT("SimSun"),RGB(0x37,0x37,0x37),12  //����д�궨�岻֪�����ʲ�
//#define BACK_BUTTON_PAGE_CUSTOM_FONT_COLOR_SIZE_VISTA          _TEXT("Microsoft YaHei"),RGB(0x37,0x37,0x37),18  //����д�궨�岻֪�����ʲ�
const TCHAR BACK_LEFT_ARROW_FILE[]           =  _TEXT("back_left_arrow.png");
const int BACK_LEFT_ARROW_POS_X          =  8;
const int BACK_LEFT_ARROW_POS_Y          =  10;

const int CUSTOM_BUTTON_POS_X          =  326;
const int CUSTOM_BUTTON_POS_Y          =  292;
const int CUSTOM_BUTTON_SIZE_W         =  94;
const int CUSTOM_BUTTON_SIZE_H         =  30;
const FONT_COLOR_SIZE CUSTOM_BUTTON_FONT_COLOR_SIZE_XP = 
{
	_TEXT("SimSun"), 
	RGB(55,55,55), 
	12
};
const FONT_COLOR_SIZE CUSTOM_BUTTON_FONT_COLOR_SIZE_VISTA = 
{
	_TEXT("Microsoft YaHei"), 
	RGB(55,55,55), 
	18
};
//#define CUSTOM_BUTTON_FONT_COLOR_SIZE_XP             _TEXT("SimSun"),RGB(55,55,55),12  //����д�궨�岻֪�����ʲ�
//#define CUSTOM_BUTTON_FONT_COLOR_SIZE_VISTA          _TEXT("Microsoft YaHei"),RGB(55,55,55),18  //����д�궨�岻֪�����ʲ�
const TCHAR CUSTOM_RIGHT_ARROW_FILE[]        =     _TEXT("custom_right_arrow.png");
const int CUSTOM_RIGHT_ARROW_POS_X          =  75;
const int CUSTOM_RIGHT_ARROW_POS_Y          =  10;


//YY������������У���������̰�װ�����ر����������װ
const int YYEXPLORER_RUNNING_STATIC_POS_X    =  80;
const int YYEXPLORER_RUNNING_STATIC_POS_Y    =  251;
const int YYEXPLORER_RUNNING_STATIC_SIZE_W   =  350;
const int YYEXPLORER_RUNNING_STATIC_SIZE_H   =  18;
//�û����Э�� ����
const int LICENCE_TITLE_STATIC_POS_X    =  20;
const int LICENCE_TITLE_STATIC_POS_Y    =  25;
const int LICENCE_TITLE_STATIC_SIZE_W   =  225;
const int LICENCE_TITLE_STATIC_SIZE_H   =  20;
const FONT_COLOR_SIZE LICENCE_TITLE_STATIC_FONT_COLOR_SIZE_XP = 
{
	_TEXT("SimSun"), 
	RGB(0x33, 0x48, 0x58), 
	18
};
const FONT_COLOR_SIZE LICENCE_TITLE_STATIC_FONT_COLOR_SIZE_VISTA = 
{
	_TEXT("Microsoft YaHei"), 
	RGB(0x33, 0x48, 0x58), 
	23
};
//#define LICENCE_TITLE_STATIC_FONT_COLOR_SIZE_VISTA             _TEXT("Microsoft YaHei"),RGB(0x33, 0x48, 0x58),23
//#define LICENCE_TITLE_STATIC_FONT_COLOR_SIZE_XP                _TEXT("SimSun"),RGB(0x33, 0x48, 0x58),18
//YY�����0.1.7beta�� ����
const int INSTALL_TITLE_STATIC_POS_X    =  12;
const int INSTALL_TITLE_STATIC_POS_Y    =  12;
const int INSTALL_TITLE_STATIC_SIZE_W   =  225;
const int INSTALL_TITLE_STATIC_SIZE_H   =  20;
const FONT_COLOR_SIZE INSTALL_TITLE_STATIC_FONT_COLOR_SIZE_XP = 
{
	_TEXT("SimSun"), 
	RGB(0x33, 0x43, 0x51), 
	12
};
const FONT_COLOR_SIZE INSTALL_TITLE_STATIC_FONT_COLOR_SIZE_VISTA = 
{
	_TEXT("Microsoft YaHei"), 
	RGB(0x33, 0x43, 0x51), 
	18
};
//#define INSTALL_TITLE_STATIC_FONT_COLOR_SIZE_VISTA             _TEXT("Microsoft YaHei"),RGB(0x33, 0x43, 0x51),18
//#define INSTALL_TITLE_STATIC_FONT_COLOR_SIZE_XP                _TEXT("SimSun"),RGB(0x33, 0x43, 0x51),12
//��װ��
const int INSTALL_PATH_STATIC_POS_X    =  40;
const int INSTALL_PATH_STATIC_POS_Y    =  334;  //335
const int INSTALL_PATH_STATIC_SIZE_W   =  55;
const int INSTALL_PATH_STATIC_SIZE_H   =  18;
//��װ���༭��
const int INSTALL_PATH_EDIT_POS_X       = 95;
const int INSTALL_PATH_EDIT_POS_Y       = 332;
const int INSTALL_PATH_EDIT_SIZE_W      = 305; //305
const int INSTALL_PATH_EDIT_SIZE_H      = 22;
//...���������·��..
const int INSTALL_NOT_CHANGE_STATIC_POS_X     = INSTALL_PATH_EDIT_POS_X;
const int INSTALL_NOT_CHANGE_STATIC_POS_Y     = (INSTALL_PATH_STATIC_POS_Y + 20);
const int INSTALL_NOT_CHANGE_STATIC_SIZE_W    = 370;
const int INSTALL_NOT_CHANGE_STATIC_SIZE_H    = 18;
//����Ŀ¼��ť
const int CHANGE_PATH_BTN_POS_X         = 406;
const int CHANGE_PATH_BTN_POS_Y         = 332;
const int CHANGE_PATH_BTN_SIZE_W        = 52;
const int CHANGE_PATH_BTN_SIZE_H        = 22;
//const FONT_COLOR_SIZE CHANGE_PATH_BTN_FONT_COLOR_SIZE_XP = 
//{
//	_TEXT("SimSun"), 
//	RGB(55,55,55), 
//	12
//};
//const FONT_COLOR_SIZE CHANGE_PATH_BTN_FONT_COLOR_SIZE_VISTA = 
//{
//	_TEXT("Microsoft YaHei"), 
//	RGB(55,55,55), 
//	18
//};
//#define CHANGE_PATH_BTN_FONT_COLOR_SIZE_XP             _TEXT("SimSun"),RGB(55,55,55),12  //����д�궨�岻֪�����ʲ�
//#define CHANGE_PATH_BTN_FONT_COLOR_SIZE_VISTA          _TEXT("Microsoft YaHei"),RGB(55,55,55),18  //����д�궨�岻֪�����ʲ�
//������ݷ�ʽcheckbox
const int SHORTCUT_CHECKBOX_POS_X       = 31;
const int SHORTCUT_CHECKBOX_POS_Y       = 207;
const int SHORTCUT_CHECKBOX_SIZE_W      = 180;           //��Ҫ��ʾ��������ݷ�ʽ��
const int SHORTCUT_CHECKBOX_SIZE_H      = 16;
//����checkbox
const int JOIN_EXP_CHECKBOX_POS_X       = 270;
const int JOIN_EXP_CHECKBOX_POS_Y       = 380;
const int JOIN_EXP_CHECKBOX_SIZE_W      = 50;
const int JOIN_EXP_CHECKBOX_SIZE_H      = 16;
//�û�����ƻ�����
const int JOIN_EXP_STATIC_POS_X         = 320;
const int JOIN_EXP_STATIC_POS_Y         = 381;
const int JOIN_EXP_STATIC_SIZE_W        = 100;           
const int JOIN_EXP_STATIC_SIZE_H        = 16;
//��ΪĬ�������checkbox
const int SET_DEFAULT_CHECKBOX_POS_X    = 225;
const int SET_DEFAULT_CHECKBOX_POS_Y    = 207; //415
const int SET_DEFAULT_CHECKBOX_SIZE_W   = 150;            
const int SET_DEFAULT_CHECKBOX_SIZE_H   = 16;
//Э������//��Щֵ��λ��������
const int LICENCE_BROWSER_POS_X         = 21;      //���أ�22  //ԭ��4
const int LICENCE_BROWSER_POS_Y         = 64;    //���أ�578 //ԭ��390
const int LICENCE_BROWSER_SIZE_W        = 460; //408    //css -25          
const int LICENCE_BROWSER_SIZE_H        = 274; //217    //���������У���Ҫ��css//ԭ��y+174
//Э������//��Щֵ��λ������
const int LICENCE_BROWSER_WITHOUT_CUSTOM_POS_X   = 10;    
//#define LICENCE_BROWSER_WITHOUT_CUSTOM_POS_Y    LICENCE_BROWSER_WITH_CUSTOM_POS_Y //��ʱ��ȫ�򿪵���  
const int LICENCE_BROWSER_WITHOUT_CUSTOM_POS_Y   = CHANGE_PATH_BTN_POS_Y;  
const int LICENCE_BROWSER_WITH_CUSTOM_POS_X      = LICENCE_BROWSER_WITHOUT_CUSTOM_POS_X;    
const int LICENCE_BROWSER_WITH_CUSTOM_POS_Y      = 470;    
const int LICENCE_BROWSER_PIX_SIZE_W             = 435;              
const int LICENCE_BROWSER_PIX_SIZE_H             = 140;    
//����Ϊ����װYY�������n%
const int INSTALLING_STATIC_POS_X       = 46;
const int INSTALLING_STATIC_POS_Y       = 223;
const int INSTALLING_STATIC_SIZE_W      = 170;         
const int INSTALLING_STATIC_SIZE_H      = 18;
//n
const int INSTALLING_NUMBER_STATIC_POS_X       = 117;//���Ǵ�YY��ʼ�����ֽ��������ꡣ����������168
const int INSTALLING_NUMBER_STATIC_POS_Y       = 223;
const int INSTALLING_NUMBER_STATIC_SIZE_W      = 83;//32
const int INSTALLING_NUMBER_STATIC_SIZE_H      = 18;
//������
const int PROGRESS_POS_X                = 24;
const int PROGRESS_POS_Y                = 312;
const int PROGRESS_SIZE_W               = 400;         
const int PROGRESS_SIZE_H               = 8;
//movie
const int MOVIE_SIZE_W             = 274;
const int MOVIE_SIZE_H             = 274;

//logo����ÿ֡���
const int BINGO_ONE_FRAME_W             = 274;
const int BINGO_SIZE_W                  = 18632;
const int BINGO_SIZE_H                  = 274;
//logo����ÿ֡����ʱ��(ms)
const int BINGO_MS_PER_FRAME            = 300;

#endif  //! #ifndef RES_FILENAME_H_
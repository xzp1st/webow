
#include "qmdl_config.h"

#ifdef QMDL_VER_TPF

#include "qmdl_tpf.h"


//////////////////////////////////////////////////////////////////////////
void QMain::AddGridProperties(QMainTypeInfo *info)
{
    if(info == NULL)
    {
        return;
    }
    info->mapprops["GridMode"] = QMainPropInfo(PROPERTYID_GRIDMODE, EVT_ENUM);
    info->mapprops["GridLineWidth"] = QMainPropInfo(PROPERTYID_GRIDLINEWIDTH, EVT_INT);
    info->mapprops["GridStyle"] = QMainPropInfo(PROPERTYID_GRIDPENSTYLE, EVT_ENUM);
    info->mapprops["GridColor"] = QMainPropInfo(PROPERTYID_GRIDCOLOR, EVT_COLOR);
}       

void QMain::AddMultiLineTextProperties(QMainTypeInfo *info)
{
    if(info == NULL)
    {
        return;
    }
    info->mapprops["MultiLine"] = QMainPropInfo(PROPERTYID_MULTILINE, EVT_BOOL);
    info->mapprops["AutoChangeLine"] = QMainPropInfo(PROPERTYID_AUTOCHANGELINE, EVT_BOOL);
    info->mapprops["LineSpacing"] = QMainPropInfo(PROPERTYID_RE_LINESPACING, EVT_INT);
    info->mapprops["WordBreak"] = QMainPropInfo(PROPERTYID_WORDBREAK, EVT_BOOL);
}

void QMain::AddRangeProperties(QMainTypeInfo *info)
{
    if(info == NULL)
    {
        return;
    }
    info->mapprops["LowerRange"] = QMainPropInfo(PROPERTYID_LOWERRANGE, EVT_INT);
    info->mapprops["UpperRange"] = QMainPropInfo(PROPERTYID_UPPERRANGE, EVT_INT);
    info->mapprops["CurrentPosition"] = QMainPropInfo(PROPERTYID_CURPOS, EVT_INT);     
}

void QMain::AddStateBgProperties(QMainTypeInfo *info)
{
    if(info == NULL)
    {
        return;
    }
    info->mapprops["BackgroundHover"] = QMainPropInfo( PROPERTYID_BG_HOVER,    EVT_IMAGESET);
    info->mapprops["BackgroundPressed"] = QMainPropInfo(PROPERTYID_BG_PRESSED, EVT_IMAGESET);
    info->mapprops["BackgroundChecked"] = QMainPropInfo(PROPERTYID_BG_CHECKED, EVT_IMAGESET);
    info->mapprops["BackgroundDisable"] = QMainPropInfo(PROPERTYID_BG_DISABLE, EVT_IMAGESET);
}

void QMain::AddStateImageProperties(QMainTypeInfo *info)
{
    if(info == NULL)
    {
        return;
    }
    info->mapprops["Image"] = QMainPropInfo(       PROPERTYID_IMAGE, EVT_STRING);
    info->mapprops["ImageHover"] = QMainPropInfo(  PROPERTYID_IMG_HOVER, EVT_STRING);
    info->mapprops["ImageDisable"] = QMainPropInfo(PROPERTYID_IMG_DISABLE, EVT_STRING);
    info->mapprops["ImagePressed"] = QMainPropInfo(PROPERTYID_IMG_PRESSED, EVT_STRING);    
}

void QMain::InitCommonPropertyPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(0);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["XOffset"] = QMainPropInfo(	PROPERTYID_XOFFSET, EVT_INT);
    pinfo->mapprops["YOffset"] = QMainPropInfo(	PROPERTYID_YOFFSET, EVT_INT);
    pinfo->mapprops["Width"] = QMainPropInfo(	PROPERTYID_WIDTH,   EVT_INT);
    pinfo->mapprops["Height"] = QMainPropInfo(	PROPERTYID_HEIGHT,  EVT_INT);
    pinfo->mapprops["Visible"] = QMainPropInfo(	PROPERTYID_VISIBLE, EVT_BOOL);
    pinfo->mapprops["Disable"] = QMainPropInfo(	PROPERTYID_DISABLE, EVT_BOOL);
    pinfo->mapprops["Alpha"] = QMainPropInfo(	PROPERTYID_ALPHA,	EVT_BYTE);
    pinfo->mapprops["Transparent"] = QMainPropInfo( PROPERTYID_TRANSPARENT, EVT_BOOL);
    pinfo->mapprops["TransRegionEventTransparent"] = QMainPropInfo( PROPERTYID_TRANSCOLOREVENTTRANSPARENT, EVT_BOOL);
    pinfo->mapprops["CreateType"] = QMainPropInfo( PROPERTYID_CREATETYPE, EVT_STRING);
    pinfo->mapprops["CustomID"] = QMainPropInfo( PROPERTYID_CUSTOMID, EVT_DWORD);
    pinfo->mapprops["CursorID"] = QMainPropInfo( PROPERTYID_CURSORID, EVT_INT);
    pinfo->mapprops["TabStop"] = QMainPropInfo( PROPERTYID_TABSTOP, EVT_BOOL);
    pinfo->mapprops["TabStop"] = QMainPropInfo( PROPERTYID_TABSTOP, EVT_BOOL);
    pinfo->mapprops["SetID"] = QMainPropInfo( PROPERTYID_SETID, EVT_INT);

    pinfo->mapprops["Tip"] = QMainPropInfo( PROPERTYID_TIP, EVT_STRING);
    pinfo->mapprops["TextTip"] = QMainPropInfo( PROPERTYID_TEXT_TIP, EVT_STRING);		
    pinfo->mapprops["TipPosition"] = QMainPropInfo( PROPERTYID_TIPPOS, EVT_ENUM);
    pinfo->mapprops["TipName"] = QMainPropInfo( PROPERTYID_TIPNAME, EVT_STRING);
    pinfo->mapprops["NoTipWhenDisable"] = QMainPropInfo( PROPERTYID_NOTIPWHENDISABLE, EVT_BOOL);

    pinfo->mapprops["LayoutMode"] = QMainPropInfo( PROPERTYID_LAYOUTMODE, EVT_LAYOUTMODE);
    pinfo->mapprops["PostType"] = QMainPropInfo( PROPERTYID_POSTTYPE, EVT_ENUM);
    pinfo->mapprops["XPostPercent"] = QMainPropInfo( PROPERTYID_XPOSTPERCENT, EVT_INT);
    pinfo->mapprops["YPostPercent"] = QMainPropInfo( PROPERTYID_YPOSTPERCENT, EVT_INT);
    pinfo->mapprops["BaseProportion"] = QMainPropInfo( PROPERTYID_BASE_PROPORTION, EVT_INT);
    pinfo->mapprops["InnerMargins"] = QMainPropInfo( PROPERTYID_INNERMARGINS, EVT_STRING);
    pinfo->mapprops["OuterMargins"] = QMainPropInfo( PROPERTYID_OUTERMARGINS, EVT_STRING);

    pinfo->mapprops["FadeInTime"] = QMainPropInfo( PROPERTYID_FADEINTIME, EVT_INT);
    pinfo->mapprops["FadeOutTime"] = QMainPropInfo( PROPERTYID_FADEOUTTIME, EVT_INT);

    pinfo->mapprops["Content"] = QMainPropInfo( PROPERTYID_CONTENT, EVT_STRING);

    pinfo->mapprops["Background"] = QMainPropInfo(		PROPERTYID_BACKGROUND,	EVT_IMAGESET);
    pinfo->mapprops["TransColor"] = QMainPropInfo(		PROPERTYID_TRANSCOLOR,	EVT_COLOR);
    pinfo->mapprops["ImageOverturnMode"] = QMainPropInfo( PROPERTYID_IMAGEOVERTURNMODE, EVT_ENUM);

    pinfo->mapprops["HasBorder"] = QMainPropInfo(	PROPERTYID_HASBORDER,	EVT_BOOL);
    pinfo->mapprops["BorderColor"] = QMainPropInfo(	PROPERTYID_BORDERCOLOR, EVT_COLOR);

    pinfo->mapprops["AutoSize"] = QMainPropInfo( PROPERTYID_AUTOSIZE, EVT_BOOL);		

    pinfo->mapprops["Caption"] = QMainPropInfo( PROPERTYID_CAPTION, EVT_STRING);
    pinfo->mapprops["TextColor"] = QMainPropInfo( PROPERTYID_TEXTCOLOR, EVT_COLOR);
    pinfo->mapprops["LogFont"] = QMainPropInfo( PROPERTYID_LOGFONT, EVT_FONTSET);
    pinfo->mapprops["AlignMode"] = QMainPropInfo( PROPERTYID_ALIGNMODE, EVT_ALIGNMODE);			
    pinfo->mapprops["TextEffect"] = QMainPropInfo( PROPERTYID_TEXT_EFFECT, EVT_STRING);

    pinfo->mapprops["MixSEffect"] = QMainPropInfo( PROPERTYID_MIXSEFFECT, EVT_INT);
    pinfo->mapprops["RGBAlpha"] = QMainPropInfo( PROPERTYID_RGB_ALPHA, EVT_DWORD);
    pinfo->mapprops["VerticalAlignMode"] = QMainPropInfo( PROPERTYID_VERTALIGNMODE, EVT_ENUM);

    pinfo->mapprops["DataSource"] = QMainPropInfo( PROPERTYID_DATASOURCE, EVT_STRING);
    pinfo->mapprops["CustomData"] = QMainPropInfo( PROPERTYID_CUSTOMDATA, EVT_INT);
    pinfo->mapprops["NoFocusWhenLBD"] = QMainPropInfo( PROPERTYID_NOFOCUSWHENLBD, EVT_BOOL);

    pinfo->mapprops["EnableDrag"] = QMainPropInfo( PROPERTYID_ENABLEDRAG, EVT_BOOL);
    pinfo->mapprops["EnableDragIn"] = QMainPropInfo( PROPERTYID_ENABLEDRAGIN, EVT_BOOL);
    pinfo->mapprops["AlwaysOnTop"] = QMainPropInfo(	PROPERTYID_ALWAYSONTOP, EVT_BOOL);
    pinfo->mapprops["AlwaysOnBottom"] = QMainPropInfo(	PROPERTYID_ALWAYSONBOTTOM, EVT_BOOL);
    pinfo->mapprops["DragMode"] = QMainPropInfo( PROPERTYID_DRAGMODE, EVT_ENUM);
    pinfo->mapprops["DragStartMode"] = QMainPropInfo( PROPERTYID_DRAGSTARTMODE, EVT_ENUM);

    pinfo->mapprops["ShowTip"] = QMainPropInfo( PROPERTYID_SHOWTIP, EVT_BOOL);
    pinfo->mapprops["AutoSizeByChildren"] = QMainPropInfo( PROPERTYID_AUTOSIZEBYCHILDREN, EVT_BOOL);
    pinfo->mapprops["AutoSizePostType"] = QMainPropInfo( PROPERTYID_AUTOSIZEPOSTTYPE, EVT_ENUM);
    //anchor 
    pinfo->mapprops["TopLeftAnchor"] = QMainPropInfo(	PROPERTYID_ANCHOR_TOPLEFT, EVT_STRING);
    pinfo->mapprops["TopAnchor"] = QMainPropInfo(	PROPERTYID_ANCHOR_TOP, EVT_STRING);
    pinfo->mapprops["TopRightAnchor"] = QMainPropInfo(	PROPERTYID_ANCHOR_TOPRIGHT, EVT_STRING);
    pinfo->mapprops["LeftAnchor"] = QMainPropInfo(	PROPERTYID_ANCHOR_LEFT, EVT_STRING);
    pinfo->mapprops["CenterAnchor"] = QMainPropInfo(	PROPERTYID_ANCHOR_CENTER, EVT_STRING);
    pinfo->mapprops["RightAnchor"] = QMainPropInfo(	PROPERTYID_ANCHOR_RIGHT, EVT_STRING);
    pinfo->mapprops["BottomLeftAnchor"] = QMainPropInfo(	PROPERTYID_ANCHOR_BOTTOMLEFT, EVT_STRING);
    pinfo->mapprops["BottomAnchor"] = QMainPropInfo(	PROPERTYID_ANCHOR_BOTTOM, EVT_STRING);
    pinfo->mapprops["BottomRightAnchor"] = QMainPropInfo(	PROPERTYID_ANCHOR_BOTTOMRIGHT, EVT_STRING);	
    pinfo->mapprops["Anchor"] = QMainPropInfo(	PROPERTYID_ANCHOR, EVT_BOOL);

    pinfo->mapprops["AnchorRoot"] = QMainPropInfo(	PROPERTYID_ANCHOR_ROOT, EVT_BOOL);
    pinfo->mapprops["WidgetSize"] = QMainPropInfo(	PROPERTYID_ANCHOR_WIDGET_SIZE, EVT_STRING);
    pinfo->mapprops["AnchorBind"] = QMainPropInfo(	PROPERTYID_ANCHOR_BIND, EVT_STRING);

    pinfo->mapprops["LayoutModeCareHide"] = QMainPropInfo( PROPERTYID_LAYOUTMODE_CARE_HIDE, EVT_BOOL);
    pinfo->mapprops["LayoutModeSizeControl"] = QMainPropInfo( PROPERTYID_LAYOUTMODE_SIZECONTROL, EVT_BOOL);

    pinfo->mapprops["CursorPeakLayer"] = QMainPropInfo( PROPERTYID_CURSOR_PEAKLAYER, EVT_BOOL);
    pinfo->mapprops["UserTransMessage"] = QMainPropInfo( PROPERTYID_USERTRANSMESSAGE, EVT_STRING);
    pinfo->mapprops["TransGrayOpen"] = QMainPropInfo(    PROPERTYID_TRANSGRAYOPEN,  EVT_BOOL);
    pinfo->mapprops["TransGrayPath"] = QMainPropInfo(    PROPERTYID_TRANSGRAYPATH,  EVT_TRANSIMAGEPATH);
    pinfo->mapprops["TransImageControlRect"] = QMainPropInfo(   PROPERTYID_TRANSIMAGERECT,  EVT_STRING);
    pinfo->mapprops["PlayDefaultSound"] = QMainPropInfo(   PROPERTYID_PLAYDEFSOUND,  EVT_BOOL);

    //Localization
    pinfo->mapprops["Caption_LID"] = QMainPropInfo(   PROPERTYID_CAPTION,  EVT_LOCALIZATION);
    pinfo->mapprops["LogFont_LID"] = QMainPropInfo(   PROPERTYID_LOGFONT,  EVT_LOCALIZATION);
    pinfo->mapprops["Tip_LID"] = QMainPropInfo(   PROPERTYID_TIP,  EVT_LOCALIZATION);
    pinfo->mapprops["TextTip_LID"] = QMainPropInfo(   PROPERTYID_TEXT_TIP,  EVT_LOCALIZATION);
    pinfo->mapprops["Content_LID"] = QMainPropInfo(   PROPERTYID_CONTENT,  EVT_LOCALIZATION);

    mmaptypes["CommonProperty"] = pinfo;
}

void QMain::InitCommonEventPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(0);
    if(pinfo == NULL)
    {
        return;
    }

	pinfo->mapprops["OnInit"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_INIT);
	pinfo->mapprops["OnDenit"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DENIT);
	pinfo->mapprops["OnRestoreEnd"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RESTORE_END);
	pinfo->mapprops["OnCreate"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_CREATE);
	pinfo->mapprops["OnDelete"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DELETE);
	pinfo->mapprops["OnBeforeShow"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_BEFORESHOW);
	pinfo->mapprops["OnShow"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SHOW);
	pinfo->mapprops["OnHide"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_HIDE);
	pinfo->mapprops["OnMouseHover"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_MOUSEHOVER);
	pinfo->mapprops["OnMouseLeave"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_MOUSELEAVE);
	pinfo->mapprops["OnMouseMove"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_MOUSEMOVE);
	pinfo->mapprops["OnMouseWheel"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_MOUSEWHEEL);
	pinfo->mapprops["OnLButtonDown"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_LBUTTONDOWN);
	pinfo->mapprops["OnLButtonUp"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_LBUTTONUP);
	pinfo->mapprops["OnRButtonDown"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RBUTTONDOWN);
	pinfo->mapprops["OnRButtonUp"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RBUTTONUP);
	pinfo->mapprops["OnDBClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DBCLICK);
	pinfo->mapprops["OnChar"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_CHAR);
	pinfo->mapprops["OnKeyDown"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_KEYDOWN);
	pinfo->mapprops["OnKeyUp"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_KEYUP);
	pinfo->mapprops["OnAccelerator"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_ACCELERATOR);
	pinfo->mapprops["OnSetFocus"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SETFOCUS);
	pinfo->mapprops["OnKillFocus"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_KILLFOCUS);
	pinfo->mapprops["OnCaptureChanged"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_CAPTURECHANGED);
	pinfo->mapprops["OnPosChanged"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_POSCHANGED);
	pinfo->mapprops["OnSignaled"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SIGNALED);
	pinfo->mapprops["OnUnSignaled"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_UNSIGNALED);
	pinfo->mapprops["OnLayerFadeInEnd"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_LAYERFADEIN_END);
	pinfo->mapprops["OnLayerFadeOutEnd"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_LAYERFADEOUT_END);
	pinfo->mapprops["OnDragDrop"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DRAG_DROP);
	pinfo->mapprops["OnDragBegin"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DRAG_BEGIN);
	pinfo->mapprops["OnDragEnter"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DRAG_ENTER);
	pinfo->mapprops["OnDragLeave"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DRAG_LEAVE);
	pinfo->mapprops["OnDragOver"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DRAG_OVER);
	pinfo->mapprops["OnDragCancel"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DRAG_CANCEL);
	pinfo->mapprops["OnDropTest"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DROP_TEST);
	pinfo->mapprops["OnShowToolTip"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SHOWTOOLTIP);

    mmaptypes["CommonEvent"] = pinfo;
}

//////////////////////////////////////////////////////////////////////////
// Init control property map
void QMain::InitBeelinePropertyMap()
{
    //Beeline
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TBEELINE);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["LineColor"] = QMainPropInfo(PROPERTYID_LINECOLOR, EVT_COLOR);
    pinfo->mapprops["LineType"] = QMainPropInfo(PROPERTYID_LINETYPE, EVT_ENUM);
    pinfo->mapprops["IsHoriLine"] = QMainPropInfo(PROPERTYID_ISHORILINE, EVT_BOOL);

    mmaptypes["Beeline"] = pinfo;
}

void QMain::InitBehaviorDesignerPropertyMap()
{
    //BehaviorDesigner
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_BEHAVIORDESIGNER);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["BehaviorParam"] = QMainPropInfo(PROPERTYID_BHVDSGN_PARAM, EVT_BEHAVIOR);
    pinfo->mapprops["BehaviorTime"] = QMainPropInfo(	PROPERTYID_BHVDSGN_TIME, EVT_DWORD);
    pinfo->mapprops["BehaviorObject"] = QMainPropInfo(PROPERTYID_BHVDSGN_OBJECT, EVT_STRING);
    pinfo->mapprops["Action"] = QMainPropInfo(PROPERTYID_BHVDSGN_ACTION, EVT_ENUM);
    pinfo->mapprops["AccelerateTime"] = QMainPropInfo(PROPERTYID_BHVDSGN_ACCTIME, EVT_DWORD);
    pinfo->mapprops["BehaviorMutex"] = QMainPropInfo(PROPERTYID_BHVDSGN_MUTEX, EVT_BOOL);
    pinfo->mapprops["BehaviorCount"] = QMainPropInfo(PROPERTYID_BHVDSGN_COUNT, EVT_INT);

    //event
    pinfo->mapprops["OnBehaviorStart"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_BEHAVIOR_START);
    pinfo->mapprops["OnBehaviorTerminate"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_BEHAVIOR_TERMINATE);

    mmaptypes["BehaviorDesigner"] = pinfo;
}

void QMain::InitButtonPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TBUTTON);
    if(pinfo == NULL)
    {
        return;
    }

    AddStateBgProperties(pinfo);
    // button text 
    pinfo->mapprops["TextFont"] = QMainPropInfo(PROPERTYID_TEXTFONT_NORMAL, EVT_STRING);
    pinfo->mapprops["TextFontHover"] = QMainPropInfo(PROPERTYID_TEXTFONT_HOVER, EVT_STRING);
    pinfo->mapprops["TextFontPressed"] = QMainPropInfo(PROPERTYID_TEXTFONT_PRESSED, EVT_STRING);
    pinfo->mapprops["TextFontDisable"] = QMainPropInfo(PROPERTYID_TEXTFONT_DISABLE, EVT_STRING);

    pinfo->mapprops["TextColorHover"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_HOVER, EVT_COLOR);
    pinfo->mapprops["TextColorPressed"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_PRESSED, EVT_COLOR);
    pinfo->mapprops["TextColorDisable"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_DISABLE, EVT_COLOR);

    pinfo->mapprops["TextEffectHover"] = QMainPropInfo(PROPERTYID_TEXT_EFFECT_HOVER, EVT_STRING);
    pinfo->mapprops["TextEffectPressed"] = QMainPropInfo(PROPERTYID_TEXT_EFFECT_PRESSED, EVT_STRING);
    pinfo->mapprops["TextEffectDisable"] = QMainPropInfo(PROPERTYID_TEXT_EFFECT_DISABLE, EVT_STRING);
    pinfo->mapprops["TextXOffsetHover"] = QMainPropInfo(PROPERTYID_TEXT_XOFFSET_HOVER, EVT_INT);
    pinfo->mapprops["TextYOffsetHover"] = QMainPropInfo(PROPERTYID_TEXT_YOFFSET_HOVER, EVT_INT);
    pinfo->mapprops["TextXOffsetPressed"] = QMainPropInfo(PROPERTYID_TEXT_XOFFSET_PRESSED, EVT_INT);
    pinfo->mapprops["TextYOffsetPressed"] = QMainPropInfo(PROPERTYID_TEXT_YOFFSET_PRESSED, EVT_INT);

    pinfo->mapprops["ButtonType"] = QMainPropInfo(PROPERTYID_BUTTONTYPE, EVT_STRING);
    pinfo->mapprops["ClickSound"] = QMainPropInfo(PROPERTYID_SOUND_CLICK, EVT_STRING);
    pinfo->mapprops["DisableDBlClick"] = QMainPropInfo(PROPERTYID_DISABLEDBlCLICK, EVT_BOOL);
    pinfo->mapprops["ItemMargin"] = QMainPropInfo(PROPERTYID_ITEMMARGIN, EVT_INT);
    pinfo->mapprops["Checked"] = QMainPropInfo(PROPERTYID_SIGNALED, EVT_BOOL);
    pinfo->mapprops["AutoSizeImage"] = QMainPropInfo(PROPERTYID_AUTOSIZE_IMAGE, EVT_BOOL);
    pinfo->mapprops["DefaultButton"] = QMainPropInfo(PROPERTYID_DEFAULTBUTTON, EVT_BOOL);
    pinfo->mapprops["URL"] = QMainPropInfo(PROPERTYID_URL, EVT_STRING);	
    pinfo->mapprops["TextOffsetX"] = QMainPropInfo(PROPERTYID_TEXTOFFSETX, EVT_INT);
    pinfo->mapprops["TextOffsetY"] = QMainPropInfo(PROPERTYID_TEXTOFFSETY, EVT_INT);
    pinfo->mapprops["HoverSound"] = QMainPropInfo(PROPERTYID_SOUND_HOVER, EVT_STRING);
    pinfo->mapprops["VerticalCaption"] = QMainPropInfo(PROPERTYID_VERTICAL, EVT_BOOL);
    pinfo->mapprops["VertInterval"] = QMainPropInfo(PROPERTYID_CHARINTERVAL, EVT_INT);

    pinfo->mapprops["CustomEvent"] = QMainPropInfo(PROPERTYID_CUSTOMEVENT, EVT_STRING);
    pinfo->mapprops["IsLocalURL"] = QMainPropInfo(PROPERTYID_ISLOCALURL, EVT_BOOL);
    pinfo->mapprops["EnableShortcut"] = QMainPropInfo(PROPERTYID_ENABLESHORTCUT, EVT_BOOL);
    pinfo->mapprops["LBDIntervalMsgTime"] = QMainPropInfo(PROPERTYID_LBDINTERVALMSGTIME, EVT_INT);

    //Localization
    pinfo->mapprops["TextFont_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_NORMAL,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontHover_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_HOVER,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontPressed_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_PRESSED,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontDisable_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_DISABLE,  EVT_LOCALIZATION);

    //event
    pinfo->mapprops["OnClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_CLICK);
    pinfo->mapprops["OnLBDIntervalMsg"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_LBDINTERVALMSG);

    mmaptypes["Button"] = pinfo;
}

void QMain::InitTinyButtonPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TINYBUTTON);
    if(pinfo == NULL)
    {
        return;
    }

    AddStateBgProperties(pinfo);
    // button text 
    pinfo->mapprops["TextFont"] = QMainPropInfo(PROPERTYID_TEXTFONT_NORMAL, EVT_STRING);
    pinfo->mapprops["TextFontHover"] = QMainPropInfo(PROPERTYID_TEXTFONT_HOVER, EVT_STRING);
    pinfo->mapprops["TextFontPressed"] = QMainPropInfo(PROPERTYID_TEXTFONT_PRESSED, EVT_STRING);
    pinfo->mapprops["TextFontDisable"] = QMainPropInfo(PROPERTYID_TEXTFONT_DISABLE, EVT_STRING);
    pinfo->mapprops["TextColorHover"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_HOVER, EVT_COLOR);
    pinfo->mapprops["TextColorPressed"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_PRESSED, EVT_COLOR);
    pinfo->mapprops["TextColorDisable"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_DISABLE, EVT_COLOR);

    pinfo->mapprops["AutoSizeImage"] = QMainPropInfo(PROPERTYID_AUTOSIZE_IMAGE, EVT_BOOL);
    pinfo->mapprops["ButtonType"] = QMainPropInfo(PROPERTYID_BUTTONTYPE, EVT_STRING);
    pinfo->mapprops["Checked"] = QMainPropInfo(PROPERTYID_SIGNALED, EVT_BOOL);
    pinfo->mapprops["VerticalCaption"] = QMainPropInfo(PROPERTYID_VERTICAL, EVT_BOOL);
    pinfo->mapprops["VertInterval"] = QMainPropInfo(PROPERTYID_CHARINTERVAL, EVT_INT);
    //Localization
    pinfo->mapprops["TextFont_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_NORMAL,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontHover_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_HOVER,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontPressed_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_PRESSED,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontDisable_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_DISABLE,  EVT_LOCALIZATION);
    //event
    pinfo->mapprops["OnClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_CLICK);

    mmaptypes["TinyButton"] = pinfo;
}

void QMain::InitCardCtrlPropertyMap()
{
    // card control
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TCARDSCTRL);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["CardImage"] = QMainPropInfo(			PROPERTYID_CARDCTRL_CARDIMG,		EVT_STRING);
    pinfo->mapprops["CardImageSpeical"] = QMainPropInfo(	PROPERTYID_CARDCTRL_CARDIMGSPEICAL, EVT_STRING);
    pinfo->mapprops["FoldMode"] = QMainPropInfo(			PROPERTYID_CARDCTRL_FOLDMODE,		EVT_BOOL);
    pinfo->mapprops["Orientation"] = QMainPropInfo(		PROPERTYID_CARDCTRL_ORIENTATION,	EVT_BOOL);
    pinfo->mapprops["CenterX"] = QMainPropInfo(			PROPERTYID_CARDCTRL_CENTER_X,		EVT_DWORD);
    pinfo->mapprops["CenterY"] = QMainPropInfo(			PROPERTYID_CARDCTRL_CENTER_Y,		EVT_DWORD);
    pinfo->mapprops["PopHeight"] = QMainPropInfo(			PROPERTYID_CARDCTRL_POPHEIGHT,		EVT_DWORD);
    pinfo->mapprops["DeltaWidth"] = QMainPropInfo(		PROPERTYID_CARDCTRL_DELTAWIDTH,		EVT_DWORD);
    pinfo->mapprops["SelectColor"] = QMainPropInfo(		PROPERTYID_CARDCTRL_SELECT_COLOR,	EVT_COLOR);
    pinfo->mapprops["SpeicalCards"] = QMainPropInfo(		PROPERTYID_CARDCTRL_SPECAIL_CARDS,	EVT_STRING);
    pinfo->mapprops["SpeicalColor"] = QMainPropInfo(		PROPERTYID_CARDCTRL_SPECAIL_COLOR,	EVT_COLOR);	
    pinfo->mapprops["SpeicalImage"] = QMainPropInfo(		PROPERTYID_CARDCTRL_SPECAIL_IMAGE,	EVT_STRING);
    pinfo->mapprops["MaxNumOfLine"] = QMainPropInfo(		PROPERTYID_CARDCTRL_MAXNUMOFLINE,	EVT_DWORD);	
    pinfo->mapprops["DeltaOfLine"] = QMainPropInfo(		PROPERTYID_CARDCTRL_DELTAOFLINE, EVT_DWORD);	
    pinfo->mapprops["MaxNumOfColumn"] = QMainPropInfo(		PROPERTYID_CARDCTRL_MAXNUMOFCOLUMN,	EVT_DWORD);
    pinfo->mapprops["LineColumnMode"] = QMainPropInfo(		PROPERTYID_CARDCTRL_LINECOLUMNMODE,		EVT_ENUM);
    pinfo->mapprops["DeltaOfColumn"] = QMainPropInfo(		PROPERTYID_CARDCTRL_DELTAOFCOLUMN, EVT_DWORD);	
    pinfo->mapprops["ColorSelected"] = QMainPropInfo(		PROPERTYID_CARDCTRL_SELECTED_COLOR,	EVT_COLOR);
    pinfo->mapprops["ColorNotSelected"] = QMainPropInfo(	PROPERTYID_CARDCTRL_NOTSELECTED_COLOR,	EVT_COLOR);

    mmaptypes["CardCtrl"] = pinfo;
}

void QMain::InitWordCardCtrlPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_WORDCARDSCTRL);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["CardImage"] = QMainPropInfo(			PROPERTYID_WCC_CARDIMG,		EVT_STRING);
    pinfo->mapprops["Mode"] = QMainPropInfo(	PROPERTYID_WCC_MODE, EVT_ENUM);
    pinfo->mapprops["FoldDir"] = QMainPropInfo(			PROPERTYID_WCC_FOLDDIR,		EVT_ENUM);
    pinfo->mapprops["Align"] = QMainPropInfo(		PROPERTYID_WCC_ALIGN,	EVT_ENUM);
    pinfo->mapprops["SelectMode"] = QMainPropInfo(			PROPERTYID_WCC_SELMODE,		EVT_ENUM);
    pinfo->mapprops["FoldDelta"] = QMainPropInfo(			PROPERTYID_WCC_FOLDDELTA,		EVT_INT);
    pinfo->mapprops["ColDelta"] = QMainPropInfo(			PROPERTYID_WCC_COLDELTA,		EVT_INT);
    pinfo->mapprops["AlphaColorChi"] = QMainPropInfo(		PROPERTYID_WCC_ALPHACOLOR_CHI,		EVT_COLOR);
    pinfo->mapprops["AlphaColorHu"] = QMainPropInfo(		PROPERTYID_WCC_ALPHACOLOR_HU,	EVT_COLOR);
    pinfo->mapprops["SelBorderColor"] = QMainPropInfo(		PROPERTYID_WCC_SELBORDER_COLOR,	EVT_COLOR);
    pinfo->mapprops["Content"] = QMainPropInfo(		PROPERTYID_WCC_CONTENT,	EVT_STRING);
    pinfo->mapprops["Lie"] = QMainPropInfo(		PROPERTYID_WCC_LIE,	EVT_BOOL);
    pinfo->mapprops["PopHeight"] = QMainPropInfo(PROPERTYID_WCC_POPHEIGHT, EVT_INT);
    pinfo->mapprops["BeDragAlphaImage"] = QMainPropInfo(			PROPERTYID_WCC_BEDRAGALPHAIMGE,		EVT_STRING);
    pinfo->mapprops["CanDragCursorID"] = QMainPropInfo(			PROPERTYID_WCC_CURSORID_CANDRAG,		EVT_INT);

    mmaptypes["WordCardCtrl"] = pinfo;
}

void QMain::InitCheckBoxPropertyMap()
{
    // checkbox
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TCHECKBOX);
    if(pinfo == NULL)
    {
        return;
    }

    AddStateImageProperties(pinfo);
    pinfo->mapprops["ImageChecked"] = QMainPropInfo(PROPERTYID_IMG_CHECKED, EVT_STRING);
    pinfo->mapprops["ImageHoverChecked"] = QMainPropInfo(PROPERTYID_IMG_HOVERCHECKED, EVT_STRING);
    pinfo->mapprops["ImagePressedChecked"] = QMainPropInfo(PROPERTYID_IMG_PRESSEDCHECKED, EVT_STRING);
    pinfo->mapprops["ImageDisableChecked"] = QMainPropInfo(PROPERTYID_IMG_DISABLECHECKED, EVT_STRING);

    pinfo->mapprops["UnReserved"] = QMainPropInfo(PROPERTYID_UNRESERVED, EVT_BOOL);
    pinfo->mapprops["Checked"] = QMainPropInfo(PROPERTYID_SIGNALED, EVT_BOOL);

    pinfo->mapprops["TextFont"] = QMainPropInfo(PROPERTYID_TEXTFONT_NORMAL, EVT_STRING);
    pinfo->mapprops["TextFontHover"] = QMainPropInfo(PROPERTYID_TEXTFONT_HOVER, EVT_STRING);
    pinfo->mapprops["TextFontPressed"] = QMainPropInfo(PROPERTYID_TEXTFONT_PRESSED, EVT_STRING);
    pinfo->mapprops["TextFontDisable"] = QMainPropInfo(PROPERTYID_TEXTFONT_DISABLE, EVT_STRING);

    pinfo->mapprops["TextColorHover"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_HOVER, EVT_COLOR);
    pinfo->mapprops["TextColorPressed"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_PRESSED, EVT_COLOR);
    pinfo->mapprops["TextColorDisable"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_DISABLE, EVT_COLOR);	

    pinfo->mapprops["TextEffectHover"] = QMainPropInfo(PROPERTYID_TEXT_EFFECT_HOVER, EVT_STRING);
    pinfo->mapprops["TextEffectPressed"] = QMainPropInfo(PROPERTYID_TEXT_EFFECT_PRESSED, EVT_STRING);
    pinfo->mapprops["TextEffectDisable"] = QMainPropInfo(PROPERTYID_TEXT_EFFECT_DISABLE, EVT_STRING);
    //Localization
    pinfo->mapprops["TextFont_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_NORMAL,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontHover_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_HOVER,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontPressed_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_PRESSED,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontDisable_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_DISABLE,  EVT_LOCALIZATION);

    mmaptypes["CheckBox"] = pinfo;
}

void QMain::InitComboBoxPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TCOMBOBOX);
    if(pinfo == NULL)
    {
        return;
    }

    //combobox
    pinfo->mapprops["RightMenu"] = QMainPropInfo(PROPERTYID_EDIT_RIGHTMENU, EVT_STRING);
    pinfo->mapprops["UseDefaultMenu"] = QMainPropInfo(PROPERTYID_EDIT_USEDEFAULTMENU, EVT_BOOL);

    pinfo->mapprops["EditSelectBGColor"] = QMainPropInfo(PROPERTYID_EDITSELECTBGCOLOR, EVT_COLOR);
    pinfo->mapprops["EditSelectTextColor"] = QMainPropInfo(PROPERTYID_EDITSELECTTEXTCOLOR, EVT_COLOR);
    pinfo->mapprops["Droplist"] = QMainPropInfo(PROPERTYID_DROPLIST, EVT_BOOL);
    pinfo->mapprops["ButtonBg"] = QMainPropInfo(	PROPERTYID_BUTTONBG, EVT_STRING);
    pinfo->mapprops["ButtonBgHover"] = QMainPropInfo(PROPERTYID_BUTTONBGHOVER, EVT_STRING);
    pinfo->mapprops["ButtonBgPressed"] = QMainPropInfo(PROPERTYID_BUTTONBGPRESSED, EVT_STRING);
    pinfo->mapprops["ButtonBgDisable"] = QMainPropInfo(PROPERTYID_BUTTONBGDISABLE, EVT_STRING);
    pinfo->mapprops["ListBoxBackground"] = QMainPropInfo(PROPERTYID_LISTBOXBACKGROUND, EVT_STRING);
    pinfo->mapprops["VisibleItemNum"] = QMainPropInfo(PROPERTYID_VISIBLEITEMNUM, EVT_INT);
    pinfo->mapprops["PopupListBox"] = QMainPropInfo(PROPERTYID_POPUPLISTBOX, EVT_BOOL);
    pinfo->mapprops["ButtonWidth"] = QMainPropInfo(PROPERTYID_BUTTONWIDTH, EVT_INT);
    pinfo->mapprops["ButtonHeight"] = QMainPropInfo(PROPERTYID_BUTTONHEIGHT, EVT_INT);
    pinfo->mapprops["ListBoxBorder"] = QMainPropInfo(PROPERTYID_LISTBOXBORDER, EVT_BOOL);
    pinfo->mapprops["PopupDirection"] = QMainPropInfo(PROPERTYID_POPUPDIRECTION, EVT_ENUM);
    pinfo->mapprops["EditXOffset"] = QMainPropInfo(PROPERTYID_EDITXOFFSET, EVT_INT);
    pinfo->mapprops["Number"] = QMainPropInfo(PROPERTYID_NUMBERONLY, EVT_BOOL);
    pinfo->mapprops["MaxCharaters"] = QMainPropInfo(PROPERTYID_MAXCHARACTERS, EVT_INT);
    pinfo->mapprops["CaretColor"] = QMainPropInfo(PROPERTYID_CARETCOLOR, EVT_COLOR);

    pinfo->mapprops["SelectTextColor"] = QMainPropInfo(PROPERTYID_SELECTTEXTCOLOR, EVT_COLOR);
    pinfo->mapprops["HoverTextColor"] = QMainPropInfo(PROPERTYID_HOVERTEXTCOLOR, EVT_COLOR);

    pinfo->mapprops["ItemHeight"] = QMainPropInfo(PROPERTYID_ITEMHEIGHT, EVT_INT);

    pinfo->mapprops["ListBoxXOffset"] = QMainPropInfo(PROPERTYID_LISTBOXXOFFSET, EVT_INT);
    pinfo->mapprops["ListBoxAutoWidth"] = QMainPropInfo(PROPERTYID_LB_AUTOWIDTH, EVT_BOOL);
    pinfo->mapprops["ListBoxMinWidth"] = QMainPropInfo(PROPERTYID_LB_MINWIDTH, EVT_INT);
    pinfo->mapprops["ListBoxMaxWidth"] = QMainPropInfo(PROPERTYID_LB_MAXWIDTH, EVT_INT);
    pinfo->mapprops["ListBoxMargin"] = QMainPropInfo(PROPERTYID_LB_MARGIN, EVT_STRING);
    pinfo->mapprops["ListBoxTextOffset"] = QMainPropInfo(PROPERTYID_LB_TEXTOFFSET, EVT_INT);
    pinfo->mapprops["ScrollBarImagePath"] = QMainPropInfo(PROPERTYID_SB_IMAGEPATH, EVT_STRING);
    pinfo->mapprops["ScrollBarImagePostfixName"] = QMainPropInfo(PROPERTYID_SB_IMAGE_POSTFIX, EVT_STRING);
    pinfo->mapprops["MinThumbLength"] = QMainPropInfo(PROPERTYID_SB_MIN_THUMB_LENGTH, EVT_INT);
    pinfo->mapprops["ScrollBarWidth"] = QMainPropInfo(PROPERTYID_LB_SCROLLBARWIDTH, EVT_INT);
    pinfo->mapprops["SelectBackground"] = QMainPropInfo(PROPERTYID_TBL_SELECTBACKGROUND, EVT_STRING);

    pinfo->mapprops["VScrollImageConfig"] = QMainPropInfo(PROPERTYID_CB_VSCROLL_IMAGECONFIG, EVT_STRING);
    pinfo->mapprops["ThumbStretch"] = QMainPropInfo(PROPERTYID_SB_THUMBDISTORTED, EVT_BOOL);
    pinfo->mapprops["ButtonOffset"] = QMainPropInfo(PROPERTYID_BUTTONOFFSET, EVT_INT);

    //event
    pinfo->mapprops["OnCBSelChanged"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_COMBOBOX_SELCHANGED);
    pinfo->mapprops["OnCBTextChanged"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_COMBOBOX_TEXTCHANGED);
    pinfo->mapprops["OnCBDropDown"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_COMBOBOX_DROPDOWN);
    pinfo->mapprops["OnCBSelectItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_COMBOBOX_SELECTITEM);

    mmaptypes["ComboBox"] = pinfo;
}

void QMain::InitContainerPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_UICONTAINER);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["FrequentUpdate"] = QMainPropInfo(PROPERTYID_FREQUENTUPDATE, EVT_ENUM);
    pinfo->mapprops["CustomDraw"] = QMainPropInfo(PROPERTYID_CUSTOMDRAW, EVT_BOOL);
    pinfo->mapprops["ClassID"] = QMainPropInfo(PROPERTYID_CLASSID, EVT_INT);
    pinfo->mapprops["ZoomDirection"] = QMainPropInfo(PROPERTYID_CONTAINER_ZOOMDIRECTION, EVT_ENUM);
    pinfo->mapprops["OptiGroup"] = QMainPropInfo(PROPERTYID_OPTIGROUP, EVT_BOOL);

    //event
    pinfo->mapprops["OnContainerCustomDraw"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_CONTAINER_CUSTOMDRAW);

    mmaptypes["Container"] = pinfo;
}

void QMain::InitIconboxPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TICONBOX);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["MaskImage"] = QMainPropInfo(PROPERTYID_MASKFILENAME, EVT_STRING);
    pinfo->mapprops["ForgroundImage"] = QMainPropInfo(PROPERTYID_FBFILENAME, EVT_STRING);
    pinfo->mapprops["TopscriptAlign"] = QMainPropInfo(PROPERTYID_TOPSCRIPTALIGN, EVT_ENUM);
    pinfo->mapprops["SubscriptAlign"] = QMainPropInfo(PROPERTYID_SUBSCRIPTALIGN, EVT_ENUM);
    pinfo->mapprops["IconSize"] = QMainPropInfo(PROPERTYID_ICONSIZE, EVT_STRING);
    pinfo->mapprops["IconOpti"] = QMainPropInfo(PROPERTYID_ICONOPTI, EVT_BOOL);

    //event
    pinfo->mapprops["OnIBClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_CLICK);
    pinfo->mapprops["OnIBRClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_RCLICK);

    AddStateBgProperties(pinfo);

    mmaptypes["Iconbox"] = pinfo;
}

void QMain::InitIconbufferPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TICONBUFFER);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["RenderOptimize"] = QMainPropInfo(PROPERTYID_RENDEROPTIMIZE, EVT_BOOL);

    mmaptypes["Iconbuffer"] = pinfo;
}

void QMain::InitIconGridPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TICONGRID);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["MinWidth"] = QMainPropInfo(	PROPERTYID_MINWIDTH,	EVT_INT);
    pinfo->mapprops["MinHeight"] = QMainPropInfo(	PROPERTYID_MINHEIGHT,	EVT_INT);
    pinfo->mapprops["MaxWidth"] = QMainPropInfo(	PROPERTYID_MAXWIDTH,	EVT_INT);
    pinfo->mapprops["MaxHeight"] = QMainPropInfo(	PROPERTYID_MAXHEIGHT,	EVT_INT);
    pinfo->mapprops["IconboxBackground"] = QMainPropInfo(PROPERTYID_IB_BG_IMAGADDRESS, EVT_STRING);
    pinfo->mapprops["RowSpace"] = QMainPropInfo(PROPERTYID_GRIDROWSPACE, EVT_INT);
    pinfo->mapprops["ColSpace"] = QMainPropInfo(PROPERTYID_GRIDCOLSPACE, EVT_INT);
    pinfo->mapprops["IconboxWidth"] = QMainPropInfo(PROPERTYID_IB_WIDTH, EVT_INT);
    pinfo->mapprops["IconboxHeight"] = QMainPropInfo(PROPERTYID_IB_HEIGHT, EVT_INT);
    pinfo->mapprops["Margins"] = QMainPropInfo(PROPERTYID_IG_MARGIN, EVT_STRING);
    pinfo->mapprops["IconboxBGHover"] = QMainPropInfo(PROPERTYID_IB_BG_HOVER, EVT_STRING);
    pinfo->mapprops["IconboxBGPressed"] = QMainPropInfo(PROPERTYID_IB_BG_PRESSED, EVT_STRING);
    pinfo->mapprops["IconboxCount"] = QMainPropInfo(PROPERTYID_IB_COUNT, EVT_INT);
    pinfo->mapprops["Autosize"] = QMainPropInfo(PROPERTYID_IB_AUTOSIZE, EVT_BOOL);
    pinfo->mapprops["IconboxMask"] = QMainPropInfo(PROPERTYID_MASKFILENAME, EVT_STRING);
    pinfo->mapprops["IconboxForground"] = QMainPropInfo(PROPERTYID_FBFILENAME, EVT_STRING);
    pinfo->mapprops["IconSize"] = QMainPropInfo(PROPERTYID_ICONSIZE, EVT_STRING);
    pinfo->mapprops["IBTopscriptAlign"] = QMainPropInfo(PROPERTYID_TOPSCRIPTALIGN, EVT_ENUM);
    pinfo->mapprops["IBSubscriptAlign"] = QMainPropInfo(PROPERTYID_SUBSCRIPTALIGN, EVT_ENUM);
    pinfo->mapprops["RenderOptimize"] = QMainPropInfo(PROPERTYID_RENDEROPTIMIZE, EVT_BOOL);
    pinfo->mapprops["IconboxDragMode"] = QMainPropInfo(PROPERTYID_IB_DRAGMODE, EVT_ENUM);
    pinfo->mapprops["IconboxDragStartMode"] = QMainPropInfo(PROPERTYID_IB_DRAGSTARTMODE, EVT_ENUM);
    pinfo->mapprops["IconboxEnableDragIn"] = QMainPropInfo(PROPERTYID_IB_ENABLEDARGIN, EVT_BOOL);
    pinfo->mapprops["IconboxEnableDrag"] = QMainPropInfo(PROPERTYID_IB_ENABLEDARG, EVT_BOOL);
    pinfo->mapprops["IconOpti"] = QMainPropInfo(PROPERTYID_ICONOPTI, EVT_BOOL);

    //event
    pinfo->mapprops["OnIBLBtnDown"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_LBUTTONDOWN);
    pinfo->mapprops["OnIBLBtnUp"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_LBUTTONUP);
    pinfo->mapprops["OnIBRBtnDown"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_RBUTTONDOWN);
    pinfo->mapprops["OnIBRBtnUp"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_RBUTTONUP);
    pinfo->mapprops["OnIBDBClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_DBCLICK);
    pinfo->mapprops["OnIBDragBegin"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_DRAG_BEGIN);
    pinfo->mapprops["OnIBDragLeave"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_DRAG_LEAVE);
    pinfo->mapprops["OnIBDragOver"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_DRAG_OVER);
    pinfo->mapprops["OnIBDragEnter"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_DRAG_ENTER);
    pinfo->mapprops["OnIBDragDrop"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_DRAG_DROP);
    pinfo->mapprops["OnIBDragCancel"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_DRAG_CANCEL);
    pinfo->mapprops["OnIBDropTest"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_DROP_TEST);
    pinfo->mapprops["OnIBShowToolTip"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_SHOWTOOLTIP);
    pinfo->mapprops["OnIBClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_IB_CLICK);
    pinfo->mapprops["OnIBRClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_IB_RCLICK);
    pinfo->mapprops["OnIBMouseHover"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_MOUSEHOVER);
    pinfo->mapprops["OnIBMouseLeave"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_MOUSELEAVE);
    pinfo->mapprops["OnIBMouseMove"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_MOUSEMOVE);
    pinfo->mapprops["OnIBMouseWheel"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_IB_MOUSEWHEEL);
    pinfo->mapprops["OnIBDragHideCursor"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_HIDE_CURSOR);

    mmaptypes["IconGrid"] = pinfo;
}

void QMain::InitDialogPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TDIALOG);
    if(pinfo == NULL)
    {
        return;
    }

    // dialog
    pinfo->mapprops["RegionSet"] = QMainPropInfo(	PROPERTYID_REGIONSET,	EVT_STRING);
    pinfo->mapprops["SizeBox"] = QMainPropInfo(		PROPERTYID_SIZEBOX,		EVT_BOOL);
    //	pinfo->mapprops["AlwaysOnTop"] = QMainPropInfo(	PROPERTYID_ALWAYSONTOP, EVT_BOOL);
    pinfo->mapprops["ToolWindow"] = QMainPropInfo(	PROPERTYID_TOOLWINDOW,	EVT_BOOL);
    pinfo->mapprops["CaptionHeight"] = QMainPropInfo(	PROPERTYID_CAPTIONHEIGHT,	EVT_INT);
    pinfo->mapprops["MinWidth"] = QMainPropInfo(		PROPERTYID_MINWIDTH,	EVT_INT);
    pinfo->mapprops["MinHeight"] = QMainPropInfo(	PROPERTYID_MINHEIGHT,	EVT_INT);
    pinfo->mapprops["MaxWidth"] = QMainPropInfo(		PROPERTYID_MAXWIDTH,	EVT_INT);
    pinfo->mapprops["MaxHeight"] = QMainPropInfo(	PROPERTYID_MAXHEIGHT,	EVT_INT);
    pinfo->mapprops["Icon"] = QMainPropInfo(			PROPERTYID_ICON,		EVT_STRING);
    pinfo->mapprops["InitPosition"] = QMainPropInfo(PROPERTYID_INITPOSITION,EVT_STRING);
    pinfo->mapprops["LayeredWindow"] = QMainPropInfo(PROPERTYID_LAYEREDWINDOW,EVT_BOOL);
    pinfo->mapprops["EnableMini"] = QMainPropInfo(	PROPERTYID_ENABLEMINI, EVT_BOOL);
    pinfo->mapprops["EnableMaxi"] = QMainPropInfo(	PROPERTYID_ENABLEMAXI, EVT_BOOL);
    pinfo->mapprops["TaskMenu"] = QMainPropInfo(		PROPERTYID_TASKMENU, EVT_BOOL);
    pinfo->mapprops["DisableMoveOut"] = QMainPropInfo(	PROPERTYID_DISABLEMOVEOUT,	EVT_BOOL);
    pinfo->mapprops["MoveMarginLeft"] = QMainPropInfo(	PROPERTYID_DLGMOVEMARING_LEFT,	EVT_INT);
    pinfo->mapprops["MoveMarginTop"] = QMainPropInfo(	PROPERTYID_DLGMOVEMARING_TOP,	EVT_INT);
    pinfo->mapprops["MoveMarginRight"] = QMainPropInfo(	PROPERTYID_DLGMOVEMARING_RIGHT,	EVT_INT);
    pinfo->mapprops["MoveMarginBottom"] = QMainPropInfo(	PROPERTYID_DLGMOVEMARING_BOTTOM,	EVT_INT);
    pinfo->mapprops["NoActive"] = QMainPropInfo(PROPERTYID_NOACTIVE, EVT_BOOL);
    pinfo->mapprops["NoIME"] = QMainPropInfo(PROPERTYID_NOIME, EVT_BOOL);
    pinfo->mapprops["OpenSound"] = QMainPropInfo(PROPERTYID_SOUND_OPEN, EVT_STRING);
    pinfo->mapprops["MoveWith"] = QMainPropInfo(PROPERTYID_MOVEWITH, EVT_STRING);
    pinfo->mapprops["Dock"] = QMainPropInfo(PROPERTYID_DIALOGDOCK, EVT_ENUM);
    pinfo->mapprops["ExtraDragRect"] = QMainPropInfo(PROPERTYID_EXTRADRAGRECT, EVT_STRING);
    pinfo->mapprops["ModalMaskBG"] = QMainPropInfo(PROPERTYID_MODALMASKBG, EVT_STRING);
    pinfo->mapprops["OptiGroup"] = QMainPropInfo(PROPERTYID_OPTIGROUP, EVT_BOOL);

    //event
    pinfo->mapprops["OnDialogClose"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DIALOG_CLOSE);
    pinfo->mapprops["OnDialogMinPos"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DIALOG_MIN_POS);
    pinfo->mapprops["OnDialogMinimize"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DIALOG_MINIMIZE);
    pinfo->mapprops["OnDialogRestore"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DIALOG_RESTORE);
    pinfo->mapprops["OnDialogMaximize"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_DIALOG_MAXIMIZE);

    mmaptypes["Dialog"] = pinfo;
}

void QMain::InitDigitPicBoxPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_DIGITPIC);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["DigitImage"] = QMainPropInfo(	PROPERTYID_IDP_DIGITIMAGE,	EVT_STRING);
    pinfo->mapprops["Number"] = QMainPropInfo(		PROPERTYID_IDP_NUMBER,		EVT_INT);
    pinfo->mapprops["DigitNumber"] = QMainPropInfo(	PROPERTYID_IDP_DIGITNUMBER,	EVT_DWORD);
    pinfo->mapprops["DigitSpace"] = QMainPropInfo(	PROPERTYID_IDP_DIGITSPACE,	EVT_DWORD);
    pinfo->mapprops["ZeroShow"] = QMainPropInfo(		PROPERTYID_IDP_ZEROSHOW,	EVT_BOOL);

    mmaptypes["DigitPicBox"] = pinfo;
}

void QMain::InitEditPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TEDIT);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["MaxLineCount"] = QMainPropInfo(PROPERTYID_MAXLINECOUNT, EVT_INT);
    pinfo->mapprops["MaxCharaters"] = QMainPropInfo(PROPERTYID_MAXCHARACTERS, EVT_INT);
    pinfo->mapprops["CaretColor"] = QMainPropInfo(PROPERTYID_CARETCOLOR, EVT_COLOR);
    pinfo->mapprops["Number"] = QMainPropInfo(PROPERTYID_NUMBERONLY, EVT_BOOL);
    pinfo->mapprops["RightMenu"] = QMainPropInfo(PROPERTYID_EDIT_RIGHTMENU, EVT_STRING);
    pinfo->mapprops["UseDefaultMenu"] = QMainPropInfo(PROPERTYID_EDIT_USEDEFAULTMENU, EVT_BOOL);
    pinfo->mapprops["Password"] = QMainPropInfo(PROPERTYID_PASSWORD, EVT_BOOL);
    pinfo->mapprops["EditMargin"] = QMainPropInfo(PROPERTYID_EDIT_MARGIN, EVT_STRING);
    pinfo->mapprops["EditSelectBGColor"] = QMainPropInfo(PROPERTYID_EDITSELECTBGCOLOR, EVT_COLOR);
    pinfo->mapprops["EditSelectTextColor"] = QMainPropInfo(PROPERTYID_EDITSELECTTEXTCOLOR, EVT_COLOR);
    pinfo->mapprops["ReadOnly"] = QMainPropInfo(PROPERTYID_EDIT_READONLY, EVT_BOOL);
    pinfo->mapprops["CareSpecialChar"] = QMainPropInfo(PROPERTYID_CARESPECIALCHAR, EVT_BOOL);
    pinfo->mapprops["SelectAllWhenLBD"] = QMainPropInfo(PROPERTYID_EDIT_BSELECTALLWHENLBD, EVT_BOOL);

    pinfo->mapprops["CaretBlinkTime"] = QMainPropInfo(PROPERTYID_CARETBLINKTIME, EVT_INT);
    pinfo->mapprops["CaretWidth"] = QMainPropInfo(PROPERTYID_CARETWIDTH, EVT_INT);
    AddMultiLineTextProperties(pinfo);

    //event
    pinfo->mapprops["OnEditTextChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_EDIT_TEXT_CHANGE);
    pinfo->mapprops["OnSCScrollStepChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SC_SCROLLSTEPCHANGE);
    pinfo->mapprops["OnSCDocOffSetChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SC_DOCUMENTOFFSET_CHANGE);
    pinfo->mapprops["OnSCDocChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SC_DOCUMENT_CHANGE);

    mmaptypes["Edit"] = pinfo;
}

void QMain::InitExcluderPropertyMap()
{
    // Excluder
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_UIEXCLUDER);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["ExcludeItemList"] = QMainPropInfo(	PROPERTYID_EXCLUDEITEMLIST, EVT_STRING);

    mmaptypes["Excluder"] = pinfo;
}

void QMain::InitHeaderPropertyMap()
{
    //Header
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_THEADER);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["ItemMargin"] = QMainPropInfo(PROPERTYID_ITEMMARGIN, EVT_INT);
    pinfo->mapprops["Divider"] = QMainPropInfo(PROPERTYID_HDR_DIVIDER, EVT_STRING);
    pinfo->mapprops["DividerHover"] = QMainPropInfo(PROPERTYID_HDR_DIVIDERHOVER, EVT_STRING);
    pinfo->mapprops["DividerPressed"] = QMainPropInfo(PROPERTYID_HDR_DIVIDERPRESSED, EVT_STRING);
    pinfo->mapprops["DividerDisable"] = QMainPropInfo(PROPERTYID_HDR_DIVIDERDISABLE, EVT_STRING);
    pinfo->mapprops["HeaderInitParam"] = QMainPropInfo(PROPERTYID_HDR_INITPARAM, EVT_STRING);

    AddStateBgProperties(pinfo);

    //event
    pinfo->mapprops["OnHeaderButtonDBClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_HEADER_BUTTONDBCLICK);
    pinfo->mapprops["OnHeaderButtonDown"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_HEADER_BUTTONDOWN);
    pinfo->mapprops["OnHeaderButtonUp"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_HEADER_BUTTONUP);
    pinfo->mapprops["OnHeaderItemLenChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_HEADER_ITEMLENCHANGE);
    pinfo->mapprops["OnHeaderMouseMove"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_HEADER_MOUSEMOVE);
    pinfo->mapprops["OnHeaderButtonClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_HEADER_BUTTONCLICK);
    pinfo->mapprops["OnHeaderContentResize"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_HEADER_CONTENTRESIZE);

    //Localization
    pinfo->mapprops["HeaderInitParam_LID"] = QMainPropInfo(  PROPERTYID_HDR_INITPARAM,  EVT_LOCALIZATION);

    mmaptypes["Header"] = pinfo;
}

void QMain::InitWin32ImageListPropertyMap()
{
    // ImageList
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TIMAGELIST);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["ImageWidth"] = QMainPropInfo(PROPERTYID_IL_IMAGEWIDTH, EVT_INT);
    pinfo->mapprops["ImageHeight"] = QMainPropInfo(PROPERTYID_IL_IMAGEHEIGHT, EVT_INT);
    pinfo->mapprops["Images"] = QMainPropInfo(PROPERTYID_IL_IMAGES, EVT_STRING);

    mmaptypes["Win32ImageList"] = pinfo;
}

void QMain::InitTPFImageListPropertyMap()
{
    // ImageList
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TTPFIMAGELIST);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["Images"] = QMainPropInfo(PROPERTYID_IL_IMAGES, EVT_STRING);

    mmaptypes["TPFImageList"] = pinfo;
}

void QMain::InitLabelPropertyMap()
{
    //Label
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TLABEL);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["LabelType"] = QMainPropInfo(PROPERTYID_LBL_TYPE, EVT_STRING);
    pinfo->mapprops["IconAddress"] = QMainPropInfo(PROPERTYID_LBL_ICONADDRESS, EVT_STRING);
    pinfo->mapprops["IconSize"] = QMainPropInfo(PROPERTYID_ICONSIZE, EVT_STRING);
    pinfo->mapprops["Interval"] = QMainPropInfo(PROPERTYID_LBL_INTERVAL, EVT_INT);
    pinfo->mapprops["AutoSize"] = QMainPropInfo(PROPERTYID_AUTOSIZE, EVT_BOOL);
    pinfo->mapprops["CareSpecialChar"] = QMainPropInfo(PROPERTYID_CARESPECIALCHAR, EVT_BOOL);
    pinfo->mapprops["TextVertAlign"] = QMainPropInfo(PROPERTYID_TEXTVERTALIGN, EVT_ENUM);

    AddMultiLineTextProperties(pinfo);

    mmaptypes["Label"] = pinfo;
}

void QMain::InitLevelPropertyMap()
{
    //Level
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TLEVEL);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["LevelTopImage"] = QMainPropInfo(PROPERTYID_LEVELTOPIMAGE, EVT_STRING);
    pinfo->mapprops["LevelBottomImage"] = QMainPropInfo(PROPERTYID_LEVELBOTTOMIMAGE, EVT_STRING);
    pinfo->mapprops["LevelHeight"] = QMainPropInfo(PROPERTYID_LEVELHEIGHT, EVT_INT);
    pinfo->mapprops["LevelWidth"] = QMainPropInfo(PROPERTYID_LEVELWIDTH, EVT_INT);
    pinfo->mapprops["LevelTextVisible"] = QMainPropInfo(PROPERTYID_TEXTVISIBLE, EVT_BOOL);
    pinfo->mapprops["TextAlign"] = QMainPropInfo(PROPERTYID_LEVELTEXTALIGN, EVT_ENUM);
    pinfo->mapprops["LevelDrawMode"] = QMainPropInfo(PROPERTYID_LEVELDRAWMODE, EVT_ENUM);
    pinfo->mapprops["MoveTopImage"] = QMainPropInfo(PROPERTYID_LEVEL_MOVETOPIMAGE, EVT_BOOL);
    pinfo->mapprops["LevelShineImage"] = QMainPropInfo(PROPERTYID_LEVELMOVEIMAGE, EVT_STRING);
    pinfo->mapprops["ShineSpeed"] = QMainPropInfo(PROPERTYID_LEVELMOVESPEED, EVT_INT);
    pinfo->mapprops["MoveStartOffset"] = QMainPropInfo(PROPERTYID_LEVELMOVESTARTOFFSET, EVT_INT);
    pinfo->mapprops["MoveEndOffset"] = QMainPropInfo(PROPERTYID_LEVELMOVEENDOFFSET, EVT_INT);
    AddRangeProperties(pinfo);		

    mmaptypes["Level"] = pinfo;
}

void QMain::InitListBoxPropertyMap()
{
    // ListBox
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TLISTBOX);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["SelectTextColor"] = QMainPropInfo(PROPERTYID_SELECTTEXTCOLOR, EVT_COLOR);
    pinfo->mapprops["HoverTextColor"] = QMainPropInfo(PROPERTYID_HOVERTEXTCOLOR, EVT_COLOR);

    pinfo->mapprops["ItemHeight"] = QMainPropInfo(PROPERTYID_ITEMHEIGHT, EVT_INT);
    pinfo->mapprops["ListBoxAutoWidth"] = QMainPropInfo(PROPERTYID_LB_AUTOWIDTH, EVT_BOOL);
    pinfo->mapprops["ListBoxMinWidth"] = QMainPropInfo(PROPERTYID_LB_MINWIDTH, EVT_INT);
    pinfo->mapprops["ListBoxMaxWidth"] = QMainPropInfo(PROPERTYID_LB_MAXWIDTH, EVT_INT);
    pinfo->mapprops["ListBoxMargin"] = QMainPropInfo(PROPERTYID_LB_MARGIN, EVT_STRING);
    pinfo->mapprops["ListBoxTextOffset"] = QMainPropInfo(PROPERTYID_LB_TEXTOFFSET, EVT_INT);
    pinfo->mapprops["ScrollBarImagePath"] = QMainPropInfo(PROPERTYID_SB_IMAGEPATH, EVT_STRING);
    pinfo->mapprops["ScrollBarImagePostfixName"] = QMainPropInfo(PROPERTYID_SB_IMAGE_POSTFIX, EVT_STRING);
    pinfo->mapprops["MinThumbLength"] = QMainPropInfo(PROPERTYID_SB_MIN_THUMB_LENGTH, EVT_INT);
    pinfo->mapprops["ScrollBarWidth"] = QMainPropInfo(PROPERTYID_LB_SCROLLBARWIDTH, EVT_INT);
    pinfo->mapprops["SelectBackground"] = QMainPropInfo(PROPERTYID_TBL_SELECTBACKGROUND, EVT_STRING);
    pinfo->mapprops["VScrollImageConfig"] = QMainPropInfo(PROPERTYID_LB_VSCROLL_IMAGECONFIG, EVT_STRING);
    pinfo->mapprops["ThumbStretch"] = QMainPropInfo(PROPERTYID_SB_THUMBDISTORTED, EVT_BOOL);
    pinfo->mapprops["TextVertAlign"] = QMainPropInfo(PROPERTYID_TEXTVERTALIGN, EVT_ENUM);
    pinfo->mapprops["HoverBackground"] = QMainPropInfo(PROPERTYID_TBL_HOVERBACKGROUND, EVT_STRING);
    pinfo->mapprops["ChangeSelColor"] = QMainPropInfo(PROPERTYID_LB_CHANGESELCOLOR, EVT_BOOL);
    pinfo->mapprops["EnableKey"] = QMainPropInfo(PROPERTYID_LB_ENABLEKEY, EVT_BOOL);
    //event
    pinfo->mapprops["OnLBoxSelectItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_LB_SELECTITEM);
    pinfo->mapprops["OnLBoxRClickItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_LB_RCLICKITEM);
    pinfo->mapprops["OnLBoxHoverItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_LB_HOVERITEM);

    mmaptypes["ListBox"] = pinfo;
}

void QMain::InitListViewPropertyMap()
{
    //ListView
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TLISTVIEW);
    if(pinfo == NULL)
    {
        return;
    }

    //pinfo->mapprops["RightMargin"] = QMainPropInfo(PROPERTYID_LV_RIGHTMARGIN, EVT_INT);//已弃用
    pinfo->mapprops["ToTableMargin"] = QMainPropInfo(PROPERTYID_LV_TOTABLEMARGIN, EVT_STRING);

    mmaptypes["ListView"] = pinfo;
}

void QMain::InitMenuPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TMENU);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["RegionSet"] = QMainPropInfo(	PROPERTYID_REGIONSET,	EVT_STRING);
    pinfo->mapprops["MenuMargin"] = QMainPropInfo(PROPERTYID_MENU_MARGIN, EVT_STRING);
    pinfo->mapprops["ItemHeight"] = QMainPropInfo(PROPERTYID_MENU_ITEM_HEIGHT, EVT_INT);
    pinfo->mapprops["IconRect"] = QMainPropInfo(PROPERTYID_MENU_ICON_RECT, EVT_STRING);
    pinfo->mapprops["TextRect"] = QMainPropInfo(PROPERTYID_MENU_TEXT_RECT, EVT_STRING);
    pinfo->mapprops["ItemBgNormal"] = QMainPropInfo(PROPERTYID_MENU_ITEM_BG_NORMAL, EVT_STRING);
    pinfo->mapprops["ItemBgHover"] = QMainPropInfo(PROPERTYID_MENU_ITEM_BG_HOVER, EVT_STRING);
    pinfo->mapprops["ItemBgDisable"] = QMainPropInfo(PROPERTYID_MENU_ITEM_BG_DISABLE, EVT_STRING);
    pinfo->mapprops["ItemHasSubBgNormal"] = QMainPropInfo(PROPERTYID_MENU_ITEM_HAS_SUB_BG_NORMAL, EVT_STRING);
    pinfo->mapprops["ItemHasSubBgHover"] = QMainPropInfo(PROPERTYID_MENU_ITEM_HAS_SUB_BG_HOVER, EVT_STRING);
    pinfo->mapprops["ItemHasSubBgDisable"] = QMainPropInfo(PROPERTYID_MENU_ITEM_HAS_SUB_BG_DISABLE, EVT_STRING);
    pinfo->mapprops["ItemTextFontNormal"] = QMainPropInfo(PROPERTYID_MENU_ITEM_TEXTFONT_NORMAL, EVT_STRING);
    pinfo->mapprops["ItemTextFontHover"] = QMainPropInfo(PROPERTYID_MENU_ITEM_TEXTFONT_HOVER, EVT_STRING);
    pinfo->mapprops["ItemTextFontDisable"] = QMainPropInfo(PROPERTYID_MENU_ITEM_TEXTFONT_DISABLE, EVT_STRING);
    pinfo->mapprops["ItemTextColorNormal"] = QMainPropInfo(PROPERTYID_MENU_ITEM_TEXTCOLOR_NORMAL, EVT_COLOR);
    pinfo->mapprops["ItemTextColorHover"] = QMainPropInfo(PROPERTYID_MENU_ITEM_TEXTCOLOR_HOVER, EVT_COLOR);
    pinfo->mapprops["ItemTextColorDisable"] = QMainPropInfo(PROPERTYID_MENU_ITEM_TEXTCOLOR_DISABLE, EVT_COLOR);
    pinfo->mapprops["ItemSeparatorBg"] = QMainPropInfo(PROPERTYID_MENU_ITEM_SEPARATOR_BG, EVT_STRING);
    pinfo->mapprops["ItemAlignMode"] = QMainPropInfo(PROPERTYID_MENU_ITEM_TEXT_ALIGNMODE, EVT_ALIGNMODE);
    pinfo->mapprops["ItemSeparatorHeight"] = QMainPropInfo(PROPERTYID_MENU_SEPARATOR_HEIGHT, EVT_INT);
    pinfo->mapprops["SubMenuPopupDirection"] = QMainPropInfo(PROPERTYID_MENU_POPUPDIRECTION, EVT_ENUM);
    pinfo->mapprops["SubMenuOverlapWidth"] = QMainPropInfo(PROPERTYID_MENU_SUBMENULAPOVERWIDTH, EVT_INT);
    pinfo->mapprops["MinWidth"] = QMainPropInfo(PROPERTYID_MENU_MINWIDTH, EVT_INT);
    pinfo->mapprops["ItemSet"] = QMainPropInfo(PRPPERTYID_MENU_ITEMSET, EVT_STRING);
    pinfo->mapprops["EnableKey"] = QMainPropInfo(PROPERTYID_LB_ENABLEKEY, EVT_BOOL);
    pinfo->mapprops["CustomEvent"] = QMainPropInfo(PROPERTYID_CUSTOMEVENT, EVT_STRING);

    //Localization
    pinfo->mapprops["ItemTextFontNormal_LID"] = QMainPropInfo(  PROPERTYID_MENU_ITEM_TEXTFONT_NORMAL,  EVT_LOCALIZATION);
    pinfo->mapprops["ItemTextFontHover_LID"] = QMainPropInfo(  PROPERTYID_MENU_ITEM_TEXTFONT_HOVER,  EVT_LOCALIZATION);
    pinfo->mapprops["ItemTextFontDisable_LID"] = QMainPropInfo(  PROPERTYID_MENU_ITEM_TEXTFONT_DISABLE,  EVT_LOCALIZATION);
    pinfo->mapprops["ItemSet_LID"] = QMainPropInfo(  PRPPERTYID_MENU_ITEMSET,  EVT_LOCALIZATION);

    //event
    pinfo->mapprops["OnMenuClickItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_MENU_CLICKITEM);
    pinfo->mapprops["OnMenuBeforeShowSubMenu"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_MENU_BEFORESHOWSUBMENU);

    mmaptypes["Menu"] = pinfo;
}

void QMain::InitMultiColorCtrlPropertyMap()
{
    // MultiColorLayer
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TMULTICOLORCTRL);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["MCVerticalGap"] = QMainPropInfo(PROPERTYID_MCL_VERTGAP, EVT_INT);
    pinfo->mapprops["MCHorizontalGap"] = QMainPropInfo(PROPERTYID_MCL_HORIGAP, EVT_INT);
    pinfo->mapprops["MCColorBlockWidth"] = QMainPropInfo(PROPERTYID_MCL_COLORBLOCK_WIDTH, EVT_INT);
    pinfo->mapprops["MCColorBlockHeight"] = QMainPropInfo(PROPERTYID_MCL_COLORBLOCK_HEIGHT, EVT_INT);
    pinfo->mapprops["MCColorBlockHasBorder"] = QMainPropInfo(PROPERTYID_MCL_COLORBLOCK_HASBORDER, EVT_BOOL);
    pinfo->mapprops["MCColorBlockBorderColor"] = QMainPropInfo(PROPERTYID_MCL_COLORBLOCK_BORDERCOLOR, EVT_COLOR);
    pinfo->mapprops["MCColorBlocksProperty"] = QMainPropInfo(PROPERTYID_MCL_COLORBLOCKSPROPERTY, EVT_MULTICOLORSET);

    mmaptypes["MultiColorCtrl"] = pinfo;
}

void QMain::InitMultiImageVerminPropertyMap()
{
    //MultiImageVermin
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TMULTIIMAGEVERMIN);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["Depositor"] = QMainPropInfo(PROPERTYID_MIV_DEPOSITOR, EVT_STRING);
    pinfo->mapprops["AutoLoop"] = QMainPropInfo(PROPERTYID_MIV_AUTOLOOP, EVT_BOOL);
    pinfo->mapprops["CurrentFrame"] = QMainPropInfo(PROPERTYID_MIV_CURFRAME, EVT_INT);
    AddStateImageProperties(pinfo);

    //event
    pinfo->mapprops["OnMultiImageVerminAnimate"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_MULTI_IMAGE_VERMIN_ANIMATE);

    mmaptypes["MultiImageVermin"] = pinfo;
}

void QMain::InitMultiImageCtrlPropertyMap()
{
    // MultiImageControl
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TMULTIIMAGECTRL);
    if(pinfo == NULL)
    {
        return;
    }

    pinfo->mapprops["MIImageProperty"] = QMainPropInfo(PROPERTYID_MI_IMAGEPROPERTY, EVT_MULTIIMAGESET);
    pinfo->mapprops["MIImageWidth"] = QMainPropInfo(PROPERTYID_MI_IMAGEWIDTH, EVT_INT);
    pinfo->mapprops["MIImageHeight"] = QMainPropInfo(PROPERTYID_MI_IMAGEHEIGHT, EVT_INT);
    pinfo->mapprops["MIHasSelBorder"] = QMainPropInfo(PROPERTYID_MI_HASSELBODER, EVT_BOOL);
    pinfo->mapprops["MISelBorderColor"] = QMainPropInfo(PROPERTYID_MI_SELBORDERCOLOR, EVT_COLOR);
    pinfo->mapprops["MIBorderOffset"] = QMainPropInfo(PROPERTYID_MI_BORDEROFFSET, EVT_INT);
    pinfo->mapprops["MIMaxSelectIndex"] = QMainPropInfo(PROPERTYID_MI_MAXSELINDEX, EVT_INT);
    pinfo->mapprops["MIImagePaintMode"] = QMainPropInfo(PROPERTYID_MI_IMAGES_PAINTMODE, EVT_STRING);
    AddGridProperties(pinfo);

    //event
    pinfo->mapprops["OnMISelectImage"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TMULTIIMAGE_SELECTIMAGE);
    pinfo->mapprops["OnMISelectChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TMULTIIMAGE_SELECTCHANGE);

    mmaptypes["MultiImageCtrl"] = pinfo;
}

void QMain::InitOverlapContainerPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_UIOVERLAPCONTAINER);
    if(pinfo == NULL)
    {
        return;
    }
    
    pinfo->mapprops["CurrentSelect"] = QMainPropInfo(PROPERTYID_OVERLAPSELECTITEM, EVT_STRING);

    mmaptypes["MultiImageCtrl"] = pinfo;
}

void QMain::InitRichEditPropertyMap()
{
    //RichEdit
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TRICHEDIT);
    if(pinfo == NULL)
    {
        return;
    }
    AddMultiLineTextProperties(pinfo);
    pinfo->mapprops["MaxLineCount"] = QMainPropInfo(PROPERTYID_RE_MAXLINECOUNT, EVT_INT);
    pinfo->mapprops["DefaultSize"] = QMainPropInfo(PROPERTYID_RE_DEFAULTSIZE, EVT_INT);
    pinfo->mapprops["DefaultColor"] = QMainPropInfo(PROPERTYID_RE_DEFAULTCOLOR, EVT_COLOR);
    pinfo->mapprops["LinkColor"] = QMainPropInfo(PROPERTYID_RE_LINKCOLOR, EVT_COLOR);
    pinfo->mapprops["LinkHoverColor"] = QMainPropInfo(PROPERTYID_RE_LINKHOVERCOLOR, EVT_COLOR);
    pinfo->mapprops["Margin"] = QMainPropInfo(PROPERTYID_RE_MARGIN, EVT_STRING);
    pinfo->mapprops["OneByOneShowTime"] = QMainPropInfo(PRPPERTYID_RE_ONEBYONESHOWTIME, EVT_DWORD);
    pinfo->mapprops["SelectEnable"] = QMainPropInfo(EI_RE_SELECTENABLE, EVT_BOOL);
    pinfo->mapprops["RichEditMinWidth"] = QMainPropInfo(PROPERTYID_RE_MINWIDTH, EVT_INT);
    pinfo->mapprops["RichEditMaxWidth"] = QMainPropInfo(PROPERTYID_MAXWIDTH, EVT_INT);
    pinfo->mapprops["MinHeight"] = QMainPropInfo(PROPERTYID_RE_MINHEIGHT, EVT_INT);
    pinfo->mapprops["LineChangeIndent"] = QMainPropInfo(PROPERTYID_RE_LINECHANGEINDENT, EVT_INT);
    pinfo->mapprops["ReAlignMode"] = QMainPropInfo(PROPERTYID_RE_ALIGNMODE, EVT_ENUM);
    pinfo->mapprops["UseTextCaption"] = QMainPropInfo(PROPERTYID_USETEXTCAPTION, EVT_BOOL);
    pinfo->mapprops["DefaultFormat"] = QMainPropInfo(PROPERTYID_DEFAULTFORMAT, EVT_STRING);
    pinfo->mapprops["ShowClipContent"] = QMainPropInfo(PROPERTYID_SHOWCLIPCONTENT, EVT_INT);
    pinfo->mapprops["AutoScrollScreen"] = QMainPropInfo(PROPERTYID_RE_AUTOSCROLLSCREEN, EVT_BOOL);
    pinfo->mapprops["StopOBOSbyLBD"] = QMainPropInfo(PROPERTYID_RE_STOPOBOSBYLBD, EVT_BOOL);

    //event
    pinfo->mapprops["OnREClickHyperLink"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RE_CLICKHYPERLINK);
    pinfo->mapprops["OnREDBClickHyperLink"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RE_DBCLICKHYPERLINK);
    pinfo->mapprops["OnRERClickHyperLink"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RE_RCLICKHYPERLINK);
    pinfo->mapprops["OnREAutoDeleteHyperLink"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RE_AUTODELETEHYPERLINK);
    pinfo->mapprops["OnREScrollFinish"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RE_SCROLL_FINISH);
    pinfo->mapprops["OnREOneByOneShowFinish"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RE_ONEBYONESHOW_FINISH);
    pinfo->mapprops["OnREOffsetChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RE_OFFSETCHANGE);

    mmaptypes["RichEdit"] = pinfo;
}

void QMain::InitRichEdit2PropertyMap()
{
    //RichEdit2
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_RICHEDIT2);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["EditMargin"] = QMainPropInfo(PROPERTYID_RE2_MARGIN, EVT_STRING);
    pinfo->mapprops["CaretColor"] = QMainPropInfo(PROPERTYID_CARETCOLOR, EVT_COLOR);
    pinfo->mapprops["CaretSelColor"] = QMainPropInfo(PROPERTYID_RE2_CARETSELCOLOR, EVT_COLOR);
    pinfo->mapprops["CaretBlinkTime"] = QMainPropInfo(PROPERTYID_RE2_CARETBLINKTIME, EVT_INT);
    pinfo->mapprops["CaretWidth"] = QMainPropInfo(PROPERTYID_CARETWIDTH, EVT_INT);
    pinfo->mapprops["SelectTextColor"] = QMainPropInfo(PROPERTYID_RE2_TEXTSELCOLOR, EVT_COLOR);
    pinfo->mapprops["SelectTextBGColor"] = QMainPropInfo(PROPERTYID_RE2_TEXTSELBGCOLOR, EVT_COLOR);
    pinfo->mapprops["SelectHyperLinkColor"] = QMainPropInfo(PROPERTYID_RE2_HLSELCOLOR, EVT_COLOR);
    pinfo->mapprops["HyperLinkColor"] = QMainPropInfo(PROPERTYID_RE2_HYPERLINKCOLOR, EVT_COLOR);
    pinfo->mapprops["HyperLinkHoverColor"] = QMainPropInfo(PROPERTYID_RE2_HLHOVERCOLOR, EVT_COLOR);
    pinfo->mapprops["LineSpacing"] = QMainPropInfo(PROPERTYID_RE2_ROWSPACE, EVT_INT);
    pinfo->mapprops["UseTextCaption"] = QMainPropInfo(PROPERTYID_RE2_USECAPTION, EVT_BOOL);
    pinfo->mapprops["DefaultRowHeight"] = QMainPropInfo(PROPERTYID_RE2_DEFAULTROWHEIGHT, EVT_INT);
    pinfo->mapprops["AutoBottom"] = QMainPropInfo(PROPERTYID_RE2_AUTOBOTTOM, EVT_BOOL);
    pinfo->mapprops["MultiLine"] = QMainPropInfo(PROPERTYID_RE2_MULITLINE, EVT_BOOL);
    pinfo->mapprops["ClickTransparentToText"] = QMainPropInfo(PROPERTYID_RE2_CLICKTRANSPARENT2TEXT, EVT_BOOL);
    pinfo->mapprops["InputMaxCharacters"] = QMainPropInfo(PROPERTYID_MAXCHARACTERS, EVT_INT);
    pinfo->mapprops["HyperLinkEditable"] = QMainPropInfo(PROPERTYID_RE2_HLEDITABLE, EVT_BOOL);
    pinfo->mapprops["DefaultElemVAlign"] = QMainPropInfo(PROPERTYID_RE2_ELEMVERTICALALIGN, EVT_ENUM);
    pinfo->mapprops["MaxLineCount"] = QMainPropInfo(PROPERTYID_RE_MAXLINECOUNT, EVT_INT);

    //event
    pinfo->mapprops["OnRE2ClickHyperLink"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RE2_CLICKHYPERLINK);
    pinfo->mapprops["OnRE2RClickHyperLink"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RE2_RCLICKHYPERLINK);
    pinfo->mapprops["OnShowImageToolTip"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SHOWIMAGE_TOOLTIP);
    pinfo->mapprops["OnShowHyperLinkToolTip"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SHOWHYPERLINK_TOOLTIP);
    pinfo->mapprops["OnRE2RClickImage"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RE2_RCLICKIMAGE);
    pinfo->mapprops["OnRE2ClickImage"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RE2_CLICKIMAGE);

    mmaptypes["RichEdit2"] = pinfo;
}

void QMain::InitRichTextBoxPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_RICHTEXTBOX);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["Margin"] = QMainPropInfo(PROPERTYID_RE2_MARGIN, EVT_STRING);
    pinfo->mapprops["ReadOnly"] = QMainPropInfo(PROPERTYID_RE2_READONLY, EVT_BOOL);
    pinfo->mapprops["LineSpacing"] = QMainPropInfo(PROPERTYID_RE2_ROWSPACE, EVT_INT);
    pinfo->mapprops["MinWidth"] = QMainPropInfo(	PROPERTYID_MINWIDTH,	EVT_INT);
    pinfo->mapprops["MinHeight"] = QMainPropInfo(	PROPERTYID_MINHEIGHT,	EVT_INT);
    pinfo->mapprops["MaxWidth"] = QMainPropInfo(	PROPERTYID_MAXWIDTH,	EVT_INT);
    pinfo->mapprops["UseTextCaption"] = QMainPropInfo(PROPERTYID_RE2_USECAPTION, EVT_BOOL);
    pinfo->mapprops["DefaultRowHeight"] = QMainPropInfo(PROPERTYID_RE2_DEFAULTROWHEIGHT, EVT_INT);
    pinfo->mapprops["AutoScrollToBottom"] = QMainPropInfo(PROPERTYID_RE2_AUTOSCROLL, EVT_BOOL);
    pinfo->mapprops["HyperLinkColor"] = QMainPropInfo(PROPERTYID_RE2_HYPERLINKCOLOR, EVT_COLOR);
    pinfo->mapprops["HyperLinkHoverColor"] = QMainPropInfo(PROPERTYID_RE2_HLHOVERCOLOR, EVT_COLOR);
    pinfo->mapprops["WordBreak"] = QMainPropInfo(PROPERTYID_WORDBREAK, EVT_BOOL);
    pinfo->mapprops["ClickTransparentToText"] = QMainPropInfo(PROPERTYID_RE2_CLICKTRANSPARENT2TEXT, EVT_BOOL);
    pinfo->mapprops["BottomFirst"] = QMainPropInfo(PROPERTYID_RE2_BOTTOMFIRST, EVT_BOOL);
    pinfo->mapprops["ElemAlign"] = QMainPropInfo(PROPERTYID_RE2_ELEMALIGN, EVT_ENUM);
    pinfo->mapprops["DefaultElemVAlign"] = QMainPropInfo(PROPERTYID_RE2_ELEMVERTICALALIGN, EVT_ENUM);
    pinfo->mapprops["ContentFade"] = QMainPropInfo(PROPERTYID_RE2_CONTENTFADE, EVT_BOOL);
    pinfo->mapprops["ContentShowTime"] = QMainPropInfo(PROPERTYID_RE2_CONTENTSHOWTIME, EVT_INT);
    pinfo->mapprops["ContentFadeTime"] = QMainPropInfo(PROPERTYID_RE2_CONTENTFADETIME, EVT_INT);
    pinfo->mapprops["MaxLineCount"] = QMainPropInfo(PROPERTYID_RE_MAXLINECOUNT, EVT_INT);
    pinfo->mapprops["SelectEnable"] = QMainPropInfo(EI_RE_SELECTENABLE, EVT_BOOL);

    //event
    pinfo->mapprops["OnRTBClickHyperLink"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RTB_CLICKHYPERLINK);
    pinfo->mapprops["OnRTBRClickHyperLink"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RTB_RCLICKHYPERLINK);
    pinfo->mapprops["OnShowImageToolTip"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SHOWIMAGE_TOOLTIP);
    pinfo->mapprops["OnShowHyperLinkToolTip"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SHOWHYPERLINK_TOOLTIP);
    pinfo->mapprops["OnRTBRClickImage"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RTB_RCLICKIMAGE);
    pinfo->mapprops["OnRTBClickImage"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RTB_CLICKIMAGE);
    pinfo->mapprops["OnRTBHoverImage"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RTB_HOVERIMAGE);
    pinfo->mapprops["OnRTBLeaveImage"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RTB_LEAVEIMAGE);

    mmaptypes["RichTextBox"] = pinfo;
}

void QMain::InitRichEditViewPropertyMap()
{
}

void QMain::InitScrollBarPropertyMap()
{
    //ScrollBar
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TSCROLLBAR);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["ScrollBarIsHorizon"] = QMainPropInfo(PROPERTYID_SCROLLBARISHORIZON, EVT_BOOL);
    pinfo->mapprops["ScrollBarImagePath"] = QMainPropInfo(PROPERTYID_SB_IMAGEPATH, EVT_STRING);
    pinfo->mapprops["MinThumbLength"] = QMainPropInfo(PROPERTYID_SB_MIN_THUMB_LENGTH, EVT_INT);
    pinfo->mapprops["ScrollBarImagePostfixName"] = QMainPropInfo(PROPERTYID_SB_IMAGE_POSTFIX, EVT_STRING);
    pinfo->mapprops["ScrollBarImageConfig"] = QMainPropInfo(PROPERTYID_SB_IMAGECONFIG, EVT_STRING);
    pinfo->mapprops["ThumbStretch"] = QMainPropInfo(PROPERTYID_SB_THUMBDISTORTED, EVT_BOOL);
    pinfo->mapprops["HaveToStartBtn"] = QMainPropInfo(PROPERTYID_SB_HAVETOSTARTBTN, EVT_BOOL);
    pinfo->mapprops["HaveToEndBtn"] = QMainPropInfo(PROPERTYID_SB_HAVETOENDBTN, EVT_BOOL);

    AddRangeProperties(pinfo);

    //event
    pinfo->mapprops["OnSBMouseLease"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SCROLLBAR_MOUSELEAVE);
    pinfo->mapprops["OnSBThumbPosChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SCROLLBAR_THUMBPOSCHANGED);

    mmaptypes["ScrollBar"] = pinfo;
}

void QMain::InitScrollViewPropertyMap()
{
    //ScrollView
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TSCROLLVIEW);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["MinThumbLength"] = QMainPropInfo(PROPERTYID_SB_MIN_THUMB_LENGTH, EVT_INT);
    pinfo->mapprops["HScrollImagePath"] = QMainPropInfo(PROPERTYID_SV_HSCROLL_IMAGEPATH, EVT_STRING);
    pinfo->mapprops["VScrollImagePath"] = QMainPropInfo(PROPERTYID_SV_VSCROLL_IMAGEPATH, EVT_STRING);
    pinfo->mapprops["HasHScroll"] = QMainPropInfo(PROPERTYID_SV_HAS_HSCROLL, EVT_BOOL);
    pinfo->mapprops["HasVScroll"] = QMainPropInfo(PROPERTYID_SV_HAS_VSCROLL, EVT_BOOL);
    pinfo->mapprops["HScrollHeight"] = QMainPropInfo(PROPERTYID_SV_HSCROLL_HEIGHT, EVT_INT);
    pinfo->mapprops["VScrollWidth"] = QMainPropInfo(PROPERTYID_SV_VSCROLL_WIDTH, EVT_INT);
    pinfo->mapprops["ScrollBarAlwaysExist"] = QMainPropInfo(PROPERTYID_SV_SCROLLBAR_ALWAYS_EXIST, EVT_BOOL);
    pinfo->mapprops["ScrollBarImagePostfixName"] = QMainPropInfo(PROPERTYID_SB_IMAGE_POSTFIX, EVT_STRING);
    pinfo->mapprops["VScrollImageConfig"] = QMainPropInfo(PROPERTYID_SV_VSCROLL_IMAGECONFIG, EVT_STRING);
    pinfo->mapprops["HScrollImageConfig"] = QMainPropInfo(PROPERTYID_SV_HSCROLL_IMAGECONFIG, EVT_STRING);
    pinfo->mapprops["HScrollThumbStretch"] = QMainPropInfo(PROPERTYID_SV_HSCROLL_THUMBDISTORTED, EVT_BOOL);
    pinfo->mapprops["VScrollThumbStretch"] = QMainPropInfo(PROPERTYID_SV_VSCROLL_THUMBDISTORTED, EVT_BOOL);
    pinfo->mapprops["VScrollDocked"] = QMainPropInfo(PROPERTYID_SV_VSCROLL_DOCKED, EVT_ENUM);
    pinfo->mapprops["HScrollDocked"] = QMainPropInfo(PROPERTYID_SV_HSCROLL_DOCKED, EVT_ENUM);
    pinfo->mapprops["HScrollStep"] = QMainPropInfo(PROPERTYID_SV_HSCROLL_STEPLEN, EVT_INT);
    pinfo->mapprops["VScrollStep"] = QMainPropInfo(PROPERTYID_SV_VSCROLL_STEPLEN, EVT_INT);
    pinfo->mapprops["ScrollBarAutoSize"] = QMainPropInfo(PROPERTYID_SV_SCROLLBAR_AUTOSIZE, EVT_BOOL);
    pinfo->mapprops["VScrollBarRect"] = QMainPropInfo(PROPERTYID_SV_VSCROLLBAR_RECT, EVT_STRING);
    pinfo->mapprops["HScrollBarRect"] = QMainPropInfo(PROPERTYID_SV_HSCROLLBAR_RECT, EVT_STRING);

    pinfo->mapprops["HHaveToStartBtn"] = QMainPropInfo(PROPERTYID_SV_HHAVETOSTARTBTN, EVT_BOOL);
    pinfo->mapprops["HHaveToEndBtn"] = QMainPropInfo(PROPERTYID_SV_HHAVETOENDBTN, EVT_BOOL);
    pinfo->mapprops["VHaveToStartBtn"] = QMainPropInfo(PROPERTYID_SV_VHAVETOSTARTBTN, EVT_BOOL);
    pinfo->mapprops["VHaveToEndBtn"] = QMainPropInfo(PROPERTYID_SV_VHAVETOENDBTN, EVT_BOOL);
    pinfo->mapprops["Content2Max"] = QMainPropInfo(PROPERTYID_SV_CONTENT2MAX, EVT_BOOL);
    //event
    pinfo->mapprops["OnSVGetScrollStep"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SCROLLVIEW_GETSCROLLSTEP);
    pinfo->mapprops["OnSVClientChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SCROLLVIEW_CLIENTCHANGE);
    pinfo->mapprops["OnSVScrollBarDisappear"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SCROLLVIEW_SCROLLBARDISAPPEAR);
    pinfo->mapprops["OnSBThumbPosChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SCROLLVIEW_THUMBPOSCHANGED);
    pinfo->mapprops["OnSVScrollBarDisplay"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SCROLLVIEW_SCROLLBARDISPLAY);

    mmaptypes["ScrollView"] = pinfo;
}

void QMain::InitSignaledSwitchPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_SIGNALEDSWITCH);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["SourceItem"] = QMainPropInfo(PROPERTYID_SWITCHSOURCEITEM, EVT_STRING);
    pinfo->mapprops["Adapter"] = QMainPropInfo(PROPERTYID_SWITCHADAPTER, EVT_STRING);

    mmaptypes["ScrollView"] = pinfo;
}

void QMain::InitSliderPropertyMap()
{
    //Slider
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TSLIDER);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["SliderTopImage"] = QMainPropInfo(PROPERTYID_SLIDERTOPIMAGE, EVT_STRING);
    pinfo->mapprops["SliderBottomImage"] = QMainPropInfo(PROPERTYID_SLIDERBOTTOMIMAGE, EVT_STRING);
    pinfo->mapprops["ChannelSize"] = QMainPropInfo(PROPERTYID_SLIDERCHANNELHEIGHT, EVT_INT);
    pinfo->mapprops["IsHorizon"] = QMainPropInfo(PROPERTYID_SLIDERISHORIZON, EVT_BOOL);	
    pinfo->mapprops["AccessibleMin"] = QMainPropInfo(PROPERTYID_SLD_ACCESSIBLEMIN, EVT_INT);
    pinfo->mapprops["AccessibleMax"] = QMainPropInfo(PROPERTYID_SLD_ACCESSIBLEMAX, EVT_INT);
    pinfo->mapprops["PageSize"] = QMainPropInfo(PROPERTYID_SLD_PAGESIZE, EVT_INT);
    pinfo->mapprops["LineSize"] = QMainPropInfo(PROPERTYID_SLD_LINESIZE, EVT_INT);
    pinfo->mapprops["MoveToMousePos"] = QMainPropInfo(PROPERTYID_SLD_MOVETOMOUSEPOS, EVT_BOOL);
    pinfo->mapprops["BeHaveRule"] = QMainPropInfo(PROPERTYID_SLD_BEHAVERULE, EVT_ENUM);
    pinfo->mapprops["DecideMode"] = QMainPropInfo(PROPERTYID_SLD_DECIDEMODE, EVT_BOOL);
    pinfo->mapprops["UnDecidedImage"] = QMainPropInfo(PROPERTYID_SLIDERUNDECIDEDIMAGE, EVT_STRING);
    AddRangeProperties(pinfo);
    AddStateImageProperties(pinfo);

    //event
    pinfo->mapprops["OnSliderThumbPosChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SLIDER_THUMBPOSCHANGED);
    pinfo->mapprops["OnSliderVirtualPosChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SLIDER_THUMBVIRTUALPOSCHANGED);
    pinfo->mapprops["OnSliderPosChangeEnd"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SLIDER_THUMBPOSCHANGEEND);

    mmaptypes["Slider"] = pinfo;
}

void QMain::InitStaticPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TSTATIC);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["CareSpecialChar"] = QMainPropInfo(PROPERTYID_CARESPECIALCHAR, EVT_BOOL);
    pinfo->mapprops["VerticalCaption"] = QMainPropInfo(PROPERTYID_VERTICAL, EVT_BOOL);
    pinfo->mapprops["VertInterval"] = QMainPropInfo(PROPERTYID_CHARINTERVAL, EVT_INT);

    AddMultiLineTextProperties(pinfo);

    mmaptypes["Static"] = pinfo;
}

void QMain::InitTablePropertyMap()
{
    //Table
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TABLE);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["ItemInnerMargins"] = QMainPropInfo(PROPERTYID_TBL_ITEMINNERMARGINS, EVT_STRING);
    pinfo->mapprops["IsVirtualMode"] = QMainPropInfo(PROPERTYID_TBL_ISVIRTUALMODE, EVT_BOOL);
    pinfo->mapprops["DotLineColor"] = QMainPropInfo(PROPERTYID_TBL_DOTLINECOLOR, EVT_COLOR);
    pinfo->mapprops["DotLineWidth"] = QMainPropInfo(PROPERTYID_TBL_DOTLINEWIDTH, EVT_DWORD);
    pinfo->mapprops["HoverBackground"] = QMainPropInfo(PROPERTYID_TBL_HOVERBACKGROUND, EVT_STRING);
    pinfo->mapprops["SelectBackground"] = QMainPropInfo(PROPERTYID_TBL_SELECTBACKGROUND, EVT_STRING);
    pinfo->mapprops["WholeRowSelected"] = QMainPropInfo(PROPERTYID_TBL_WHOLEROWSELECTED, EVT_BOOL);
    pinfo->mapprops["TableInitParam"] = QMainPropInfo(PROPERTYID_TBL_INITPARAM, EVT_STRING);
    pinfo->mapprops["TableRowHeight"] = QMainPropInfo(PROPERTYID_TBL_ROWHEIGHT, EVT_INT);
    pinfo->mapprops["TextVertAlign"] = QMainPropInfo(PROPERTYID_TEXTVERTALIGN, EVT_ENUM);
    pinfo->mapprops["CacheRowSize"] = QMainPropInfo(PROPERTYID_TBL_CACHE_ROW, EVT_INT);
    pinfo->mapprops["ItemMultiLine"] = QMainPropInfo(PROPERTYID_MULTILINE, EVT_BOOL);	
    pinfo->mapprops["EnableKey"] = QMainPropInfo(PROPERTYID_TBL_ENABLEKEY, EVT_BOOL);
    AddGridProperties(pinfo);

    //event
    pinfo->mapprops["OnTableLButtonDown"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_LBUTTONDOWN);
    pinfo->mapprops["OnTableRButtonDown"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_RBUTTONDOWN);
    pinfo->mapprops["OnTableRButtonUp"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_RBUTTONUP);
    pinfo->mapprops["OnTableRowHeightChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_ROWHEIGHTCHANGE);
    pinfo->mapprops["OnTableItemDBClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_ITEMDBCLICK);
    pinfo->mapprops["OnTableSetLine"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_SETLINE);
    pinfo->mapprops["OnTableSetItemMaxWidth"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_SET_ITEM_MAX_WIDTH_IN_COL);
    pinfo->mapprops["OnTableDocChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_DOCUMENT_CHANGE);
    pinfo->mapprops["OnTableGetScrollInfo"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_GETSCROLLINFO);
    pinfo->mapprops["OnTableItemClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_ITEMCLICK);
    pinfo->mapprops["OnTableItemRClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_ITEMRCLICK);
    pinfo->mapprops["OnTableItemHover"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TABLE_ITEMHOVER);

    // Localization
    pinfo->mapprops["TableInitParam_LID"] = QMainPropInfo(  PROPERTYID_TBL_INITPARAM,  EVT_LOCALIZATION);

    mmaptypes["Table"] = pinfo;
}

void QMain::InitTimeKeeperPropertyMap()
{
    //TimeKeeper
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TTIMEKEEPER);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["Max"] = QMainPropInfo(PROPERTYID_TIMEKEEOER_MAX, EVT_DWORD);
    pinfo->mapprops["Min"] = QMainPropInfo(PROPERTYID_TIMEKEEOER_MIN, EVT_DWORD);
    pinfo->mapprops["Position"] = QMainPropInfo(PROPERTYID_TIMEKEEOER_POSITION, EVT_DWORD);
    pinfo->mapprops["Mode"] = QMainPropInfo(PROPERTYID_TIMEKEEOER_MODE, EVT_DWORD);
    pinfo->mapprops["Direction"] = QMainPropInfo(PROPERTYID_TIMEKEEOER_DIR, EVT_DWORD);
    pinfo->mapprops["Increase"] = QMainPropInfo(PROPERTYID_TIMEKEEOER_INCREASE, EVT_BOOL);
    pinfo->mapprops["FlashTime"] = QMainPropInfo(PROPERTYID_TIMEKEEOER_FLASHTIME, EVT_DWORD);
    pinfo->mapprops["DigitSpace"] = QMainPropInfo(PROPERTYID_TIMEKEEOER_SPACE, EVT_DWORD);
    pinfo->mapprops["DigitNumPositionX"] = QMainPropInfo(PROPERTYID_TIMEKEEOER_X, EVT_DWORD);
    pinfo->mapprops["DigitNumPositionY"] = QMainPropInfo(PROPERTYID_TIMEKEEOER_Y, EVT_DWORD);
    pinfo->mapprops["EnableZeroShow"] = QMainPropInfo(PROPERTYID_TIMEKEEOER_ZEROSHOW, EVT_BOOL);
    pinfo->mapprops["ConverLayerImage"] = QMainPropInfo(	PROPERTYID_TIMEKEEOER_COVERIMG,	EVT_STRING);
    pinfo->mapprops["AlphaLayerImage"] = QMainPropInfo(	PROPERTYID_TIMEKEEOER_ALPHAIMG,	EVT_STRING);
    pinfo->mapprops["DigitImage"] = QMainPropInfo(	PROPERTYID_TIMEKEEOER_DIGITIMG,	EVT_STRING);
    pinfo->mapprops["Autostart"] = QMainPropInfo(	PROPERTYID_TIMEKEEOER_AUTOSTART, EVT_BOOL);	
    pinfo->mapprops["FlashEnable"] = QMainPropInfo(	PROPERTYID_TIMEKEEOER_FLASHABLE, EVT_BOOL);	

    //event
    pinfo->mapprops["OnTimeAlert"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TIMEKEEPER_TIMEALERT);
    pinfo->mapprops["OnTimeOut"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TIMEKEEPER_TIMEOUT);

    mmaptypes["TimeKeeper"] = pinfo;
}

void QMain::InitTitleBarPropertyMap()
{
    // titlebar
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TITLEBAR);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["HostName"] = QMainPropInfo(			PROPERTYID_HOSTNAME, EVT_STRING);
    pinfo->mapprops["MaxButtonName"] = QMainPropInfo(	PROPERTYID_MAXBUTTONNAME, EVT_STRING);
    pinfo->mapprops["MinButtonName"] = QMainPropInfo(	PROPERTYID_MINBUTTONNAME, EVT_STRING);
    pinfo->mapprops["RestoreButtonName"] = QMainPropInfo(PROPERTYID_RESTOREBUTTONNAME, EVT_STRING);
    pinfo->mapprops["CloseButtonName"] = QMainPropInfo(	PROPERTYID_CLOSEBUTTONNAME, EVT_STRING);

    mmaptypes["TitleBar"] = pinfo;
}

void QMain::InitToolTipPropertyMap()
{
    //ToolTip
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TTOOLTIP);
    if(pinfo == NULL)
    {
        return;
    }
    //	AddMultiLineTextProperties(pinfo);
    pinfo->mapprops["MultiLine"] = QMainPropInfo(PROPERTYID_MULTILINE, EVT_BOOL);
    pinfo->mapprops["AutoChangeLine"] = QMainPropInfo(PROPERTYID_AUTOCHANGELINE, EVT_BOOL);
    pinfo->mapprops["LineSpacing"] = QMainPropInfo(PROPERTYID_RE2_ROWSPACE, EVT_INT);
    pinfo->mapprops["WordBreak"] = QMainPropInfo(PROPERTYID_WORDBREAK, EVT_BOOL);
    pinfo->mapprops["MinWidth"] = QMainPropInfo(	PROPERTYID_MINWIDTH,	EVT_INT);
    pinfo->mapprops["MinHeight"] = QMainPropInfo(	PROPERTYID_MINHEIGHT,	EVT_INT);
    pinfo->mapprops["MaxWidth"] = QMainPropInfo(	PROPERTYID_MAXWIDTH,	EVT_INT);
    pinfo->mapprops["IsGlobalTip"] = QMainPropInfo(PROPERTYID_TIP_ISGLOBAL, EVT_BOOL);
    pinfo->mapprops["Margins"] = QMainPropInfo(PROPERTYID_TIP_MARGINS, EVT_STRING);
    pinfo->mapprops["ShowTime"] = QMainPropInfo(PROPERTYID_TIP_SHOWTIME, EVT_INT);
    pinfo->mapprops["DelayTime"] = QMainPropInfo(PROPERTYID_TIP_DELAYTIME, EVT_INT);
    pinfo->mapprops["ReAlignMode"] = QMainPropInfo(PROPERTYID_RE_ALIGNMODE, EVT_ENUM);
    pinfo->mapprops["CareSpecialChar"] = QMainPropInfo(PROPERTYID_CARESPECIALCHAR, EVT_BOOL);
    pinfo->mapprops["HyperLinkColor"] = QMainPropInfo(PROPERTYID_RE2_HYPERLINKCOLOR, EVT_COLOR);
    pinfo->mapprops["HyperLinkHoverColor"] = QMainPropInfo(PROPERTYID_RE2_HLHOVERCOLOR, EVT_COLOR);
    pinfo->mapprops["LayeredWindow"] = QMainPropInfo(PROPERTYID_LAYEREDWINDOW,EVT_BOOL);

    //event
    pinfo->mapprops["OnToolTipAutoMoved"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TOOLTIP_AUTOMOVED);
    pinfo->mapprops["OnToolTipChangeText"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TOOLTIP_CHANGETEXT);

    mmaptypes["ToolTip"] = pinfo;
}

void QMain::InitWin32TreePropertyMap()
{
    // win32tree
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TTREE);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["ImageList"] = QMainPropInfo(PROPERTYID_TREE_IMAGELIST, EVT_STRING);	

    //event
    pinfo->mapprops["OnTreeSelectItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_SELECTITEM);
    pinfo->mapprops["OnTreeExpandItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_EXPANDITEM);
    pinfo->mapprops["OnTreeCollapseItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_COLLAPSEITEM);
    pinfo->mapprops["OnTreeDBClickItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_DBCLICKITEM);
    pinfo->mapprops["OnTreeRClickItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_RCLICKITEM);

    mmaptypes["Win32Tree"] = pinfo;
}

void QMain::InitTPFTreePropertyMap()
{
    // tpftree
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TTPFTREE);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["ImageList"] = QMainPropInfo(PROPERTYID_TREE_IMAGELIST, EVT_STRING);		
    pinfo->mapprops["ItemHeight"] = QMainPropInfo(PROPERTYID_TREE_ITEMHEIGHT, EVT_INT);		
    pinfo->mapprops["ItemLayout"] = QMainPropInfo(PROPERTYID_TREE_ITEMLAYOUT, EVT_STRING);		
    pinfo->mapprops["ExpandingImageID"] = QMainPropInfo(PROPERTYID_TREE_EXPANDING_IMAGEID, EVT_INT);		
    pinfo->mapprops["CollapsingImageID"] = QMainPropInfo(PROPERTYID_TREE_COLLAPSING_IMAGEID, EVT_INT);		
    pinfo->mapprops["IndentLength"] = QMainPropInfo(PROPERTYID_TREE_INDENTLEN, EVT_INT);		
    pinfo->mapprops["ItemXOffset"] = QMainPropInfo(PROPERTYID_TREE_ITEMXOFFSET, EVT_INT);		
    //pinfo->mapprops["Margin"] = QMainPropInfo(PROPERTYID_TREE_MARGIN, EVT_STRING);	

    pinfo->mapprops["SelectTextColor"] = QMainPropInfo(PROPERTYID_SELECTTEXTCOLOR, EVT_COLOR);
    pinfo->mapprops["HoverBackground"] = QMainPropInfo(PROPERTYID_TBL_HOVERBACKGROUND, EVT_STRING);
    pinfo->mapprops["SelectBackground"] = QMainPropInfo(PROPERTYID_TBL_SELECTBACKGROUND, EVT_STRING);
    pinfo->mapprops["TextVertAlign"] = QMainPropInfo(PROPERTYID_TEXTVERTALIGN, EVT_ENUM);
    pinfo->mapprops["SelectMode"] = QMainPropInfo(PROPERTYID_SELECTMODE, EVT_ENUM);

    pinfo->mapprops["bUseCustomMode"] = QMainPropInfo(PROPERTYID_TREE_USECUSTOMMODE, EVT_BOOL);
    pinfo->mapprops["bUseItemPool"] = QMainPropInfo(PROPERTYID_TREE_USEITEMPOOL, EVT_BOOL);
    pinfo->mapprops["ItemPoolSize"] = QMainPropInfo(PROPERTYID_TREE_ITEMPOOLSIZE, EVT_INT);
    pinfo->mapprops["bUseReplaceItem"] = QMainPropInfo(PROPERTYID_TREE_USEREPLACEITEM, EVT_BOOL);
    pinfo->mapprops["ReplaceItemLayout"] = QMainPropInfo(PROPERTYID_TREE_REPLACEITEMLAYOUT, EVT_STRING);

    pinfo->mapprops["DragCursorID"] = QMainPropInfo(PROPERTYID_TREE_DRAG_CURSORID, EVT_INT);
    pinfo->mapprops["DragFrobidCursorID"] = QMainPropInfo(PROPERTYID_TREE_DRAG_FORBID_CURSORID, EVT_INT);

    pinfo->mapprops["EnableTreeItemDrag"] = QMainPropInfo(PROPERTYID_TREE_CANDRAG, EVT_BOOL);

    //event
    pinfo->mapprops["OnTreeSelectItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_SELECTITEM);
    pinfo->mapprops["OnTreeExpandItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_EXPANDITEM);
    pinfo->mapprops["OnTreeCollapseItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_COLLAPSEITEM);
    pinfo->mapprops["OnTreeDBClickItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_DBCLICKITEM);
    pinfo->mapprops["OnTreeRClickItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_RCLICKITEM);
    pinfo->mapprops["OnTreeRBDownItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_RBDOWN);
    pinfo->mapprops["OnTreeClickItem"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_CLICKITEM);
    pinfo->mapprops["OnTreeDragBegin"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_DRAG_BEGIN);
    pinfo->mapprops["OnTreeDragDrop"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_DRAG_DROP);
    pinfo->mapprops["OnTreeDragCancel"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_TREE_DRAG_CANCEL);
    pinfo->mapprops["OnSCScrollStepChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SC_SCROLLSTEPCHANGE);
    pinfo->mapprops["OnSCDocOffSetChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SC_DOCUMENTOFFSET_CHANGE);
    pinfo->mapprops["OnSCDocChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SC_DOCUMENT_CHANGE);

    mmaptypes["TPFTree"] = pinfo;
}

void QMain::InitWebBrowserPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TWEBBROWSER);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["HaveMenu"] = QMainPropInfo(PROPERTYID_WB_MENU, EVT_BOOL);
    pinfo->mapprops["HaveScrollBar"] = QMainPropInfo(PROPERTYID_WB_SCROLL, EVT_BOOL);
    pinfo->mapprops["URL"] = QMainPropInfo(PROPERTYID_URL, EVT_STRING);

    //event
    pinfo->mapprops["OnWBNavigateFail"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_WB_NAVIGATEFAIL);
    pinfo->mapprops["OnWBBeforeNavigate"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_WB_BEFORENAVIGATE);
    pinfo->mapprops["OnWBNavigateComplete"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_WB_NAVIGATECOMPLETE);
    pinfo->mapprops["OnWBWindowClosing"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_WB_WINDOWCLOSING);

    mmaptypes["WebBrowser"] = pinfo;
}

void QMain::InitTurnTablePropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TURNTABLE);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["Image"] = QMainPropInfo(		PROPERTYID_IMAGE, EVT_STRING);
    pinfo->mapprops["AddTime"] = QMainPropInfo(	PROPERTYID_ITT_ADDTIME,	EVT_FLOAT);
    pinfo->mapprops["SubTime"] = QMainPropInfo(	PROPERTYID_ITT_SUBTIME,	EVT_FLOAT);
    pinfo->mapprops["AvTime"] = QMainPropInfo(	PROPERTYID_ITT_AVTIME,	EVT_FLOAT);
    pinfo->mapprops["TargetS"] = QMainPropInfo(	PROPERTYID_ITT_TARGETS,	EVT_FLOAT);
    pinfo->mapprops["MaxV"] = QMainPropInfo(		PROPERTYID_ITT_MAXV,	EVT_FLOAT);		
    pinfo->mapprops["ActionPlay"] = QMainPropInfo(PROPERTYID_ACTION_PLAY, EVT_BOOL);

    mmaptypes["TurnTable"] = pinfo;
}

void QMain::InitPosterPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_POSTER);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["IntervalTime"] = QMainPropInfo(	PROPERTYID_POSTER_INTERVALTIME, EVT_INT);
    pinfo->mapprops["ShowTime"] = QMainPropInfo(		PROPERTYID_POSTER_SHOWTIME,	EVT_INT);

    mmaptypes["Poster"] = pinfo;
}

void QMain::InitQQMJPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_QQMJCONTROL);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["DrawMode"] = QMainPropInfo(	PROPERTYID_QQMJ_MODE,	EVT_ENUM);
    pinfo->mapprops["DrawDir"] = QMainPropInfo(	PROPERTYID_QQMJ_DIR,	EVT_ENUM);
    pinfo->mapprops["ColorImage"] = QMainPropInfo(	PROPERTYID_QQMJ_COLOR_IMAGE,	EVT_STRING);
    pinfo->mapprops["FrameResPath"] = QMainPropInfo(	PROPERTYID_QQMJ_FRAME_RESPATH,		EVT_STRING);
    pinfo->mapprops["ShadowImage"] = QMainPropInfo(	PROPERTYID_QQMJ_SHADOW_IMAGE,	EVT_STRING);
    pinfo->mapprops["DeltaParam"] = QMainPropInfo(	PROPERTYID_QQMJ_DELTAPARAM,			EVT_STRING);
    pinfo->mapprops["MulLineParam"] = QMainPropInfo(	PROPERTYID_QQMJ_MULLINEPARAM,		EVT_STRING);
    pinfo->mapprops["ColorOffsetYonStandTile"] = QMainPropInfo(	PROPERTYID_QQMJ_COLOROFFSETY_ON_STANDTILE,	EVT_INT);
    pinfo->mapprops["FrontImage"] = QMainPropInfo(	PROPERTYID_QQMJ_FRONT_IMAGE,	EVT_STRING);
    pinfo->mapprops["FrontColor"] = QMainPropInfo(	PROPERTYID_QQMJ_FRONT_COLOR,	EVT_COLOR);

    mmaptypes["QQMJ"] = pinfo;
}

void QMain::InitUserObjectPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TUSEROBJECT);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["AutoLoad"] = QMainPropInfo(PROPERTYID_AUTOLOAD, EVT_BOOL);
    pinfo->mapprops["ParentMode"] = QMainPropInfo(PROPERTYID_PARENTMODE, EVT_BOOL);

    //event
    pinfo->mapprops["OnRestoreSuccess"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_RESTORE_SUCCESS);

    mmaptypes["UserObject"] = pinfo;
}

void QMain::InitSplitterPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TSPLITTER);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["HostName"] = QMainPropInfo(PROPERTYID_HOSTNAME, EVT_STRING);
    pinfo->mapprops["IsHorizon"] = QMainPropInfo(PROPERTYID_SPLITTER_HORIZON, EVT_BOOL);
    pinfo->mapprops["LowerRange"] = QMainPropInfo(PROPERTYID_LOWERRANGE, EVT_INT);
    pinfo->mapprops["UpperRange"] = QMainPropInfo(PROPERTYID_UPPERRANGE, EVT_INT);

    //event
    pinfo->mapprops["OnSplitterPosChange"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_SPLITTER_SPLITTERPOSCHANGED);

    mmaptypes["Splitter"] = pinfo;
}

void QMain::InitToolTipBinderPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TOOLTIPBINDER);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["HostTipName"] = QMainPropInfo(PROPERTYID_TTB_HOSTTIPNAME, EVT_STRING);
    pinfo->mapprops["BindTipsList"] = QMainPropInfo(PROPERTYID_TTB_BINDTIPLISTNAME, EVT_STRING);

    mmaptypes["ToolTipBinder"] = pinfo;
}

void QMain::InitRadarPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TTRADAR);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["ClassID"] = QMainPropInfo(PROPERTYID_CLASSID, EVT_INT);
    pinfo->mapprops["ZoomDirection"] = QMainPropInfo(PROPERTYID_CONTAINER_ZOOMDIRECTION, EVT_ENUM);

    mmaptypes["Radar"] = pinfo;
}

void QMain::InitSoftKeyBoardPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TSOFTKEYBOARD);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["TextColorNormal"] = QMainPropInfo(EPARAMID_SKB_TEXTCOLORNORMAL, EVT_COLOR);
    pinfo->mapprops["TextColorGray"] = QMainPropInfo(EPARAMID_SKB_TEXTCOLORGRAY, EVT_COLOR);

    mmaptypes["SoftKeyBoard"] = pinfo;
}

void QMain::InitMarqueePropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_MARQUEE);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["LineSpacing"] = QMainPropInfo(PROPERTYID_RE2_ROWSPACE, EVT_INT);
    pinfo->mapprops["DefaultElemVAlign"] = QMainPropInfo(PROPERTYID_RE2_ELEMVERTICALALIGN, EVT_ENUM);
    pinfo->mapprops["DefaultRowHeight"] = QMainPropInfo(PROPERTYID_RE2_DEFAULTROWHEIGHT, EVT_INT);
    pinfo->mapprops["PauseWhenHover"] = QMainPropInfo(PROPERTYID_MQ_PAUSEWHENHOVER, EVT_BOOL);

    //event
    pinfo->mapprops["OnMarqueeEnd"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_MARQUEE_END);

    mmaptypes["Marquee"] = pinfo;
}

void QMain::InitAccordionPropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_TACCORDION);
    if(pinfo == NULL)
    {
        return;
    }
    AddStateBgProperties(pinfo);
    pinfo->mapprops["InitPages"] = QMainPropInfo(PROPERTYID_AD_PAGES, EVT_STRING);
    pinfo->mapprops["ButtonHeight"] = QMainPropInfo(PROPERTYID_AD_TABHEIGHT, EVT_INT);
    pinfo->mapprops["TabIndex"] = QMainPropInfo(PROPERTYID_AD_INDEX, EVT_INT);
    pinfo->mapprops["TabSpace"] = QMainPropInfo(PROPERTYID_AD_TABSPACE, EVT_INT);
    // button text 
    pinfo->mapprops["TextFont"] = QMainPropInfo(PROPERTYID_TEXTFONT_NORMAL, EVT_STRING);
    pinfo->mapprops["TextFontHover"] = QMainPropInfo(PROPERTYID_TEXTFONT_HOVER, EVT_STRING);
    pinfo->mapprops["TextFontPressed"] = QMainPropInfo(PROPERTYID_TEXTFONT_PRESSED, EVT_STRING);
    pinfo->mapprops["TextFontDisable"] = QMainPropInfo(PROPERTYID_TEXTFONT_DISABLE, EVT_STRING);

    pinfo->mapprops["TextColorHover"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_HOVER, EVT_COLOR);
    pinfo->mapprops["TextColorPressed"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_PRESSED, EVT_COLOR);
    pinfo->mapprops["TextColorDisable"] = QMainPropInfo(PROPERTYID_TEXTCOLOR_DISABLE, EVT_COLOR);

    pinfo->mapprops["TextEffectHover"] = QMainPropInfo(PROPERTYID_TEXT_EFFECT_HOVER, EVT_STRING);
    pinfo->mapprops["TextEffectPressed"] = QMainPropInfo(PROPERTYID_TEXT_EFFECT_PRESSED, EVT_STRING);
    pinfo->mapprops["TextEffectDisable"] = QMainPropInfo(PROPERTYID_TEXT_EFFECT_DISABLE, EVT_STRING);
    pinfo->mapprops["TextXOffsetHover"] = QMainPropInfo(PROPERTYID_TEXT_XOFFSET_HOVER, EVT_INT);
    pinfo->mapprops["TextYOffsetHover"] = QMainPropInfo(PROPERTYID_TEXT_YOFFSET_HOVER, EVT_INT);
    pinfo->mapprops["TextXOffsetPressed"] = QMainPropInfo(PROPERTYID_TEXT_XOFFSET_PRESSED, EVT_INT);
    pinfo->mapprops["TextYOffsetPressed"] = QMainPropInfo(PROPERTYID_TEXT_YOFFSET_PRESSED, EVT_INT);

    pinfo->mapprops["ButtonType"] = QMainPropInfo(PROPERTYID_BUTTONTYPE, EVT_STRING);

    //Localization
    pinfo->mapprops["TextFont_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_NORMAL,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontHover_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_HOVER,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontPressed_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_PRESSED,  EVT_LOCALIZATION);
    pinfo->mapprops["TextFontDisable_LID"] = QMainPropInfo(  PROPERTYID_TEXTFONT_DISABLE,  EVT_LOCALIZATION);
    pinfo->mapprops["InitPages_LID"] = QMainPropInfo(  PROPERTYID_AD_PAGES,  EVT_LOCALIZATION);

    //event
    pinfo->mapprops["OnClick"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_CLICK);

    mmaptypes["Accordion"] = pinfo;
}

void QMain::InitBehaviorDesigner2PropertyMap()
{
    QMainTypeInfo *pinfo;
    pinfo = new QMainTypeInfo(COMPONENTID_BEHAVIORDESIGNER2);
    if(pinfo == NULL)
    {
        return;
    }
    pinfo->mapprops["BehaviorParam"] = QMainPropInfo(PROPERTYID_BD2_PARAM, EVT_STRING);
    pinfo->mapprops["BehaviorCount"] = QMainPropInfo(PROPERTYID_BD2_COUNT, EVT_DWORD);
    pinfo->mapprops["Action"] = QMainPropInfo(PROPERTYID_BHVDSGN_ACTION, EVT_ENUM);

    //event
    pinfo->mapprops["OnPastKeyFrameX"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_BD2_PASTKEYFRAME_X);
    pinfo->mapprops["OnPastKeyFrameY"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_BD2_PASTKEYFRAME_Y);
    pinfo->mapprops["OnPastKeyFrameW"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_BD2_PASTKEYFRAME_W);
    pinfo->mapprops["OnPastKeyFrameH"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_BD2_PASTKEYFRAME_H);
    pinfo->mapprops["OnPastKeyFrameA"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_BD2_PASTKEYFRAME_A);
    pinfo->mapprops["OnBehaviorEnd"] = QMainPropInfo(PROPERTYID_EVENTFUNC_BEGIN+ESE_ON_BD2_BEHAVIOREND);

    mmaptypes["BehaviorDesigner2"] = pinfo;
}

void QMain::InitTypes()
{
	// Common Event properties for script supporting
	InitCommonEventPropertyMap();
	InitCommonPropertyPropertyMap();

	// Init Control Map begin
	InitBeelinePropertyMap();
	InitBehaviorDesignerPropertyMap();
	InitButtonPropertyMap();
	InitTinyButtonPropertyMap();
	InitCardCtrlPropertyMap();
	InitWordCardCtrlPropertyMap();
	InitCheckBoxPropertyMap();
	InitComboBoxPropertyMap();
	InitContainerPropertyMap();
	InitDialogPropertyMap();
	InitDigitPicBoxPropertyMap();
	InitEditPropertyMap();
	InitExcluderPropertyMap();
	InitHeaderPropertyMap();
	InitWin32ImageListPropertyMap();
	InitTPFImageListPropertyMap();
	InitLabelPropertyMap();
	InitLevelPropertyMap();
	InitListBoxPropertyMap();
	InitListViewPropertyMap();
	InitMenuPropertyMap();
	InitMultiColorCtrlPropertyMap();
	InitMultiImageVerminPropertyMap();
	InitMultiImageCtrlPropertyMap();
	InitOverlapContainerPropertyMap();
	InitRichEditPropertyMap();
	InitRichEditViewPropertyMap();
	InitScrollBarPropertyMap();
	InitScrollViewPropertyMap();
	InitSignaledSwitchPropertyMap();
	InitSliderPropertyMap();
	InitStaticPropertyMap();
	InitTablePropertyMap();
	InitTimeKeeperPropertyMap();
	InitTitleBarPropertyMap();
	InitToolTipPropertyMap();
	InitWin32TreePropertyMap();
	InitTPFTreePropertyMap();
	InitWebBrowserPropertyMap();
	InitTurnTablePropertyMap();			
	InitPosterPropertyMap();
	InitQQMJPropertyMap();
	InitUserObjectPropertyMap();
	InitSplitterPropertyMap();
	InitIconboxPropertyMap();
	InitIconGridPropertyMap();
	InitRichEdit2PropertyMap();
	InitRichTextBoxPropertyMap();
	InitToolTipBinderPropertyMap();
	InitIconbufferPropertyMap();
	InitRadarPropertyMap();
	InitSoftKeyBoardPropertyMap();
	InitMarqueePropertyMap();
	InitBehaviorDesigner2PropertyMap();
	InitAccordionPropertyMap();
}

void QMain::ExitTypes()
{
	std::map<string, QMainTypeInfo*>::iterator type = mmaptypes.begin();
	while(type != mmaptypes.end())
	{
		QMainTypeInfo *pinfo = type->second;
		if(pinfo != NULL)
		{
            pinfo->mapprops.clear();
			delete pinfo;
		}
		++type;
	}
	mmaptypes.clear();
}

#endif

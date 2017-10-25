//
//  ResolutionDef.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/9.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#ifndef SinaKTVAide_ResolutionDef_h
#define SinaKTVAide_ResolutionDef_h

/*
 主页按钮区的分辨率设置：
 ABSOLUTE打头的分辨率是实际分辨率，根据不同的设备需要除以不同的scale
 RELATIVE打头的分辨率是相对分辨率,不需要再根据设备做除法了
 */
//首页按钮整个的top_margin
#define ABSOLUTE_HEIGHT_BTNVIEW_TOPMARGIN 378
//首页按钮整个高度
#define ABSOLUTE_HEIGHT_BTNVIEW 542
//首页按钮区整个上下间隙
#define ABSOLUTE_HEIGHT_BTNVIEW_TB 24
//首页按钮区图片的top_margin
#define ABSOLUTE_HEIGHT_IMAGE_TOPMARGIN 19
//首页按钮区button 图片左右间隙
#define ABSOLUTE_WIDTH_IMAGE_SPACE 58
#define ABSOLUTE_HEIGHT_TXT_BT 10

#define RELATIVE_WH_IMAGE 44
#define RELATIVE_H_TXT 20

/*
 资源设置
 */

#define NUMBER_BTN 9
const char RES_BTN_TEXT[NUMBER_BTN][50] = { "弹幕互动","领取红包","包厢预定","歌手","主题","新歌","排行榜","语种","扫一扫" };
const char RES_BTN_IMAGE[NUMBER_BTN][50] = {"index_barrage","index_red","index_book","index_singer","index_theme","index_song","index_ranking","index_language","scan_code_ic_1"};

#define COLOR_TEXT 0x8e8e8e
#define COLOR_SPLITER 0xebebeb

#endif

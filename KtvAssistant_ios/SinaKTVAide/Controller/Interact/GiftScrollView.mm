//
//  GiftContentView.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-8-12.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "GiftScrollView.h"
#import "DataType.h"
#import "NetUtilCallBack.h"
#import "KTVBaseConfig.h"
#import "com_ios_NetUtil.h"

@implementation GiftScrollView

- (id)initWithFrame:(CGRect)frame type:(NSInteger) type
{
    self = [super initWithFrame:frame];
    if (self) {
        _views = [[NSMutableArray alloc] init];
        giftType = type;
        if (type == 1)
            count = (int)([NetUtilCallBack getInstance]->m_localLuckGiftInfo).size();
        else if (type == 2)
            count = (int)([NetUtilCallBack getInstance]->m_localCommonGiftInfo).size();
        else
            count = (int)([NetUtilCallBack getInstance]->m_localLuxuryGiftInfo).size();
        int aa = (count%8==0)?0:1;
        int bb = count/8;
        int size = aa + bb;
        for (int i = 0; i< size; i++)
        {
            UIView *view = [[UIView alloc] initWithFrame:CGRectMake(frame.size.width * i, 0, frame.size.width, 135)];
            [self loadGift:view index:i];
        }
        _scroll = [[CustomScroll alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height) views:_views];
        _scroll.pageControl.selectColor = [UIColor blueColor];
        _scroll.pageControl.freeColor = UIColorFromRGB(0xbbbfc4);
        
        if (!IOS7_OR_LATER)
            [_scroll.pageControl updateDots6];
        _scroll.backgroundColor = [UIColor clearColor];
        [self addSubview: _scroll];
    }
    return self;
}


-(void)loadGift:(UIView *)view index:(NSInteger)index
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *docDir = [paths objectAtIndex:0];
    for (NSInteger i = 0; i<8; i++)
    {
        if ((i + index * 8)>=count)
            break;
        GiftInfo *info = new GiftInfo();
        if (giftType == 1)
            *info = [NetUtilCallBack getInstance]->m_localLuckGiftInfo[i + index * 8];
        else if (giftType == 2)
            *info = [NetUtilCallBack getInstance]->m_localCommonGiftInfo[i + index * 8];
        else
            *info = [NetUtilCallBack getInstance]->m_localLuxuryGiftInfo[i + index * 8];
        NSString *imgLocation = [NSString stringWithFormat:@"%@%@%@",docDir,@"/images/",[NSString stringWithUTF8String:info->image_name]];
        UIButton *btn = [[UIButton alloc] initWithFrame:CGRectMake((_MainScreen_Width-240)/5 * (i % 4 + 1) + 60 * (i % 4), 10 + (i>=4?1:0) * 63 , 60, 63)];
        btn.tag = info->id;
        btn.userInteractionEnabled = YES;
        btn.backgroundColor = [UIColor clearColor];
        [btn addTarget:self action:@selector(btnClick:) forControlEvents:UIControlEventTouchUpInside];
        UIImageView *btnImg = [[UIImageView alloc] initWithFrame:CGRectMake(20, 5, 25, 25)];
        [btnImg setImage:[UIImage imageWithContentsOfFile: imgLocation]];
        [btn addSubview:btnImg];
        UILabel *lblGiftName = [[UILabel alloc] initWithFrame:CGRectMake(0, 33, 60, 11)];
        lblGiftName.text = [NSString stringWithUTF8String:info->name];
        lblGiftName.textAlignment = NSTextAlignmentCenter;
        lblGiftName.font = [UIFont systemFontOfSize:11];
        lblGiftName.textColor = [UIColor blackColor];
        lblGiftName.backgroundColor = [UIColor clearColor];
        [btn addSubview:lblGiftName];
        UILabel *lblGoldTip = [[UILabel alloc] initWithFrame:CGRectMake(8, 47, 27, 14)];
        lblGoldTip.text = @"金币:";
        lblGoldTip.font = [UIFont systemFontOfSize:11];
        lblGoldTip.textColor = [UIColor blackColor];
        lblGoldTip.backgroundColor = [UIColor clearColor];
        [btn addSubview:lblGoldTip];
        UILabel *lblGold = [[UILabel alloc] initWithFrame:CGRectMake(36, 47, 24, 14)];
        lblGold.text = [NSString stringWithFormat:@"%d",info->cash];
        lblGold.font = [UIFont systemFontOfSize:11];
        lblGold.textColor = UIColorFromRGB(0xe4397d);
        lblGold.backgroundColor = [UIColor clearColor];
        [lblGold sizeToFit];
        [btn addSubview:lblGold];
        [view addSubview:btn];
    }
    [_views addObject:view];
}

-(void)btnClick:(id)sender
{
    UIButton *btn = (UIButton *)sender;
    if (tempBtn != nil)
    {
        [tempBtn setBackgroundColor:[UIColor clearColor]];
        [btn setBackgroundColor:UIColorFromRGB(0xdbebeb)];
    }    tempBtn = btn;
    
    GiftInfo *giftinfo = new GiftInfo();
    GetGiftInfoFromLocal(btn.tag, giftinfo);
    NSInteger price = giftinfo->cash;
    [_delegate GiftClick:btn.tag :price];
}

@end

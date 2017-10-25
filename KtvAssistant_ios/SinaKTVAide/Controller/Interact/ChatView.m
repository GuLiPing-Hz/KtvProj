//
//  PublicChatView.m
//  chat_rebot
//
//  Created by test123 on 14-3-26.
//  Copyright (c) 2014年 Lipeng. All rights reserved.
//

#import "ChatView.h"
#import "UserClass.h"
#import "NSString+Regex.h"
#import "UIImageView+Cache.h"
#import "PopoverView.h"
#import "HttpRequest.h"
#import "AudienceView.h"

@implementation ChatView

- (id)initWithFrame:(CGRect)frame roomid:(NSInteger)roomid
{
    self = [super initWithFrame:frame];
    if (self) {
        self.roomidx = roomid;
        
        /**
         *  用来显示彩条
         */
        colourBars = [[NSDictionary alloc] initWithObjectsAndKeys:
                      @"真好听", @"/zht",
                      @"赞", @"/zan",
                      @"你真棒", @"/tbl",
                      @"非常好听", @"/fcht",
                      @"啪啪啪", @"/ppp",
                      @"加油", @"/jy",
                      @"红心", @"/hx",
                      @"太感动", @"/tgd",
                      @"太强悍", @"/tqh",
                      @"冒个泡", @"/mgp",nil];
        
        nc = UIColorFromRGBWithAlpha(0xe6975e, 1);
        ic = UIColorFromRGBWithAlpha(0x979797, 1);
        tf = [UIFont systemFontOfSize:13.0f];
        
        message = [[NSMutableArray alloc] init];
        
        tableview = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)];
        tableview.backgroundColor = [UIColor whiteColor];
        tableview.delegate = self;
        tableview.dataSource = self;
        tableview.autoresizingMask = UIViewAutoresizingFlexibleHeight;
        tableview.separatorStyle = UITableViewCellSeparatorStyleNone;
        [tableview setAllowsSelection:NO];
        [self addSubview:tableview];
    }
    return self;
}

-(NSString*)colourBarsContent:(NSString*)content
{
    NSString* msg = content;
    for (NSString* key in [colourBars allKeys]) {
        if( [content rangeOfString:key].location != NSNotFound) {
            NSString* value = [colourBars objectForKey:key];
            msg = [msg stringByReplacingOccurrencesOfString:key withString:value];
            break;
        }
    }
    
    NSRange range = [msg rangeOfString:@"fontoptions{color:#"];
    if( range.location != NSNotFound)
    {
        range.length = range.location;
        range.location = 0;
        msg = [msg substringWithRange:range];
    }
    return msg;
}

/**
 *  自动适配屏幕大小
 */
- (CGRect)adapterScreen:(UIView *)view
{
    CGRect rect = view.frame;
    if (GetXW(rect) > _adapterWidth) {
        [view setFrame:CGRectMake(0.0f, GetY(rect)+max_Height, GetW(rect), GetH(rect))];
        max_Height = 16.0f;
    }
    return view.frame;
}

/**
 *  创建VIP等级 和 用户等级 改
 */
- (CGRect)createIcon:(int)vip level:(int)level rect:(CGRect)rect cell:(UIView *)cell source:(int)source identity:(int)identity
{
    UIImageView* img = nil;
    //创建手机图标 - 用户来源 0 PC用户，1安卓用户，2 Iphone用户，3 Ipad用户，4 客户端用户
    if (source == 1) {
        img = [[UIImageView alloc] initWithFrame:CGRectMake(GetXW(rect)+kSpace, GetY(rect), 10, 13)];
        img.image = [UIImage imageNamed:@"android_hint"];
        [cell addSubview:img];
        rect = [self adapterScreen:img];
    } else if (source == 2) {
        img = [[UIImageView alloc] initWithFrame:CGRectMake(GetXW(rect)+kSpace, GetY(rect), 10, 13)];
        img.image = [UIImage imageNamed:@"iPhone_hint"];
        [cell addSubview:img];
        rect = [self adapterScreen:img];
    }
    
    //创建身份，显示 “房主” / “超管” / “巡管” 图标
    switch (identity) {
        case Master:    // 房主
            img = [[UIImageView alloc] initWithFrame:CGRectMake(GetXW(rect)+kSpace, GetY(rect), 0, 0)];
            img.image = [UIImage imageNamed:@"anchor"];
            [img sizeToFit];
            [cell addSubview:img];
            return [self adapterScreen:img]; //若果是房主直接返回
        case Constable: // 超管
            img = [[UIImageView alloc] initWithFrame:CGRectMake(GetXW(rect)+kSpace, GetY(rect), 31, 13)];
            img.image = [UIImage imageNamed:@"chaoguan"];
            [cell addSubview:img];
            rect = [self adapterScreen:img];
            break;
        case Patrol:    // 巡管
            img = [[UIImageView alloc] initWithFrame:CGRectMake(GetXW(rect)+kSpace, GetY(rect), 31, 13)];
            img.image = [UIImage imageNamed:@"xunguan"];
            [cell addSubview:img];
            rect = [self adapterScreen:img];
            break;
    }
    
    //创建 userLevel
    if (level >= 1 && level <= 28) {
        img = [[UIImageView alloc] initWithFrame:CGRectMake(GetXW(rect)+kSpace, GetY(rect), kW, kH)];
        img.image = [UIImage imageNamed:[NSString stringWithFormat:@"u_level.bundle/v%d.png",level]];
        [cell addSubview:img];
        rect = [self adapterScreen:img];
    }
    
    //创建 VIP
    if (vip >= 1 && vip <= 5) {
        img = [[UIImageView alloc] initWithFrame:CGRectMake(GetXW(rect)+kSpace, GetY(rect), 30, kH)];
        img.image = [UIImage imageNamed:[NSString stringWithFormat:@"vip_%d", vip]];
        [cell addSubview:img];
        rect = [self adapterScreen:img];
    }
    return rect;
}

/**
 *  创建文字标签 改
 */
- (CGRect)createLbl:(NSString *)title rect:(CGRect)rect font:(UIFont *)font tc:(UIColor *)tc cell:(UIView *)cell flag:(BOOL)flag idx:(int)idx
{
    UILabel* lbl = [[UILabel alloc] initWithFrame:CGRectMake(GetXW(rect), GetY(rect), 5, 5)];
    if (flag) {
        [lbl setFrame:CGRectMake(GetXW(rect)+kSpace, GetY(rect), 5, 5)];
        if ([UserClass instance].idx != idx && idx >= 0) {
            lbl.tag = idx;
            lbl.userInteractionEnabled = YES;
            UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapped:)];
            [lbl addGestureRecognizer:tap];
        }
    }
    lbl.text = title;
    lbl.font = font;
    lbl.textColor = tc;
    lbl.backgroundColor = [UIColor clearColor];
    [lbl sizeToFit];
    [cell addSubview:lbl];
//    [lbl.layer setBorderColor:[[UIColor redColor] CGColor]];
//    [lbl.layer setBorderWidth:1];
    
    return [self adapterScreen:lbl];
}

/**
 *  创建礼物 改
 */
- (CGRect)createGift:(int)idx rect:(CGRect)rect cell:(UIView *)cell
{
    UIImageView* img = [[UIImageView alloc] initWithFrame:CGRectMake(GetXW(rect)+kSpace, GetY(rect), 22, 22)];
    [img setImageWithGiftIdx:idx placeholder:@"gift_icon"];
    [cell addSubview:img];
    max_Height = (max_Height > 22) ? max_Height : 22;
    return [self adapterScreen:img];
}

/**
 *  创建内容
 */
- (CGRect)createContent:(NSString *)content rect:(CGRect)rect font:(UIFont *)font  tc:(UIColor *)tc cell:(UIView *)cell
{
    rect.origin.x += kSpace;
    content = [self colourBarsContent:content];
    
    NSMutableArray* array = [content regexfaceContent];
    if(array)
    {
        int count = [array count];
        for (NSUInteger i = 0; i < count; i ++) {
            NSString* text = [array objectAtIndex:i];
            if (([text hasPrefix:@"[a/"] && [text hasSuffix:@"]"]) ||
                ([text hasPrefix:@"[b/"] && [text hasSuffix:@"]"]) ||
                ([text hasPrefix:@"[c/"] && [text hasSuffix:@"]"])) { //[b/23]
                
                NSString *imgn = [text substringWithRange:NSMakeRange(1, text.length - 2)];
                NSString* p = [NSString stringWithFormat:@"emot.bundle/%@.gif", imgn];
                CGSize image = [[UIImage imageNamed:p] size];
                NSData* gif = [NSData dataWithContentsOfFile:[[NSBundle mainBundle] pathForResource:p ofType:nil]];
                UIWebView* face = [[UIWebView alloc] initWithFrame:CGRectMake(GetXW(rect), GetY(rect), image.width, image.height)];
                face.userInteractionEnabled = NO;
                [face loadData:gif MIMEType:@"image/gif" textEncodingName:nil baseURL:nil];
                [cell addSubview:face];
                rect = [self adapterScreen:face];
                max_Height = (max_Height > GetH(rect)) ? max_Height : GetH(rect);
            } else {
                for (int j = 0; j < [text length]; j++) {
                    NSString *tmpText = [text substringWithRange:NSMakeRange(j, 1)];
                    rect = [self createLbl:tmpText rect:rect font:font tc:tc cell:cell flag:NO idx:0];
                }
            }
        }
    }
    else //全部是文字
    {
        for (int j = 0; j < [content length]; j++) {
            NSString *tmpText = [content substringWithRange:NSMakeRange(j, 1)];
            rect = [self createLbl:tmpText rect:rect font:font tc:tc cell:cell flag:NO idx:0];
        }
    }
    rect.size.height = (max_Height > GetH(rect)) ? max_Height : GetH(rect);
    return rect;
}

/**
 *  定制消息视图 改
 */
- (CGFloat)customPublic:(SenderMessage *)msg rect:(CGRect)rect cell:(UIView *)cell
{
    if (msg) {
        CGRect r1 = [self createIcon:msg.vipLevel level:msg.userLevel rect:rect cell:cell source:msg.source identity:msg.identity];
        CGRect r2 =  [self createLbl:msg.nickname rect:r1 font:tf tc:nc cell:cell flag:YES idx:msg.idx];
        CGRect r3 = [self createContent:msg.content rect:r2 font:tf tc:ic cell:cell];
        return GetYH(r3);
    }
    return 0.f;
}

- (CGFloat)customShow:(ReceiverMessage *)msg rect:(CGRect)rect cell:(UIView *)cell
{
    if (msg) {
        CGRect r1 = [self createLbl:@"【系统消息】" rect:rect font:tf tc:ic cell:cell flag:NO idx:0];
        CGRect r2 = [self createIcon:msg.recVipLevel level:msg.recUserLevel rect:r1 cell:cell source:msg.recSource identity:msg.recIdentity];
        CGRect r3 = [self createLbl:msg.recNickname rect:r2 font:tf tc:nc cell:cell flag:YES idx:msg.recIdx];
        r3.origin.x += kSpace;
        CGRect r4 = [self createLbl:@"被" rect:r3 font:tf tc:ic cell:cell flag:NO idx:0];
        CGRect r5 = [self createIcon:msg.vipLevel level:msg.userLevel rect:r4 cell:cell source:msg.source identity:msg.identity];
        CGRect r6 = [self createLbl:msg.nickname rect:r5 font:tf tc:nc cell:cell flag:YES idx:msg.idx];
        CGRect r7 = [self createContent:msg.content rect:r6 font:tf tc:ic cell:cell];
        return GetYH(r7);
    }
    return 0.f;
}

- (CGFloat)customBomb:(ReceiverMessage *)msg rect:(CGRect)rect cell:(UIView *)cell
{
    if (msg) {
        CGRect r1 = [self createLbl:@"【系统消息】" rect:rect font:tf tc:ic cell:cell flag:NO idx:0];
        CGRect r2 = [self createIcon:msg.recVipLevel level:msg.recUserLevel rect:r1 cell:cell source:msg.recSource identity:msg.recIdentity];
        CGRect r3 = [self createLbl:msg.recNickname rect:r2 font:tf tc:nc cell:cell flag:YES idx:msg.recIdx];
        r3.origin.x += kSpace;
        CGRect r4 = [self createLbl:@"被" rect:r3 font:tf tc:ic cell:cell flag:NO idx:0];
        CGRect r5 = [self createIcon:msg.vipLevel level:msg.userLevel rect:r4 cell:cell source:msg.source identity:msg.identity];
        CGRect r6 = [self createLbl:msg.nickname rect:r5 font:tf tc:nc cell:cell flag:YES idx:msg.idx];
        CGRect r7 = [self createContent:msg.content rect:r6 font:tf tc:nc cell:cell];
        return GetYH(r7);
    }
    return 0.f;
}

/**
 *  定制礼物 改
 */
- (CGFloat)customGift:(ReceiverMessage *)msg rect:(CGRect)rect cell:(UIView *)cell
{
    if (msg) {
        CGRect r1 = [self createIcon:msg.vipLevel level:msg.userLevel rect:rect cell:cell source:msg.source identity:msg.identity];
        CGRect r2 = [self createLbl:msg.nickname rect:r1 font:tf tc:nc cell:cell flag:YES idx:msg.idx];
        r2.origin.x += kSpace;
        CGRect r3 = [self createLbl:@"送给" rect:r2 font:tf tc:ic cell:cell flag:NO idx:0];
        CGRect r4 = [self createIcon:msg.recVipLevel level:msg.recUserLevel rect:r3 cell:cell source:msg.recSource identity:msg.recIdentity];
        CGRect r5 = [self createLbl:msg.recNickname rect:r4 font:tf tc:nc cell:cell flag:YES idx:msg.recIdx];
        CGRect r6 = [self createContent:[NSString stringWithFormat:@"%d个", msg.num] rect:r5 font:tf tc:ic cell:cell];
        CGRect r7 = [self createGift:msg.giftIdx rect:r6 cell:cell];
        return GetYH(r7);
    }
    return 0.f;
}

- (CGFloat)customPrivate:(ReceiverMessage *)msg rect:(CGRect)rect cell:(UIView *)cell p2p:(BOOL)p2p
{
    if (msg) {
        CGRect r1 = [self createIcon:msg.vipLevel level:msg.userLevel rect:rect cell:cell source:msg.source identity:msg.identity];
        CGRect r2 = [self createLbl:msg.nickname rect:r1 font:tf tc:nc cell:cell flag:YES idx:msg.idx];
        r2.origin.x += kSpace;
        CGRect r3 = [self createLbl:@"对" rect:r2 font:tf tc:ic cell:cell flag:NO idx:0];
        CGRect r4 = [self createIcon:msg.recVipLevel level:msg.recUserLevel rect:r3 cell:cell source:msg.recSource identity:msg.recIdentity];
        CGRect r5 = [self createLbl:msg.recNickname rect:r4 font:tf tc:nc cell:cell flag:YES idx:msg.recIdx];
        NSString* content = [NSString stringWithFormat:((p2p)?@"悄悄的说：%@":@"说：%@"), msg.content];
        CGRect r6 = [self createContent:content rect:r5 font:tf tc:ic cell:cell];
        return GetYH(r6);
    }
    return 0.f;
}

- (CGFloat)customFireworks:(ReceiverMessage *)msg rect:(CGRect)rect cell:(UIView *)cell
{
    if (msg) {
        CGRect r1 = [self createLbl:@"【系统消息】" rect:rect font:tf tc:ic cell:cell flag:NO idx:0];
        CGRect r2 = [self createIcon:msg.vipLevel level:msg.userLevel rect:r1 cell:cell source:msg.source identity:msg.identity];
        CGRect r3 = [self createLbl:msg.nickname rect:r2 font:tf tc:nc cell:cell flag:YES idx:msg.idx];
        CGRect r4 = [self createLbl:@"燃放" rect:r3 font:tf tc:ic cell:cell flag:NO idx:0];
        CGRect r5 = [self createGift:msg.giftIdx rect:r4 cell:cell];
        CGRect r6 = [self createIcon:msg.recVipLevel level:msg.recUserLevel rect:r5 cell:cell source:msg.recSource identity:msg.recIdentity];
        CGRect r7 = [self createLbl:msg.recNickname rect:r6 font:tf tc:nc cell:cell flag:NO idx:0];
        NSString* coin = [NSString stringWithFormat:@"已经获得%d个秀币！", msg.num];
        CGRect r8 = [self createContent:coin rect:r7 font:tf tc:ic cell:cell];
        return GetYH(r8);
    }
    return 0.f;
}

- (CGFloat)customLuckygift:(ReceiverMessage *)msg rect:(CGRect)rect cell:(UIView *)cell
{
    if (msg) {
        CGRect r1 = [self createLbl:@"【系统消息】恭喜" rect:rect font:tf tc:ic cell:cell flag:NO idx:0];
        CGRect r2 = [self createIcon:msg.recVipLevel level:msg.recUserLevel rect:r1 cell:cell source:msg.recSource identity:msg.recIdentity];
        CGRect r3 = [self createLbl:msg.recNickname rect:r2 font:tf tc:nc cell:cell flag:YES idx:msg.recIdx];
        r3.origin.x += kSpace;
        CGRect r4 = [self createLbl:@"喜中" rect:r3 font:tf tc:ic cell:cell flag:NO idx:0];
        CGRect r5 = [self createGift:msg.giftIdx rect:r4 cell:cell];
        CGRect r6 = [self createContent:msg.content rect:r5 font:tf tc:ic cell:cell];
        return GetYH(r6);
    }
    return 0.f;
}

- (CGFloat)creategiftstarinfo:(NSString *)name cell:(UIView *)cell
{
    self.adapterWidth = 160;
    UIFont* f = [UIFont systemFontOfSize:15.0f];
    UIColor* c = UIColorFromRGBWithAlpha(0xe6975e, 1);
    CGRect r1 = [self createLbl:@"恭喜" rect:CGRectZero font:f tc:[UIColor whiteColor] cell:cell flag:NO idx:0];
    CGRect r2 = [self createLbl:name rect:r1 font:f tc:c cell:cell flag:NO idx:0];
    r2.origin.x -= kSpace;
    CGRect r3 = [self createContent:@"获得礼物之星！" rect:r2 font:f tc:[UIColor whiteColor] cell:cell];
    return GetYH(r3);
}

/**
 *  进场特效
 */
- (CGFloat)customEnterEffect:(SenderMessage *)msg rect:(CGRect)rect cell:(UIView *)cell
{
    if(msg) {
//        for (id ctr in [cell subviews]) {
//            [ctr removeFromSuperview];
//            NSLog(@"removeFromSuperview 时间...");
//        }
//        UIImageView* img = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 300, 200)];
//        [img setImageWithUrl:Effect([msg.reserved intValue]) placeholder:nil];
//        [cell addSubview:img];
//        CGRect time = [self createLbl:msg.time rect:CGRectMake(0, 200, 0, 0) font:tf tc:ic cell:cell flag:NO idx:0];
        
        rect = [self createLbl:@"【系统消息】" rect:rect font:tf tc:ic cell:cell flag:NO idx:0];
        rect = [self createIcon:msg.vipLevel level:msg.userLevel rect:rect cell:cell source:msg.source identity:msg.identity];
        rect = [self createLbl:msg.nickname rect:rect font:tf tc:nc cell:cell flag:YES idx:msg.idx];
        rect.origin.x += kSpace;
        rect = [self createLbl:@"开着" rect:rect font:tf tc:ic cell:cell flag:NO idx:0];
        rect = [self createLbl:msg.content rect:rect font:tf tc:[UIColor redColor] cell:cell flag:NO idx:0];
        rect = [self createContent:@"进入房间" rect:rect font:tf tc:ic cell:cell];
        UIImageView* img = [[UIImageView alloc] initWithFrame:CGRectMake(GetXW(rect)+kSpace, GetY(rect), 150, 100)];
        [img setImageWithUrl:Effect([msg.reserved intValue]) placeholder:nil];
        [cell addSubview:img];
        rect = [self adapterScreen:img];
        return GetYH(rect);
    }
    return 0.f;
}

- (UIView *)customMessage:(BaseMessage *)msg
{
    if (msg) {
        self.adapterWidth = GetW(self.frame)-LRSPACE*2;
        max_Height = 16.0f;
        
        UIView *cell= [[UIView alloc] initWithFrame:CGRectZero];
        CGFloat cell_height = 0.0f;
        CGRect time = [self createLbl:msg.time rect:CGRectZero font:tf tc:ic cell:cell flag:NO idx:0];
        switch (msg.operateCode) {
            case Public_chat_1100://公聊
                msg.content = [NSString stringWithFormat:@"说：%@", msg.content];
                cell_height = [self customPublic:(SenderMessage *)msg rect:time cell:cell];
                break;
            case Notice_newToAll_1021: //显示进入
                cell_height = [self customPublic:(SenderMessage *)msg rect:time cell:cell];
                break;
            case Send_giftSuccess_1401: //送礼物
                cell_height = [self customGift:(ReceiverMessage *)msg rect:time cell:cell];
                break;
            case Private_chat_1200://‘VIP+昵称 对 VIP+昵称 说：’/‘我 对 VIP+昵称 说：’、‘VIP+昵称 对 我 说：’
                cell_height = [self customPrivate:(ReceiverMessage *)msg rect:time cell:cell p2p:NO];
                break;
            case P2p_chat_1300: //悄悄话
                cell_height = [self customPrivate:(ReceiverMessage *)msg rect:time cell:cell p2p:YES];
                break;
            case Fireworks_notice_1405://烟花道具，得币通知
                cell_height = [self customFireworks:(ReceiverMessage *)msg rect:time cell:cell];
                break;
            case Lucky_gift_1410: //（幸运礼物，中奖500倍通知）ack
            case Lucky_gift_1420: //（幸运礼物，非500倍中奖通知）ack
                cell_height = [self customLuckygift:(ReceiverMessage *)msg rect:time cell:cell];
                break;
            case Notice_user_bomb_7000: //通知房间内用户，XX被 炸出房间
                cell_height = [self customBomb:(ReceiverMessage *)msg rect:time cell:cell];
                break;
            case Kickedout_success_1901://踢人成功
            case Banned_success_1701: //禁言成功
            case Return_toSpeak_success_1801: //恢复发言成功
                cell_height = [self customShow:(ReceiverMessage *)msg rect:time cell:cell];
                break;
            case New_adv_2701: //新公告
            case Room_info_1001: //本房间消息
            case Room_sysInfo_1104: { //本房间系统消息
                CGRect rect = [self createContent:msg.content rect:time font:tf tc:ic cell:cell];
                cell_height = GetYH(rect);
                break;
            }
            case Enter_effect_1028: //进场特效
                cell_height = [self customEnterEffect:(SenderMessage *)msg rect:time cell:cell];
                break;
            default:
                break;
        }
        if (cell_height <= 0.0f) return nil;
        [cell setFrame:CGRectMake(LRSPACE, 0, GetW(self.frame)-LRSPACE*2, cell_height)];
        return cell;
    }
    return nil;
}

- (void)pushMessage:(BaseMessage *)msg
{
    //公聊私聊最多展示100条公聊信息，超过则删除之前的前50条公聊信息
    if ([message count] >= 100 ) {
        for (NSInteger i = 49; i >= 0; i --) {
            [message removeObjectAtIndex:i];
        }
        [tableview reloadData];
        [tableview scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:[message count]-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
    }
    
    UIView* cellView = [self customMessage:msg];
    if (cellView) {
        [message addObject:cellView];
        if (!self.isHidden) {
            NSIndexPath* indexPath = [NSIndexPath indexPathForRow:[message count]-1 inSection:0];
            [tableview insertRowsAtIndexPaths:[NSArray arrayWithObject:indexPath] withRowAnimation:UITableViewRowAnimationNone];
            [tableview scrollToRowAtIndexPath:indexPath atScrollPosition:UITableViewScrollPositionBottom animated:NO];
        }
    }
}

- (void)setHidden:(BOOL)hidden
{
    if (!hidden) {
        [tableview reloadData];
        if ([message count] > 3) {
            [tableview scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:[message count]-1 inSection:0] atScrollPosition:UITableViewScrollPositionBottom animated:NO];
        }
    }
    [super setHidden:hidden];
}

- (void)clearMessage
{
    if (message.count) {
        @synchronized (message) {
            [message removeAllObjects];
            [tableview reloadData];
        }
    }
}

#pragma mark - Table view data source
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return message.count;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UIView* cellView = [message objectAtIndex:indexPath.row];
    return cellView.frame.size.height+5.0f;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    } else {
        for (UIView* subview in cell.contentView.subviews) {
            [subview removeFromSuperview];
        }
    }
    NSLog(@"聊天位：row=%d, count=%d", indexPath.row, message.count);
    
    if (indexPath.row < message.count) {
        UIView* cellView = [message objectAtIndex:indexPath.row];
        [cell.contentView addSubview:cellView];
    }
    return cell;
}

-(void)tapped:(UITapGestureRecognizer *)tap
{
    UILabel* lbl = (UILabel *)tap.view;
    NSArray* data = [NSArray arrayWithObjects:@"赠送礼物", @"与TA聊天", @"踢出房间", @"禁言", @"恢复发言", nil];
    PopoverView* pv = [[PopoverView alloc] initWithSize:CGSizeMake(120, 180) data:data title:lbl.text];
    [pv setClickCallback:^(NSUInteger index, NSString* title) {
        if (_delegate && [_delegate respondsToSelector:@selector(chatViewOperateWithIndex:idx:name:)]) {
            [_delegate chatViewOperateWithIndex:index idx:lbl.tag name:lbl.text];
        }
    }];
    [pv showAtPoint:[tap locationInView:self] inView:self orientation:ArrowDirectionLeftAndRight];
}

- (void)dealloc
{
#if DEBUG
    NSLog(@"%@ dealloc", self);
#endif
}

@end

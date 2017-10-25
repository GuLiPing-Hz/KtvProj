//
//  InteractCellTableViewCell.m
//  SinaKTVAide
//
//  Created by cxp on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "PublicChatCell.h"
#import "User.h"
#import "UserSessionManager.h"
#import "com_ios_NetUtil.h"
#import "KTVBaseConfig.h"
#import "NSString+Regex.h"
#import "NSMutableAttributedString+Weibo.h"
#import "NetUtilCallBack.h"


@implementation PublicChatCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        [self setFrame:CGRectMake(0, 0, _MainScreen_Width, 0)];
        talkContentView = [[UIView alloc] init];
        talkContentView.backgroundColor = [UIColor clearColor];
        [self addSubview:talkContentView];
        
        imgHead =[[UIImageView alloc] init];
        imgHead.backgroundColor = [UIColor clearColor];
        imgHead.layer.masksToBounds = YES;
        imgHead.layer.cornerRadius = 22;
        imgHead.userInteractionEnabled = YES;
        imgHead.hidden = YES;
        [self addSubview:imgHead];
        
        lblName = [[UILabel alloc] init];
        lblName.backgroundColor = [UIColor clearColor];
        lblName.font = [UIFont systemFontOfSize:14];
        lblName.textColor = UIColorFromRGB(0xe4397d);
        lblName.userInteractionEnabled = YES;
        lblName.hidden = YES;
        [self addSubview:lblName];
        
        imgBack =[[UIImageView alloc] init];
        imgBack.backgroundColor = [UIColor clearColor];
        [talkContentView addSubview:imgBack];
        imgBack.userInteractionEnabled = YES;
        
        hbBtn =[[UIButton alloc] init];
        hbBtn.backgroundColor = [UIColor clearColor];
        [hbBtn addTarget:self action: @selector(onClickImage:) forControlEvents:UIControlEventTouchUpInside];
        [talkContentView addSubview:hbBtn];
        
        textContainer = [[UIView alloc]init];
        textContainer.backgroundColor = [UIColor clearColor];
        [talkContentView addSubview:textContainer];
        
        _textView = [[CoreTextView alloc] initWithFrame:CGRectMake(18, 11, 200, 50)];
        _textView.backgroundColor = [UIColor clearColor];
        [textContainer addSubview:_textView];
        
        gift = [[UIImageView alloc] init];
        gift.backgroundColor = [UIColor clearColor];
        gift.hidden = YES;
        [talkContentView addSubview:gift];
        
        lblroomname = [[UILabel alloc]init];
        lblroomname.backgroundColor = [UIColor clearColor];
        lblroomname.font = [UIFont systemFontOfSize:11];
        lblroomname.hidden = YES;
        [talkContentView addSubview:lblroomname];
        
        lblRedtype = [[UILabel alloc] initWithFrame:CGRectMake(10, 72, 60, 20)];
        lblRedtype.font = [UIFont systemFontOfSize:13.0f];
        lblRedtype.textColor = UIColorFromRGB(0x999999);
        lblRedtype.hidden = YES;
        lblRedtype.backgroundColor = [UIColor clearColor];
        [talkContentView addSubview:lblRedtype];
        
        lblget = [[UILabel alloc]initWithFrame:CGRectMake(52, 40, 60, 15)];
        lblget.textColor = UIColorFromRGB(0xffffff);
        lblget.text = @"领取红包";
        lblget.font = [UIFont systemFontOfSize:13.0f];
        lblget.backgroundColor = [UIColor clearColor];
        lblget.hidden = YES;
        [talkContentView addSubview:lblget];
        
        lblmark = [[UILabel alloc] init];
        lblmark.textColor = UIColorFromRGB(0xffffff);
        lblmark.font = [UIFont systemFontOfSize:14.0f];
        lblmark.backgroundColor = [UIColor clearColor];
        lblmark.hidden = YES;
        [talkContentView addSubview:lblmark];
        
        lblsys = [[UILabel alloc] init];
        lblsys.font = [UIFont systemFontOfSize:14.0f];
        lblsys.textColor = UIColorFromRGB(0x5c606a);
        lblsys.backgroundColor = [UIColor clearColor];
        lblsys.numberOfLines = 0;
        lblsys.hidden = YES;
        [talkContentView addSubview:lblsys];
        
        imgbroad =[[UIImageView alloc] init];
        imgbroad.backgroundColor = [UIColor clearColor];
        imgbroad.layer.masksToBounds = YES;
        imgbroad.layer.cornerRadius = 10;
        imgbroad.userInteractionEnabled = YES;
        imgbroad.hidden = YES;
        [self addSubview:imgbroad];
        
    }
    return self;
}

-(void)onClickImage:(id)sender{
    if (_Info.type != 8) {
        return;
    }
    
    if(self.mHBDelegate != nil)
        [self.mHBDelegate onHBClick:self.Info];
}

-(void)imgChange
{
    if (lblName.hidden && _Info.type != 8) {
        return;
    }
    
    //屏蔽系统用户
    if(self.Info.idx == 1)
        return;
    
    if([self.mHBDelegate respondsToSelector:@selector(onUserHeadClick:view:name:idx:)])
    {
        [self.mHBDelegate onUserHeadClick:CGPointMake(0, 0) view:self name:lblName.text idx:self.Info.idx];
    }
    /*
     NSString *uid = [NSString stringWithFormat:@"%d",_Info.idx];
     NSString *y = [NSString stringWithFormat:@"%d",imgHead.tag];

    NSDictionary * userInfo;
    userInfo = [NSDictionary dictionaryWithObjectsAndKeys:y,@"y",lblName.text,@"username",uid,@"userid", nil];
    [[NSNotificationCenter defaultCenter] postNotificationName:KTVASSISTANT_CHAT_IMG_LABEL_CLICK_NOTIFICATION object:nil userInfo:userInfo];
     */
}

-(void)setY:(NSInteger)y
{
    _y = y;
    imgHead.tag = _y;
}

-(void)setIsShow:(BOOL)isShow
{
    _isShow = isShow;
}

-(void)setInfo:(AllInfo)Info
{
    _Info = Info;
    imgBack.hidden = YES;
    imgHead.hidden = YES;
    gift.hidden = YES;
    lblName.hidden = YES;
    lblroomname.hidden = YES;
    textContainer.hidden = YES;
    lblRedtype.hidden = YES;
    lblget.hidden = YES;
    lblmark.hidden = YES;
    imgbroad.hidden = YES;
    lblsys.hidden = YES;
    hbBtn.hidden = YES;
    
    if (Info.idx == 0 || Info.idx == [UserSessionManager getInstance].currentUser.uid) {
        [self myChat];
    }
    else
    {
        [self someoneChat];
        
        UITapGestureRecognizer *imgtapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(imgChange)];
        imgtapGesture.cancelsTouchesInView = NO;
        [imgHead addGestureRecognizer:imgtapGesture];
    }
    
}

//创建别人内容
-(void)someoneChat
{
    NSInteger contentWidth = 225;
    NSInteger contentheight = 0;
    
    NSString *sayname = [NSString stringWithCString:_Info.name encoding:NSUTF8StringEncoding];
    NSString *headphoto = [NSString stringWithUTF8String:_Info.headphoto] ;
    
    lblName.text = sayname;
    CGSize nameSize = [lblName.text sizeWithFont:[UIFont systemFontOfSize:14] constrainedToSize:CGSizeMake(MAXFLOAT, 20)];
    if (_y == 0 && _isShow) {
        lblName.frame = CGRectMake(65, 63, nameSize.width, nameSize.height);
        imgHead.frame = CGRectMake(11, 64, 44, 44);
        imgbroad.frame = CGRectMake(35, 97, 20, 20);
    }
    else
    {
        lblName.frame = CGRectMake(65, 6, nameSize.width, nameSize.height);
        imgHead.frame = CGRectMake(11, 7, 44, 44);
        imgbroad.frame = CGRectMake(35, 40, 20, 20);
    }
    //头像
    
    if(self.Info.idx == 1)//系统专用头像
    {
        [imgHead setImage:[UIImage imageNamed:@"ic_system_profile"]];
    }
    else//用户头像
    {
        [imgHead setImageWithURL:[NSURL URLWithString:headphoto] placeholderImage:[UIImage imageNamed:@"userFace_normal.png"]];
    }
    
    imgbroad.image = [UIImage imageNamed:@"face_icon_speaker_0"];
    talkContentView.backgroundColor = [UIColor clearColor];
    
    //聊天内容背景
    switch (_Info.type) {
        case 0:
            {
                imgBack.hidden = NO;
                imgHead.hidden = NO;
                lblName.hidden = NO;
                lblroomname.hidden = NO;
                textContainer.hidden = NO;
                imgbroad.hidden = YES;
                imgBack.image = [[UIImage imageNamed:@"talk_bubble0"] stretchableImageWithLeftCapWidth:22 topCapHeight:19 ];
            }
            break;
        case 1:
            {
                imgBack.hidden = NO;
                imgHead.hidden = NO;
                lblName.hidden = NO;
                lblroomname.hidden = NO;
                textContainer.hidden = NO;
                imgbroad.hidden = YES;
                imgBack.image = [[UIImage imageNamed:@"talk_bubble3"] stretchableImageWithLeftCapWidth:22 topCapHeight:19 ];
            }
            break;
        case 3:
            {
                imgBack.hidden = NO;
                imgHead.hidden = NO;
                lblName.hidden = NO;
                lblroomname.hidden = NO;
                textContainer.hidden = NO;
                imgbroad.hidden = YES;
                imgBack.image = [[UIImage imageNamed:@"talk_bubble0"] stretchableImageWithLeftCapWidth:22 topCapHeight:19 ];
            }
            break;
        case 4:
            {
                imgSpeaker.hidden = NO;
                imgBack.hidden = NO;
                imgHead.hidden = NO;
                lblName.hidden = NO;
                lblroomname.hidden = YES;
                textContainer.hidden = NO;
                imgbroad.hidden = NO;
                imgBack.image = [[UIImage imageNamed:@"talk_bubble1"] stretchableImageWithLeftCapWidth:22 topCapHeight:19 ];
            }
            break;
        case 8:
            {
                imgBack.hidden = NO;
                imgHead.hidden = NO;
                lblName.hidden = YES;
                lblroomname.hidden = YES;
                textContainer.hidden = YES;
                lblRedtype.hidden = NO;
                lblget.hidden = NO;
                lblmark.hidden = NO;
                imgbroad.hidden = YES;
                lblsys.hidden = YES;
                hbBtn.hidden = NO;
                imgBack.hidden = YES;
                [hbBtn setBackgroundImage:[UIImage imageNamed:@"red_information_btn_0"] forState:UIControlStateNormal];
                [hbBtn setBackgroundImage:[UIImage imageNamed:@"red_information_btn_2"] forState:UIControlStateHighlighted];
            }
            break;
        default:
            {
                imgBack.hidden = YES;
                imgHead.hidden = YES;
                lblName.hidden = YES;
                lblroomname.hidden = YES;
                textContainer.hidden = YES;
                lblmark.hidden = YES;
                lblRedtype.hidden = YES;
                lblget.hidden = YES;
                imgbroad.hidden = YES;
                lblsys.hidden = NO;
            }
            break;
    }

    [self createRoomNameLabel:0];
    [self createTextContentLabel:0];
    
    if (_Info.type == 5 || _Info.type == 6 || _Info.type == 7) {
        return;
    }
    
    if (lblroomname.hidden)
        contentheight = 18 + _textView.frame.size.height;
    else
        contentheight = 27 + lblroomname.frame.size.height + _textView.frame.size.height;
    if (_Info.type == 3 || !lblroomname.hidden)
        contentWidth = 225;
    else
        contentWidth = _textView.frame.size.width + 25;
    
    if (_Info.type == 8) {
        lblName.hidden = YES;
        talkContentView.frame = CGRectMake(lblName.frame.origin.x - 9, 9, 215.5, 93);
        contentWidth = 215.5;
        contentheight = 111;
    }
    else
    {
        talkContentView.frame = CGRectMake(lblName.frame.origin.x - 9, lblName.frame.origin.y + lblName.frame.size.height + 5, contentWidth, contentheight);
    }
    lblroomname.frame = CGRectMake(talkContentView.frame.size.width-10-lblroomname.frame.size.width, talkContentView.frame.size.height- lblroomname.frame.size.height - 8, lblroomname.frame.size.width, lblroomname.frame.size.height);
    imgBack.frame = CGRectMake(0, 0, contentWidth,talkContentView.frame.size.height);
    hbBtn.frame = imgBack.frame;
}

//创建自己内容
-(void)myChat
{
    NSInteger contentWidth = 225;
    NSInteger contentheight = 0;
    
    //头像
    [imgHead setImageWithURL:[NSURL URLWithString:[UserSessionManager getInstance].currentUser.headphoto] placeholderImage:[UIImage imageNamed:@"userFace_normal.png"]];
    imgHead.frame = CGRectMake(self.frame.size.width - 55, 7, 44, 44);
    imgbroad.frame = CGRectMake(self.frame.size.width - 57, 40, 20, 20);
    imgbroad.image = [UIImage imageNamed:@"face_icon_speaker_1"];
    talkContentView.backgroundColor = [UIColor clearColor];
    //聊天内容背景
    switch (_Info.type) {
        case 0:
        case 1:
        case 3:
            {
                imgBack.hidden = NO;
                imgHead.hidden = NO;
                lblName.hidden = NO;
                lblroomname.hidden = NO;
                textContainer.hidden = NO;
                imgbroad.hidden = YES;
                imgBack.image = [[UIImage imageNamed:@"talk_bubble2b"] stretchableImageWithLeftCapWidth:13 topCapHeight:19 ];
            }
            break;
        case 4:
            {
                
                imgSpeaker.hidden = NO;
                imgBack.hidden = NO;
                imgHead.hidden = NO;
                lblName.hidden = NO;
                lblroomname.hidden = YES;
                textContainer.hidden = NO;
                imgbroad.hidden = NO;
                imgBack.image = [[UIImage imageNamed:@"talk_bubble1b"] stretchableImageWithLeftCapWidth:13 topCapHeight:19 ];
            }
            break;
        case 8:
            {
                imgBack.hidden = NO;
                imgHead.hidden = NO;
                lblName.hidden = NO;
                lblroomname.hidden = YES;
                textContainer.hidden = YES;
                lblRedtype.hidden = NO;
                lblget.hidden = NO;
                lblmark.hidden = NO;
                imgbroad.hidden = YES;
                hbBtn.hidden = NO;
                imgBack.hidden = YES;
                [hbBtn setBackgroundImage:[UIImage imageNamed:@"red_information_btn_1"] forState:UIControlStateNormal];
                [hbBtn setBackgroundImage:[UIImage imageNamed:@"red_information_btn_3"] forState:UIControlStateHighlighted];
            }
            break;
        default:
            {
                imgBack.hidden = YES;
                imgHead.hidden = YES;
                lblName.hidden = YES;
                lblroomname.hidden = YES;
                textContainer.hidden = YES;
                lblmark.hidden = YES;
                lblRedtype.hidden = YES;
                lblget.hidden = YES;
                imgbroad.hidden = YES;
                lblsys.hidden = NO;
            }
            break;
    }
    
    [self createRoomNameLabel:1];
    [self createTextContentLabel:1];
    if (_Info.type == 5 || _Info.type == 6 || _Info.type == 7) {
        return;
    }
    lblName.hidden = YES;
    if (lblroomname.hidden)
        contentheight = 18 + _textView.frame.size.height;
    else
        contentheight = 27 + lblroomname.frame.size.height + _textView.frame.size.height;
    if (_Info.type == 3 || !lblroomname.hidden)
        contentWidth = 225;
    else
        contentWidth = _textView.frame.size.width + 25;
    if (_Info.type == 8) {
        talkContentView.frame = CGRectMake(imgHead.frame.origin.x- 215.5, 9, 215.5, 93);
        contentWidth = 215.5;
        contentheight = 111;
    }
    else
        talkContentView.frame = CGRectMake(imgHead.frame.origin.x- contentWidth, 9, contentWidth, contentheight);
    lblroomname.frame = CGRectMake(talkContentView.frame.size.width-15-lblroomname.frame.size.width, talkContentView.frame.size.height- lblroomname.frame.size.height - 8, lblroomname.frame.size.width, lblroomname.frame.size.height);
    imgBack.frame = CGRectMake(0, 0, contentWidth,talkContentView.frame.size.height);
    hbBtn.frame = imgBack.frame;
}

//创建房间名字信息 type:0:他人信息 1:自己信息
-(void)createRoomNameLabel:(NSInteger) type
{
    NSString *roomname = nil;
    NSString *recvroomname = nil;
    CGSize roomNameSize;
    switch (_Info.type) {
        case 0:
        case 1:
            {
                roomname = [NSString stringWithUTF8String:_Info.roomname];
                recvroomname = [NSString stringWithUTF8String:_Info.recv_room];
                if (_Info.roomid == 0) {
                    lblroomname.hidden = YES;
                }
                else
                {
                    lblroomname.text = [NSString stringWithFormat:@"来自于-  %@",roomname];
                    roomNameSize = [lblroomname.text sizeWithFont:[UIFont systemFontOfSize:11] constrainedToSize:CGSizeMake(MAXFLOAT, 20)];
                }
            }
            break;
        case 3:
            {
                roomname = [NSString stringWithUTF8String:_Info.roomname];
                recvroomname = [NSString stringWithUTF8String:_Info.recv_room];
                lblroomname.hidden = NO;
                if (_Info.roomid > 0 && _Info.recv_roomid > 0)
                    lblroomname.text = [NSString stringWithFormat:@"从 %@  送到  %@",roomname,recvroomname];
                else if(_Info.roomid > 0 && _Info.recv_roomid <= 0)
                    lblroomname.text = [NSString stringWithFormat:@"从 %@  送出",roomname];
                else if(_Info.recv_roomid > 0 && _Info.roomid <= 0)
                    lblroomname.text = [NSString stringWithFormat:@"在 %@  收到",recvroomname];
                else
                {
                    lblroomname.text = @"";
                    lblroomname.hidden = YES;
                }
                roomNameSize = [lblroomname.text sizeWithFont:[UIFont systemFontOfSize:11] constrainedToSize:CGSizeMake(200, 11)];
            }
            break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            {
                return;
            }
        default:
            break;
    }
    if (roomNameSize.width >= 200)
        roomNameSize.width = 200;
    lblroomname.frame = CGRectMake(0, 0, roomNameSize.width, roomNameSize.height);
    if (type == 0 || _Info.type == 4)
        lblroomname.textColor = UIColorFromRGB(0x666666);
    else
        lblroomname.textColor = [UIColor whiteColor];
}
//创建信息内容  type:0:他人信息 1:自己信息
-(void)createTextContentLabel:(NSInteger) type
{
    NSInteger textWidth = 200;
    CGFloat textHegiht = 0;
    NSString *toname = [NSString stringWithUTF8String:_Info.toname];
    NSString *message = [NSString stringWithCString:_Info.content encoding:NSUTF8StringEncoding];
    NSString *name =[UserSessionManager getInstance].currentUser.name;
    
    switch (_Info.type) {
        case 1:
            {
                message = [NSString stringWithFormat:@"@%@ %@",toname,message];
            }
            break;
        case 3:
            {
                message = [NSString stringWithFormat:@"@%@ %@",toname,message];
                message = [message stringByReplacingOccurrencesOfString:name withString:@"我"];
                [self createGift:type];
            }
            break;
        case 5:
        case 6:
        case 7:
            {
                message = [NSString stringWithFormat:@"【系统消息】%@",message];
                lblsys.text = message;
                lblsys.lineBreakMode = UILineBreakModeWordWrap;
                CGSize size = [message sizeWithFont:[UIFont boldSystemFontOfSize:14.0f]
                                  constrainedToSize:CGSizeMake(_MainScreen_Width-66, MAXFLOAT)
                                       lineBreakMode:UILineBreakModeWordWrap];

                lblsys.frame = CGRectMake(8,7,size.width, size.height);
                talkContentView.frame = CGRectMake(25, 9, _MainScreen_Width-50, size.height+14);
                talkContentView.layer.cornerRadius = 7;
                talkContentView.backgroundColor = UIColorFromRGB(0xd5d5d5);
                return;
            }
            break;
        case 8:
            {
                if (type == 0) {
                    lblmark.frame = CGRectMake(58, 18, 148, 16);
                    lblget.frame = CGRectMake(58, 40, 60, 15);
                    lblRedtype.frame = CGRectMake(16, 72, 60, 20);
                }
                else
                {
                    lblmark.frame = CGRectMake(52, 18, 148, 16);
                    lblget.frame = CGRectMake(52, 40, 60, 15);
                    lblRedtype.frame = CGRectMake(10, 72, 60, 20);
                }
                lblmark.text = message;
                switch ((int)_Info.redtype) {
                    case 0:
                        lblRedtype.text = @"个人红包";
                        break;
                    case 1:
                        lblRedtype.text = @"包厢红包";
                        break;
                    case 3:
                        lblRedtype.text = @"大款红包";
                        break;
                    default:
                        lblRedtype.text = @"红包";
                        break;
                }
                return;
            }
        default:
            break;
    }
    
    if (message.length)
    {
        NSInteger colorType = 0;
        if(type == 1 && _Info.type != 4)
            colorType = 1;
        if (type == 0 && _Info.type == 1)
            colorType = 2;
        
        //取最长的一行
        NSArray *newarray;
        if (_Info.type == 4) {
            NSArray *array = [message componentsSeparatedByString:@"\n"];
            newarray = [array sortedArrayUsingComparator:^NSComparisonResult(id obj1, id obj2) {
                NSUInteger len0 = [(NSString *)obj1 length];
                NSUInteger len1 = [(NSString *)obj2 length];
                return len0 > len1 ? NSOrderedAscending : NSOrderedDescending;
            }];
        }
        NSString *sortedmsg = [newarray objectAtIndex:0];
        for (int i = 1; i < newarray.count; i++) {
            NSString *string = [newarray objectAtIndex:i];
            sortedmsg = [sortedmsg stringByAppendingString:@"\n"];
            sortedmsg = [sortedmsg stringByAppendingString:string];
        }
    
        NSString *newmsg = [newarray objectAtIndex:0];
        NSMutableAttributedString *newText = [NSMutableAttributedString weiboAttributedStringWithString:message type:colorType];
        
        NSMutableAttributedString *broadText = [NSMutableAttributedString weiboAttributedStringWithString:newmsg type:colorType];
        
        _textView.autoresizingMask = UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleBottomMargin;
        _textView.backgroundColor = [UIColor clearColor];
        
        if (_Info.type == 4) {
            textWidth = [CoreTextView adjustSizeWidthWithAttributedString:broadText maxHeight:30].width;
        }
        else
            textWidth = [CoreTextView adjustSizeWidthWithAttributedString:newText maxHeight:30].width;
        _textView.attributedString = newText;
        if (_Info.type == 3)
            textWidth = 170;
        else if (textWidth > 200 || !lblroomname.hidden)
            textWidth = 200;
        else
            textWidth = textWidth;
      
        textHegiht = [CoreTextView adjustSizeWithAttributedString:newText maxWidth:textWidth].height;
        if (textHegiht < gift.frame.size.height && _Info.type == 3) //送礼的时候 如果内容比图片低 则以图片高度为准
            textHegiht = gift.frame.size.height;
        
        _textView.frame = CGRectMake(0, 0, textWidth, textHegiht);
    }
    NSInteger pWidth = 0;
    if (_Info.type == 4)
        pWidth += 5;
    if (type == 1)
        textContainer.frame = CGRectMake(10, 9, _textView.frame.size.width, _textView.frame.size.height);
    else
        textContainer.frame = CGRectMake(15 + pWidth, 9, _textView.frame.size.width, _textView.frame.size.height);
}
//创建礼物图片
- (void)createGift:(NSInteger)type
{
    if (_Info.type != 3)
    {
        gift.hidden =YES;
    }
    else
    {
        gift.hidden =NO;
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *docDir = [paths objectAtIndex:0];
        int flag = 0;
        NSString *imgLocation = nil;
        VECTGIFTINFO::iterator it = [NetUtilCallBack getInstance]->m_localLuckGiftInfo.begin();
        for (; it!=[NetUtilCallBack getInstance]->m_localLuckGiftInfo.end(); it++) {
            if (it->id == _Info.gift_id) {
                imgLocation = [NSString stringWithFormat:@"%@%@%@",docDir,@"/images/",[NSString stringWithUTF8String:it->image_name]];
                flag = 1;
                break;
            }
        }
        if (flag != 1) {
            VECTGIFTINFO::iterator it = [NetUtilCallBack getInstance]->m_localCommonGiftInfo.begin();
            for (; it!=[NetUtilCallBack getInstance]->m_localCommonGiftInfo.end(); it++) {
                if (it->id == _Info.gift_id) {
                    imgLocation = [NSString stringWithFormat:@"%@%@%@",docDir,@"/images/",[NSString stringWithUTF8String:it->image_name]];
                    flag = 1;
                    break;
                }
            }
        }
        
        if (flag != 1) {
            VECTGIFTINFO::iterator it = [NetUtilCallBack getInstance]->m_localLuxuryGiftInfo.begin();
            for (; it!=[NetUtilCallBack getInstance]->m_localLuxuryGiftInfo.end(); it++) {
                if (it->id == _Info.gift_id) {
                    imgLocation = [NSString stringWithFormat:@"%@%@%@",docDir,@"/images/",[NSString stringWithUTF8String:it->image_name]];
                    flag = 1;
                    break;
                }
            }
        }
        [gift setImage:[UIImage imageWithContentsOfFile: imgLocation]];
        if (type== 1)
            gift.frame = CGRectMake(185, 9, 25, 25);
        else
            gift.frame = CGRectMake(190, 9, 25, 25);
    }
}

-(void)createSpeaker:(NSInteger)type
{
    imgSpeaker.hidden = YES;
    if (_Info.type != 4)
    {
        return;
    }
    imgSpeaker.hidden = NO;
    if (type == 1)
    {
        imgSpeaker.frame = CGRectMake(self.frame.size.width - 33, 35, 20, 20);
    }
    else
    {
        imgSpeaker.frame = CGRectMake(33, 35, 20, 20);
    }
}

+(CGFloat)CountRowHeight:(AllInfo)Info
{
    CGFloat height = 14;
    NSInteger textWidth = 200;
    NSString *message = [NSString stringWithCString:Info.content encoding:NSUTF8StringEncoding];
    NSString *toname = [NSString stringWithUTF8String:Info.toname];
    switch (Info.type) {
        case 1:
            {
                message = [NSString stringWithFormat:@"@%@ %@",toname,message];
            }
            break;
        case 3:
            {
                message = [NSString stringWithFormat:@"@%@ %@",toname,message];
                message = [message stringByReplacingOccurrencesOfString:[UserSessionManager getInstance].currentUser.name withString:@"我"];
                textWidth = 170;
                height += 13;
            }
            break;
        case 4:
            break;
        case 5:
        case 6:
        case 7:
            {
                message = [NSString stringWithFormat:@"【系统消息】%@",message];
                CGSize size = [message sizeWithFont:[UIFont boldSystemFontOfSize:14.0f]
                              constrainedToSize:CGSizeMake(_MainScreen_Width-66, MAXFLOAT)
                                  lineBreakMode:UILineBreakModeWordWrap];
                return size.height+31;
            }
            break;
        case 8:
            return 108;
            break;
        default:
            break;
    }
    
    NSArray *newarray;
    if (Info.type == 4) {
        //取最长的一行
        NSArray *array = [message componentsSeparatedByString:@"\n"];
        newarray = [array sortedArrayUsingComparator:^NSComparisonResult(id obj1, id obj2) {
            NSUInteger len0 = [(NSString *)obj1 length];
            NSUInteger len1 = [(NSString *)obj2 length];
            return len0 > len1 ? NSOrderedAscending : NSOrderedDescending;
        }];
    }
    NSString *sortedmsg = [newarray objectAtIndex:0];
    for (int i = 1; i < newarray.count; i++) {
        NSString *string = [newarray objectAtIndex:i];
        sortedmsg = [sortedmsg stringByAppendingString:@"\n"];
        sortedmsg = [sortedmsg stringByAppendingString:string];
    }
    
    NSMutableAttributedString *newText = [NSMutableAttributedString weiboAttributedStringWithString:message type:0];

    NSMutableAttributedString *broadText = [NSMutableAttributedString weiboAttributedStringWithString:sortedmsg type:0];
    
    NSMutableAttributedString *arrayText = [NSMutableAttributedString weiboAttributedStringWithString:[newarray objectAtIndex:0] type:0];
    
    if (Info.type == 4) {
        textWidth = [CoreTextView adjustSizeWidthWithAttributedString:arrayText maxHeight:30].width;
    }
    else
        textWidth = [CoreTextView adjustSizeWidthWithAttributedString:newText maxHeight:30].width;
    if (textWidth > 200 || !(Info.recv_roomid ==0 && Info.roomid == 0))
        textWidth = 200;
    else
        textWidth = textWidth;
    
    height += [CoreTextView adjustSizeWithAttributedString:newText maxWidth:textWidth].height;
    if (Info.idx == 0 || Info.idx == [UserSessionManager getInstance].currentUser.uid)
        height += 14;
    else
        height += 34;
    if (!(Info.recv_roomid ==0 && Info.roomid == 0))
        height += 20;
    height += 9;
    return height;
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];
    
    // Configure the view for the selected state
}

@end

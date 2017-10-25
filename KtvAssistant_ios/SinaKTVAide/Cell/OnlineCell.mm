//
//  OnlineCell.m
//  SinaKTVAide
//
//  Created by cxp on 14-7-29.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "OnlineCell.h"
#import "KTVBaseConfig.h"
#import "UserSessionManager.h"

@implementation OnlineCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        
        self.selectedBackgroundView = [[UIView alloc]init];       
        
        self.selectionStyle = UITableViewCellSelectionStyleNone;
        
        separate = [[UIImageView alloc] initWithFrame:CGRectMake(65, 53, _MainScreen_Width-65, 0.5)];
        separate.backgroundColor = UIColorFromRGB(0xebebeb);
        [self addSubview:separate];
        
        imgHead = [[UIImageView alloc]init];
        imgHead.frame = CGRectMake(11, 4, 42, 42);
        imgHead.layer.masksToBounds = YES;
        imgHead.layer.cornerRadius = 22;
        [self addSubview:imgHead];
        
        lblName = [[UILabel alloc]init];
        lblName.font = [UIFont systemFontOfSize:16];
        [lblName setBackgroundColor:[UIColor clearColor]];
        lblName.textColor = UIColorFromRGB(0xe4397d);
        [self addSubview:lblName];
        
        imgsex = [[UIImageView alloc]init];
        imgsex.backgroundColor = [UIColor clearColor];
        [self addSubview:imgsex];
        
        imginroom =[[UIImageView alloc] init];
        imginroom.backgroundColor = [UIColor clearColor];
        imginroom.image = [UIImage imageNamed:@"room_ic"];
        imginroom.hidden = YES;
        [self addSubview:imginroom];
        
        imgAngle =[[UIImageView alloc] init];
        imgAngle.backgroundColor = [UIColor clearColor];
        imgAngle.image = [UIImage imageNamed:@"icon_angle"];
        imgAngle.hidden = YES;
        [self addSubview:imgAngle];
        
        lblid = [[UILabel alloc]init];
        lblid.text = @"ID:";
        lblid.font = [UIFont systemFontOfSize:11];
        [lblid setBackgroundColor:[UIColor clearColor]];
        lblid.frame = CGRectMake(65, 34, 18, 9);
        lblid.textColor = UIColorFromRGB(0x666666);
        [self addSubview:lblid];
    
        lbluserid = [[UILabel alloc]init];
        lbluserid.font = [UIFont systemFontOfSize:11];
        [lbluserid setBackgroundColor:[UIColor clearColor]];
        lbluserid.textColor = UIColorFromRGB(0x666666);
        [self addSubview:lbluserid];
        
        _moreView = [[UIImageView alloc]init];
        _moreView.frame = CGRectMake(0, 48, _MainScreen_Width, 56);
        _moreView.image = [UIImage imageNamed:@"userlist_pannel.png"];
        _moreView.hidden = NO;
        _moreView.userInteractionEnabled = YES;
        
        chatBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        chatBtn.frame = CGRectMake(((_MainScreen_Width/3)-57)/2, 5, 57, 50);
        //_collection.backgroundColor = [UIColor yellowColor];
        [chatBtn setTitle:@"和ta私信" forState:UIControlStateNormal];
        [chatBtn setTitleEdgeInsets:UIEdgeInsetsMake(29, 0, 0, 0)];
        [chatBtn setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
        chatBtn.titleLabel.font = [UIFont systemFontOfSize:11];
        [chatBtn setBackgroundImage:[UIImage imageNamed:@"userlist_btn_chat0.png"] forState:UIControlStateNormal];
        [chatBtn setBackgroundImage:[UIImage imageNamed:@"userlist_btn_chat1.png"] forState:UIControlStateHighlighted];
        [chatBtn addTarget:self action:@selector(pressChat) forControlEvents:UIControlEventTouchUpInside];
        [_moreView addSubview:chatBtn];
        
        sendGiftbtn = [UIButton buttonWithType:UIButtonTypeCustom];
        sendGiftbtn.frame = CGRectMake((_MainScreen_Width/3)+((_MainScreen_Width/3)-57)/2, 5, 57, 50);
        [sendGiftbtn setTitle:@"送ta礼物" forState:UIControlStateNormal];
        [sendGiftbtn setTitleEdgeInsets:UIEdgeInsetsMake(29, 0, 0, 0)];
        [sendGiftbtn setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
        sendGiftbtn.titleLabel.font = [UIFont systemFontOfSize:11];
        [sendGiftbtn setBackgroundImage:[UIImage imageNamed:@"userlist_btn_gift0.png"] forState:UIControlStateNormal];
        [sendGiftbtn setBackgroundImage:[UIImage imageNamed:@"userlist_btn_gift1.png"] forState:UIControlStateHighlighted];
        [sendGiftbtn addTarget:self action:@selector(pressGift) forControlEvents:UIControlEventTouchUpInside];
        [_moreView addSubview:sendGiftbtn];
        

        redBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        redBtn.frame = CGRectMake((_MainScreen_Width/3)+(_MainScreen_Width/3)+((_MainScreen_Width/3)-57)/2, 5, 57, 50);
        [redBtn setTitle:@"送ta红包" forState:UIControlStateNormal];
        [redBtn setTitleEdgeInsets:UIEdgeInsetsMake(29, 0, 0, 0)];
        [redBtn setTitleColor:UIColorFromRGB(0xffffff) forState:UIControlStateNormal];
        redBtn.titleLabel.font = [UIFont systemFontOfSize:11];
        [redBtn setBackgroundImage:[UIImage imageNamed:@"red_btn_chat2"] forState:UIControlStateNormal];
        [redBtn setBackgroundImage:[UIImage imageNamed:@"red_btn_chat1"] forState:UIControlStateHighlighted];
        [redBtn addTarget:self action:@selector(pressRed) forControlEvents:UIControlEventTouchUpInside];
        [_moreView addSubview:redBtn];
        
        [self addSubview: _moreView];
        
        listBtn = [[UIImageView alloc]init];
        listBtn.frame = CGRectMake(_MainScreen_Width-30, 15, 15, 15);
        
        [self.contentView addSubview:listBtn];

    }
    return self;
}
-(void)setInfo:(OnlineInfo)Info
{
    CGSize size;
    [imgHead setImageWithURL:[NSURL URLWithString:[NSString stringWithUTF8String:Info.headphoto] ] placeholderImage:[UIImage imageNamed:@"userFace_normal.png"]];
    
    NSString *name = [NSString stringWithCString:Info.name encoding:NSUTF8StringEncoding];
    lblName.text = name;
    
    if (Info.idx == [UserSessionManager getInstance].currentUser.uid)
    {
        lblName.text = @"我";
        listBtn.hidden = YES;
    }
    else
        listBtn.hidden = NO;
    
    size = [lblName.text sizeWithFont:[UIFont systemFontOfSize:16] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    lblName.frame = CGRectMake(65, 9, size.width, size.height);
    
    if (strncmp(Info.sex, "0", sizeof("1"))) {
        imgsex.image = [UIImage imageNamed:@"icon_men.png"];
    }
    else
        imgsex.image = [UIImage imageNamed:@"icon_women.png"];
    imgsex.frame = CGRectMake(65+lblName.frame.size.width+3, 11, 14, 14);
    
    if(Info.type == '0')
    {
        imginroom.frame = CGRectMake(65+lblName.frame.size.width+3+14+3, 13, 26, 10.5);
        imginroom.hidden = NO;
        imgAngle.frame = CGRectMake(13, 37, 38, 11.2);
        imgAngle.hidden = NO;
    }
    else
    {
        imginroom.hidden = YES;
        imgAngle.hidden = YES;
    }
    
    lbluserid.text = [NSString stringWithFormat:@"%d",Info.idx];
    
    
    size = [lbluserid.text sizeWithFont:[UIFont systemFontOfSize:11] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    lbluserid.frame = CGRectMake(65+lblid.frame.size.width+3, 32, size.width, size.height);
    
    listBtn.tag = Info.idx;
}

-(void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];
}

- (void)selectRow
{
    _moreView.hidden = NO;
    listBtn.image = [UIImage imageNamed:@"btn_list_top.png"];
}
- (void)deSelectRow
{
    _moreView.hidden = YES;
    listBtn.image = [UIImage imageNamed:@"btn_list_bottom.png"];
}

- (void)pressGift
{
    if (listBtn.tag == [UserSessionManager getInstance].currentUser.uid) {
        return;
    }
    NSDictionary * userInfo = [NSDictionary dictionaryWithObjectsAndKeys:lblName.text,@"username",lbluserid.text,@"useridx", nil];
    
    [[NSNotificationCenter defaultCenter] postNotificationName:@"ktvassistant_online_gif_send_notification" object:nil userInfo:userInfo];
    
}

- (void)pressRed
{
    if (listBtn.tag == [UserSessionManager getInstance].currentUser.uid) {
        return;
    }
    
    NSDictionary * userInfo = [NSDictionary dictionaryWithObjectsAndKeys:lbluserid.text,@"useridx", nil];
    
    [[NSNotificationCenter defaultCenter] postNotificationName:@"ktvassistant_online_sendRed" object:nil userInfo:userInfo];
}

- (void)pressChat
{
    if (listBtn.tag == [UserSessionManager getInstance].currentUser.uid) {
        return;
    }

    NSDictionary * userInfo = [NSDictionary dictionaryWithObjectsAndKeys:lblName.text,@"username",lbluserid.text,@"useridx", nil];
    
    [[NSNotificationCenter defaultCenter] postNotificationName:@"ktvassistant_online_sendChat" object:nil userInfo:userInfo];
}


@end

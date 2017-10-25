//
//  UserCenterLogCell.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-21.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "UserCenterLogCell.h"

@implementation UserCenterLogCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier cellType:(NSInteger)type
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        CGFloat height = self.frame.size.height;
        [self setFrame:CGRectMake(0, 0, _MainScreen_Width, height)];
        cellType = type;
        UIImageView *separate = [[UIImageView alloc] initWithFrame:CGRectMake(70, 77, self.frame.size.width - 70, 1)];
        separate.backgroundColor = UIColorFromRGB(0xbfbfbf);
        [self addSubview:separate];
        
        imgHead =[[UIImageView alloc] initWithFrame:CGRectMake(12, 12, 44, 44)];
        imgHead.backgroundColor = [UIColor clearColor];
        imgHead.layer.cornerRadius = 22;
        imgHead.layer.masksToBounds = YES;
        [self addSubview:imgHead];
        
        
        lblName = [[UILabel alloc] init];
        lblName.backgroundColor = [UIColor clearColor];
        lblName.font = [UIFont systemFontOfSize:14];
        lblName.textColor = UIColorFromRGB(0xe4397d);
        [self addSubview:lblName];
        
        lblPlaceName = [[UILabel alloc] init];
        lblPlaceName.backgroundColor = [UIColor clearColor];
        lblPlaceName.font = [UIFont systemFontOfSize:12];
        lblPlaceName.textColor = UIColorFromRGB(0x666666);
        lblPlaceName.hidden = YES;
        [self addSubview:lblPlaceName];
        
        lblThing = [[UILabel alloc] init];
        lblThing.backgroundColor = [UIColor clearColor];
        lblThing.font = [UIFont systemFontOfSize:12];
        lblThing.textColor = UIColorFromRGB(0x666666);
        [self addSubview:lblThing];
        
        
        lblNum = [[UILabel alloc] init];
        lblNum.backgroundColor = [UIColor clearColor];
        lblNum.font = [UIFont systemFontOfSize:14];
        lblNum.textColor = UIColorFromRGB(0xe4397d);
        [self addSubview:lblNum];
        
        lblUnit = [[UILabel alloc] init];
        lblUnit.backgroundColor = [UIColor clearColor];
        lblUnit.font = [UIFont systemFontOfSize:12];
        lblUnit.textColor = UIColorFromRGB(0x666666);
        [self addSubview:lblUnit];
        
        lblYouGet = [[UILabel alloc] init];
        lblYouGet.backgroundColor = [UIColor clearColor];
        lblYouGet.font = [UIFont systemFontOfSize:12];
        lblYouGet.textColor = UIColorFromRGB(0x666666);
        [self addSubview:lblYouGet];
        
        lblGetGold = [[UILabel alloc] init];
        lblGetGold.backgroundColor = [UIColor clearColor];
        lblGetGold.font = [UIFont systemFontOfSize:14];
        lblGetGold.textColor = UIColorFromRGB(0xe4397d);
        [self addSubview:lblGetGold];
        
        lblGoldName = [[UILabel alloc] init];
        lblGoldName.text = @"金币";
        lblGoldName.backgroundColor = [UIColor clearColor];
        lblGoldName.font = [UIFont systemFontOfSize:12];
        lblGoldName.textColor = UIColorFromRGB(0x666666);
        [self addSubview:lblGoldName];
        
        lblTime =[[UILabel alloc]initWithFrame:CGRectMake(10, 58, 48, 8)];
        lblTime.backgroundColor = [UIColor clearColor];
        lblTime.font = [UIFont systemFontOfSize:8];
        lblTime.textColor =	UIColorFromRGB(0x666666);
        [self addSubview:lblTime];
        
        
    }
    return self;
}

-(void)setInfo:(UserCenterLogInfo *)Info
{
    CGSize size;
    _Info = Info;
    NSMutableString *placeName = [[NSMutableString alloc] initWithCapacity:14];
    if (cellType == 0)
    {
        lblName.text = @"您";
        if (_Info.type<6 || _Info.type>7)
        {
            lblThing.text = [[NSString alloc]initWithFormat:@"%@奖励",_Info.rewardName,nil];
            lblNum.text = @"";
            lblUnit.text = @"";
        }
        else
        {
            NSArray *arr = [_Info.detail componentsSeparatedByString:@"|"];
            if (arr.count != 2)
            {
                lblThing.text = _Info.detail;
                lblNum.text = @"";
                lblUnit.text = @"";
            }
            else
            {
                if(_Info.type == 7)
                    lblUnit.text = @"分";
                else
                    lblUnit.text = @"分进入排行榜";
                lblThing.text = [[NSString alloc] initWithFormat:@"演唱《%@》",[arr objectAtIndex:0]];;
                lblNum.text = [[NSString alloc] initWithFormat:@" %@ ",[arr objectAtIndex:1]];
            }
        }
        
        imgHead.image = [UIImage imageNamed:[[NSString alloc]initWithFormat:@"user_center_reward%d.png", _Info.type]];
        lblTime.text = _Info.addTime;
        lblYouGet.text = @"您得到";
    }
    else
    {
        if (_Info.nickName.length>8)
            lblName.text = [_Info.nickName substringToIndex:8];
        else
            lblName.text = _Info.nickName;
        [imgHead setImageWithURL:[NSURL URLWithString:[CommenTools GetURLWithResolutionScaleTransfered:_Info.headPhoto scale:0]] placeholderImage:[UIImage imageNamed:@"pic_top100_admin.png"]];
        lblTime.text = _Info.tradeDate;
        if (cellType == 1)
        {
            lblThing.text = @"送给您";
            lblYouGet.text = @"您得到";
        }
        else
        {
            lblThing.text = @"收到您";
            lblYouGet.text = @"您消费";
        }
        lblNum.text = [[NSString alloc] initWithFormat:@" %d ",_Info.giftNum,nil];
        lblUnit.text = [[NSString alloc] initWithFormat:@"%@%@",_Info.unit,_Info.giftName,nil];
    }
    lblGetGold.text = [[NSString alloc]initWithFormat:@" %d ",_Info.gold,nil];
    
    size = [lblName.text sizeWithFont:[UIFont systemFontOfSize:14] constrainedToSize:CGSizeMake(350, 150)];
    lblName.frame = CGRectMake(70, 12, size.width, size.height);
    
    size = [lblThing.text sizeWithFont:[UIFont systemFontOfSize:12] constrainedToSize:CGSizeMake(350, 12)];
    lblThing.frame = CGRectMake(70, 33, size.width, size.height);
    
    size = [lblNum.text sizeWithFont:[UIFont systemFontOfSize:14] constrainedToSize:CGSizeMake(350, 150)];
    lblNum.frame = CGRectMake(70 + lblThing.frame.size.width, 32, size.width, size.height);
    
    size = [lblUnit.text sizeWithFont:[UIFont systemFontOfSize:12] constrainedToSize:CGSizeMake(350, 150)];
    lblUnit.frame = CGRectMake(lblNum.frame.origin.x + lblNum.frame.size.width, 33, size.width, size.height);
    
    if ((lblThing.frame.size.width + lblNum.frame.size.width + lblUnit.frame.size.width) > 245) {
        lblThing.frame = CGRectMake(70, 33, 245  - lblNum.frame.size.width - lblUnit.frame.size.width, size.height);
        lblNum.frame = CGRectMake(70 + lblThing.frame.size.width, 32, lblNum.frame.size.width, size.height);
        lblUnit.frame = CGRectMake(lblNum.frame.origin.x + lblNum.frame.size.width, 33, lblUnit.frame.size.width, size.height);
    }
    
    size = [lblYouGet.text sizeWithFont:[UIFont systemFontOfSize:12] constrainedToSize:CGSizeMake(350, 150)];
    lblYouGet.frame = CGRectMake(70, 54, size.width, size.height);
    
    size = [lblGetGold.text sizeWithFont:[UIFont systemFontOfSize:14] constrainedToSize:CGSizeMake(350, 150)];
    lblGetGold.frame = CGRectMake(70 + lblYouGet.frame.size.width, 53, size.width, size.height);
    
    size = [lblGoldName.text sizeWithFont:[UIFont systemFontOfSize:12] constrainedToSize:CGSizeMake(350, 150)];
    lblGoldName.frame = CGRectMake(lblGetGold.frame.origin.x + lblGetGold.frame.size.width, 54, size.width, size.height);
    
    
    if ([_Info.ktvName compare:@""] != NSOrderedSame || [_Info.roomName compare:@""] != NSOrderedSame) {
        lblPlaceName.hidden = NO;
        [placeName appendString:@" 在 "];
        if (_Info.ktvName)
            [placeName appendString:_Info.ktvName];
        if (_Info.roomName)
            [placeName appendString:_Info.roomName];
        lblPlaceName.frame = CGRectMake(70 + lblName.frame.size.width, 14, 12, 30);
        lblPlaceName.text = placeName;
        [lblPlaceName sizeToFit];
    }
    else
        lblPlaceName.text = @"";
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];
    
    
}

@end

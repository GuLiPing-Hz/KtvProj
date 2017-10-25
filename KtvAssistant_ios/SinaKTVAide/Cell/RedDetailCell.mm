//
//  RedDetailCell.mm
//  SinaKTVAide
//
//  Created by cxp on 15/5/27.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "RedDetailCell.h"

@implementation RedDetailCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        separate = [[UIImageView alloc] initWithFrame:CGRectMake(0, 54.5, _MainScreen_Width, 0.5)];
        separate.backgroundColor = UIColorFromRGB(0xebebeb);
        [self addSubview:separate];
        
        imgHead = [[UIImageView alloc]init];
        imgHead.frame = CGRectMake(11, 10, 35, 35);
        imgHead.layer.masksToBounds = YES;
        imgHead.layer.cornerRadius = 17.5;
        [self addSubview:imgHead];
        
        lblName = [[UILabel alloc]init];
        lblName.font = [UIFont systemFontOfSize:14];
        [lblName setBackgroundColor:[UIColor clearColor]];
        lblName.textColor = UIColorFromRGB(0x333333);
        [self addSubview:lblName];
        
        lbltime = [[UILabel alloc]init];
        lbltime.font = [UIFont systemFontOfSize:11];
        [lbltime setBackgroundColor:[UIColor clearColor]];
        lbltime.textColor = UIColorFromRGB(0xc2c2c2);
        [self addSubview:lbltime];
        
        lblgold = [[UILabel alloc]init];
        lblgold.font = [UIFont systemFontOfSize:14];
        [lblgold setBackgroundColor:[UIColor clearColor]];
        lblgold.textColor = UIColorFromRGB(0x333333);
        [self addSubview:lblgold];

        imgStar = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"star_ic_0"]];
        imgStar.frame = CGRectMake(_MainScreen_Width-72, 32, 14, 13.5);
        imgStar.layer.masksToBounds = YES;
        imgStar.backgroundColor = [UIColor clearColor];
        [self addSubview:imgStar];
        imgStar.hidden = YES;

        lblluck = [[UILabel alloc] initWithFrame:CGRectMake(_MainScreen_Width-56, 34, 49, 11)];
        lblluck.text = @"最佳手气";
        lblluck.font = [UIFont systemFontOfSize:11];
        [lblluck setBackgroundColor:[UIColor clearColor]];
        lblluck.textColor = UIColorFromRGB(0xffc600);
        [self addSubview:lblluck];
        lblluck.hidden = YES;
        
    }
    return self;
}
-(void)setInfo:(HBGetDetail)Info
{
    [imgHead setImageWithURL:[NSURL URLWithString:[NSString stringWithUTF8String:Info.headurl] ] placeholderImage:[UIImage imageNamed:@"userFace_normal.png"]];
    
    if (strlen(Info.name) == 0)
    {
        lblName.text = [NSString stringWithFormat:@"%d",Info.idx ];
    }
    else
        lblName.text = [NSString stringWithCString:Info.name encoding:NSUTF8StringEncoding];
    
    CGSize size = [lblName.text sizeWithFont:[UIFont systemFontOfSize:16] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    lblName.frame = CGRectMake(54, 11, size.width, size.height);
    
    if(Info.isStar)
    {
        imgStar.hidden = NO;
        lblluck.hidden = NO;
    }
    else
    {
        imgStar.hidden = YES;
        lblluck.hidden = YES;
    }
    
    lblgold.text = [NSString stringWithFormat:@"%d金币",Info.gold];
    size = [lblgold.text sizeWithFont:[UIFont systemFontOfSize:14.0f] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    lblgold.frame = CGRectMake(_MainScreen_Width-10-size.width, 13, size.width, size.height);
    
    NSDate *date=[NSDate dateWithTimeIntervalSince1970:Info.time];
    NSDateFormatter *format=[[NSDateFormatter alloc]init];
    [format setDateFormat:@"hh:mm"];
    NSString *nsdate=[format stringFromDate:date];
    lbltime.text = nsdate;
    [lbltime sizeToFit];
    size = [lbltime.text sizeWithFont:[UIFont systemFontOfSize:11.0f] constrainedToSize:CGSizeMake(MAXFLOAT, 50)];
    lbltime.frame = CGRectMake(56, 34, size.width, size.height);
}



- (void)awakeFromNib {
    // Initialization code
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];

    // Configure the view for the selected state
}

@end

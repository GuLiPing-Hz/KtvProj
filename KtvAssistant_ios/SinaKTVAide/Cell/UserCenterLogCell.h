//
//  UserCenterLogCell.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-21.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UserCenterLogInfo.h"


@interface UserCenterLogCell : UITableViewCell
{
    UIImageView     *imgHead;
    UILabel         *lblTime;
    UILabel         *lblName;
    UILabel         *lblPlaceName;
    UILabel         *lblNum;
    UILabel         *lblThing;
    UILabel         *lblUnit;
    UILabel         *lblYouGet;
    UILabel         *lblGetGold;
    UILabel         *lblGoldName;
    NSInteger       cellType;
}


@property (nonatomic,retain) UserCenterLogInfo *Info;
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier cellType:(NSInteger)type;
@end

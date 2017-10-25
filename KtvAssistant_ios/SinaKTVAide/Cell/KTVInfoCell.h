//
//  KTVInfoCell.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "KTVInfo.h"



@interface KTVInfoCell : UITableViewCell
{
    UILabel         *lblKtvName;
    UILabel         *lblKtvAddress;
    UIButton        *btnMore;
}

@property (nonatomic,retain) KTVInfo *model;

@end

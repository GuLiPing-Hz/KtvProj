//
//  RedDetailCell.h
//  SinaKTVAide
//
//  Created by cxp on 15/5/27.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "iosdatadefine.h"

@interface RedDetailCell : UITableViewCell
{
    UIImageView     *imgHead;
    UILabel         *lblName;
    UILabel         *lbltime;
    UILabel         *lblgold;
    UIImageView     *separate;
    UILabel         *lblluck;
    UIImageView     *imgStar;
}

@property (nonatomic,assign) HBGetDetail Info;
@end

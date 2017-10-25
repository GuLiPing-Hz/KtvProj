//
//  BoardTipView.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-9-4.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MessageInfoStruct.h"
#import "CoreTextView.h"

@interface BoardTipView : UIView
{
    UIImageView     *imgSpearker;
    UILabel         *lblName;
    UILabel         *lbltime;
    UILabel         *lblFrom;
}
@property (nonatomic,retain) CoreTextView *textView;

- (id)initWithInfo:(NSMutableDictionary *)dic;
@end

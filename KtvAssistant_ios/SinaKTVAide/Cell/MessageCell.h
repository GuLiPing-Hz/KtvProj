//
//  MessageCell.h
//  SinaKTVAide
//
//  Created by Li Pan on 13-12-20.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MessageCell : UITableViewCell
@property (nonatomic,retain) UILabel *labMessage;
@property (nonatomic,retain) UIImageView *bgImageView;
@property (nonatomic,retain) UILabel *labDate;
@property (nonatomic,retain) UILabel *scoreLabel;
@property (nonatomic,retain) UIImageView *badgeImage;
//@property (nonatomic,retain) UIImageView *separator;

- (NSString *)getDate:(NSString *)dateString;

@end

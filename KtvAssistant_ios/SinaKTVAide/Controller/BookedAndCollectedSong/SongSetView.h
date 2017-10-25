//
//  SongSetController.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-14.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface SongSetView: UIView

@property (nonatomic ,retain) IBOutlet UIButton *voice1;
@property (nonatomic ,retain) IBOutlet UIButton *voice2;
@property (nonatomic ,retain) IBOutlet UIButton *mic1;
@property (nonatomic ,retain) IBOutlet UIButton *mic2;
@property (nonatomic ,retain) IBOutlet UIButton *playAndPause;
@property (nonatomic ,retain) IBOutlet UIButton *scoreMode;
@property (nonatomic ,retain) IBOutlet UIButton *replay;
@property (nonatomic ,retain) IBOutlet UIButton *singMode;
@property (nonatomic ,retain) IBOutlet UIButton *tone1;
@property (nonatomic ,retain) IBOutlet UIButton *tone2;
@property (nonatomic ,retain) IBOutlet UIButton *effect1;
@property (nonatomic ,retain) IBOutlet UIButton *effect2;
@property (nonatomic ,retain) IBOutlet UIButton *effect3;
@property (nonatomic ,retain) IBOutlet UIButton *effect4;
@property (nonatomic ,retain) IBOutlet UIButton *effect5;
@property (nonatomic ,retain) IBOutlet UIButton *effect6;
@property (nonatomic ,retain) IBOutlet UIButton *effect7;
@property (nonatomic ,retain) IBOutlet UIButton *effect8;
@property (nonatomic ,retain) IBOutlet UIButton *mute;
@property (nonatomic ,retain) IBOutlet UIButton *tone3;


-(void)setAllBtnBack;
-(IBAction)btnClick:(id)sender;

@end

//
//  UserProfileViewController.h
//  SinaKTVAide

//  用户个人中心 界面

//  Created by Li Pan on 13-12-5.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "MarqueeLabel.h"
#import "ZBarReaderViewController.h"
#import "TJSpinner.h"

@interface UserProfileViewController : BaseViewController<UIActionSheetDelegate,UITableViewDataSource,UITableViewDelegate,UIImagePickerControllerDelegate,UINavigationControllerDelegate,UIAlertViewDelegate,ZBarReaderDelegate>
{
    NSString *keyword_;
}
@property (nonatomic,strong)  ZBarReaderViewController *reader;
@property (nonatomic, retain) NSString                 *keyWord;
@property (nonatomic,retain)  UIButton                 *headButton;
@property (nonatomic,retain)  UIImageView             *backgroundImage;
@property (nonatomic,retain)  UIActionSheet            *changeHeadPhotoSheet;
@property (nonatomic,retain)  UITableView              *totalTable;
@property (nonatomic,retain)  NSMutableArray           *arrayTotal;
@property (nonatomic,retain)  UILabel                  *AnimationTotal;
@property (nonatomic,retain)  UIImageView              *sex;
@property (nonatomic,retain)  UIImageView              *separate;
@property (nonatomic,retain)  UIButton                 *left;
@property (nonatomic,retain)  UIButton                 *right;
@property (nonatomic,retain)  UILabel                  *userName;
@property (nonatomic,retain)  UILabel                  *total;
//@property (nonatomic,assign)  long int                   points;
@property (nonatomic,strong)  UIAlertView              *altExitRoom;
@property (nonatomic,strong)  UIAlertView              *altOutLogin;
@property (nonatomic,strong)  UIImage                  *changeHeadImage;
@property (nonatomic,strong)  UIImageView             *egoHeadImage;
@property (nonatomic,strong)  UIView                   * noMessageView;
@property (nonatomic,strong)  UIActivityIndicatorView  *actListMessage;
@property (nonatomic, retain) TJSpinner                *beachBallSpinner;

@end


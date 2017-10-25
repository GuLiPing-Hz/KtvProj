//
//  KTVMainViewController.h
//  SinaKtv
//
//  Created by Zhang Chen on 13-10-22.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "ZBarSDK.h"
#import "KTVAppDelegate.h"
#import "MarqueeLabel.h"
#import "KTVAppDelegate.h"
#import "UserSessionManager.h"
#import "NetUtilCallBackDelegate.h"
#import "NIBadgeView.h"
#import "NetUtilCallBackProtocol.h"

typedef enum : NSUInteger {
    SCAN_ROOMPWD,
    SCAN_GIFT,
    SCAN_COMMON,
} ScanType;

@interface KTVMainViewController : BaseViewController<UIActionSheetDelegate,ZBarReaderDelegate,UISearchBarDelegate,UIScrollViewDelegate,UIAlertViewDelegate,UIApplicationDelegate,NetUtilCallBackDelegate,NetUtilCallBackProtocol>
{
    NSString *keyword_;
}
@property(nonatomic,strong)NIBadgeView* mBarrageTip;
@property(nonatomic,strong)NIBadgeView* mHongBaoTip;
@property (nonatomic, retain) UIActionSheet            *boxVodActionSheet;
@property (nonatomic, retain) NSString                 *keyWord;
@property (nonatomic, strong) UISearchBar              *songSearchBar;
@property (nonatomic, strong) UIView                   *view_loading;
@property (nonatomic, strong) NSMutableArray           *arrayImage;
@property (nonatomic, strong) UIPageControl            *pageImage;
@property (nonatomic, strong) UIScrollView             *scrollImage;
@property (nonatomic, strong) NSTimer                  *timer;
@property (nonatomic, strong) NSMutableArray           *arrayListTable;
@property (nonatomic, strong) UIView                   *spView;
@property (nonatomic, strong) ZBarReaderViewController *reader;
@property (nonatomic, strong) NSMutableDictionary      * downLoadImage;
@property (nonatomic, strong) NSMutableArray           * bannerListArray;
@property (nonatomic, strong) UITapGestureRecognizer   * tapBanner;
@property (nonatomic, strong) NSMutableArray           * alreadyOrderedSongsList;
@property (nonatomic, assign) BOOL                       scrollLeft;
@property (nonatomic,strong)  UIAlertView              *altExitRoom;
@property (nonatomic, strong) NSTimer                  *bannerScrollTimer;
@property (nonatomic, strong) UIImageView              *bannerAdmin;
@property (nonatomic, assign) BOOL                     alreadyLoad;
@property (nonatomic, strong) UIView                   *btnView;

//扫码
@property (nonatomic,retain) UIAlertView    *mAlertExchange;
@property(nonatomic,strong) NSMutableDictionary* mMapData;

@property(nonatomic,assign) int mScanType;
-(void)login;

@end

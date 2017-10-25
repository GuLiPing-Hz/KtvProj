//
//  FaceBoard.h
//
//  Created by blue on 12-9-26.
//  Copyright (c) 2012年 blue. All rights reserved.
//  Email - 360511404@qq.com
//  http://github.com/bluemood


#import <UIKit/UIKit.h>

#define  faceBoardHeight 216 //(320.0 / 7 * 3 + 20)
#define  toolBarHeight (171.0/2)
#define Time  0.25
#define UIColorFromRGBWithAlpha(rgbValue,a) [UIColor \
colorWithRed:((float)((rgbValue & 0xFF0000) >> 16))/255.0 \
green:((float)((rgbValue & 0xFF00) >> 8))/255.0 \
blue:((float)(rgbValue & 0xFF))/255.0 alpha:a]

@protocol FaceBoardDelegate <NSObject>
@optional
- (void)textViewDidChange:(UITextView *)textView;
@end


@interface FaceBoard : UIView <UITableViewDataSource, UITableViewDelegate>
{
    UITableView *faceView;
    UIPageControl *facePageControl;
    
    NSMutableArray *faceMap;
}

- (void)backFace:(UIButton *)sender;

@property (nonatomic, assign) id<FaceBoardDelegate> delegate;
@property (nonatomic, retain) UITextField *inputTextField;
//@property (nonatomic, retain) UITextView *inputTextView;

@end

@interface FaceTableViewCell : UITableViewCell

@property (nonatomic, strong) NSMutableArray* faces;

@end

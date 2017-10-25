//
//  FaceBoard.m
//
//  Created by cxp on 14-7-28.
//  Copyright (c) 2014年 cxp. All rights reserved.
//  Email - 360511404@qq.com
//  http://github.com/bluemood

#import "FaceBoard.h"
#import "NSString+Regex.h"

//#define FACE_COUNT_PAGE ( FACE_COUNT_ROW * FACE_COUNT_CLU )
//#define FACE_ICON_SIZE  44

//#define FACE_ALL  (57+52)
#define FACE_ROW  27

@implementation FaceBoard

@synthesize delegate;

@synthesize inputTextField = _inputTextField;
//@synthesize inputTextView = _inputTextView;

- (void)dealloc
{
#if DEBUG
    NSLog(@"%@ dealloc", self);
#endif
}

- (void)initFacemapDic
{
    faceMap = [[NSMutableArray alloc] init];

//    NSMutableArray* aArr = [NSMutableArray array];
//    NSMutableArray* bArr = [NSMutableArray array];
//    [self.faceMap setObject:aArr forKey:@"a"];
//    [self.faceMap setObject:bArr forKey:@"b"];
    
    // a 57
    for (NSUInteger a = 1; a <= 56; a ++) {
        if (a == 25 || a == 4 || a == 6 || a == 20 || a == 43) {
            continue;
        }
        NSString* value = [NSString stringWithFormat:@"[a/%d]", a];
        NSString* key   = [NSString stringWithFormat:@"emot.bundle/a/%d_s.gif",a];
        NSDictionary* dic = [NSDictionary dictionaryWithObject:value forKey:key];
        [faceMap addObject:dic];
    }
    // b 53
    for (NSUInteger b = 1; b <= 53; b ++) {
        if (b == 43) {
            continue;
        }
        NSString* value = [NSString stringWithFormat:@"[b/%d]", b];
        NSString* key   = [NSString stringWithFormat:@"emot.bundle/b/%d_s.gif",b];
        NSDictionary* dic = [NSDictionary dictionaryWithObject:value forKey:key];
        [faceMap addObject:dic];
    }
//    // c 5
//    for (NSUInteger c = 1; c <= 5; c ++) {
//        NSString* value = [NSString stringWithFormat:@"[c/%d]", c];
//        NSString* key   = [NSString stringWithFormat:@"emot.bundle/c/%d.gif",c];
//        [_faceMap setObject:value forKey:key];
//    }
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
//        self.backgroundColor = [UIColor colorWithRed:236.0/255.0 green:236.0/255.0 blue:236.0/255.0 alpha:1];
        self.backgroundColor = UIColorFromRGBWithAlpha(0xfafafa, 1);
        
        UIView* v = [[UIView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 0.5)];
        v.backgroundColor = UIColorFromRGBWithAlpha(0xc8c8c8, 1);
        [self addSubview:v];
        
        [self initFacemapDic];
        //表情盘
        CGFloat height = _MainScreen_Width/7*4;
        faceView = [[UITableView alloc]initWithFrame:CGRectMake((_MainScreen_Width-height)/2, -(_MainScreen_Width-height)/2+10, height, _MainScreen_Width)];
        faceView.backgroundView = nil;
        faceView.backgroundColor = [UIColor clearColor];
        faceView.transform = CGAffineTransformMakeRotation(-M_PI/2);
        faceView.pagingEnabled = YES;
        faceView.showsHorizontalScrollIndicator = NO;
        faceView.showsVerticalScrollIndicator = NO;
        faceView.delegate = self;
        faceView.dataSource = self;
        faceView.separatorStyle = UITableViewCellSeparatorStyleNone;
        faceView.rowHeight = _MainScreen_Width;
        [self addSubview:faceView];
        [faceView reloadData];
//        [faceView.layer setBorderColor:[[UIColor blueColor] CGColor]];
//        [faceView.layer setBorderWidth:1.0f];

        CGSize size = faceView.contentSize;
        int h = (int)size.height;
        int num;
        if (h % (int)_MainScreen_Width == 0) num = h/(int)_MainScreen_Width;
        else num = h/(int)_MainScreen_Width+1;
        
        //添加PageControl
        facePageControl = [[UIPageControl alloc]initWithFrame:CGRectMake(0, height+10, _MainScreen_Width, faceBoardHeight-height-10)];
        [facePageControl addTarget:self action:@selector(pageChange:) forControlEvents:UIControlEventValueChanged];
        facePageControl.currentPageIndicatorTintColor = [UIColor colorWithRed:0/255.0 green:150/255.0 blue:250/255.0 alpha:1];
        facePageControl.pageIndicatorTintColor = [UIColor lightGrayColor];
        facePageControl.numberOfPages = num;
        facePageControl.currentPage = 0;
        [self addSubview:facePageControl];
//        [facePageControl.layer setBorderColor:[[UIColor cyanColor] CGColor]];
//        [facePageControl.layer setBorderWidth:1.0f];

        //删除键
//        UIButton *back = [UIButton buttonWithType:UIButtonTypeCustom];
//        [back setTitle:@"删除" forState:UIControlStateNormal];
//        [back setImage:[UIImage imageNamed:@"face_close"] forState:UIControlStateNormal];
//        [back addTarget:self action:@selector(backFace:) forControlEvents:UIControlEventTouchUpInside];
//        back.frame = CGRectMake(310.0/7*6-(310.0/7-20)/2, 310/7*2+(310.0/7-15)/2, 20, 15);
//        [self addSubview:back];
    }
    return self;
}

//停止滚动的时候
- (void)scrollViewDidEndDecelerating:(UIScrollView *)scrollView
{
    int y = ((CGPoint)faceView.contentOffset).y;
    int page = 0;
    if (y%(int)_MainScreen_Width==0) {
        page = y/(int)_MainScreen_Width;
    } else {
        page = y/(int)_MainScreen_Width+1;
    }
    [facePageControl setCurrentPage:page];
}

- (void)pageChange:(UIPageControl *)sender
{
    CGFloat height = 0.0f;
    CGSize size = faceView.contentSize;
    if (sender.currentPage*_MainScreen_Width+_MainScreen_Width>size.height) {
        height = sender.currentPage*_MainScreen_Width + (_MainScreen_Width-(sender.currentPage*_MainScreen_Width+_MainScreen_Width-size.height))-_MainScreen_Width;
    } else {
        height = sender.currentPage*_MainScreen_Width;
    }
    [faceView setContentOffset:CGPointMake(0, height) animated:YES];
}

#pragma mark - Table view data source

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (faceMap) {
        int num = 0;
        if ([faceMap count]%FACE_ROW == 0)
            num = [faceMap count]/FACE_ROW;
        else
            num = [faceMap count]/FACE_ROW+1;
        return num;
    } else{
        return 0;
    }
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"FaceTableViewCell";
    FaceTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[FaceTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
        cell.contentView.transform = CGAffineTransformMakeRotation(M_PI/2);
        cell.contentView.backgroundColor = [UIColor clearColor];
        cell.backgroundView = nil;
        cell.backgroundColor = [UIColor clearColor];
        cell.selectionStyle = UITableViewCellSeparatorStyleNone;
        for (NSUInteger i = 0; i < FACE_ROW+1; i ++) {
            
            if (i == FACE_ROW) {
                [((UIButton *)[cell.faces objectAtIndex:i]) addTarget:self action:@selector(backFace:) forControlEvents:UIControlEventTouchUpInside];
            } else {
                [((UIButton *)[cell.faces objectAtIndex:i]) addTarget:self action:@selector(clickFaceBtn:) forControlEvents:UIControlEventTouchUpInside];
            }
        }
    }
    NSUInteger nodeCount = [faceMap count];
    for (NSUInteger i = 0; i < FACE_ROW; i ++) {
        UIButton* btn = (UIButton *)[cell.faces objectAtIndex:i];
        
        if ((indexPath.row * FACE_ROW + i) < nodeCount) {
            NSDictionary* dic = [faceMap objectAtIndex:(indexPath.row*FACE_ROW+i)];
            NSString* key = [[dic allKeys] objectAtIndex:0];
//            [btn setImage: forState:UIControlStateNormal];
            btn.tag = (indexPath.row*FACE_ROW+i);
            btn.titleLabel.text = [dic objectForKey:key];
            btn.hidden = NO;
            id subBtn = [[btn subviews] objectAtIndex:0];
            if (subBtn && [subBtn isKindOfClass:[UIButton class]]) {
                [((UIButton *)subBtn) setImage:[UIImage imageNamed:key] forState:UIControlStateNormal];
            }
            
        } else {
            btn.hidden = YES;
        }
    }
    return cell;
}

- (void)clickFaceBtn:(UIButton *)sender
{
    if (self.inputTextField) {
        if (self.inputTextField.text == nil) {
            self.inputTextField.text = @"";
        }
        NSMutableString *faceString = [[NSMutableString alloc] initWithString:self.inputTextField.text];
        [faceString appendString:sender.titleLabel.text];
        self.inputTextField.text = faceString;
    }
//    else if (self.inputTextView) {
//        NSMutableString *faceString = [[NSMutableString alloc]initWithString:self.inputTextView.text];
//        [faceString appendString:sender.titleLabel.text];
//        self.inputTextView.text = faceString;
//        [delegate textViewDidChange:self.inputTextView];
//    }
}

- (void)backFace:(UIButton *)sender
{
    NSString *inputString;
    inputString = self.inputTextField.text;
//    if ( self.inputTextView ) {
//        inputString = self.inputTextView.text;
//    }
    if ( inputString.length ) {
        NSString *string = nil;
        NSArray* arr = [inputString regexfaceContent];
        if (arr && arr.count) {
            NSString* tmp = [arr objectAtIndex:(arr.count-1)];
            if ((tmp.length == 5 || tmp.length == 6) && (
                [[tmp substringToIndex:3] isEqualToString:@"[a/"] ||
                [[tmp substringToIndex:3] isEqualToString:@"[b/"] ||
                [[tmp substringToIndex:3] isEqualToString:@"[c/"] )) {//"[a/8]",
                string = [inputString substringToIndex:(inputString.length-tmp.length)];
            } else {
                string = [inputString substringToIndex:(inputString.length-1)];
            }
        } else {
            string = [inputString substringToIndex:(inputString.length-1)];
        }
        if ( self.inputTextField ) {
            self.inputTextField.text = string;
        }
    }
}

@end
@implementation FaceTableViewCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        PLog(@"%f", self.frame.size.width);
        self.faces = [[NSMutableArray alloc] initWithCapacity:FACE_ROW];
        CGFloat width = _MainScreen_Width/7;
        for (NSUInteger y = 0; y < 4; y ++) {
            for (NSUInteger x = 0; x < 7; x ++) {
                UIButton* btn = [UIButton buttonWithType:UIButtonTypeCustom];
                btn.frame = CGRectMake(width*x, width*y, width, width);
                NSLog(@"%f", btn.frame.size.width);
                [self.contentView addSubview:btn];
                [self.faces addObject:btn];
                if (y == 3 && x == 6) {
                    CGRect rect = CGRectMake((width-20)/2.0, (width-15)/2.0, 20, 15);
                    UIImageView* img = [[UIImageView alloc] initWithFrame:rect];
                    img.image = [UIImage imageNamed:@"face_close"];
                    [btn addSubview:img];
                } else {
                    CGRect rect = CGRectMake((width-30)/2.0, (width-30)/2.0, 30, 30);
                    UIButton* subBtn = [UIButton buttonWithType:UIButtonTypeCustom];
                    subBtn.frame = rect;
                    subBtn.userInteractionEnabled = NO;
                    [btn addSubview:subBtn];
                    
//                    [subBtn.layer setBorderColor:[[UIColor blueColor] CGColor]];
//                    [subBtn.layer setBorderWidth:1.0f];

                }
//                [btn.layer setBorderColor:[[UIColor orangeColor] CGColor]];
//                [btn.layer setBorderWidth:1.0f];
            }
        }
    }
    return self;
}

@end

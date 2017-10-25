//
//  RedDetailView.mm
//  SinaKTVAide
//
//  Created by cxp on 15/5/27.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "RedDetailView.h"
#import "NetUtilCallBack.h"
#import "RedDetailCell.h"
#import "KTVAppDelegate.h"
#import "DDMenuController.h"

@implementation RedDetailView

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [self.navView initOnlyBackButton:@"红包领取详情"];
    self.navView.hidden = NO;
    self.navView.backButton.hidden = NO;
    self.navView.backgroundColor = UIColorFromRGB(0xf04f4f);
   
    [self createTop];
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 180, _MainScreen_Width, _MainScreen_Height-180) style:UITableViewStylePlain];
    _tableView.dataSource = self;
    _tableView.delegate = self;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    _tableView.tableFooterView = [[UIView alloc] init];//多余的cell屏蔽掉
    [self.mainContentView addSubview:_tableView];
    [_tableView reloadData];
}

-(void)createTop{
    UIView *topview = [[UIView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, _MainScreen_Height)];
    topview.backgroundColor = UIColorFromRGB(0xf04f4f);
    [self.mainContentView addSubview:topview];
    
    UIImageView *headimg = [[UIImageView alloc] initWithFrame:CGRectMake((_MainScreen_Width-54)/2, 9, 54, 54)];
    NSString *head = [NSString stringWithUTF8String:_sendinfo.headurl];
    if(self.sendinfo.sendidx == 1)
        [headimg setImage:[UIImage imageNamed:@"ic_system_profile"]];
    else
        [headimg setImageWithURL:[NSURL URLWithString:head] placeholderImage:[UIImage imageNamed:@"userFace_normal_b.png"]];
    headimg.backgroundColor = [UIColor clearColor];
    headimg.layer.cornerRadius = 27;
    headimg.layer.masksToBounds = YES;
    [topview addSubview:headimg];
    
    UILabel *lblName = [[UILabel alloc] init];
    NSString *name = [NSString stringWithUTF8String:_sendinfo.name];
    lblName.text = [NSString stringWithFormat:@"%@发的红包", name];
    lblName.font = [UIFont systemFontOfSize:15.0f];
    CGSize size = [lblName.text sizeWithFont:[UIFont systemFontOfSize:15.0f] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    lblName.frame = CGRectMake((_MainScreen_Width-size.width)/2, 74, size.width, size.height);
    lblName.textColor = UIColorFromRGB(0xffffff);
    [topview addSubview:lblName];
    
    UILabel *lblMark = [[UILabel alloc] init];
    lblMark.text = [NSString stringWithUTF8String:_sendinfo.mark];
    lblMark.font = [UIFont systemFontOfSize:13.0f];
    lblMark.textColor = UIColorFromRGB(0xf9c5c5);
    size = [lblMark.text sizeWithFont:[UIFont systemFontOfSize:13.0f] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    lblMark.frame = CGRectMake((_MainScreen_Width-size.width)/2, 100, size.width, size.height);
    [topview addSubview:lblMark];
    
    UILabel *lblgold = [[UILabel alloc] init];
    lblgold.text = [NSString stringWithFormat:@"%d",_getgold];
    lblgold.font = [UIFont systemFontOfSize:40.0f];
    lblgold.textColor = UIColorFromRGB(0xffe13b);
    size = [lblgold.text sizeWithFont:[UIFont systemFontOfSize:40.0f] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    [topview addSubview:lblgold];
    
    UILabel *lblunit = [[UILabel alloc] init];
    lblunit.text = @"金币";
    lblunit.font = [UIFont systemFontOfSize:14.0f];
    lblunit.textColor = UIColorFromRGB(0xffe13b);
    CGSize unitsize = [lblunit.text sizeWithFont:[UIFont systemFontOfSize:14.0f] constrainedToSize:CGSizeMake(_MainScreen_Width, 160)];
    [topview addSubview:lblunit];
    
    lblgold.frame = CGRectMake((_MainScreen_Width-size.width-unitsize.width-4)/2, 125, size.width, size.height);
    lblunit.frame = CGRectMake((_MainScreen_Width-size.width-unitsize.width-4)/2+size.width+4, 147, unitsize.width, unitsize.height);
    
    if (_sendinfo.num == 0) {
        lblgold.hidden = YES;
        lblunit.hidden = YES;
    }
    else
    {
        lblgold.hidden = NO;
        lblunit.hidden = NO;
    }
    
    if (_getgold == 0) {
        lblgold.hidden = YES;
        lblunit.hidden = YES;
    }
    else
    {
        lblgold.hidden = NO;
        lblunit.hidden = NO;
    }
}

-(CGFloat) tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 55;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (_sendinfo.num == 0) {
        return 0;
    }
    return [NetUtilCallBack getInstance]->m_hbdetail.size();
}

#define SECTION_HEIGHT 33
- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
{
    return SECTION_HEIGHT;
}
- (UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section
{
    UILabel *lbldetail = [[UILabel alloc] init];
    lbldetail.textColor = UIColorFromRGB(0xbbbbbb);
    lbldetail.font = [UIFont systemFontOfSize:13.0f];
    lbldetail.backgroundColor = UIColorFromRGB(0xf6f6f7);
    lbldetail.frame = CGRectMake(0, 0, _MainScreen_Width, SECTION_HEIGHT);
    
    if (_sendinfo.num == 0) {
        lbldetail.text = @"   等待对方领取";
        return lbldetail;
    }

    if (_sendinfo.type == (char)0) {
        lbldetail.text = @"   红包已领";
        return lbldetail;
    }
    
    int getnum = [NetUtilCallBack getInstance]->m_hbdetail.size();
    
    VECTHBDETAIL::iterator beginit = [NetUtilCallBack getInstance]->m_hbdetail.begin();
    VECTHBDETAIL::iterator endit = [NetUtilCallBack getInstance]->m_hbdetail.end();
    endit--;
    if ([NetUtilCallBack getInstance]->m_hbdetail.size() >= _sendinfo.num) {
        int time = beginit->time - _sendinfo.sendtime;
        NSString* title = nil;
        if (time <= 60) {
            title = [NSString stringWithFormat:@"   %d个红包，%d秒被抢完",_sendinfo.num,time];
        }
        else if (time >60 && time <=3600) {
            int min = time/60;
            int sec = time%60;
            if (sec == 0) {
                title = [NSString stringWithFormat:@"   %d个红包，%d分被抢完",_sendinfo.num,min];
            }
            else
                title = [NSString stringWithFormat:@"   %d个红包，%d分%d秒被抢完",_sendinfo.num,min,sec];
        }
        else
        {
            int hour = time/3600;
            int tmp = time%3600;
            int min = tmp/60;
            int sec = tmp%60;
            if (min == 0 && sec == 0) {
                title = [NSString stringWithFormat:@"   %d个红包，%d小时被抢完",_sendinfo.num,hour];
            }
            else if(min == 0 && sec != 0){
                title = [NSString stringWithFormat:@"   %d个红包，%d小时%d秒被抢完",_sendinfo.num,hour,sec];
            }
            else if(min != 0 && sec == 0)
            {
                title = [NSString stringWithFormat:@"   %d个红包，%d小时%d分被抢完",_sendinfo.num,hour,min];
            }
            else
            {
                title = [NSString stringWithFormat:@"   %d个红包，%d小时%d分%d秒被抢完",_sendinfo.num,hour,min,sec];
            }
        }
        lbldetail.text = title;
    }
    else
    {
        int getgold = 0;
        for (; beginit!=[NetUtilCallBack getInstance]->m_hbdetail.end(); beginit++) {
            getgold += beginit->gold;
        }
        lbldetail.text = [NSString stringWithFormat:@"   已领取%d/%d个，共%d/%d元",getnum,_sendinfo.num,getgold,_sendinfo.gold];
    }
    return lbldetail;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    NSUInteger row = [indexPath row];
    if ([NetUtilCallBack getInstance]->m_hbdetail.size()<=0) {
        return [[UITableViewCell alloc]init];
    }
        
    HBGetDetail detail = [NetUtilCallBack getInstance]->m_hbdetail[row];
    HBGetDetail& info = detail;
    RedDetailCell *cell =[tableView dequeueReusableCellWithIdentifier:@"cell" ];
    if (cell == nil) {
        cell = [[RedDetailCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
        cell.backgroundView = nil;
        cell.backgroundColor = [UIColor clearColor];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    if (_sendinfo.type != (char)0) {
        int maxgold = max([NetUtilCallBack getInstance]->m_hbdetail);
        if([NetUtilCallBack getInstance]->m_hbdetail.size() >= _sendinfo.num)
        {
            if (maxgold == info.gold) {
                info.isStar = YES;
            }
            else
                info.isStar = NO;
        }
        else
            info.isStar = NO;
    }
    cell.Info = info;
    return cell;
}

//求向量最大值
int max(VECTHBDETAIL a)
{
    int maxgold = a[0].gold;
    for(int i=1; i<a.size(); i++)
    {
        if (a[i].gold > maxgold)
            maxgold = a[i].gold;
    }
    return maxgold;
}
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self.appDelegate.menuController setEnableGesture:NO];
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.appDelegate.menuController setEnableGesture:YES];
    [NetUtilCallBack getInstance]->m_hbdetail.clear();
    [NetUtilCallBack getInstance].thedelegate = nil;
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end

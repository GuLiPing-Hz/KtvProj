//
//  UserCenterTableView.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-21.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "UserCenterTableView.h"
#import "UserCenterLogCell.h"
#import "PKtvAssistantAPI.h"

@implementation UserCenterTableView

- (id)initWithFrame:(CGRect)frame type:(NSInteger)_type
{
    self = [super initWithFrame:frame];
    if (self) {
        _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height) style:UITableViewStylePlain];
        _tableView.dataSource = self;
        _tableView.delegate = self;
        _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        _tableView.tableFooterView = [[UIView alloc] init];//多余的cell屏蔽掉
        [self addSubview:_tableView];
        isLoad = false;
        tableType = _type;
        index = 1;
        _data  = [[NSMutableArray alloc] init];
        [self loadData];
    }
    return self;
}

-(void)loadData
{
    if (isLoad) {
        return;
    }
    isLoad = true;
    NSString *strUrl;
    long uid=[UserSessionManager getInstance].currentUser.uid;
    if (tableType == 0)
        strUrl = [PKtvAssistantAPI getUserRewardUrl:uid begin:index num:5];
    else
        strUrl = [PKtvAssistantAPI getChatGiftLogUrl:uid page:index size:5 type:tableType];
    NSURL *url =[[NSURL alloc] initWithString:strUrl];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation * operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *html = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString *status = [html objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [html objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"list"];
            if (tableType != 0) {
                
            }
            for (NSDictionary *dic in dataList) {
                UserCenterLogInfo *info;
                if (tableType == 0)
                    info = [UserCenterLogInfo initWithDictionaryReward:dic];
                else
                    info = [UserCenterLogInfo initWithDictionaryGift:dic];
                [_data addObject:info];
            }
            [_tableView reloadData];
            index++;
        }
        else
        {
            
        }
        isLoad = NO;
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        isLoad = NO;
    }];
    [operation start];
}


#pragma mark tableviewdelegate

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _data.count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UserCenterLogInfo *info = [_data objectAtIndex:indexPath.row];
    UserCenterLogCell *cell =[tableView dequeueReusableCellWithIdentifier:@"cell" ];
    if (cell == nil) {
        cell = [[UserCenterLogCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell" cellType:tableType];
        cell.backgroundView = nil;
        cell.backgroundColor = [UIColor clearColor];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    cell.Info = info;
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return  78;
}

-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if (scrollView == _tableView) {
        if (_tableView.contentOffset.y + _tableView.frame.size.height > (_tableView.contentSize.height + 20)) {
            [self loadData];
        }
        
    }
}

@end

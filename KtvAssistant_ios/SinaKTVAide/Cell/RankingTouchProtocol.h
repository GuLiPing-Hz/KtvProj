//
//  Header.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/21.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#ifndef SinaKTVAide_RankingTouchProtocol_h
#define SinaKTVAide_RankingTouchProtocol_h

@protocol RankingTouchProtocol <NSObject>

@optional
-(void)onTouchIn:(long) topicId title:(NSString*)title;

@end

#endif

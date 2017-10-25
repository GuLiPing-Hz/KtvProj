//
//  NSString+SSStringCategory.h
//  Vgirl
//
//  Created by Ming Jianhua on 12-2-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (SSStringCategory)
- (NSString *)getImageName;
- (NSString *)getImageType;
-(NSString *) stringByStrippingHTML;
- (NSString *)clearWhiteSpaceAndNewLine;
@end

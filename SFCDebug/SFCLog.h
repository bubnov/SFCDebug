//
//  SFCLog.h
//  SFCDebug
//
//  Created by Slavik Bubnov on 21.11.11.
//  Copyright (c) 2011 Slavik Bubnov, bubnovslavik@gmail.com. All rights reserved.
//

#import "DDLog.h"

static NSArray *_SFCLogArrayMake(NSArray *array, NSUInteger count) {
   NSRange range = NSIntersectionRange(NSMakeRange(0, count), NSMakeRange(0, array.count));
   NSArray *subarray = [array subarrayWithRange:range];
   if (subarray.count && subarray.count < array.count) {
      return [subarray arrayByAddingObject:[NSString stringWithFormat:@"(...and %lu more...)", (unsigned long)(array.count - subarray.count)]];
   } else {
      return subarray;
   }
}

static inline NSArray *SFCLogArrayMake(NSArray *array) {
   return _SFCLogArrayMake(array, NSUIntegerMax);
}


typedef NSUInteger SFCLoggerID;

static SFCLoggerID SFCLoggerIDConsole = (1 << 0);  // 0...00001
static SFCLoggerID SFCLoggerIDFile = (1 << 1);     // 0...00010


@interface SFCLog : DDLog


/**
 Configurate logger for DEBUG mode
 If we're in DEBUG mode - add ASL, TTY and Remote loggers.
 */
+ (void)debug;


/**
 Return current log level
 Used in macroses instead of |ddLogLevel|
 */
+ (NSInteger)logLevel;


/**
 Set log level
 @return NSInteger old log level
 */
+ (NSInteger)setLogLevel:(NSInteger)newLogLevel;

#pragma mark - Loggers

+ (NSArray *)allRegisteredLoggerIDs;
+ (void)addLogger:(id<DDLogger>)logger withID:(SFCLoggerID)ID;

#pragma mark - Tokens

+ (void)enableAllTokensForLoggerID:(SFCLoggerID)loggerID;
+ (void)disableAllTokensForLoggerID:(SFCLoggerID)loggerID;
+ (void)enableToken:(id<NSCopying>)token forLoggerID:(SFCLoggerID)loggerID;
+ (void)disableToken:(id<NSCopying>)token forLoggerID:(SFCLoggerID)loggerID;
+ (BOOL)isTokenEnabled:(id<NSCopying>)token forLoggerID:(SFCLoggerID)loggerID;

#pragma mark - Syntax Sugar (Console Logger)

+ (void)enableToken:(id<NSCopying>)token;
+ (void)disableToken:(id<NSCopying>)token;
+ (void)enableAllTokens;
+ (void)disableAllTokens;
+ (BOOL)isTokenEnabled:(id<NSCopying>)token;

/**
 This method is a fix to manage some compiler optimization behaviour.
 Without it one can't operate with SFCLog class in debug console and will get something like this:
 
 (lldb) po SFCLog
 error: use of undeclared identifier 'SFCLog'
 error: 1 errors parsing expression
 
 So, the trick is to add at least one instance method (declaration and implementation).
 */
- (void)magic;

@end


@interface DDAbstractLogger (SFCSharedInstance)

+ (instancetype)sharedInstance;

@end

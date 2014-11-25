//
//  SFCLog.m
//  SFCDebug
//
//  Created by Slavik Bubnov on 21.11.11.
//  Copyright (c) 2011 Slavik Bubnov, bubnovslavik@gmail.com. All rights reserved.
//

#import "SFCLog.h"
#import <objc/runtime.h>
#import "SFCLogDefines.h"
#import "DDASLLogger.h"
#import "DDTTYLogger.h"
#import "DDFileLogger.h"


@implementation SFCLog

+ (instancetype)shared {
   static SFCLog *_sharedInstance;
   static dispatch_once_t onceToken;
   dispatch_once(&onceToken, ^{
      _sharedInstance = [[self class] new];
   });
   return _sharedInstance;
}

+ (void)debug {
   [self setLogLevel:LOG_LEVEL_DEBUG];
   
   [self addLogger:[DDASLLogger sharedInstance] withID:SFCLoggerIDConsole];
   [self addLogger:[DDTTYLogger sharedInstance] withID:SFCLoggerIDConsole];
   
   // We want to direct our log messages to a file.
   // So we're going to setup file logging.
   // We start by creating a file logger.
   DDFileLogger *fileLogger = [DDFileLogger sharedInstance];
   
   // Configure some sensible defaults for an iPhone application.
   // Roll the file when it gets to be 512 KB or 24 Hours old (whichever comes first).
   // Also, only keep up to 4 archived log files around at any given time.
   // We don't want to take up too much disk space.
   fileLogger.maximumFileSize = 1024 * 1024 * 200;  // 200 Mb
   fileLogger.rollingFrequency = 60 * 60 * 24 * 10; // 10 Days
   fileLogger.logFileManager.maximumNumberOfLogFiles = 200;
   
   // Add our file logger to the logging system.
   [self addLogger:fileLogger withID:SFCLoggerIDFile];
}


static NSInteger _logLevel = LOG_LEVEL_OFF;

+ (NSInteger)logLevel {
   return _logLevel;
}

+ (NSInteger)setLogLevel:(NSInteger)newLogLevel {
   NSInteger oldLogLevel = _logLevel;
   _logLevel = newLogLevel;
   return oldLogLevel;
}

#pragma mark - Loggers

+ (void)addLogger:(id<DDLogger>)logger withID:(SFCLoggerID)ID {
   [self _tokensForLoggerID:ID];
   [self addLogger:logger withLogLevel:(int)ID];
}

+ (NSArray *)allRegisteredLoggerIDs {
   NSArray *__block IDs;
   @synchronized(self) {
      IDs = [[self _tokens] allKeys];
   }
   return IDs;
}

#pragma mark - Tokens

+ (void)enableToken:(id<NSCopying>)token forLoggerID:(SFCLoggerID)loggerID {
   if (!token) {
      return;
   }
   @synchronized(self) {
      NSMutableDictionary *tokens = [self _tokensForLoggerID:loggerID];
      if (tokens[token] && [tokens[token] boolValue]) {
         NSLog(@"Log token '%@' is already enabled.", token);
      } else {
         tokens[token] = @YES;
      }
   }
}

+ (void)disableToken:(id<NSCopying>)token forLoggerID:(SFCLoggerID)loggerID {
   if (!token) {
      return;
   }
   @synchronized(self) {
      NSMutableDictionary *tokens = [self _tokensForLoggerID:loggerID];
      if (tokens[token] && ! [tokens[token] boolValue]) {
         NSLog(@"Log token '%@' is already disabled.", token);
      } else {
         tokens[token] = @NO;
      }
   }
}

+ (BOOL)isTokenEnabled:(id<NSCopying>)token forLoggerID:(SFCLoggerID)loggerID {
   if (!token) {
      return NO;
   }
   // If there is no such token - add it
   @synchronized(self) {
      if ( ! [self _tokensForLoggerID:loggerID][token]) {
         if ([self _allowsAllTokensForLoggerID:loggerID]) {
            [self enableToken:token forLoggerID:loggerID];
         } else {
            [self disableToken:token forLoggerID:loggerID];
         }
      }
      return [[self _tokensForLoggerID:loggerID][token] boolValue];
   }
}

+ (void)enableAllTokensForLoggerID:(SFCLoggerID)loggerID {
   [self _setAllTokensEnabled:YES forLogger:loggerID];
}

+ (void)disableAllTokensForLoggerID:(SFCLoggerID)loggerID {
   [self _setAllTokensEnabled:NO forLogger:loggerID];
}

+ (void)_setAllTokensEnabled:(BOOL)enabled forLogger:(SFCLoggerID)loggerID {
   @synchronized(self) {
      NSMutableDictionary *tokens = [self _tokensForLoggerID:loggerID];
      objc_setAssociatedObject(tokens, &_SFCLogTokensAllowedKey, @(enabled), OBJC_ASSOCIATION_RETAIN);
      NSMutableDictionary *newTokens = [NSMutableDictionary dictionaryWithCapacity:[tokens count]];
      for (id<NSCopying> token in tokens) {
         newTokens[token] = @(enabled);
      }
      tokens[@(loggerID)] = newTokens;
   }
}

+ (BOOL)_allowsAllTokensForLoggerID:(SFCLoggerID)loggerID {
   BOOL __block isAllowed;
   @synchronized(self) {
      NSDictionary *tokens = [self _tokensForLoggerID:loggerID];
      NSNumber *allowed = objc_getAssociatedObject(tokens, &_SFCLogTokensAllowedKey);
      isAllowed = [allowed boolValue];
   }
   return isAllowed;
}

#pragma mark - Syntax Sugar (Console Logger)

+ (void)enableToken:(id<NSCopying>)token {
   [self enableToken:token forLoggerID:SFCLoggerIDConsole];
}

+ (void)disableToken:(id<NSCopying>)token {
   [self disableToken:token forLoggerID:SFCLoggerIDConsole];
}

+ (BOOL)isTokenEnabled:(id<NSCopying>)token {
   return [self isTokenEnabled:token forLoggerID:SFCLoggerIDConsole];
}

+ (void)enableAllTokens {
   [self enableAllTokensForLoggerID:SFCLoggerIDConsole];
}

+ (void)disableAllTokens {
   [self disableAllTokensForLoggerID:SFCLoggerIDConsole];
}

#pragma mark - Private

static char *_SFCLogTokensKey;
static char *_SFCLogTokensAllowedKey;

+ (NSMutableDictionary *)_tokens {
   NSMutableDictionary *tokens = objc_getAssociatedObject(self, &_SFCLogTokensKey);
   if (!tokens) {
      tokens = [NSMutableDictionary new];
      objc_setAssociatedObject(self, &_SFCLogTokensKey, tokens, OBJC_ASSOCIATION_RETAIN);
   }
   return tokens;
}

+ (NSMutableDictionary *)_tokensForLoggerID:(SFCLoggerID)loggerID {
   NSMutableDictionary *tokensPerLogger = [self _tokens];
   NSMutableDictionary *tokens = tokensPerLogger[@(loggerID)];
   if (!tokens) {
      tokens = [NSMutableDictionary new];
      tokensPerLogger[@(loggerID)] = tokens;
   }
   return tokens;
}

#pragma mark -

- (void)magic {
   NSLog(@"This method is a fix to manage some compiler optimization behaviour. "
         @"Without it one can't operate with SFCLog class in debug console");
}

@end


@implementation DDAbstractLogger (SFCSharedInstance)

+ (instancetype)sharedInstance {
   static id _sharedInstance;
   static dispatch_once_t onceToken;
   dispatch_once(&onceToken, ^{
      _sharedInstance = [[self class] new];
   });
   return _sharedInstance;
}

@end

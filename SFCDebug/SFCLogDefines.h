//
//  SFCLogDefines.h
//  SFCDebug
//
//  Created by Slavik Bubnov on 21.11.11.
//  Copyright (c) 2011 Slavik Bubnov, bubnovslavik@gmail.com. All rights reserved.
//

#import "SFCLog.h"

// Undefine the default stuff we don't want to use.
#undef LOG_FLAG_ERROR
#undef LOG_FLAG_WARN 
#undef LOG_FLAG_INFO
#undef LOG_FLAG_VERBOSE

#undef LOG_LEVEL_ERROR
#undef LOG_LEVEL_WARN
#undef LOG_LEVEL_INFO
#undef LOG_LEVEL_VERBOSE

#undef LOG_ERROR
#undef LOG_WARN
#undef LOG_INFO
#undef LOG_VERBOSE

#undef DDLogError
#undef DDLogWarn
#undef DDLogInfo
#undef DDLogVerbose

#undef DDLogCError
#undef DDLogCWarn
#undef DDLogCInfo
#undef DDLogCVerbose

// Flags and levels
#define LOG_FLAG_ERROR    (1 << 0)  // 0...00001
#define LOG_FLAG_WARN     (1 << 1)  // 0...00010
#define LOG_FLAG_INFO     (1 << 2)  // 0...00100
#define LOG_FLAG_DEBUG    (1 << 3)  // 0...01000
#define LOG_FLAG_VERBOSE  (1 << 4)  // 0...10000

#define LOG_LEVEL_OFF     0
#define LOG_LEVEL_ERROR   (LOG_FLAG_ERROR)                                                                          // 0...00001
#define LOG_LEVEL_WARN    (LOG_FLAG_ERROR | LOG_FLAG_WARN)                                                          // 0...00011
#define LOG_LEVEL_INFO    (LOG_FLAG_ERROR | LOG_FLAG_WARN | LOG_FLAG_INFO)                                          // 0...00111
#define LOG_LEVEL_DEBUG   (LOG_FLAG_ERROR | LOG_FLAG_WARN | LOG_FLAG_INFO | LOG_FLAG_DEBUG)                         // 0...01111
#define LOG_LEVEL_VERBOSE (LOG_FLAG_ERROR | LOG_FLAG_WARN | LOG_FLAG_INFO | LOG_FLAG_DEBUG | LOG_FLAG_VERBOSE)      // 0...11111

// Current log level
#define SF_LOG_LEVEL    [SFCLog logLevel]

// Log macroses
// All log macroses are synchronous because async logs sometimes missed
#ifdef DEBUG

#define SFCLogFatal(frmt, ...)    SYNC_LOG_OBJC_MAYBE(SF_LOG_LEVEL, LOG_FLAG_FATAL,  0, frmt, ##__VA_ARGS__)
#define SFCLogError(frmt, ...)    SYNC_LOG_OBJC_MAYBE(SF_LOG_LEVEL, LOG_FLAG_ERROR,  0, frmt, ##__VA_ARGS__)
#define SFCLogWarn(frmt, ...)     SYNC_LOG_OBJC_MAYBE(SF_LOG_LEVEL, LOG_FLAG_WARN,   0, frmt, ##__VA_ARGS__)
#define SFCLogNotice(frmt, ...)   SYNC_LOG_OBJC_MAYBE(SF_LOG_LEVEL, LOG_FLAG_NOTICE, 0, frmt, ##__VA_ARGS__)
#define SFCLogInfo(frmt, ...)     SYNC_LOG_OBJC_MAYBE(SF_LOG_LEVEL, LOG_FLAG_INFO,   0, frmt, ##__VA_ARGS__)
#define SFCLogDebug(frmt, ...)    SYNC_LOG_OBJC_MAYBE(SF_LOG_LEVEL, LOG_FLAG_DEBUG,  0, frmt, ##__VA_ARGS__)

#define SFCLogCFatal(frmt, ...)      SYNC_LOG_C_MAYBE(SF_LOG_LEVEL, LOG_FLAG_FATAL,  0, frmt, ##__VA_ARGS__)
#define SFCLogCError(frmt, ...)      SYNC_LOG_C_MAYBE(SF_LOG_LEVEL, LOG_FLAG_ERROR,  0, frmt, ##__VA_ARGS__)
#define SFCLogCWarn(frmt, ...)       SYNC_LOG_C_MAYBE(SF_LOG_LEVEL, LOG_FLAG_WARN,   0, frmt, ##__VA_ARGS__)
#define SFCLogCNotice(frmt, ...)     SYNC_LOG_C_MAYBE(SF_LOG_LEVEL, LOG_FLAG_NOTICE, 0, frmt, ##__VA_ARGS__)
#define SFCLogCInfo(frmt, ...)       SYNC_LOG_C_MAYBE(SF_LOG_LEVEL, LOG_FLAG_INFO,   0, frmt, ##__VA_ARGS__)
#define SFCLogCDebug(frmt, ...)      SYNC_LOG_C_MAYBE(SF_LOG_LEVEL, LOG_FLAG_DEBUG,  0, frmt, ##__VA_ARGS__)

// Conditional log macroses
#define SFCLog(token, frmt, ...)    ({ \
   for (NSNumber *packedID in [SFCLog allRegisteredLoggerIDs]) { \
      SFCLoggerID loggerID = [packedID unsignedIntegerValue]; \
      _SFCLog(token, loggerID, frmt, ##__VA_ARGS__); \
   } \
}) \

#define _SFCLog(token, loggerID, frmt, ...) ({ \
if ([SFCLog isTokenEnabled:token forLoggerID:loggerID]) {  \
   SYNC_LOG_OBJC_MAYBE(loggerID, loggerID,  0, _SFCLogFormatString(token, frmt), ##__VA_ARGS__); \
} \
}) \


#define SFCLogC(flag, frmt, ...)    if ([SFCLog isTokenEnabled:flag]) SFCLogCInfo(_SFCLogFormatString(flag, frmt), ##__VA_ARGS__)

static inline NSString *_SFCLogFormatString(id flag, NSString *format) NS_FORMAT_ARGUMENT(2);
static inline NSString *_SFCLogFormatString(id flag, NSString *format) {
   return [NSString stringWithFormat:@"[%@] %@", [flag description], format];
}


#else

#define  SFCLog(flag, frmt, ...)
#define  SFCLogC(flag, frmt, ...)
#define  SFCLogFatal(frmt, ...)
#define  SFCLogError(frmt, ...)
#define  SFCLogWarn(frmt, ...)
#define  SFCLogNotice(frmt, ...)
#define  SFCLogInfo(frmt, ...)
#define  SFCLogDebug(frmt, ...)
#define  SFCLogCFatal(frmt, ...)
#define  SFCLogCError(frmt, ...)
#define  SFCLogCWarn(frmt, ...)
#define  SFCLogCNotice(frmt, ...)
#define  SFCLogCInfo(frmt, ...)
#define  SFCLogCDebug(frmt, ...)

#endif

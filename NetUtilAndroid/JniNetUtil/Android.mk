#author GLP
#
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS := -llog
LOCAL_MODULE    := JniNetUtil
LOCAL_SRC_FILES := com_jni_netutil_CJniUtil.cpp \
								JniResponse.cpp \
								NetUtil/charset/CharsetCodec.cpp \
								NetUtil/charset/Chinese.cpp \
								NetUtil/charset/Utf.cpp \
								NetUtil/localStorage/NotifyMsg.cpp \
								NetUtil/localStorage/DataMgr.cpp \
								NetUtil/localStorage/LocalStorage.cpp \
								NetUtil/localStorage/ServerData.cpp \
								NetUtil/localStorage/UserInfoData.cpp \
								NetUtil/wrap/ClientSocket.cpp \
								NetUtil/wrap/Counter.cpp \
								NetUtil/wrap/crypt.cpp \
								NetUtil/wrap/DataBlock.cpp \
								NetUtil/wrap/DataDecoder.cpp \
								NetUtil/wrap/event.cpp \
								NetUtil/wrap/EventHandler.cpp \
								NetUtil/wrap/HttpDownload.cpp \
								NetUtil/wrap/HttpDownloadMgr.cpp \
								NetUtil/wrap/md5.cpp \
								NetUtil/wrap/NonCopyable.cpp \
								NetUtil/wrap/NUtility.cpp \
								NetUtil/wrap/reactor.cpp \
								NetUtil/wrap/RWStream.cpp \
								NetUtil/wrap/SeqMap.cpp \
								NetUtil/wrap/sleep.cpp \
								NetUtil/wrap/thread.cpp \
								NetUtil/wrap/ThreadMgr.cpp \
								NetUtil/wrap/Timer.cpp \
								NetUtil/wrap/CFileMgr.cpp \
								NetUtil/xml/tinystr.cpp \
								NetUtil/xml/tinyxml.cpp \
								NetUtil/xml/tinyxmlerror.cpp \
								NetUtil/xml/tinyxmlparser.cpp \
								NetUtil/zip/ioapi.c \
								NetUtil/zip/unzip.c \
								NetUtil/zip/zip.c \
								NetUtil/zlib/adler32.c \
								NetUtil/zlib/compress.c \
								NetUtil/zlib/crc32.c \
								NetUtil/zlib/deflate.c \
								NetUtil/zlib/gzclose.c \
								NetUtil/zlib/gzlib.c \
								NetUtil/zlib/gzread.c \
								NetUtil/zlib/gzwrite.c \
								NetUtil/zlib/infback.c \
								NetUtil/zlib/inffast.c \
								NetUtil/zlib/inflate.c \
								NetUtil/zlib/inftrees.c \
								NetUtil/zlib/trees.c \
								NetUtil/zlib/uncompr.c \
								NetUtil/zlib/zutil.c \
								NetUtil/DataDecoderAuth.cpp \
								NetUtil/DataDecoderChannellist.cpp \
								NetUtil/DataDecoderLobby.cpp \
								NetUtil/DataDecoderLobbyNew.cpp \
								NetUtil/DataDecoderLobbyOld.cpp \
								NetUtil/DataDecoderRoom.cpp \
								NetUtil/DataDecoderRoom1.cpp \
								NetUtil/DataDecoderRoom2.cpp \
								NetUtil/DataDecodeUtilty.cpp \
								NetUtil/dbresult.cpp \
								NetUtil/HttpUniformCallBack.cpp \
								NetUtil/logsystem.cpp \
								NetUtil/MessageFilter.cpp \
								NetUtil/NetApp.cpp \
								NetUtil/RequestBase.cpp \
								NetUtil/ReserveData.cpp \
								NetUtil/ThreadInformer.cpp \
								NetUtil/TMSeqMap.cpp \
								NetUtil/Tunnel.cpp \
								NetUtil/ChatCompatibility.cpp
								

#LOCAL_CFLAGS := -ANDROID_NETUTIL
																							
include $(BUILD_SHARED_LIBRARY)

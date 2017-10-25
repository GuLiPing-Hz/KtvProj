# written by glp
#编译接口文件
#java 文件需要是ASCII编码
1 javac -d class StructInfo.java CJniUtil.java   	 #指定生成路径
2 javah -classpath class com.jni.netutil.CJniUtil	 #指定类路径

#tips
#javap -classpath class -s com.jni.netutil.CJniUtil >zCJniUtilSig.txt 查看某个类的具体jni标记


eclipse javah 配置
Location ： Browse File System 选到javah.exe

Working Directory : Browse Workspace 选到bin/classes目录

Arguments : -bootclasspath ${AndroidSDK}\platforms\android-19\android.jar -classpath ${workspace_loc:/CocosProject2/bin/classes} -d ${workspace_loc:/CocosProject2/jni} org.cocos2dx.cpp.AppActivity

注 AndroidSDK 就是AndroidSDK目录
   Arguments中的classpath与Working Directory基本一致
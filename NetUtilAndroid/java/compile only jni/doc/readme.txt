# written by glp
#����ӿ��ļ�
#java �ļ���Ҫ��ASCII����
1 javac -d class StructInfo.java CJniUtil.java   	 #ָ������·��
2 javah -classpath class com.jni.netutil.CJniUtil	 #ָ����·��

#tips
#javap -classpath class -s com.jni.netutil.CJniUtil >zCJniUtilSig.txt �鿴ĳ����ľ���jni���


eclipse javah ����
Location �� Browse File System ѡ��javah.exe

Working Directory : Browse Workspace ѡ��bin/classesĿ¼

Arguments : -bootclasspath ${AndroidSDK}\platforms\android-19\android.jar -classpath ${workspace_loc:/CocosProject2/bin/classes} -d ${workspace_loc:/CocosProject2/jni} org.cocos2dx.cpp.AppActivity

ע AndroidSDK ����AndroidSDKĿ¼
   Arguments�е�classpath��Working Directory����һ��
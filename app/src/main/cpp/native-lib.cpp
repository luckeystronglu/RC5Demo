#include <jni.h>
#include <string>
#include "rc5.cpp"



extern "C"
{
jstring Java_com_yzh_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env, jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

void Java_com_yzh_myapplication_MainActivity_XOREncrypt(JNIEnv *env, jobject instance, jbyteArray src_,
jbyteArray desc_, jint len,
jbyteArray macSource_,jint lenMacSource);
void Java_com_yzh_myapplication_MainActivity_MD5Encode(JNIEnv *env, jobject instance, jbyteArray in_, jbyteArray out_);
void Java_com_yzh_myapplication_MainActivity_MD5Decode(JNIEnv *env, jobject instance, jbyteArray in_, jbyteArray out_);
void Java_com_yzh_myapplication_MainActivity_RC5Encode(JNIEnv *env, jobject instance, jbyteArray in_, jbyteArray out_);
void Java_com_yzh_myapplication_MainActivity_RC5Decode(JNIEnv *env, jobject instance, jbyteArray in_, jbyteArray out_);
};


//RC5加密
JNIEXPORT void JNICALL
Java_com_yzh_myapplication_MainActivity_RC5Encode(JNIEnv *env, jobject instance, jbyteArray in_,
                                                  jbyteArray out_) {
    jbyte * in = env->GetByteArrayElements(in_, 0);
    jbyte * out = env->GetByteArrayElements(out_, 0);

    // TODO
    jsize len = env->GetArrayLength(in_);
    EnDeCryptInit();
    RC5_Encrypt((uint8_t *) in, (uint8_t *) out, (uint8_t)len);
    env->ReleaseByteArrayElements(in_, in, 0);
    env->ReleaseByteArrayElements(out_, out, 0);
}

//RC5解密
JNIEXPORT void JNICALL
Java_com_yzh_myapplication_MainActivity_RC5Decode(JNIEnv *env, jobject instance, jbyteArray in_,
                                                  jbyteArray out_) {
    jbyte * in = env->GetByteArrayElements(in_, 0);
    jbyte * out = env->GetByteArrayElements(out_, 0);

    // TODO
    jsize len = env->GetArrayLength(in_);
    EnDeCryptInit();
    RC5_Decrypt((uint8_t *) in, (uint8_t *) out, (uint8_t)len);
    env->ReleaseByteArrayElements(in_, in, 0);
    env->ReleaseByteArrayElements(out_, out, 0);
}

JNIEXPORT void JNICALL
Java_com_yzh_myapplication_MainActivity_MD5Encode(JNIEnv *env, jobject instance, jbyteArray in_,
                                                  jbyteArray out_) {
    jbyte *in = env->GetByteArrayElements(in_, NULL);
    jbyte *out = env->GetByteArrayElements(out_, NULL);

    // TODO
//    int i =0,j,len;
//    MD5Context context;
//    unsigned char buff[16];
//    unsigned char buffsize[1024*4];
//
//    for(i = 0;i < sizeof(buffsize);i++)
//    {
//        buffsize[i] =  (unsigned char)in[i];
//    }
//
//    len = sizeof(buffsize);
//    MD5_Init(&context);
//    MD5_Update(&context,(unsigned char *)buffsize, len);
//    MD5_Final(&context,buff);
//    for(i = 0;i < sizeof(buff);i++)
//    {
//        out[i] =  buff[i];
//    }

    env->ReleaseByteArrayElements(in_, in, 0);
    env->ReleaseByteArrayElements(out_, out, 0);
}


JNIEXPORT void JNICALL
Java_com_yzh_myapplication_MainActivity_MD5Decode(JNIEnv *env, jobject instance, jbyteArray in_,
                                                  jbyteArray out_) {
    jbyte *in = env->GetByteArrayElements(in_, NULL);
    jbyte *out = env->GetByteArrayElements(out_, NULL);

    // TODO
//    jsize len = env->GetArrayLength(in_);
//    MD5_Decode((unsigned int *) out, (unsigned char *) in,len);
    env->ReleaseByteArrayElements(in_, in, 0);
    env->ReleaseByteArrayElements(out_, out, 0);
}

JNIEXPORT void JNICALL
Java_com_yzh_myapplication_MainActivity_XOREncrypt(JNIEnv *env, jobject instance, jbyteArray src_,
                                                   jbyteArray desc_, jint len,
                                                   jbyteArray macSource_,jint lenMacSource) {
    jbyte *src = env->GetByteArrayElements(src_, NULL);
    jbyte *desc = env->GetByteArrayElements(desc_, NULL);
    jbyte *macSource = env->GetByteArrayElements(macSource_, NULL);

    // TODO
    jint i = 0;
    for (i = 0; i < len; i++) {
        *(desc + i) = *(src + i) ^ macSource[i % lenMacSource];
    }
    env->ReleaseByteArrayElements(src_, src, 0);
    env->ReleaseByteArrayElements(desc_, desc, 0);
    env->ReleaseByteArrayElements(macSource_, macSource, 0);
}


#include "unknow_jperl_PerlHash.h"
#include "jperl.h"

JNIEXPORT void JNICALL Java_unknow_jperl_PerlHash_create(JNIEnv *env, jobject this, jlong perl)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)perl;
	HV* hv=newHV();
	(*env)->SetLongField(env, this, perlScalar_sv, (jlong)hv);
	(*env)->SetLongField(env, this, perlScalar_perl, perl);
	}

JNIEXPORT jint JNICALL Java_unknow_jperl_PerlHash_size(JNIEnv *env, jobject this)
	{
	HV *hv=(HV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	return HvUSEDKEYS(hv);
	}

JNIEXPORT void JNICALL Java_unknow_jperl_PerlHash_clear(JNIEnv *env, jobject this)
	{
	HV *hv=(HV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	hv_clear(hv);
	}

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlHash_remove(JNIEnv *env, jobject this, jstring key)
	{
	HV *hv=(HV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	char *k=c_string(env, key);
	SV *sv=hv_delete(hv, k, (I32)strlen(k), 0);
	free(k);
	return jperl_create_scalar(env, my_perl, sv);
	}

JNIEXPORT jboolean JNICALL Java_unknow_jperl_PerlHash_containsKey(JNIEnv *env, jobject this, jstring key)
	{
	HV *hv=(HV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	char *k=c_string(env, key);
	jboolean r=hv_exists(hv, k, (I32)strlen(k));
	free(k);
	return r;
	}

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlHash_get(JNIEnv *env, jobject this, jstring key, jboolean create)
	{
	HV *hv=(HV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	char *k=c_string(env, key);
	SV **sv=hv_fetch(hv, k, (I32)strlen(k), create);
	free(k);
	if(sv==NULL)
		return NULL;
	return jperl_create_scalar(env, my_perl, *sv);
	}

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlHash_put(JNIEnv *env, jobject this, jstring key, jobject scalar)
	{
	HV *hv=(HV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	char *k=c_string(env, key);
	SV *v=(SV*)(*env)->GetLongField(env, scalar, perlScalar_sv);
	SV **sv=hv_store(hv, k, (I32)strlen(k), v, 0);
	free(k);
	if(sv!=0)
		SvREFCNT_inc(v);
	return NULL;
	}

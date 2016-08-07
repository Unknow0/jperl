#include "unknow_jperl_PerlScalar.h"
#include "jperl.h"

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlScalar_createUndef(JNIEnv *env, jclass class, jlong p)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)p;
	SV *sv=newSV_type(SVt_NULL);
	return jperl_create_scalar(env, my_perl, sv);
	}
JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlScalar_createIV(JNIEnv *env, jclass class, jlong p, jlong v)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)p;
	SV *sv=newSViv((IV)v);
	return jperl_create_scalar(env, my_perl, sv);
	}
JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlScalar_createNV(JNIEnv *env, jclass class, jlong p, jdouble v)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)p;
	SV *sv=newSVnv((NV)v);
	return jperl_create_scalar(env, my_perl, sv);
	}
JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlScalar_createPV(JNIEnv *env, jclass class, jlong p, jstring v)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)p;
	char *s=c_string(env, v);
	SV *sv=newSVpv(s, 0);
	free(s);
	return jperl_create_scalar(env, my_perl, sv);
	}

JNIEXPORT void JNICALL Java_unknow_jperl_PerlScalar_destroy(JNIEnv *env, jobject this)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	SV* sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	(*env)->SetLongField(env, this, perlScalar_sv, (jlong)0);
	SvREFCNT_dec_NN(sv);
	}

JNIEXPORT jstring JNICALL Java_unknow_jperl_PerlScalar_ref(JNIEnv *env, jobject this)
	{
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	const char *s=sv_reftype(sv, 1);
	return j_string(env, s);
	}

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlScalar_deref(JNIEnv *env, jobject this)
	{
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	SV *s;
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	if(!SvROK(sv))
		return NULL;
	s=SvRV(sv);
	SvREFCNT_inc_void_NN(s);
	return jperl_create_scalar(env, my_perl, s);
	}

JNIEXPORT jint JNICALL Java_unknow_jperl_PerlScalar_type(JNIEnv *env, jobject this)
	{
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	return SvTYPE(sv);
	}

JNIEXPORT jboolean JNICALL Java_unknow_jperl_PerlScalar_isUndefined(JNIEnv *env, jobject this)
	{
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	return !SvOK(sv);
	}
JNIEXPORT jboolean JNICALL Java_unknow_jperl_PerlScalar_toBoolean(JNIEnv *env, jobject this)
	{
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	return SvTRUE(sv);
	}
JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlScalar_toInt(JNIEnv *env, jobject this)
	{
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	if(SvOK(sv))
		return toInt(env, (jint)SvIV(sv));
	return NULL;
	}
JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlScalar_toLong(JNIEnv *env, jobject this)
	{
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	if(SvOK(sv))
		return toLong(env, (jlong)SvIV(sv));
	return NULL;
	}
JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlScalar_toDouble(JNIEnv *env, jobject this)
	{
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	if(SvOK(sv))
		return toDouble(env, SvNV(sv));
	return NULL;
	}
JNIEXPORT jstring JNICALL Java_unknow_jperl_PerlScalar_toString(JNIEnv *env, jobject this)
	{
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	char *s=SvPV_nolen(sv);
	return j_string(env, s);
	}

JNIEXPORT void JNICALL Java_unknow_jperl_PerlScalar_refInc(JNIEnv *env, jobject this)
	{
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	SvREFCNT_inc_void_NN(sv);
	}

#include "unknow_jperl_PerlScalar.h"
#include "jperl.h"

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlScalar_create(JNIEnv *env, jclass class, jlong sv, jlong perl)
	{
	jobject ret=(*env)->AllocObject(env, class);
	(*env)->SetLongField(env, ret, perlScalar_sv, sv);
	(*env)->SetLongField(env, ret, perlScalar_perl, perl);

	return ret;
	}

JNIEXPORT void JNICALL Java_unknow_jperl_PerlScalar_destroy(JNIEnv *env, jobject this)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	SV* sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	(*env)->SetLongField(env, this, perlScalar_sv, (jlong)0);
	sv_unref(sv);
	}

JNIEXPORT jstring JNICALL Java_unknow_jperl_PerlScalar_toString(JNIEnv *env, jobject this)
	{
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	char *s=SvPV_nolen(sv);
	return j_string(env, s);
	}

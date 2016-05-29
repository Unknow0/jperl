#include "unknow_jperl_PerlScalar.h"
#include "jperl.h"

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlScalar_create(JNIEnv *env, jclass class, jlong sv, jlong perl)
	{
	jobject ret=(*env)->AllocObject(env, class);
	(*env)->SetLongField(env, ret, perlScalar_sv, sv);
	(*env)->SetLongField(env, ret, perlScalar_perl, perl);

	return ret;
	}

JNIEXPORT jstring JNICALL Java_unknow_jperl_PerlScalar_toString(JNIEnv *env, jobject this)
	{
	STRLEN len;
	SV *sv=(SV*)(*env)->GetLongField(env, this, perlScalar_sv);
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	char *s=SvPV(sv, len);
	return j_string(env, s);
	}

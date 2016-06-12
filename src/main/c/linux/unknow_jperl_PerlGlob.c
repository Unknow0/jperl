#include "unknow_jperl_PerlGlob.h"
#include "jperl.h"


JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlGlob_scalar(JNIEnv *env, jobject this)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	GV *gv=(GV*)(*env)->GetLongField(env, this, perlScalar_sv);
	SV *sv=GvSV(gv);
	if(sv==NULL)
		return NULL;
	return jperl_create_scalar(env, my_perl, sv);
	}

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlGlob_array(JNIEnv *env, jobject this)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	GV *gv=(GV*)(*env)->GetLongField(env, this, perlScalar_sv);
	AV *av=GvAV(gv);
	if(av==NULL)
		return NULL;
	return jperl_create_scalar(env, my_perl, (SV*)av);
	}

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlGlob_hash(JNIEnv *env, jobject this)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	GV *gv=(GV*)(*env)->GetLongField(env, this, perlScalar_sv);
	HV *hv=GvHV(gv);
	if(hv==NULL)
		return NULL;
	return jperl_create_scalar(env, my_perl, (SV*)hv);
	}

JNIEXPORT void JNICALL Java_unknow_jperl_PerlGlob_setScalar(JNIEnv *env, jobject this, jobject scalar)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	GV *gv=(GV*)(*env)->GetLongField(env, this, perlScalar_sv);
	SV *sv=(SV*)(*env)->GetLongField(env, scalar, perlScalar_sv);
	GvSV(gv)=sv;
	}

JNIEXPORT void JNICALL Java_unknow_jperl_PerlGlob_setArray(JNIEnv *env, jobject this, jobject array)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	GV *gv=(GV*)(*env)->GetLongField(env, this, perlScalar_sv);
	AV *av=(AV*)(*env)->GetLongField(env, array, perlScalar_sv);
	GvAV(gv)=av;
	}

JNIEXPORT void JNICALL Java_unknow_jperl_PerlGlob_setHash(JNIEnv *env, jobject this, jobject hash)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	GV *gv=(GV*)(*env)->GetLongField(env, this, perlScalar_sv);
	HV *hv=(HV*)(*env)->GetLongField(env, hash, perlScalar_sv);
	GvHV(gv)=hv;
	}

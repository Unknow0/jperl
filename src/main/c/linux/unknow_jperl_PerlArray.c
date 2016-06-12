#include "jperl.h"
#include "unknow_jperl_PerlArray.h"

JNIEXPORT void JNICALL Java_unknow_jperl_PerlArray_create(JNIEnv *env, jobject this, jlong perl)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)perl;
	AV *av=newAV();
	(*env)->SetLongField(env, this, perlScalar_sv, (jlong)av);
	(*env)->SetLongField(env, this, perlScalar_perl, perl);
	}

JNIEXPORT void JNICALL Java_unknow_jperl_PerlArray_clear(JNIEnv *env, jobject this)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	AV *av=(AV*)(*env)->GetLongField(env, this, perlScalar_sv);
	av_clear(av);
	}

JNIEXPORT jint JNICALL Java_unknow_jperl_PerlArray_size(JNIEnv *env, jobject this)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	AV *av=(AV*)(*env)->GetLongField(env, this, perlScalar_sv);
	return av_top_index(av)+1;
	}

JNIEXPORT void JNICALL Java_unknow_jperl_PerlArray_push(JNIEnv *env, jobject this, jobject scalar)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	AV *av=(AV*)(*env)->GetLongField(env, this, perlScalar_sv);
	SV *sv=(SV*)(*env)->GetLongField(env, scalar, perlScalar_sv);
	av_push(av, sv);
	}

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlArray_get(JNIEnv *env, jobject this, jint i)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	AV *av=(AV*)(*env)->GetLongField(env, this, perlScalar_sv);
	SV **v=av_fetch(av, i, 1);
	if(v==NULL)
		return NULL;
	return jperl_create_scalar(env, my_perl, *v);
	}

JNIEXPORT void JNICALL Java_unknow_jperl_PerlArray_set(JNIEnv *env, jobject this, jint i, jobject scalar)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	AV *av=(AV*)(*env)->GetLongField(env, this, perlScalar_sv);
	SV *sv=(SV*)(*env)->GetLongField(env, scalar, perlScalar_sv);
	SV **v=av_store(av, i, sv);
	if(v!=NULL)
		SvREFCNT_inc_void_NN(sv);
	}

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlArray_pop(JNIEnv *env, jobject this)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	AV *av=(AV*)(*env)->GetLongField(env, this, perlScalar_sv);
	SV *sv=av_pop(av);
	return jperl_create_scalar(env, my_perl, sv);
	}

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlArray_shift(JNIEnv *env, jobject this)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlScalar_perl);
	AV *av=(AV*)(*env)->GetLongField(env, this, perlScalar_sv);
	SV *sv=av_shift(av);
	return jperl_create_scalar(env, my_perl, sv);
	}

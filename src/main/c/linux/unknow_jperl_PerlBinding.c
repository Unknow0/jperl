#include "unknow_jperl_PerlBinding.h"
#include "unknow_jperl_PerlScalar.h"
#include "unknow_jperl_PerlHash.h"
#include "jperl.h"

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlBinding_create(JNIEnv *env, jobject this, jlong perl, jstring pack)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)perl;
	char *package=c_string(env, pack);
	HV *hv=gv_stashpv(package, GV_ADD|SVf_UTF8);
	free(package);
	return jperl_create_scalar(env, my_perl, (SV*)hv);
	}

/*
 * Class:     unknow_jperl_PerlBinding
 * Method:    clear
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_unknow_jperl_PerlBinding_clear
  (JNIEnv *, jobject);

/*
 * Class:     unknow_jperl_PerlBinding
 * Method:    containsKey
 * Signature: (Ljava/lang/Object;)Z
 */
JNIEXPORT jboolean JNICALL Java_unknow_jperl_PerlBinding_containsKey
  (JNIEnv *, jobject, jobject);

/*
 * Class:     unknow_jperl_PerlBinding
 * Method:    remove
 * Signature: (Ljava/lang/Object;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlBinding_remove
  (JNIEnv *, jobject, jobject);

JNIEXPORT void JNICALL Java_unknow_jperl_PerlBinding_define(JNIEnv *env, jobject this, jlong perl, jlong stash, jstring name, jlong sv)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)perl;
	HV *hv=(HV*)stash;
	GV *gv=(GV*)sv;
	char *n=c_string(env, name);
	gv_init_pv(gv, hv, n, SVf_UTF8);
	free(n);
	}

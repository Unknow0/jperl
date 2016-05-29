#include "unknow_jperl_PerlBinding.h"
#include "unknow_jperl_PerlScalar.h"
#include "jperl.h"

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlBinding_define(JNIEnv *env, jobject this, jlong perl, jstring package, jstring name, jstring value)
	{
	SV *sv;
	PerlInterpreter *my_perl=(PerlInterpreter*)perl;
	char *p=c_string(env, package);
	char *n=c_string(env, name);
	char *v=c_string(env, value);
	char *s=malloc(18+strlen(p)+strlen(n)+strlen(v));
	s[0]=0;

	strcat(s, "package ");
	strcat(s, p);
	strcat(s, ";\nour $");
	strcat(s, n);
	strcat(s, "=");
	strcat(s, v);
	strcat(s, ";");

	sv=eval_pv(s, TRUE);
	
	free(p);
	free(n);
	free(v);
	free(s);
	
	return Java_unknow_jperl_PerlScalar_create(env, perlScalar, (jlong)sv, perl);
	}

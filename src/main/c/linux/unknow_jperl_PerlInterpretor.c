#include "unknow_jperl_PerlInterpretor.h"
#include "unknow_jperl_PerlScalar.h"
#include "jperl.h"
#include <string.h>

jclass perlScalar;
jfieldID perlScalar_sv;
jfieldID perlScalar_perl;
jfieldID perlInterpretor_perl;

JNIEXPORT void JNICALL Java_unknow_jperl_PerlInterpretor_init(JNIEnv *env, jclass class)
	{
	int argc=0;
	char **argv=NULL;
	char **envp=NULL;

	perlScalar=(*env)->FindClass(env, "unknow/jperl/PerlScalar");
	perlInterpretor_perl=(*env)->GetFieldID(env, class, "perl", "J");
	perlScalar_sv=(*env)->GetFieldID(env, perlScalar, "sv", "J");
	perlScalar_perl=(*env)->GetFieldID(env, perlScalar, "perl", "J");

	PERL_SYS_INIT3(&argc,&argv,&envp);
	}
JNIEXPORT void JNICALL Java_unknow_jperl_PerlInterpretor_deinit(JNIEnv *env, jclass class)
	{
	PERL_SYS_TERM();
	}

char *argv[]={"", "-e", "use utf8;"};
JNIEXPORT void JNICALL Java_unknow_jperl_PerlInterpretor_create(JNIEnv *env, jobject this)
	{
	PerlInterpreter *p=perl_alloc();
	(*env)->SetLongField(env, this, perlInterpretor_perl, (jlong)p);
	perl_construct(p);
	perl_parse(p, xs_init, 3, argv, NULL);
	perl_run(p);
	}
JNIEXPORT void JNICALL Java_unknow_jperl_PerlInterpretor_destroy(JNIEnv *env, jobject this)
	{
	PerlInterpreter *p=(PerlInterpreter*)(*env)->GetLongField(env, this, perlInterpretor_perl);
	perl_destruct(p);
	perl_free(p);
	}

JNIEXPORT jobject JNICALL Java_unknow_jperl_PerlInterpretor_eval(JNIEnv *env, jobject this, jstring str, jstring package)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlInterpretor_perl);
	char *s=c_string(env, str);
	char *pa=c_string(env, package);
	char *code=malloc(11+strlen(s)+strlen(pa));
	SV *sv;

	code[0]=0;
	strcat(code, "package ");
	strcat(code, pa);
	strcat(code, ";\n");
	strcat(code, s);

	sv=eval_pv(code, TRUE);

	free(s);
	free(pa);
	free(code);

	return Java_unknow_jperl_PerlScalar_create(env, perlScalar, (jlong)sv, (jlong)my_perl);
	}

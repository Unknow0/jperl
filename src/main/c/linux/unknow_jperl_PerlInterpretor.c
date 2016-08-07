#include "unknow_jperl_PerlInterpretor.h"
#include "unknow_jperl_PerlScalar.h"
#include "jperl.h"
#include <string.h>

JNIEXPORT void JNICALL Java_unknow_jperl_PerlInterpretor_create(JNIEnv *env, jobject this, jstring bf)
	{
	PerlInterpreter *my_perl=perl_alloc();
	SV *sv;
	if(!my_perl)
		{
		(*env)->ThrowNew(env, scriptException, "failled to allocate perl");
		return;
		}
	(*env)->SetLongField(env, this, perlInterpretor_perl, (jlong)my_perl);
	perl_construct(my_perl);
		{
		char *s=c_string(env, bf);
		char *argv[]={"jperl", "-I", s, "-e", "0"};
		int i=perl_parse(my_perl, xs_init, 5, argv, NULL);
		free(s);
		if(i)
			{
			(*env)->ThrowNew(env, scriptException, "failled to init perl");
			return;
			}
		}
	perl_run(my_perl);
	load_module(PERL_LOADMOD_NOIMPORT, newSVpv("jperl", 5), NULL);
	
	//sv=get_sv("jperl::internal::jni", GV_ADD);
	//SvUV_set(sv, (UV)env);
		{
		HV *stash=gv_stashpvn("jperl::internal", 15, 0);
		GV *gv=newGV(my_perl, stash, "jni");
		GvSV(gv)=newSVuv((UV)env);
		}
	}
JNIEXPORT void JNICALL Java_unknow_jperl_PerlInterpretor_destroy(JNIEnv *env, jobject this)
	{
	PerlInterpreter *p=(PerlInterpreter*)(*env)->GetLongField(env, this, perlInterpretor_perl);
	perl_destruct(p);
	perl_free(p);
	}

JNIEXPORT jobjectArray JNICALL Java_unknow_jperl_PerlInterpretor_eval(JNIEnv *env, jobject this, jstring str, jstring package)
	{
	PerlInterpreter *my_perl=(PerlInterpreter*)(*env)->GetLongField(env, this, perlInterpretor_perl);
	char *s=c_string(env, str);
	char *pa=c_string(env, package);
	char *code=malloc(11+strlen(s)+strlen(pa));
	SV *sv;
	I32 n;
	jobjectArray ret;

	dSP;

	ENTER;
	SAVETMPS;

	code[0]=0;
	strcat(code, "package ");
	strcat(code, pa);
	strcat(code, ";\n");
	strcat(code, s);
	sv=newSVpv(code, 0);
	
	PUSHMARK(SP);

	n=eval_sv(sv, G_ARRAY);
	SvREFCNT_dec(sv);

	SPAGAIN;
	if(n>0)
		{
		ret=(*env)->NewObjectArray(env, n, javaObject, NULL);
		while(n--)
			{
			SV *s=POPs;
			if(sv_derived_from_pvn(s, "jperl::Object", 13, 0))
				(*env)->SetObjectArrayElement(env, ret, n, (jobject)SvUV(SvRV(s)));
			else
				{
				SvREFCNT_inc_void_NN(s);
				(*env)->SetObjectArrayElement(env, ret, n, jperl_create_scalar(env, my_perl, s));
				}
			}
		}
	else
		{
		SV *err=ERRSV;
		if(SvTRUE(err))
			{
			int len;
			char *l=strdup(SvPV(err, len));
			l[len-1]=0; // strip line feed
			// TODO windows ?
			(*env)->ThrowNew(env, scriptException, l);
			free(l);
			}
		}

	PUTBACK;
	FREETMPS;
	LEAVE;

	free(s);
	free(pa);
	free(code);

	PerlIO_flush(PerlIO_stdout());
	fflush(stdout);
	
	return ret;
	}

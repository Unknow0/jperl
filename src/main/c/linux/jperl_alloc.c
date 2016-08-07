#include "jperl.h"
#include "XSUB.h"

void jperl_alloc(pTHX_ CV *cv)
	{
	dXSARGS; // set up items

	SV *sv=get_sv("jperl::internal::jni", SVf_UTF8);
	JNIEnv *env;
	jmethodID m;
	AV *av;
	jclass c;
	jobject o;
	jvalue *arg;
	int i;

	if(sv==NULL)
		{
		croak("jperl::internal::jni not initialized");
		XSRETURN_UNDEF;
		}
	if(items<3)
		{
		croak_xs_usage(cv, "jclass, jmethodID, sig, args...");
		XSRETURN_UNDEF;
		}
	env=(JNIEnv*)SvUV(sv);
	c=(jclass)SvUV(ST(0));
	m=(jmethodID)SvUV(ST(1));
	av=(AV*)SvRV(ST(2));
	arg=calloc(items-3, sizeof(jvalue));
	for(i=0; i<items-3; i++)
		{
		SV *sv=ST(i+3);
		SV **s=av_fetch(av, i, 0);
		char *e=SvPV_nolen(*s);
		convert(arg[i], e, sv);
		}
	o=(*env)->NewObjectA(env, c, m, arg);
	if(o)
		{
		o=(*env)->NewGlobalRef(env, o); // avoid java gc this object
		XSRETURN_UV((UV)o);
		}
	XSRETURN_UNDEF;
	}

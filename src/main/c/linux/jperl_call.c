#include "jperl.h"
#include "XSUB.h"

void jperl_call(pTHX_ CV *cv)
	{
	dXSARGS;
	int i;
	SV *n=cv_name(cv, NULL, CV_NAME_NOTQUAL);
	int len;
	char *name;
	SV *this=ST(0);
	char *pack;
	AV *arg=newAV();
	HV *m;
	SV **s;
	int r;
	for(i=1; i<items; i++)
		av_push(arg, ST(i));
	
		{
		const char *c;
		int len;
		if(sv_isobject(this))
			c=sv_reftype(SvRV(this), 1);
		else
			c=SvPV_nolen(this);
		len=strlen(c)+8;
		pack=malloc(len);
		pack[0]=0;
		strcat(pack, c);
		strcat(pack, "::class");
		}
	
	m=get_hv(pack, 0);
	s=hv_fetch(m, "method", 6, 0);
	if(!s)
		{
		croak("invalide class data (can't find methods)");
		XSRETURN_UNDEF;
		}
	m=(HV*)SvRV(*s);
	name=SvPV(n, len);
	s=hv_fetch(m, name, len, 0);
	if(!s)
		{
		croak("invalide class data (can't find method name)");
		XSRETURN_UNDEF;
		}

	PUSHMARK(SP);
	EXTEND(SP, 2);
	PUSHs(*s);
	PUSHs(newRV_noinc(arg));
	PUTBACK;
	r=call_pv("jperl::internal::find_method", G_ARRAY);
	SPAGAIN;
		{
		SV *ret=POPs;
		if(!SvOK(ret))
			{
			croak("no method found");
			goto end;
			}

		HV *hv=(HV*)SvRV(ret);
		SV *sv=get_sv("jperl::internal::jni", SVf_UTF8);
		SV **s;
		JNIEnv *env;
		jmethodID m;
		if(sv==NULL)
			{
			croak("jperl::internal::jni not initialized");
			goto end;
			}
		s=hv_fetch(hv, "m", 1, 0);
		env=(JNIEnv*)SvUV(sv);
		m=(jmethodID)SvUV(*s);
		s=hv_fetch(hv, "param", 5, 0);
		AV *av=(AV*)SvRV(*s);
		jobject obj=(jobject)SvUV(SvRV(this));
		jvalue *a=calloc(items-1, sizeof(jvalue));
		for(i=0; i<items-1; i++)
			{
			SV **sv=av_fetch(arg, i, 0);
			SV **s=av_fetch(av, i, 0);
			char *e=SvPV_nolen(*s);
			convert(a[i], e, *sv);
			}
		// TODO handle static call
		jobject o=(*env)->CallObjectMethodA(env, obj, m, a);
		{
		jclass c=(*env)->CallObjectMethod(env, o, object_class);
		IMPORT(c);
		ret=newSVuv((UV)(*env)->NewGlobalRef(env, o));
		ret=newSVrv(ret, "java::lang::String");
		PUSHs(sv_mortal(ret));
		}

		XSRETURN(1);
		}
end:
	XSRETURN_UNDEF;
	}

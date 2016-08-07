#include "jperl.h"
#include "XSUB.h"

void jperl_destroy(pTHX_ CV *cv)
	{
	dXSARGS;
	SV *sv=get_sv("jperl::internal::jni", SVf_UTF8);
	SV *self;
	jobject o;
	JNIEnv *env;

	if(sv==NULL)
		{
		croak("jperl::internal::jni not initialized");
		XSRETURN_UNDEF;
		}
	if(items!=1)
		{
		croak_xs_usage(cv, "self");
		XSRETURN_UNDEF;
		}
	env=(JNIEnv*)SvUV(sv);
	self=SvRV(ST(0));
	o=(jobject)SvUV(self);
	(*env)->DeleteGlobalRef(env, o);
	XSRETURN_UNDEF;
	}

EXTERN_C void boot_DynaLoader(pTHX_ CV* cv);
EXTERN_C void xs_init(pTHX)
	{
	char *file = __FILE__;
	/* DynaLoader is a special case */
	newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, file);

	newXS("jperl::import", jperl_import, file);
	newXS("jperl::internal::alloc", jperl_alloc, file);
	newXS("jperl::Object::DESTROY", jperl_destroy, file);
	}

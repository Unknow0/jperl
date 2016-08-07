#include "jperl.h"
#include "XSUB.h"

#define REPLACE(str,c,r) \
	{\
	char *s=str;\
	while(*s)\
		{\
		if(*s==c)\
			*s=r;\
		s++;\
		}\
	}

void load_param(PerlInterpreter *my_perl, JNIEnv *env, HV *m, jobjectArray p, jclass cl, char *name, jclass ret)
	{
	jsize len=(*env)->GetArrayLength(env, p);
	jstring s=(*env)->CallStaticObjectMethod(env, javaWrapper, wrapper_buildSig, p, ret);
	char *sig=c_string(env, s);
	jsize i;
	AV *param=newAV();
	AV *gen=newAV();
	av_fill(gen, len-1);
	jmethodID mid=(*env)->GetMethodID(env, cl, name, sig);
	free(sig);

	for(i=0; i<len; i++)
		{
		jobject pa=(*env)->GetObjectArrayElement(env, p, i);
		jobjectArray generic=(jobjectArray)(*env)->CallStaticObjectMethod(env, javaWrapper, wrapper_generic, pa);
		jstring v=(*env)->CallStaticObjectMethod(env, javaWrapper, wrapper_internal, pa);
		char *inter=c_string(env, v);
		av_push(param, newSVpv(inter, strlen(inter)));
		free(inter);
		
		if(generic)
			{
			AV *g=newAV();
			jint l=(*env)->GetArrayLength(env, generic);
			jint j;
			for(j=0; j<l; j++)
				{
				jstring str=(*env)->GetObjectArrayElement(env, generic, j);
				char *s=c_string(env, str);
				av_push(g, newSVpv(s, strlen(s)));
				free(s);
				(*env)->DeleteLocalRef(env, str);
				}
			av_store(gen, newRV_noinc((SV*)g), i);
			}
		(*env)->DeleteLocalRef(env, v);
		(*env)->DeleteLocalRef(env, generic);
		(*env)->DeleteLocalRef(env, pa);
		}

	hv_store(m, "param", 5, newRV_noinc((SV*)param), 0);
	hv_store(m, "gen", 3, newRV_noinc((SV*)gen), 0);
	hv_store(m, "count", 5, newSVuv((UV)len), 0);
	hv_store(m, "m", 1, newSVuv((UV)mid), 0);

	(*env)->DeleteLocalRef(env, s);
	}

int jperl_import_internal(PerlInterpreter *my_perl, JNIEnv *env, char *pack, jclass class)
	{
	AV *isa;
	HV *stash=gv_stashpv(pack, SVf_UTF8);
	int ret=1;
	
	if(stash)
		return 2;
	printf("import %s\n", pack);
	
	stash=gv_stashpv(pack, GV_ADD|SVf_UTF8);
	isa=newAV();
	
	jclass c=(*env)->GetSuperclass(env, class);
	if(c!=NULL)
		{
		jstring name=(*env)->CallObjectMethod(env, c, class_getName);
		char *n=c_string(env, name);
		char *p=to_perl_pack(n);
		free(n);

		av_push(isa, newSVpvn(p, strlen(p)));
		ret=jperl_import_internal(my_perl, env, p, c);
		free(p);
		if(!ret)
			return ret;
		(*env)->DeleteLocalRef(env, name);
		}
	printf("inter\n");	
		{ // add interfaces
		jobjectArray inter=(*env)->CallStaticObjectMethod(env, javaWrapper, wrapper_listInter, class);
		jint len=(*env)->GetArrayLength(env, inter);
		jint i;
		for(i=0; i<len; i++)
			{
			jstring p=(jstring)(*env)->GetObjectArrayElement(env, inter, i);
			char *s=c_string(env, p);
			av_push(isa, newSVpv(s, strlen(s)));
			free(s);
			(*env)->DeleteLocalRef(env, p);
			}
		(*env)->DeleteLocalRef(env, inter);
		}
	av_push(isa, newSVpvn("jperl::Object", 13));
		{
		GV *gv=newGV(my_perl, stash, "ISA");
		GvAV(gv)=isa;
		}
	printf("const\n");
		{ // load class data
		HV *classData=newHV();
		HV *method=newHV();
			{	// load constructors
			jobjectArray a=(jobjectArray)(*env)->CallObjectMethod(env, class, class_constructors);
			jsize l=(*env)->GetArrayLength(env, a);
			AV *constructors=newAV();
			for(;l>0; --l)
				{
				HV *m=newHV();
				jobject c=(*env)->GetObjectArrayElement(env, a, l-1);
				jobjectArray p=(jobjectArray)(*env)->CallObjectMethod(env, c, exe_getParam);
				load_param(my_perl, env, m, p, class, "<init>", NULL);
				av_push(constructors, newRV_noinc((SV*)m));
				(*env)->DeleteLocalRef(env, p);
				(*env)->DeleteLocalRef(env, c);
				}
			hv_store(method, "<init>", 6, newRV_noinc((SV*)constructors), 0);
			(*env)->DeleteLocalRef(env, a);
			}
			printf("meth\n");
			{ // load method
			size_t len=strlen(pack)+3;
			char *n=malloc(len+4);
			jobjectArray a=(jobjectArray)(*env)->CallObjectMethod(env, javaWrapper, wrapper_methods, class);
			jsize l=(*env)->GetArrayLength(env, a);

			n[0]=0;
			strcat(n, pack);
			strcat(n, "::");
			for(;l>0; --l)
				{
				HV *m=newHV();
				jobject c=(*env)->GetObjectArrayElement(env, a, l-1);
				jobjectArray p=(jobjectArray)(*env)->CallObjectMethod(env, c, exe_getParam);
				jstring s=(*env)->CallObjectMethod(env, c, exe_name);
				jclass ret=(*env)->CallObjectMethod(env, c, method_ret);
				jint flags=(*env)->CallStaticIntMethod(env, javaWrapper, wrapper_flags, c);
				char *name=c_string(env, s);
				printf("load %d %s\n", l, name);
				AV *methods;
				SV **sv=hv_fetch(method, name, strlen(name), 0);
				if(sv)
					methods=(AV*)SvRV(*sv);
				else
					{
					methods=newAV();
					hv_store(method, name, strlen(name), newRV_noinc((SV*)methods), 0);

					n[len-1]=0;
					strcat(n, name);
					newXS(n, jperl_call, __FILE__);
					}
				load_param(my_perl, env, m, p, class, name, ret);
				printf(" -> load_param\n");
				hv_store(m, "flags", 5, newSVuv((UV)flags), 0);
				av_push(methods, newRV_noinc((SV*)m));

				free(name);
				(*env)->DeleteLocalRef(env, ret);
				(*env)->DeleteLocalRef(env, s);
				(*env)->DeleteLocalRef(env, p);
				(*env)->DeleteLocalRef(env, c);
				}
			free(n);
			(*env)->DeleteLocalRef(env, a);
			}
		printf("end\n");
		hv_store(classData, "id", 2, newSVuv((UV)(*env)->NewGlobalRef(env, class)), 0);
		hv_store(classData, "method", 6, newRV_noinc((SV*)method), 0);
			{
			GV *gv=newGV(my_perl, stash, "class");
			GvHV(gv)=classData;
			}
		}
	return 1;
	}

void jperl_import(pTHX_ CV* cv)
	{
	dXSARGS; // set up items
	SV *sv=get_sv("jperl::internal::jni", SVf_UTF8);
	char *c;
	JNIEnv *env;
	
	if(sv==NULL||SvUV(sv)==0)
		{
		croak("jperl::internal::jni not initialized");
		XSRETURN_IV(0);
		}
	if(items!=1)
		{
		croak_xs_usage(cv, "p");
		XSRETURN_IV(0);
		}
	env=(JNIEnv*)SvUV(sv);
	char *n=(char *)SvPV_nolen(ST(0));
	c=strdup(n);
	REPLACE(c, '.', '/');
	jclass cl=(*env)->FindClass(env, c);
	free(c);
	if(cl)
		{
		char *p=to_perl_pack(n);
		int i=jperl_import_internal(my_perl, env, p, cl);
		free(p);
		XSRETURN_IV(i);
		}
	XSRETURN_IV(0);
	}

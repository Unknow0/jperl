#include "jperl.h"

jobject jperl_create_scalar(JNIEnv *env, PerlInterpreter *my_perl, SV *sv)
	{
	jobject ret;
	jclass class;
	switch(SvTYPE(sv))
		{
		case SVt_NULL:
		case SVt_IV:
		case SVt_NV:
		case SVt_PV:
		case SVt_PVIV:
		case SVt_PVNV:
		case SVt_PVMG:
		case SVt_INVLIST:
		case SVt_REGEXP:
		case SVt_PVLV: 
			class=perlScalar;
			break;
		case SVt_PVGV:
			class=perlGlob;
			break;
		case SVt_PVAV:
			class=perlArray;
			break;
		case SVt_PVHV:
			class=perlHash;
			break;
		case SVt_PVCV: // sub
		case SVt_PVFM: // format
		case SVt_PVIO: // io
		default:
			class=perlScalar;
			break;
		}

	ret =(*env)->AllocObject(env, class);
	(*env)->SetLongField(env, ret, perlScalar_sv, (jlong)sv);
	(*env)->SetLongField(env, ret, perlScalar_perl, (jlong)my_perl);
	return ret;
	}

jobject toDouble(JNIEnv *env, jdouble v)
	{
	return (*env)->CallStaticObjectMethod(env, javaDouble, double_valueOf, v);
	}
jobject toInt(JNIEnv *env, jint v)
	{
	return (*env)->CallStaticObjectMethod(env, javaInteger, integer_valueOf, v);
	}
jobject toLong(JNIEnv *env, jlong v)
	{
	return (*env)->CallStaticObjectMethod(env, javaLong, long_valueOf, v);
	}

jsize len_utf8(const char *str)
	{
	jsize len=0;
	unsigned char *s=(unsigned char *)str;
	// count 
	while(*s!=0)
		{
		if(*s<0xF0)
			{
			if(*s<0x80)
				s++;
			else if(*s<0xE0)
				s+=2;
			else
				s+=3;
			len++;
			}
		else
			{
			s+=4;
			len+=2;
			}
		}
	return len;
	}

size_t len_utf16(const jchar *str, jsize jlen)
	{
	size_t len=0;
	jsize i;
	for(i=0; i<jlen; i++)
		{
		if(str[i]<0x80)
			len++;
		else if(str[i]<0x800)
			len+=2;
		else if(str[i]<10000)
			len+=3;
		else
			len+=4;
		}
	return len;
	}

char *c_string(JNIEnv *env, jstring str)
	{
	jsize i;
	size_t j=0;
	const jchar *jstr=(*env)->GetStringChars(env, str, JNI_FALSE);
	jsize jlen=(*env)->GetStringLength(env, str);
	size_t len=len_utf16(jstr, jlen);
	char *s=malloc(len+1);
	s[len]=0;

	for(i=0; i<jlen; i++)
		{
		if(jstr[i]<0x80)
			s[j++]=jstr[i]&0x7F;
		else if(jstr[i]<0x800)
			{
			s[j++]=(char)(0xC0+((jstr[i]&0x7FF)>>6));
			s[j++]=(char)(0x80+(jstr[i]&0x3F));
			}
		else if(jstr[i]<0x10000)
			{
			s[j++]=(char)(0xE0+((jstr[i]&0x3FFF)>>12));
			s[j++]=(char)(0x80+((jstr[i]&0x3FF)>>6));
			s[j++]=(char)(0x80+(jstr[i]&0x3F));
			}
		else
			{
			int c=1+((jstr[i]&0x3FF)>>6);
			s[j++]=(char)(0xF0+(c>>2));
			s[j++]=(char)(0x80+((c&0x03)<<4)+(jstr[i]&0x3FF));
			s[j]=(char)(0x80+((jstr[i]&0x03)<<4));
			s[j]=(char)(s[j]+((jstr[++i]&0x3FF)>>6));
			j++;
			s[j++]=(char)(0x80+(jstr[i]&0x3F));
			}
		}
	(*env)->ReleaseStringChars(env, str, jstr);

	return s;
	}

jstring j_string(JNIEnv *env, const char *str)
	{
	jsize i=0;
	jsize len=len_utf8(str);
	jchar *res=malloc((size_t)len);
	char *s=(char*)str;

	while(*s!=0)
		{
		if(*s<0x80)
			res[i++]=(jchar)(*(s++))&0x7F;
		else if(*s<0xE0)
			{
			int v=(*s&0x1F)<<6;
			v+=res[i]+((*(s++)&03)<<6);
			res[i++]=(jchar)(v+(*(s++)&0x3F));
			}
		else if(*s<0xF0)
			{
			int v=(*(s++)&0x0F)<<12;
			v+=(*s&0x3F)<<6;
			v+=(*(s++)&0x03)<<6;
			res[i++]=(jchar)(v+(*(s++)&0x3F));
			}
		else
			{
			int c=*(s++)&0x03;
			c+=(*s&0x3F>>4)-1;
			c=0xD800+(c<<6)+((c&0x03)<<6)+(*(s++)&0x0F<<2);
			res[i++]=(jchar)(c+(*s&0x3F>>4));
			c=0xDC00+(*s&0x0F<<6);
			c+=*(s++)&0x03;
			res[i++]=(jchar)(c+(*s&0x3F));
			}
		}

	return (*env)->NewString(env, res, len);
	}

char *to_perl_pack(char *str)
	{
	char *t;
	size_t l=1;
	char *out;
	for(t=str; *t; t++)
		{
		if(*t=='.')
			l+=2;
		else
			l++;
		}
	out=malloc(l);
	for(t=str, l=0; *t; t++)
		{
		if(*t=='.')
			{
			out[l++]=':';
			out[l++]=':';
			}
		else
			out[l++]=*t;
		}
	out[l]=0;
	return out;
	}

GV *newGV(PerlInterpreter *my_perl, HV *stash, char *name)
	{
	GV *gv=(GV*)newSV_type(SVt_NULL);
	gv_init_pv(gv, stash, name, GV_ADDMULTI);
	hv_store(stash, name, (I32)strlen(name), (SV*)gv, 0);
	return gv;
	}

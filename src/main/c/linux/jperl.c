#include "jperl.h"

EXTERN_C void boot_DynaLoader(pTHX_ CV* cv);
EXTERN_C void xs_init(pTHX)
	{
	char *file = __FILE__;
	/* DynaLoader is a special case */
	newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, file);
	}

jsize len_utf8(char *str)
	{
	jsize len=0;
	unsigned char *s=str;
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

size_t len_utf16(jchar *str, jsize jlen)
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
			s[j++]=0xC0+(jstr[i]&0x7FF)>>6;
			s[j++]=0x80+jstr[i]&0x3F;
			}
		else if(jstr[i]<0x10000)
			{
			s[j++]=0xE0+(jstr[i]&0x3FFF)>>12;
			s[j++]=0x80+(jstr[i]&0x3FF)>>6;
			s[j++]=0x80+jstr[i]&0x3F;
			}
		else
			{
			int c=1+(jstr[i]&0x3FF)>>6;
			s[j++]=0xF0+c>>2;
			s[j++]=0x80+((c&0x03)<<4)+jstr[i]&0x3FF;
			s[j++]=0x80+((jstr[i]&0x03)<<4)+(jstr[++i]&0x3FF)>>6;
			s[j++]=0x80+jstr[i]&0x3F;
			}
		}
	(*env)->ReleaseStringChars(env, str, jstr);

	return s;
	}

jstring j_string(JNIEnv *env, char *str)
	{
	jsize i=0;
	jsize len=len_utf8(str);
	jchar *res=malloc((size_t)len);
	char *s=str;

	while(*s!=0)
		{
		if(*s<0x80)
			res[i++]=*(s++)&0x7F;
		else if(*s<0xE0)
			res[i++]=((*s&0x1F)<<6)+((*(s++)&03)<<6)+(*(s++)&0x3F);
		else if(*s<0xF0)
			res[i++]=((*(s++)&0x0F)<<12)+((*s&0x3F)<<6)+((*(s++)&0x03)<<6)+(*(s++))&0x3F;
		else
			{
			int c=(*(s++)&0x03)+(*s&0x3F>>4)-1;
			res[i++]=0xD800+(c<<6)+(c&0x03<<6)+(*(s++)&0x0F<<2)+(*s&0x3F>>4);
			res[i++]=0xDC00+(*s&0x0F<<6)+*(s++)&0x03+(*s&0x3F);
			}
		}

	return (*env)->NewString(env, res, len);
	}

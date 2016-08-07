#include "unknow_jperl_PerlBootstrap.h"
#include "unknow_jperl_PerlScalar.h"
#include "jperl.h"
#include <string.h>

jclass javaInteger;
jmethodID integer_valueOf;
jclass javaLong;
jmethodID long_valueOf;
jclass javaDouble;
jmethodID double_valueOf;

jclass javaObject;
jmethodID object_class;
jclass javaClass;
jmethodID class_getName;
jmethodID class_constructors;
jmethodID class_getCName;

jclass javaExecutable;
jmethodID exe_getParam;
jmethodID exe_name;
jmethodID method_ret;

jmethodID param_type;

jclass perlScalar;
jfieldID perlScalar_sv;
jfieldID perlScalar_perl;
jclass perlArray;
jclass perlHash;
jclass perlGlob;
jfieldID perlInterpretor_perl;

jclass javaWrapper;
jmethodID wrapper_buildSig;
jmethodID wrapper_internal;
jmethodID wrapper_listInter;
jmethodID wrapper_generic;
jmethodID wrapper_methods;
jmethodID wrapper_flags;

jclass javaBoolean;
jmethodID boolValue;
jclass javaChar;
jmethodID charValue;

jclass number;
jmethodID numByte;
jmethodID numShort;
jmethodID numInt;
jmethodID numLong;
jmethodID numFloat;
jmethodID numDouble;

jclass scriptException;

#define loadClass(cl, name) \
	cl=(*env)->FindClass(env, name);\
	cl=(jclass)(*env)->NewGlobalRef(env, cl);

JNIEXPORT void JNICALL Java_unknow_jperl_PerlBootstrap_init(JNIEnv *env, jclass class)
	{
	int argc=0;
	char **argv=NULL;
	char **envp=NULL;

	PERL_SYS_INIT3(&argc,&argv,&envp);
	
	loadClass(javaObject, "java/lang/Object");
	object_class=(*env)->GetMethodID(env, javaObject, "getClass", "()Ljava/lang/Class;");
	loadClass(class, "unknow/jperl/PerlInterpretor");
	loadClass(javaInteger, "java/lang/Integer");
	integer_valueOf=(*env)->GetStaticMethodID(env, javaInteger, "valueOf", "(I)Ljava/lang/Integer;");
	loadClass(javaLong, "java/lang/Long");
	long_valueOf=(*env)->GetStaticMethodID(env, javaLong, "valueOf", "(J)Ljava/lang/Long;");
	
	loadClass(javaDouble, "java/lang/Double");
	double_valueOf=(*env)->GetStaticMethodID(env, javaDouble, "valueOf", "(D)Ljava/lang/Double;");

	loadClass(javaClass, "java/lang/Class");
	class_getName=(*env)->GetMethodID(env, javaClass, "getName", "()Ljava/lang/String;");
	class_getCName=(*env)->GetMethodID(env, javaClass, "getCanonicalName", "()Ljava/lang/String;");
	class_constructors=(*env)->GetMethodID(env, javaClass, "getConstructors", "()[Ljava/lang/reflect/Constructor;");

	loadClass(javaBoolean, "java/lang/Boolean");
	boolValue=(*env)->GetMethodID(env, javaBoolean, "booleanValue", "()Z");
	loadClass(javaChar, "java/lang/Character");
	charValue=(*env)->GetMethodID(env, javaChar, "charValue", "()C");
	
	loadClass(number, "java/lang/Number");
	numByte=(*env)->GetMethodID(env, number, "byteValue", "()B");
	numShort=(*env)->GetMethodID(env, number, "shortValue", "()S");
	numInt=(*env)->GetMethodID(env, number, "intValue", "()I");
	numLong=(*env)->GetMethodID(env, number, "longValue", "()J");
	numFloat=(*env)->GetMethodID(env, number, "floatValue", "()F");
	numDouble=(*env)->GetMethodID(env, number, "doubleValue", "()D");

	loadClass(javaExecutable, "java/lang/reflect/Executable");
	exe_getParam=(*env)->GetMethodID(env, javaExecutable, "getParameters", "()[Ljava/lang/reflect/Parameter;");
	exe_name=(*env)->GetMethodID(env, javaExecutable, "getName", "()Ljava/lang/String;");
	jclass method;
	loadClass(method, "java/lang/reflect/Method");
	method_ret=(*env)->GetMethodID(env, method, "getReturnType", "()Ljava/lang/Class;");

	jclass javaParam;
	loadClass(javaParam, "java/lang/reflect/Parameter");
	param_type=(*env)->GetMethodID(env, javaParam, "getType", "()Ljava/lang/Class;");

	loadClass(perlScalar, "unknow/jperl/PerlScalar");
	perlScalar_sv=(*env)->GetFieldID(env, perlScalar, "sv", "J");
	perlScalar_perl=(*env)->GetFieldID(env, perlScalar, "perl", "J");
	loadClass(perlArray, "unknow/jperl/PerlArray");
	loadClass(perlHash, "unknow/jperl/PerlHash");
	loadClass(perlGlob, "unknow/jperl/PerlGlob");
	perlInterpretor_perl=(*env)->GetFieldID(env, class, "perl", "J");

	loadClass(javaWrapper, "unknow/jperl/JavaWrapper");
	wrapper_buildSig=(*env)->GetStaticMethodID(env, javaWrapper, "buildSig", "([Ljava/lang/reflect/Parameter;Ljava/lang/Class;)Ljava/lang/String;");
	wrapper_internal=(*env)->GetStaticMethodID(env, javaWrapper, "toInternal", "(Ljava/lang/reflect/Parameter;)Ljava/lang/String;");
	wrapper_listInter=(*env)->GetStaticMethodID(env, javaWrapper, "listInterface", "(Ljava/lang/Class;)[Ljava/lang/String;");
	wrapper_generic=(*env)->GetStaticMethodID(env, javaWrapper, "generic", "(Ljava/lang/reflect/Parameter;)[Ljava/lang/String;");
	wrapper_methods=(*env)->GetStaticMethodID(env, javaWrapper, "methods", "(Ljava/lang/Class;)[Ljava/lang/reflect/Method;");
	wrapper_flags=(*env)->GetStaticMethodID(env, javaWrapper, "flags", "(Ljava/lang/reflect/Method;)I");

	loadClass(scriptException, "javax/script/ScriptException");
	}
JNIEXPORT void JNICALL Java_unknow_jperl_PerlBootstrap_deinit(JNIEnv *env, jclass class)
	{
	PERL_SYS_TERM();
	}

package unknow.jperl;

import java.lang.reflect.*;
import java.util.*;

@SuppressWarnings({"rawtypes"})
public class JavaWrapper
	{
	public static final int STATIC=1<<1;
	public static final int ARRAY=1<<2;
	private static final Map<String,String> NATIVE;
	static
		{
		NATIVE=new HashMap<String,String>();
		NATIVE.put("void", "V");
		NATIVE.put("boolean", "Z");
		NATIVE.put("byte", "B");
		NATIVE.put("char", "C");
		NATIVE.put("short", "S");
		NATIVE.put("int", "I");
		NATIVE.put("long", "J");
		NATIVE.put("float", "F");
		NATIVE.put("double", "D");
		}

	public static String buildSig(Parameter[] param, Class ret)
		{
		StringBuilder sb=new StringBuilder("(");
		for(int i=0; i<param.length; i++)
			appendSig(sb, param[i].getType());
		sb.append(')');
		appendSig(sb, ret==null?void.class:ret);
		return sb.toString();
		}

	public static void appendSig(StringBuilder sb, Class c)
		{
		if(c.isArray())
			sb.append(c.getName());
		else if(c.isPrimitive())
			sb.append(NATIVE.get(c.getName()));
		else
			sb.append('L').append(c.getName().replace('.', '/')).append(';');
		}

	public static String toInternal(Parameter p)
		{
		return toInternal(p.getType());
		}

	public static String toInternal(Class c)
		{
		if(c.isArray())
			return c.getName();
		else if(c.isPrimitive())
			return NATIVE.get(c.getName());
		else
			return "L"+c.getName().replace('.', '/')+";";
		}

	public static String[] generic(Parameter p)
		{
		Type t=p.getParameterizedType();
		if(t instanceof ParameterizedType)
			{
			Type[] a=((ParameterizedType)t).getActualTypeArguments();
			if(a.length==0)
				return null;
			String[] s=new String[a.length];
			for(int i=0; i<a.length; i++)
				s[i]=a[i].getTypeName();
			return s;
			}
		return null;
		}

	public static void listInterface(Class c, List<Class> list)
		{
		Class[] inter=c.getInterfaces();
		for(int i=0; i<inter.length; i++)
			{
			list.add(inter[i]);
			listInterface(inter[i], list);
			}
		}

	public static String[] listInterface(Class c)
		{
		List<Class> list=new ArrayList<Class>();
		listInterface(c, list);
		String[] s=new String[list.size()];
		for(int i=0; i<list.size(); i++)
			s[i]=list.get(i).getName().replaceAll("[.$]", "::");
		return s;
		}

	private static final int exclude=Modifier.INTERFACE|Modifier.ABSTRACT|Modifier.PRIVATE|Modifier.PROTECTED;

	public static Method[] methods(Class c)
		{
		Method[] m=c.getDeclaredMethods();
		Method[] r=new Method[m.length];
		int j=0;
		for(int i=0; i<m.length; i++)
			{
			if((m[i].getModifiers()&exclude)==0)
				r[j++]=m[i];
			}
		return Arrays.copyOf(r, j);
		}

	public static int flags(Method m)
		{
		return 0;
		}
	}
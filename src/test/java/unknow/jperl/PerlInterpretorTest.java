package unknow.jperl;

import javax.script.*;

import org.junit.*;

public class PerlInterpretorTest
	{
	@Test
	public void test1() throws ScriptException
		{
		PerlInterpretor p=new PerlInterpretor();
		System.out.println(p.eval("return 'test';"));
		}
	}

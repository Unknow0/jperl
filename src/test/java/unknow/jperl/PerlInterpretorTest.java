package unknow.jperl;

import static org.junit.Assert.*;

import java.util.*;

import javax.script.*;

import org.junit.*;

@SuppressWarnings({"unchecked", "restriction"})
public class PerlInterpretorTest
	{
	@Test
	public void test_Scalar() throws ScriptException
		{
		PerlInterpretor p=new PerlInterpretor();
		PerlScalar eval=p.eval("42");
		assertEquals("42", eval.toString());
		p.finalize();
		}

	@Test
	public void test_ref() throws ScriptException
		{
		PerlInterpretor p=new PerlInterpretor();
		PerlScalar eval=p.eval("42");
		assertEquals("SCALAR", eval.ref());
		assertNull(eval.deref());

		eval=p.eval("'test'");
		assertEquals("SCALAR", eval.ref());
		assertNull(eval.deref());

		eval=p.eval("@a=(1, 'test'); \\@a");
		assertEquals("REF", eval.ref());
		eval=eval.deref();
		assertNotNull(eval);
		assertEquals("ARRAY", eval.ref());

		eval=p.eval("{1=>'test', foo=>'bar'}");
		assertEquals("REF", eval.ref());
		eval=eval.deref();
		assertNotNull(eval);
		assertEquals("HASH", eval.ref());

		p.finalize();
		}

	@Test
	public void test_conv() throws ScriptException
		{
		PerlInterpretor p=new PerlInterpretor();

		PerlScalar sv=p.eval("42");
		assertFalse(sv.isUndefined());
		assertEquals(42, (int)sv.toInt());
		assertEquals(42, (double)sv.toDouble(), 0.0001);
		assertEquals("42", sv.toString());
		assertTrue(sv.toBoolean());

		sv=p.eval("0");
		assertFalse(sv.isUndefined());
		assertFalse(sv.toBoolean());

		sv=p.eval("42.23");
		assertFalse(sv.isUndefined());
		assertEquals(42, (int)sv.toInt());
		assertEquals(42.23, (double)sv.toDouble(), 0.0001);
		assertEquals("42.23", sv.toString());
		assertTrue(sv.toBoolean());

		sv=p.eval("$t=undef");
		assertTrue(sv.isUndefined());
		assertFalse(sv.toBoolean());

		sv=p.eval("test");
		assertFalse(sv.isUndefined());
		assertTrue(sv.toBoolean());

		sv=p.eval("$t='';");
		assertFalse(sv.isUndefined());
		assertFalse(sv.toBoolean());
		}

	@Test
	public void test_array() throws ScriptException
		{
		PerlInterpretor p=new PerlInterpretor();
		PerlScalar perl=p.toPerl(Arrays.asList(42, "Tets", "23"));

		assertEquals(PerlArray.class, perl.getClass());
		PerlArray pa=(PerlArray)perl;

		assertEquals(3, pa.size());

		assertEquals(42, (long)pa.get(0).toLong());
		assertEquals("Test", pa.get(1).toString());
		assertEquals("23", pa.get(2).toString());

		PerlScalar ps=PerlScalar.createPV(p.perl, "Test");
		pa.push(ps);
		assertEquals(4, pa.size());
		assertEquals(ps.toString(), pa.pop().toString());
		assertEquals(3, pa.size());

		ps=pa.shift();
		assertEquals(2, pa.size());
		assertEquals("42", pa.toString());

		pa.clear();
		assertTrue(pa.isEmpty());
		assertEquals(0, pa.size());
		}

	@Test
	public void test_hash() throws ScriptException
		{
		Map<String,String> map=new HashMap<String,String>();
		map.put("k1", "v1");
		map.put("k2", "v2");
		map.put("k3", "v3");

		PerlInterpretor p=new PerlInterpretor();
		PerlScalar perl=p.toPerl(map);

		assertEquals(PerlHash.class, perl.getClass());
		PerlHash ph=(PerlHash)perl;

		assertEquals(3, ph.size());

		for(String k:map.keySet())
			{
			PerlScalar v=ph.get(k);
			assertNotNull(v);
			assertEquals(map.get(k), v.toString());
			}

		PerlScalar ps=PerlScalar.createPV(p.perl, "Test");
		ph.put("t", ps);
		assertEquals(4, ph.size());
		assertTrue(ph.containsKey("t"));
		assertEquals(ph.get("t").toString(), ps.toString());

		ph.remove("t");
		assertEquals(3, ph.size());
		assertFalse(ph.containsKey("t"));
		assertNull(ph.get("t"));

		ph.clear();
		assertTrue(ph.isEmpty());
		assertEquals(0, ph.size());
		assertNull(ph.get("truc"));
		}

	@Test
	public void test_bind() throws ScriptException
		{
		PerlInterpretor p=new PerlInterpretor();
		PerlBinding b=new PerlBinding("test", p);
		assertEquals(0, b.size());
		b.put("v0", "test");
		assertEquals(1, b.size());
		assertEquals("test", b.get("v0").asGlob().scalar().toString());
		}

	@Test
	public void test_context() throws ScriptException
		{
		PerlInterpretor p=new PerlInterpretor();
		PerlContext ctx1=p.getContext();
		PerlContext ctx2=new PerlContext(p, "test1");
		ctx1.getBindings(ScriptContext.ENGINE_SCOPE).put("test", "context1");
		ctx2.getBindings(ScriptContext.ENGINE_SCOPE).put("test", "context2");
		assertEquals("context1", p.eval("$test;", ctx1).toString());
		assertEquals("context2", p.eval("$test;", ctx2).toString());
		p.finalize();
		}
	}

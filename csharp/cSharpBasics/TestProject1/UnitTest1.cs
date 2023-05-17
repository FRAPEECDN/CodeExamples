using BasicExample;

namespace ExampleUnitTest
{
    [TestClass]
    public class UnitTest1
    {
        int addExample(int a, int b)
        {
            return a + b;
        }

        [TestMethod]
        public void TestMethod1() 
        {
            Assert.IsTrue(addExample(1, 2) == 3);
            Assert.IsFalse(addExample(1, 2) == 4);
        }

        [TestMethod]
        public void TestMethod2()
        {
            int a = 3;
            int b = 4;
            Assert.AreNotEqual(a, b);
            b = 3;
            Assert.AreEqual(a, b);
        }

        [TestMethod]
        public void TestMethod3()
        {
            ExampleStruct example = new ExampleStruct();
            Assert.AreEqual(example.a, 0);
            Assert.AreEqual(example.b, "");
        }
    }
}